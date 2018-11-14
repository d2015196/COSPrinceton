#ifndef FUN_VISITOR_VISITOR_H
#define FUN_VISITOR_VISITOR_H

namespace fun {

class BaseAST;
class BinExpAST;
class CallExpAST;
class ConstrainExpAST;
class FunDeclAST;
class FunTypeAST;
class IdExpAST;
class IfExpAST;
class IntTypeAST;
class LetExpAST;
class IdExpAST;
class IntExpAST;
class ProgramAST;
class ProjExpAST;
class RefTypeAST;
class SeqExpAST;
class TupleExpAST;
class TupleTypeAST;
class UnExpAST;
class WhileExpAST;

class Visitor {
public:
  virtual ~Visitor() {}

  virtual void visit(BinExpAST *n) = 0;
  virtual void visit(CallExpAST *n) = 0;
  virtual void visit(ConstrainExpAST *n) = 0;
  virtual void visit(FunDeclAST *n) = 0;
  virtual void visit(FunTypeAST *n) = 0;
  virtual void visit(IdExpAST *n) = 0;
  virtual void visit(IfExpAST *n) = 0;
  virtual void visit(IntTypeAST *n) = 0;
  virtual void visit(LetExpAST *n) = 0;
  virtual void visit(IntExpAST *n) = 0;
  virtual void visit(ProgramAST *n) = 0;
  virtual void visit(ProjExpAST *n) = 0;
  virtual void visit(RefTypeAST *n) = 0;
  virtual void visit(SeqExpAST *n) = 0;
  virtual void visit(TupleExpAST *n) = 0;
  virtual void visit(TupleTypeAST *n) = 0;
  virtual void visit(UnExpAST *n) = 0;
  virtual void visit(WhileExpAST *n) = 0;
};

}

#endif
