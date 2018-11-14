#ifndef FUN_VISITOR_VALUEVISITOR_H
#define FUN_VISITOR_VALUEVISITOR_H

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
class MyValue;
class ProgramAST;
class ProjExpAST;
class RefTypeAST;
class SeqExpAST;
class TupleExpAST;
class TupleTypeAST;
class UnExpAST;
class WhileExpAST;

class ValueVisitor {
public:
  virtual ~ValueVisitor() {}

  virtual MyValue * visit(BinExpAST *n) = 0;
  virtual MyValue * visit(CallExpAST *n) = 0;
  virtual MyValue * visit(ConstrainExpAST *n) = 0;
  virtual MyValue * visit(FunDeclAST *n) = 0;
  virtual MyValue * visit(FunTypeAST *n) = 0;
  virtual MyValue * visit(IdExpAST *n) = 0;
  virtual MyValue * visit(IfExpAST *n) = 0;
  virtual MyValue * visit(IntTypeAST *n) = 0;
  virtual MyValue * visit(LetExpAST *n) = 0;
  virtual MyValue * visit(IntExpAST *n) = 0;
  virtual MyValue * visit(ProgramAST *n) = 0;
  virtual MyValue * visit(ProjExpAST *n) = 0;
  virtual MyValue * visit(RefTypeAST *n) = 0;
  virtual MyValue * visit(SeqExpAST *n) = 0;
  virtual MyValue * visit(TupleExpAST *n) = 0;
  virtual MyValue * visit(TupleTypeAST *n) = 0;
  virtual MyValue * visit(UnExpAST *n) = 0;
  virtual MyValue * visit(WhileExpAST *n) = 0;
};

}

#endif
