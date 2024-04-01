#ifndef AST_H
#define AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

// Forward declarations of classes used in the AST
class AST;
class Expr;
class Program;
class Declaration;
class Final;
class BinaryOp;
class UnaryOp;
class SignedNumber;
class NegExpr;
class Assignment;
class Logic;
class Comparison;
class LogicalExpr;
class IfStmt;
class WhileStmt;
class elifStmt;
class ForStmt;
class PrintStmt;

// ASTVisitor class defines a visitor pattern to traverse the AST
class ASTVisitor
{
public:
  // Virtual visit functions for each AST node type
  virtual void visit(AST &) {}               // Visit the base AST node
  virtual void visit(Expr &) {}              // Visit the expression node
  virtual void visit(Program &) {}           // Visit the group of expressions node
  virtual void visit(Final &) = 0;           // Visit the Final node
  virtual void visit(BinaryOp &) = 0;        // Visit the binary operation node
  virtual void visit(UnaryOp &) = 0;
  virtual void visit(SignedNumber &) = 0;
  virtual void visit(NegExpr &) = 0;
  virtual void visit(Assignment &) = 0;      // Visit the assignment expression node
  virtual void visit(Declaration &) = 0;     // Visit the variable declaration node
  virtual void visit(Logic &) {}             // Visit the Logic node
  virtual void visit(Comparison &) = 0;      // Visit the Comparison node
  virtual void visit(LogicalExpr &) = 0;     // Visit the LogicalExpr node
  virtual void visit(IfStmt &) = 0;          // Visit the IfStmt node
  virtual void visit(WhileStmt &) = 0;        // Visit the IterStmt node
  virtual void visit(elifStmt &) = 0;        // Visit the elifStmt node
  virtual void visit(ForStmt &) = 0;
  virtual void visit(PrintStmt &) = 0;
};

// AST class serves as the base class for all AST nodes
class AST
{
public:
  virtual ~AST() {}
  virtual void accept(ASTVisitor &V) = 0;    // Accept a visitor for traversal
};

// Expr class represents an expression in the AST
class Expr : public AST
{
public:
  Expr() {}
};

class Logic : public AST
{
public:
  Logic() {}
};

// Program class represents a group of expressions in the AST
class Program : public AST
{
  using dataVector = llvm::SmallVector<AST *>;

private:
  dataVector data;                          // Stores the list of expressions

public:
  Program(llvm::SmallVector<AST *> data) : data(data) {}
  Program() = default;

  llvm::SmallVector<AST *> getdata() { return data; }

  dataVector::const_iterator begin() { return data.begin(); }

  dataVector::const_iterator end() { return data.end(); }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

// Declaration class represents a variable declaration with an initializer in the AST
class Declaration : public Program
{
  using VarVector = llvm::SmallVector<llvm::StringRef, 8>;
  using ValueVector = llvm::SmallVector<Expr *, 8>;
  VarVector Vars;                           // Stores the list of variables
  ValueVector Values;                       // Stores the list of initializers

public:
  // Declaration(llvm::SmallVector<llvm::StringRef, 8> Vars, Expr *E) : Vars(Vars), E(E) {}
  Declaration(llvm::SmallVector<llvm::StringRef, 8> Vars, llvm::SmallVector<Expr *, 8> Values) : Vars(Vars), Values(Values) {}

  VarVector::const_iterator varBegin() { return Vars.begin(); }

  VarVector::const_iterator varEnd() { return Vars.end(); }

  ValueVector::const_iterator valBegin() { return Values.begin(); }

  ValueVector::const_iterator valEnd() { return Values.end(); }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};


// Final class represents a Final in the AST (either an identifier or a number or true or false)
class Final : public Expr
{
public:
  enum ValueKind
  {
    Ident,
    Number,
    True,
    False
  };

private:
  ValueKind Kind;                            // Stores the kind of Final (identifier or number or true or false)
  llvm::StringRef Val;                       // Stores the value of the Final

public:
  Final(ValueKind Kind, llvm::StringRef Val) : Kind(Kind), Val(Val) {}

  ValueKind getKind() { return Kind; }

  llvm::StringRef getVal() { return Val; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

// BinaryOp class represents a binary operation in the AST (plus, minus, multiplication, division)
class BinaryOp : public Expr
{
public:
  enum Operator
  {
    Plus,
    Minus,
    Mul,
    Div,
    Mod,
    Exp
  };

private:
  Expr *Left;                               // Left-hand side expression
  Expr *Right;                              // Right-hand side expression
  Operator Op;                              // Operator of the binary operation

public:
  BinaryOp(Operator Op, Expr *L, Expr *R) : Op(Op), Left(L), Right(R) {}

  Expr *getLeft() { return Left; }

  Expr *getRight() { return Right; }

  Operator getOperator() { return Op; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

// naryOp class represents a unary operation in the AST (plus plus, minus minus)
class UnaryOp : public Expr
{
public:
  enum Operator
  {
    Plus_plus,
    Minus_minus
  };

private:
  llvm::StringRef Ident;                      
  Operator Op;                              // Operator of the unary operation

public:
  UnaryOp(Operator Op, llvm::StringRef I) : Op(Op), Ident(I) {}

  llvm::StringRef getIdent() { return Ident; }

  Operator getOperator() { return Op; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class SignedNumber : public Expr
{
public:
  enum Sign
  {
    Plus,
    Minus
  };

private:
  llvm::StringRef Value;                              
  Sign s;                              

public:
  SignedNumber(Sign S, llvm::StringRef V) : s(S), Value(V) {}

  llvm::StringRef getValue() { return Value; }

  Sign getSign() { return s; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class NegExpr : public Expr
{

private:
  Expr *expr;                              

public:
  SignedNumber(Expr *E) : expr(E) {}

  Expr *getExpr() { return expr; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

// Assignment class represents an assignment expression in the AST
class Assignment : public Program
{
  public:
  enum AssignKind
  {
    Assign,         // =
    Minus_assign,   // -=
    Plus_assign,    // +=
    Star_assign,    // *=
    Slash_assign,   // /=
};
private:
  Final *Left;                             // Left-hand side Final (identifier)
  Expr *RightExpr;                         // Right-hand side expression
  LogicalExpr *RightLogicExpr;             // Right-hand side logical expression
  AssignKind AK;                           // Kind of assignment

public:
  Assignment(Final *L, Expr *RE, AssignKind AK, LogicalExpr *RL) : Left(L), RightExpr(RE), AK(AK), RightLogicExpr(RL) {}

  Final *getLeft() { return Left; }

  Expr *getRight() { return RightExpr; }

  LogicalExpr *getRightLogic() { return RightLogicExpr; }

  AssignKind getAssignKind() { return AK; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

// Comparison class represents a comparison expression in the AST
class Comparison : public Logic
{
  public:
  enum Operator
  {
    Equal,          // ==
    Not_equal,      // !=
    Greater,        // >
    Less,           // <
    Greater_equal,  // >=
    Less_equal,      // <=
    True,           //CHECK???
    False,
    Ident
  };
    
private:
  Expr *Left;                                // Left-hand side expression
  Expr *Right;                               // Right-hand side expression
  Operator Op;                               // Kind of assignment
  llvm::StringRef Value;                     // (if there is no op, it contains true, false or ident)

public:
  Comparison(Expr *L, Expr *R, Operator Opm ,llvm::StringRef V) : Left(L), Right(R), Op(Op), Value(V) {}

  Expr *getLeft() { return Left; }

  Expr *getRight() { return Right; }

  Operator getOperator() { return Op; }

  llvm::StringRef getValue() { return Value; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

// LogicalExpr class represents a logical expression in the AST
class LogicalExpr : public Logic
{
  public:
  enum Operator
  {
    And,          // &&
    Or,           // ||
  };

private:
  Logic *Left;                                // Left-hand side expression
  Logic *Right;                               // Right-hand side expression
  Operator Op;                                // Kind of assignment

public:
  LogicalExpr(Logic *L, Logic *R, Operator Op) : Left(L), Right(R), Op(Op) {}

  Logic *getLeft() { return Left; }

  Logic *getRight() { return Right; }

  Operator getOperator() { return Op; }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class elifStmt : public AST
{
using assignmentsVector = llvm::SmallVector<Assignment *, 8>;
using unaryVector = llvm::SmallVector<UnaryOp *, 8>;
using ifVector = llvm::SmallVector<IfStmt *, 8>;
using whileVector = llvm::SmallVector<WhileStmt *, 8>;
using forVector = llvm::SmallVector<ForStmt *, 8>;
using printVector = llvm::SmallVector<PrintStmt *, 8>;
assignmentsVector assignments;
unaryVector unarays;
ifVector ifs;
whileVector whiles;
forVector fors;
printVector prints;


private:
  Logic *Cond;

public:
  elifStmt(Logic *Cond, assignmentsVector assignments) : Cond(Cond), assignments(assignments) {}

  Logic *getCond() { return Cond; }

  assignmentsVector::const_iterator begin() { return assignments.begin(); }

  assignmentsVector::const_iterator end() { return assignments.end(); }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }

};

class IfStmt : public Program
{
using assignmentsVector = llvm::SmallVector<Assignment *, 8>;
using unaryVector = llvm::SmallVector<UnaryOp *, 8>;
using ifVector = llvm::SmallVector<IfStmt *, 8>;
using whileVector = llvm::SmallVector<WhileStmt *, 8>;
using forVector = llvm::SmallVector<ForStmt *, 8>;
using printVector = llvm::SmallVector<PrintStmt *, 8>;
using elifVector = llvm::SmallVector<elifStmt *, 8>;

assignmentsVector ifassignments;
unaryVector ifunarays;
ifVector ififs;
whileVector ifwhiles;
forVector iffors;
printVector ifprints;
assignmentsVector elseassignments;
unaryVector elseunarays;
ifVector elseifs;
whileVector elsewhiles;
forVector elsefors;
printVector elseprints;
elifVector elifStmts;


private:
  Logic *Cond;

public:
  IfStmt(Logic *Cond, llvm::SmallVector<Assignment *, 8> ifAssignments, llvm::SmallVector<Assignment *, 8> elseAssignments, llvm::SmallVector<elifStmt *, 8> elifStmts) : Cond(Cond), ifAssignments(ifAssignments), elseAssignments(elseAssignments), elifStmts(elifStmts) {}

  Logic *getCond() { return Cond; }

  assignmentsVector::const_iterator begin() { return ifAssignments.begin(); }

  assignmentsVector::const_iterator end() { return ifAssignments.end(); }

  assignmentsVector::const_iterator beginElse() { return elseAssignments.begin(); }

  assignmentsVector::const_iterator endElse() { return elseAssignments.end(); }

  elifVector::const_iterator beginElif() { return elifStmts.begin(); }

  elifVector::const_iterator endElif() { return elifStmts.end(); }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class WhileStmt : public Program
{
using assignmentsVector = llvm::SmallVector<Assignment *, 8>;
assignmentsVector assignments;

private:
  Logic *Cond;

public:
  WhileStmt(Logic *Cond, llvm::SmallVector<Assignment *, 8> assignments) : Cond(Cond), assignments(assignments) {}

  Logic *getCond() { return Cond; }

  assignmentsVector::const_iterator begin() { return assignments.begin(); }

  assignmentsVector::const_iterator end() { return assignments.end(); }

  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

#endif
