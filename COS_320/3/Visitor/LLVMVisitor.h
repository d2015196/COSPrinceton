#ifndef FUN_VISITOR_LLVMVISITOR_H
#define FUN_VISITOR_LLVMVISITOR_H

namespace llvm {
  class Value;
}

namespace fun {

class BaseAST;
class BinExpAST;
class CallExpAST;
class ConstrainExpAST;
class FunDeclAST;
class FunTypeAST;
class ExpAST;
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
class TypeAST;
class UnExpAST;
class WhileExpAST;

using namespace llvm;

class LLVMVisitor {
public:
  virtual ~LLVMVisitor() {}

  virtual Value *visit(BinExpAST *n) = 0;
  virtual Value *visit(CallExpAST *n) = 0;
  virtual Value *visit(ConstrainExpAST *n) = 0;
  virtual Value *visit(FunDeclAST *n) = 0;
  virtual Value *visit(FunTypeAST *n) = 0;
  virtual Value *visit(IdExpAST *n) = 0;
  virtual Value *visit(IfExpAST *n) = 0;
  virtual Value *visit(IntTypeAST *n) = 0;
  virtual Value *visit(LetExpAST *n) = 0;
  virtual Value *visit(IntExpAST *n) = 0;
  virtual Value *visit(ProgramAST *n) = 0;
  virtual Value *visit(ProjExpAST *n) = 0;
  virtual Value *visit(RefTypeAST *n) = 0;
  virtual Value *visit(SeqExpAST *n) = 0;
  virtual Value *visit(TupleExpAST *n) = 0;
  virtual Value *visit(TupleTypeAST *n) = 0;
  virtual Value *visit(UnExpAST *n) = 0;
  virtual Value *visit(WhileExpAST *n) = 0;
};

}

#endif
