#include "Visitor/CodePrinter.h"
#include "AST/AST.h"
#include <assert.h>
#include <iostream>
#include <regex>

using namespace fun;

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

void CodePrinter::run(std::ostream &out) {
  visit(prg);
  std::string code = sout.str();
  // Compress more than 1 blank lines to 1 blank line
  // gcc versions lower than 4.9 do not correctly support <regex>
#if GCC_VERSION >= 40900
  code = std::regex_replace(code, std::regex("\n\\s*\n\\s*\n+"), "\n\n");
#else
  code = replaceStrUntilDone(code, " \n", "\n");
  code = replaceStrUntilDone(code, "\t\n", "\n");
  code = replaceStrUntilDone(code, "\n\n\n", "\n\n");
#endif
  out << code;
  out.flush();
}

std::string &CodePrinter::replaceStrUntilDone(
    std::string &str, const std::string &from, const std::string &to) {
  // This is very naive form of string replacement in case there is no
  // <regex> support in gcc version lower than 4.9
  bool changed = false;
  do {
    changed = false;
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
      break;
    changed = true;
    str.replace(start_pos, from.length(), to);
  } while (changed);
  return str;
}

void CodePrinter::visit(BinExpAST *n) {
  parenProlog(n);
  n->getExp1AST()->accept(*this);
  sout << " " << n->getOpStr() << " ";
  n->getExp2AST()->accept(*this);
  parenEpilog(n);
}

void CodePrinter::visit(CallExpAST *n) {
  parenProlog(n);
  n->getFunExpAST()->accept(*this);
  sout << "(";
  n->getArgExpAST()->accept(*this);
  sout << ")";
  parenEpilog(n);
}

void CodePrinter::visit(ConstrainExpAST *n) {
  parenProlog(n);
  n->getExpAST()->accept(*this);
  sout << ":";
  n->getTypeAST()->accept(*this);
  parenEpilog(n);
}

void CodePrinter::visit(FunDeclAST *n) {
  sout << "fun " << n->getName() << "(" << n->getParamName() << ":";
  n->getParamTypeAST()->accept(*this);
  sout << "):";
  n->getRetTypeAST()->accept(*this);
  sout << " =" << incindent << nl;
  n->getBodyExpAST()->accept(*this);
  sout << decindent << nl;
}

void CodePrinter::visit(FunTypeAST *n) {
  parenProlog(n);
  n->getParamTypeAST()->accept(*this);
  sout << "->";
  n->getRetTypeAST()->accept(*this);
  parenEpilog(n);
}

void CodePrinter::visit(IdExpAST *n) {
  sout << n->getName();
}

void CodePrinter::visit(IfExpAST *n) {
  parenProlog(n);
  sout << "if ";
  n->getCondExpAST()->accept(*this);
  sout << " then" << incindent << nl;
  n->getThenExpAST()->accept(*this);
  sout << decindent;

  if (n->getElseExpAST()) {
    sout << nl << "else" << incindent << nl;
    n->getElseExpAST()->accept(*this);
    sout << decindent;
  }
  sout << nl;
  parenEpilog(n);
}

void CodePrinter::visit(IntExpAST *n) {
  sout << n->getNum();
}

void CodePrinter::visit(IntTypeAST *) {
  sout << "int";
}

void CodePrinter::visit(LetExpAST *n) {
  parenProlog(n);
  sout << "let " << n->getVarName() << " = ";
  n->getVarExpAST()->accept(*this);
  sout << " in";

  // We print all consecutive 'let' expressions with the same indent level
  // ex) let a = A_EXP in
  //     let b = B_EXP in
  //       BODY_EXP
  if (n->getBodyExpAST()->getOp() != OP_Let) sout << incindent;
  sout << nl;
  n->getBodyExpAST()->accept(*this);
  if (n->getBodyExpAST()->getOp() != OP_Let) sout << decindent;
  parenEpilog(n);
}

void CodePrinter::visit(ProgramAST *n) {
  std::map<std::string, FunDeclAST*> &funDecls = n->getFunDeclASTs();
  std::map<std::string, FunDeclAST*>::iterator it;
  std::map<std::string, FunDeclAST*>::iterator itn;
  for (it = funDecls.begin(); it != funDecls.end(); ++it) {
    it->second->accept(*this);
    itn = it;
    ++itn;
    if (itn != funDecls.end()) sout << nl;
  }
}

void CodePrinter::visit(ProjExpAST *n) {
  parenProlog(n);
  sout << "#" << n->getIndex() << " ";
  n->getTargetTupleExpAST()->accept(*this);
  parenEpilog(n);
}

void CodePrinter::visit(RefTypeAST *n) {
  parenProlog(n);
  n->getBaseTypeAST()->accept(*this);
  sout << " ref";
  parenEpilog(n);
}

void CodePrinter::visit(SeqExpAST *n) {
  parenProlog(n);
  n->getExp1AST()->accept(*this);
  sout << ";" << nl;
  n->getExp2AST()->accept(*this);
  parenEpilog(n);
}

void CodePrinter::visit(TupleExpAST *n) {
  sout << "<";
  const std::vector<ExpAST*> &exps = n->getExpASTs();
  std::vector<ExpAST*>::const_iterator it;
  for (it = exps.begin(); it != exps.end(); ++it) {
    (*it)->accept(*this);
    if (it + 1 != exps.end()) sout << ", ";
  }
  sout << ">";
}

void CodePrinter::visit(TupleTypeAST *n) {
  sout << "<";
  const std::vector<TypeAST*> &types = n->getTypeASTs();
  std::vector<TypeAST*>::const_iterator it;
  for (it = types.begin(); it != types.end(); ++it) {
    (*it)->accept(*this);
    if (it + 1 != types.end()) sout << ", ";
  }
  sout << ">";
}

void CodePrinter::visit(UnExpAST *n) {
  parenProlog(n);
  if (n->getOpAssoc() == OA_UnaryLeft) {
    n->getExp1AST()->accept(*this);
    sout << n->getOpStr();
  } else if (n->getOpAssoc() == OA_UnaryRight) {
    sout << n->getOpStr();
    n->getExp1AST()->accept(*this);
  } else
    assert(false);
  parenEpilog(n);
}

void CodePrinter::visit(WhileExpAST *n) {
  parenProlog(n);
  sout << "while ";
  n->getCondExpAST()->accept(*this);
  sout << " do" << incindent << nl;
  n->getBodyExpAST()->accept(*this);
  sout << decindent << nl;
  parenEpilog(n);
}

int CodePrinter::indentIdx = -1; // invalid start state

std::ostream &CodePrinter::incindent(std::ostream &out) {
  if (indentIdx == -1)
    indentIdx = out.xalloc();
  ++out.iword(indentIdx);
  return out;
}

std::ostream &CodePrinter::decindent(std::ostream &out) {
  if (indentIdx == -1)
    indentIdx = out.xalloc();
  --out.iword(indentIdx);
  return out;
}

std::ostream &CodePrinter::nl(std::ostream &out) {
  if (indentIdx == -1)
    indentIdx = out.xalloc();
  out << "\n";
  for (int i = 0; i < out.iword(indentIdx); i++)
    out << "  ";
  return out;
}

bool CodePrinter::needParen(const BaseAST *n) const {
  if (allParen) return true;

  BaseAST *parentNode = n->getParent();
  if (!parentNode) return false; // Topmost node

  // Function call arguments are already enclosed in ()
  if (parentNode->getOp() == OP_Call && parentNode->isRightChild(n))
    return false;

  // Note: Big number has lower precedence
  // If current prec is higher than the parent, we don't need ()
  if (n->getOpPrec() < parentNode->getOpPrec()) return false;
  // If current prec is lower than the parent, we need ()
  if (n->getOpPrec() > parentNode->getOpPrec()) return true;

  // Now, the current node has the same precedence with the parent node

  // When the current node is unary op
  if (n->isUnaryOp()) {
    // If both the parent and the current node are unary ops,
    // we put () only if their associativities are different.
    // ex) When $ is left-assoc unary op and % is right-assoc unary op,
    //     we can't omit () in "(% A) $" or "% (A $)"
    //     But if $ and % are both right-assoc, we don't need () in "$% A"
    if (parentNode->isUnaryOp())
      return n->getOpAssoc() != parentNode->getOpAssoc();

    // When the parent is binary op and the current is unary op,
    // we put () based on the current node's position.
    // ex) When $ is right-assoc unary op and this unary node is a first child
    //     of parent '%' binary op node,
    //     we can't omit () in "($ A) % B"
    if (parentNode->isBinaryOp()) {
      if ((n->getOpAssoc() == OA_UnaryLeft && parentNode->isRightChild(n)) ||
          (n->getOpAssoc() == OA_UnaryRight && parentNode->isLeftChild(n)))
        return true;
      else
        return false;
    }

    return false;
  }

  // When the current node is binary op
  if (n->isBinaryOp()) {
    // When current node is binary op and the parent node is unary op,
    // we put ()
    // ex) When % is right-assoc unary op and $ is binary op,
    //     we can't omit () in %(A $ B)
    if (parentNode->isUnaryOp()) return true;

    // When both the parent is binary op and the current one are binary ops,
    // we put () based on the current node's position.
    // ex) When $ is left-assoc binary op,
    //     we can't omit () in A $ (B $ C)
    // Also, we need () in x:= (y:= c) - otherwise it can't be parsed
    if (parentNode->isBinaryOp()) {
      if ((n->getOpAssoc() == OA_BinaryLeft && parentNode->isRightChild(n)) ||
          (n->getOpAssoc() == OA_BinaryRight && parentNode->isLeftChild(n)) ||
          (n->getOpAssoc() == OA_None && parentNode->getOpAssoc() == OA_None))
        return true;
      else
        return false;
    }

    else
      return false;
  }

  return false;
}

void CodePrinter::parenProlog(const BaseAST *n) {
  if (needParen(n)) sout << "(";
}

void CodePrinter::parenEpilog(const BaseAST *n) {
  if (needParen(n)) sout << ")";
}
