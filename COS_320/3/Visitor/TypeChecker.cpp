#include "Visitor/TypeChecker.h"
#include "AST/AST.h"
#include <assert.h>
#include <sstream>

using namespace fun;

// TODO Implement bodies of functions below. You can also add new functions.

// returns true if t1 < t2 based on inference rule 1, false otherwise  
bool TypeChecker::sub_equal(MyType *t1, MyType *t2) { 
  
  if(MyType::equals(t1, t2)){
        return true;
  }

  return false; 

}

// returns true if t1 < t2 for tuple types, t1 and t2  
bool TypeChecker::sub_tuple(MyType *t1, MyType *t2) { 

  int length_t1 = MyType::toTupleType(t1)->getLength(); 
  int length_t2 = MyType::toTupleType(t2)->getLength(); 

  if ((length_t1 == length_t2) || (length_t2 < length_t1)) { 

    // check if for every element tj in t1, t1j < t2j 
    for (int i = 0; i < length_t1; i++) { 
	
	  if (!TypeChecker::sub(MyType::toTupleType(t1)->getType(i), MyType::toTupleType(t2)->getType(i))) { 
		  return false; 
	  }	

    }

    return true; 

  }

  return false; 
}

// returns true if t1 < t2 where t1 and t2 are function types, returns false otherwise  
bool TypeChecker::sub_fun(MyType *t1, MyType *t2) { 
  
  MyType *param_t1 = MyType::toFunType(t1)->getParamType(); 
  MyType *param_t2 = MyType::toFunType(t2)->getParamType();  

  MyType *ret_t1 = MyType::toFunType(t1)->getRetType(); 
  MyType *ret_t2 = MyType::toFunType(t2)->getRetType(); 

  if (!TypeChecker::sub(param_t2, param_t1) || !TypeChecker::sub(ret_t1, ret_t2)) { 
	return false; 
  }

  else { 
	return true; 
  }

}

// returns true if t1 < t2, where t1 and t2 are ref types. Otherwise returns false
bool TypeChecker::sub_ref(MyType *t1, MyType *t2) { 
  
  MyType * t1_baseType = MyType::toRefType(t1)->getBaseType();
  MyType * t2_baseType = MyType::toRefType(t2)->getBaseType();   

  if (!TypeChecker::sub(t1_baseType, t2_baseType) || !TypeChecker::sub(t2_baseType, t1_baseType)) { 
	return false; 
  }

  return true;  

}

bool TypeChecker::sub(MyType *t1, MyType *t2) {

  if (TypeChecker::sub_equal(t1, t2)) { 
	return true; 
  }

  // call on given method based on types of t1 and t2
  if (t1->isTupleType() && t2->isTupleType()) { 
	return TypeChecker::sub_tuple(t1, t2); 
  }

  if (t1->isRefType() && t2->isRefType()) { 
	return TypeChecker::sub_ref(t1, t2); 
  }

  if (t1->isFunType() && t2->isFunType()) { 
	return TypeChecker::sub_fun(t1, t2); 
  }
  
  return false;   
 
}

MyType *TypeChecker::join(MyType *t1, MyType *t2) {
  return NULL;
}

MyType *TypeChecker::visit(BinExpAST *n) {

  // type check the two expressions 
  ExpAST *exp1 = n->getExp1AST();
  ExpAST *exp2 = n->getExp2AST();

  MyType *exp1v = exp1->accept(*this); 
  MyType *exp2v = exp2->accept(*this); 

  if (n->getOp() == OP_Set) {
	
	if (exp1v->isRefType()) { 
		return MyType::getUnitType(); 
	}

	else { 
		reportError(exp1->getSrcLoc(), "Setting a non-ref type");
		return MyType::getIntType();  
	}
  }

  else { 
	
	// both expressions, exp1 and exp2 should have type int 
	if (!exp1v->isIntType() || !exp2v->isIntType()) { 
		reportError(n->getSrcLoc(), "binary operations not done on a pair of integer values"); 
	}
	
	return MyType::getIntType(); 

  } 

 
}

MyType *TypeChecker::visit(CallExpAST *n) {

  /* function name */ 
  ExpAST *funExp = n->getFunExpAST();
  MyType *funV = funExp->accept(*this); 
 
  if (!funV->isFunType()) { 
	
	std::stringstream ss; 
	ss << "Called function does not have a function type"; 
	reportError(funExp->getSrcLoc(), ss.str());
	return MyType::getIntType(); 
  
  }
    
  /* function parameter */ 
  MyType *funExp_paramType = MyType::toFunType(funV)->getParamType();
  MyType *funExp_retType = MyType::toFunType(funV)->getRetType(); 
   
  /* function arguments */ 
  ExpAST *argExp = n->getArgExpAST();  
  MyType *argV = argExp->accept(*this); 

  /* check if arg < parameter of function */ 
  if (!TypeChecker::sub(argV, funExp_paramType)) { 
	reportError(funExp->getSrcLoc(), "Function call gave parameters of wrong type");
	return MyType::getIntType(); 	
  }
  
  return funExp_retType; 
 
}

MyType *TypeChecker::visit(ConstrainExpAST *n) {
	return n->getExpAST()->accept(*this); 
}

MyType *TypeChecker::visit(FunDeclAST *n) {


  // param type  
  TypeAST* funDecl_paramType = n->getParamTypeAST(); 
  MyType* funDecl_paramTypev = funDecl_paramType->getType();   
 
  // return type  
  TypeAST* funDecl_retType = n->getRetTypeAST(); 
  MyType* funDecl_retTypev = funDecl_retType->getType(); 
  

  // bind param name with its type
  ctxt.bind(n->getParamName(), funDecl_paramTypev);

  // body 
  ExpAST* funDecl_body = n->getBodyExpAST();
  MyType* funDecl_bodyv = funDecl_body->accept(*this);  
   
  if (!MyType::equals(funDecl_bodyv, funDecl_retTypev)) { 
	reportError(n->getSrcLoc(), "function " + n->getName() +  " does not return type indicated in signature");
	return MyType::getIntType(); 
  }
 
  // restore check point  
  ctxt.undoOne(); 
 
  // once have checked, return the function type 
  IdExpAST* n_name_AST = new IdExpAST(n->getSrcLoc(), n->getName());    
  return(n_name_AST->accept(*this)); 

}

MyType *TypeChecker::visit(IdExpAST *n) {

// grabs the type for that id from context 
  if (!ctxt.has(n->getName())) { 
	
	std::stringstream ss; 
	ss << "Unbound symbol '" << n->getName() << "' detected"; 
	reportError(n->getSrcLoc(), ss.str());
	return MyType::getIntType();  
  }
	
  return ctxt.get(n->getName()); 

}


MyType *TypeChecker::visit(IfExpAST *n) {
  return NULL;
}

MyType *TypeChecker::visit(IntExpAST *) {
  return MyType::getIntType();
}

MyType *TypeChecker::visit(LetExpAST *n) {
  
  ctxt.bind(n->getVarName(), n->getVarExpAST()->accept(*this)); 
  MyType *bodyV = n->getBodyExpAST()->accept(*this); 
  ctxt.undoOne(); 
  return bodyV; 

}

MyType *TypeChecker::visit(ProgramAST *n) {

  //  Bind all function declarations to their types. 
  for (auto &kv : n->getFunDeclASTs()) { 
	
	FunDeclAST *fn = kv.second; 
	// check no two functions have the same name 
	if (ctxt.has(fn->getName())) { 
		std::stringstream ss; 
		ss << "Function name " << fn->getName() << "already exists"; 
		reportError(fn->getSrcLoc(), ss.str()); 		 
	}
	
	// construct type for the function 
	MyType *paramTypev = fn->getParamTypeAST()->getType(); 
	MyType *retTypev = fn->getRetTypeAST()->getType();

 	// create function type with param and ret types 
	MyType *fnTypev = MyType::getFunType(paramTypev, retTypev); 

	// create binding
	ctxt.bind(fn->getName(), fnTypev); 
  }

// bind print int 
  ctxt.bind("printint", MyType::getFunType(MyType::getIntType(), MyType::getUnitType())); 

// check that there is a "main" function 
  if (!ctxt.has("main")) { 
	reportError(n->getSrcLoc(), "No function main");  
  }

  // now typecheck the functions 
  for (auto &kv : n->getFunDeclASTs()) { 
	
	FunDeclAST *fn = kv.second; 	
	fn->accept(*this); 	

  }
 

  return NULL;  

}

MyType *TypeChecker::visit(ProjExpAST *n) {

	int idx = n->getIndex(); 
	ExpAST *targetVtupleExp = n->getTargetTupleExpAST(); 	
	MyType *targetV = targetVtupleExp->accept(*this); 

	if (!targetV->isTupleType()) { 
		std::stringstream ss; 
		ss << "Invalid tuple type for # op";
		reportError(targetVtupleExp->getSrcLoc(), ss.str());
		return MyType::getIntType();   
	}

	if (MyType::toTupleType(targetV)->getLength() < idx + 1) { 
		std::stringstream ss; 
		ss << "Tuple size is less than " << idx + 1; 
		reportError(targetVtupleExp->getSrcLoc(), ss.str());
		return MyType::getIntType(); 
	}
	
	return MyType::toTupleType(targetV)->getType(idx); 
}

MyType *TypeChecker::visit(SeqExpAST *n) {

	n->getExp1AST()->accept(*this); 
	return n->getExp2AST()->accept(*this); 
}

MyType *TypeChecker::visit(TupleExpAST *n) {

  std::vector<MyType*>values;  

  for (auto &e : n->getExpASTs()) { 
	
	values.push_back(e->accept(*this)); 
  
  }

  return MyType::getTupleType(values); 

}

MyType *TypeChecker::visit(UnExpAST *n) {

  ExpAST* exp1 = n->getExp1AST(); 
  MyType* exp1v = exp1->accept(*this); 

  // OP_Ref
  if (n->getOp() == OP_Ref) { 
	
	return MyType::getRefType(exp1v); 
	
  }

  // OP_Get 
  else if (n->getOp() == OP_Get) { 
	
	if (!exp1v->isRefType()) { 
		reportError(exp1->getSrcLoc(), "Dereference of non-reference type"); 
	}

	return MyType::toRefType(exp1v)->getBaseType(); 
 }

  // OP_UMinus
  else if (n->getOp() == OP_UMinus) { 

	if (!exp1v->isIntType()) { 
		reportError(exp1->getSrcLoc(), "Uminus of a non-int type"); 
	}
	
	return MyType::getIntType(); 

  }

  // OP_Not
  else { 

	if (!exp1v->isIntType()) { 
		reportError(exp1->getSrcLoc(), "Not of a non-int type"); 
	}

	return MyType::getIntType(); 
	
  }



}

MyType *TypeChecker::visit(WhileExpAST *n) {	
  
  ExpAST *condExp = n->getCondExpAST(); 
  ExpAST *bodyExp = n->getBodyExpAST(); 
  MyType *condV = condExp->accept(*this); 

  if (!condV->isIntType()) { 
	std::stringstream ss; 
	ss << "Condition of while loop is not int type"; 
	reportError(condExp->getSrcLoc(), ss.str());  
  
 } 

  bodyExp->accept(*this);     

  return MyType::getUnitType(); 
}
