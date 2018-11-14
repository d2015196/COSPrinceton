#ifndef FUN_VISITOR_TYPECHECKER_H
#define FUN_VISITOR_TYPECHECKER_H

#include "TypeVisitor.h"
#include "AST/SrcLoc.h"
#include "Util/Context.h"

namespace fun {

class MyType;

class TypeChecker : public TypeVisitor {
public:
  TypeChecker(ProgramAST *prg, std::string srcFileName,
              bool abortOnError=false)
    : prg(prg), srcFileName(srcFileName), abortOnError(abortOnError) {}

  virtual MyType *visit(BinExpAST *n);
  virtual MyType *visit(CallExpAST *n);
  virtual MyType *visit(ConstrainExpAST *n);
  virtual MyType *visit(FunDeclAST *n);
  virtual MyType *visit(FunTypeAST *) { return NULL; }
  virtual MyType *visit(IdExpAST *n);
  virtual MyType *visit(IfExpAST *n);
  virtual MyType *visit(IntExpAST *);
  virtual MyType *visit(IntTypeAST *) { return NULL; }
  virtual MyType *visit(LetExpAST *n);
  virtual MyType *visit(ProgramAST *n);
  virtual MyType *visit(ProjExpAST *n);
  virtual MyType *visit(RefTypeAST *) { return NULL; }
  virtual MyType *visit(SeqExpAST *n);
  virtual MyType *visit(TupleExpAST *n);
  virtual MyType *visit(TupleTypeAST *) { return NULL; }
  virtual MyType *visit(UnExpAST *n);
  virtual MyType *visit(WhileExpAST *n);

  // Performs type checking and returns the number of total errors found
  int run() {
    numError = 0;
    ctxt.clear();
    visit(prg);
    return numError;
  }

private:
  // Returns true if t1 is a subtype of t2
  static bool sub(MyType *t1, MyType *t2);

  // Returns a least common supertype of t1 and t2.
  // If no such type exists, returns NULL.
  static MyType *join(MyType *t1, MyType *t2);

  // TODO You may add new functions or members here if you need to.


  // Report error with source file location
  void reportError(SrcLoc srcLoc, std::string msg) {
    std::cerr << srcFileName << ":" << srcLoc.line << ":" << srcLoc.col
              << ": error: " << msg << "\n";
    numError++;
    if (abortOnError)
      assert(false);
  }

  ProgramAST *prg;
  std::string srcFileName;
  bool abortOnError;
  Context<std::string, MyType*> ctxt;
  int numError;
};

}

#endif
