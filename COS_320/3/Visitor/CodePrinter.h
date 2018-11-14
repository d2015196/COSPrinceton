#ifndef FUN_VISITOR_CODEPRINTER_H
#define FUN_VISITOR_CODEPRINTER_H

#include "Visitor.h"
#include <sstream>

namespace fun {

class CodePrinter : public Visitor {
public:
  CodePrinter(ProgramAST *prg, std::string srcFileName, bool allParen=false)
    : prg(prg), srcFileName(srcFileName), allParen(allParen) {}

  virtual void visit(BinExpAST *n);
  virtual void visit(CallExpAST *n);
  virtual void visit(ConstrainExpAST *n);
  virtual void visit(FunDeclAST *n);
  virtual void visit(FunTypeAST *n);
  virtual void visit(IdExpAST *n);
  virtual void visit(IfExpAST *n);
  virtual void visit(IntExpAST *n);
  virtual void visit(IntTypeAST *);
  virtual void visit(LetExpAST *n);
  virtual void visit(ProgramAST *n);
  virtual void visit(ProjExpAST *n);
  virtual void visit(RefTypeAST *n);
  virtual void visit(SeqExpAST *n);
  virtual void visit(TupleExpAST *n);
  virtual void visit(TupleTypeAST *n);
  virtual void visit(UnExpAST *n);
  virtual void visit(WhileExpAST *n);

  void run(std::ostream &out);

private:
  // Replace 'from' by 'to' until there is no 'from' left in 'str'
  std::string &replaceStrUntilDone(std::string &str, const std::string &from,
                                   const std::string &to);

  // Increases/decreases indent level
  static std::ostream &incindent(std::ostream &out);
  static std::ostream &decindent(std::ostream &out);
  // Prints a newline followed by indents
  static std::ostream &nl(std::ostream &out);

  // Index for "indent" variable storage in stream internal array
  static int indentIdx;

  // Returns true if the current node needs to be parenthesized
  bool needParen(const BaseAST *n) const;
  void parenProlog(const BaseAST *n);
  void parenEpilog(const BaseAST *n);

  ProgramAST *prg;
  std::string srcFileName;
  bool allParen; // true if we put () around every expression
  std::stringstream sout;
};

}

#endif
