#include "Visitor/Interpreter.h"
#include "AST/AST.h"
#include <assert.h>
#include <sstream>

using namespace fun;

MyValue *Interpreter::run(int argc) {
  visit(prg);
  FunDeclAST *funDecl = prg->getFunDeclAST("main");
  ctxt.bind(funDecl->getParamName(), MyValue::getIntValue(argc));
  MyValue *retV = funDecl->getBodyExpAST()->accept(*this);
  ctxt.undoOne();
  return retV;
}

MyValue *Interpreter::visit(BinExpAST *n) {
  ExpAST *exp1 = n->getExp1AST();
  ExpAST *exp2 = n->getExp2AST();
  MyValue *v1 = exp1->accept(*this);

  // AND and OR should be handled separately because of Short-circuit
  // conjunction & disjunction.
  // AND: if e1 is false we don't evaluate e2
  // OR: if e1 is true we don't evaluate e2
  if (n->getOp() == OP_And) {
    if (!v1->isIntValue())
      reportErrorAndExit(exp1->getSrcLoc(), "Operand is not int type");
    int num1 = MyValue::toIntValue(v1)->getNum();
    if (num1 == 0)
      return MyValue::getIntValue(0);
    else {
      MyValue *v2 = exp2->accept(*this);
      if (!v2->isIntValue())
        reportErrorAndExit(exp1->getSrcLoc(), "Operand is not int type");
      int num2 = MyValue::toIntValue(v2)->getNum();
      return MyValue::getIntValue(num1 && num2);
    }
  }
  if (n->getOp() == OP_Or) {
    if (!v1->isIntValue())
      reportErrorAndExit(exp1->getSrcLoc(), "Operand is not int type");
    int num1 = MyValue::toIntValue(v1)->getNum();
    if (num1 != 0)
      return MyValue::getIntValue(1);
    else {
      MyValue *v2 = exp2->accept(*this);
      if (!v2->isIntValue())
        reportErrorAndExit(exp1->getSrcLoc(), "Operand is not int type");
      int num2 = MyValue::toIntValue(v2)->getNum();
      return MyValue::getIntValue(num1 || num2);
    }
  }

  MyValue *v2 = exp2->accept(*this);

  if (n->getOp() == OP_Set) {
    if (!v1->isRefValue()) {
      std::stringstream ss;
      ss << "Lhs of := operator is not a reference type";
      reportErrorAndExit(exp1->getSrcLoc(), ss.str());
    }

    MyRefValue *refV1 = MyValue::toRefValue(v1);
    refV1->setBaseValue(v2);
    return MyValue::getUnitValue();
  }

  // At this point, all operands should be ints
  if (!v1->isIntValue())
    reportErrorAndExit(exp1->getSrcLoc(), "Operand is not int type");
  if (!v2->isIntValue())
    reportErrorAndExit(exp1->getSrcLoc(), "Operand is not int type");

  int num1 = MyValue::toIntValue(v1)->getNum();
  int num2 = MyValue::toIntValue(v2)->getNum();
  switch (n->getOp()) {
  case OP_Mul:
    return MyValue::getIntValue(num1 * num2);
  case OP_Add:
    return MyValue::getIntValue(num1 + num2);
  case OP_Sub:
    return MyValue::getIntValue(num1 - num2);
  case OP_Equal:
    return MyValue::getIntValue(num1 == num2);
  case OP_LT:
    return MyValue::getIntValue(num1 < num2);
  default:
    assert(false);
  }
}

MyValue *Interpreter::visit(CallExpAST *n) {
  ExpAST *funExp = n->getFunExpAST();
  ExpAST *argExp = n->getArgExpAST();
  MyValue *funV = funExp->accept(*this);
  MyValue *argV = argExp->accept(*this);

  if (!funV->isFunValue()) {
    std::stringstream ss;
    ss << funV->toString() << " is not a function";
    reportErrorAndExit(funExp->getSrcLoc(), ss.str());
  }

  std::string funName = MyValue::toFunValue(funV)->getName();
  if (funName == "printint") {
    if (!argV->isIntValue())
      reportErrorAndExit(argExp->getSrcLoc(), "Argument is not int type");
    std::cout << MyValue::toIntValue(argV)->getNum() << std::endl;
    return MyValue::getUnitValue();
  }

  FunDeclAST *funDecl = prg->getFunDeclAST(funName);
  ctxt.bind(funDecl->getParamName(), argV);
  MyValue *retV = funDecl->getBodyExpAST()->accept(*this);
  ctxt.undoOne();

  return retV;
}

MyValue *Interpreter::visit(ConstrainExpAST *n) {
  return n->getExpAST()->accept(*this);
}

MyValue *Interpreter::visit(IdExpAST *n) {
  if (!ctxt.has(n->getName())) {
    std::stringstream ss;
    ss << "Unbound symbol '" << n->getName() << "' detected";
    reportErrorAndExit(n->getSrcLoc(), ss.str());
  }
  return ctxt.get(n->getName());
}

MyValue *Interpreter::visit(IfExpAST *n) {
  ExpAST *condExp = n->getCondExpAST();
  ExpAST *thenExp = n->getThenExpAST();
  ExpAST *elseExp = n->getElseExpAST();

  MyValue *condV = condExp->accept(*this);
  if (!condV->isIntValue()) {
    std::stringstream ss;
    ss << "Condition of if expression is not int type";
    reportErrorAndExit(condExp->getSrcLoc(), ss.str());
  }

  if (MyValue::toIntValue(condV)->getNum() != 0) {
    MyValue *thenV = thenExp->accept(*this);
    if (!elseExp)
      return MyValue::getUnitValue();
    return thenV;
  }

  if (!elseExp)
    return MyValue::getUnitValue();
  return elseExp->accept(*this);
}

MyValue *Interpreter::visit(IntExpAST *n) {
  return MyValue::getIntValue(n->getNum());
}

MyValue *Interpreter::visit(LetExpAST *n) {
  ctxt.bind(n->getVarName(), n->getVarExpAST()->accept(*this));
  MyValue *bodyV = n->getBodyExpAST()->accept(*this);
  ctxt.undoOne();
  return bodyV;
}

MyValue *Interpreter::visit(ProgramAST *n) {
  // Predefined function printint
  ctxt.bind("printint", MyValue::getFunValue("printint"));

  // Bind all function signatures first
  for (auto &kv : n->getFunDeclASTs()) {
    FunDeclAST *fn = kv.second;
    // Check no two functions have the same name
    if (ctxt.has(fn->getName())) {
      std::stringstream ss;
      ss << "Function name " << fn->getName() << " already exists";
      reportErrorAndExit(fn->getSrcLoc(), ss.str());
    }

    ctxt.bind(fn->getName(), MyValue::getFunValue(fn->getName()));
  }

  // main function check
  if (!ctxt.has("main"))
    reportErrorAndExit(n->getSrcLoc(), "No function main");

  // In the interpreter, we don't go into each functions - each function will
  // be evaluated as it is called

  return NULL;
}

MyValue *Interpreter::visit(ProjExpAST *n) {
  int idx = n->getIndex();
  ExpAST *targetVupleExp = n->getTargetTupleExpAST();
  MyValue *targetV = targetVupleExp->accept(*this);

  if (!targetV->isTupleValue()) {
    std::stringstream ss;
    ss << "Invalid tuple type for # op";
    reportErrorAndExit(targetVupleExp->getSrcLoc(), ss.str());
  }

  if (MyValue::toTupleValue(targetV)->getLength() < idx + 1) {
    std::stringstream ss;
    ss << "Tuple size is less than " << idx + 1;
    reportErrorAndExit(targetVupleExp->getSrcLoc(), ss.str());
  }

  return MyValue::toTupleValue(targetV)->getValue(idx);
}

MyValue *Interpreter::visit(SeqExpAST *n) {
  n->getExp1AST()->accept(*this);
  return n->getExp2AST()->accept(*this);
}

MyValue *Interpreter::visit(TupleExpAST *n) {
  std::vector<MyValue*> values;
  for (auto &e : n->getExpASTs())
    values.push_back(e->accept(*this));
  return MyValue::getTupleValue(values);
}

MyValue *Interpreter::visit(UnExpAST *n) {
  ExpAST *exp1 = n->getExp1AST();
  MyValue *v1 = exp1->accept(*this);

  if (n->getOp() == OP_Ref)
    return MyValue::getRefValue(v1);

  if (n->getOp() == OP_Get) {
    if (!v1->isRefValue()) {
      std::stringstream ss;
      ss << "Dereference of non-reference type";
      reportErrorAndExit(exp1->getSrcLoc(), ss.str());
    }
    return MyValue::toRefValue(v1)->getBaseValue();
  }

  // At this point the operand should be an int
  if (!v1->isIntValue())
    reportErrorAndExit(exp1->getSrcLoc(), "Operand is not int type");

  int num1 = MyValue::toIntValue(v1)->getNum();
  switch (n->getOp()) {
  case OP_UMinus:
    return MyValue::getIntValue(0 - num1);
  case OP_Not:
    return MyValue::getIntValue(num1 != 0 ? 0 : 1);
  default:
    assert(false);
  }
}

MyValue *Interpreter::visit(WhileExpAST *n) {
  ExpAST *condExp = n->getCondExpAST();
  ExpAST *bodyExp = n->getBodyExpAST();
  MyValue *condV = condExp->accept(*this);

  if (!condV->isIntValue()) {
    std::stringstream ss;
    ss << "Condition of while loop is not int type";
    reportErrorAndExit(condExp->getSrcLoc(), ss.str());
  }

  int condNum = MyValue::toIntValue(condV)->getNum();

  if (condNum == 0)
    return MyValue::getUnitValue();

  bodyExp->accept(*this);

  // Run the body again (until the condition is not satisfied)
  return visit(n);
}
