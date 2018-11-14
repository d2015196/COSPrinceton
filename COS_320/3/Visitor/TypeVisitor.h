#ifndef FUN_VISITOR_TYPEVISITOR_H
#define FUN_VISITOR_TYPEVISITOR_H

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
class MyType;
class ProgramAST;
class ProjExpAST;
class RefTypeAST;
class SeqExpAST;
class TupleExpAST;
class TupleTypeAST;
class UnExpAST;
class WhileExpAST;

class TypeVisitor {
public:
  virtual ~TypeVisitor() {}

  virtual MyType * visit(BinExpAST *n) = 0;
  virtual MyType * visit(CallExpAST *n) = 0;
  virtual MyType * visit(ConstrainExpAST *n) = 0;
  virtual MyType * visit(FunDeclAST *n) = 0;
  virtual MyType * visit(FunTypeAST *n) = 0;
  virtual MyType * visit(IdExpAST *n) = 0;
  virtual MyType * visit(IfExpAST *n) = 0;
  virtual MyType * visit(IntTypeAST *n) = 0;
  virtual MyType * visit(LetExpAST *n) = 0;
  virtual MyType * visit(IntExpAST *n) = 0;
  virtual MyType * visit(ProgramAST *n) = 0;
  virtual MyType * visit(ProjExpAST *n) = 0;
  virtual MyType * visit(RefTypeAST *n) = 0;
  virtual MyType * visit(SeqExpAST *n) = 0;
  virtual MyType * visit(TupleExpAST *n) = 0;
  virtual MyType * visit(TupleTypeAST *n) = 0;
  virtual MyType * visit(UnExpAST *n) = 0;
  virtual MyType * visit(WhileExpAST *n) = 0;
};

}

#endif
