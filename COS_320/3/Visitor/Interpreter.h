#ifndef FUN_VISITOR_INTERPRETER_H
#define FUN_VISITOR_INTERPRETER_H

#include "ValueVisitor.h"
#include "AST/SrcLoc.h"
#include "Util/Context.h"

namespace fun {

class MyValue;

class Interpreter : public ValueVisitor {
public:
  Interpreter(ProgramAST *prg, std::string srcFileName)
    : prg(prg), srcFileName(srcFileName) {}

  virtual MyValue *visit(BinExpAST *n);
  virtual MyValue *visit(CallExpAST *n);
  virtual MyValue *visit(ConstrainExpAST *n);
  virtual MyValue *visit(FunDeclAST *n) { return NULL; }
  virtual MyValue *visit(FunTypeAST *) { return NULL; }
  virtual MyValue *visit(IdExpAST *n);
  virtual MyValue *visit(IfExpAST *n);
  virtual MyValue *visit(IntExpAST *);
  virtual MyValue *visit(IntTypeAST *) { return NULL; }
  virtual MyValue *visit(LetExpAST *n);
  virtual MyValue *visit(ProgramAST *n);
  virtual MyValue *visit(ProjExpAST *n);
  virtual MyValue *visit(RefTypeAST *) { return NULL; }
  virtual MyValue *visit(SeqExpAST *n);
  virtual MyValue *visit(TupleExpAST *n);
  virtual MyValue *visit(TupleTypeAST *) { return NULL; }
  virtual MyValue *visit(UnExpAST *n);
  virtual MyValue *visit(WhileExpAST *n);

  // Evaluate the program and return the result value
  MyValue *run(int argc);

private:
  // Report error with source file location
  void reportErrorAndExit(SrcLoc srcLoc, std::string msg) const {
    std::cerr << srcFileName << ":" << srcLoc.line << ":" << srcLoc.col
              << ": error: " << msg << "\n";
    assert(false);
  }

  ProgramAST *prg;
  std::string srcFileName;
  Context<std::string, MyValue*> ctxt;
};

}

#endif
