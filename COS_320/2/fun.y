%{
#include "AST/AST.h"

using namespace fun;

int yylex();
void yyerror(const char *s);
extern int yylineno;
extern int yycolumn;
extern int yyleng;


#define SRC_LOC SrcLoc(yylineno, yycolumn)
/* program */ 
 
/* ProgramAST *program = new ProgramAST(SRC_LOC); */  
  
ProgramAST *program = NULL;   
 
%}
		
%union {
  int num;
  char *id;
  
  TypeAST *tp_ast;
  ExpAST *exp_ast;
  FunDeclAST *fundecl_ast;
  ProgramAST *program_ast;
  ExpASTList *exp_list_ast; 
  TupleExpAST *format_exp_list_ast;  
  TypeASTList *tp_list_ast;
  TupleTypeAST *format_tp_list_ast;
  
  /* TODO Add more AST classes here */
}



%start program_nonterm

/* TODO Fill in declarations here: union, token, type, operator associativities, ... */

%type <tp_ast> tp 
%type <exp_ast> exp
%type <fundecl_ast> fundecl 
%type <program_ast> program_nonterm

%type <exp_list_ast> exp_list
%type <format_exp_list_ast> format_exp_list 
%type <tp_list_ast> tp_list
%type <format_tp_list_ast> format_tp_list
%token <id> ID 
%token <num> NUM  
%token OR ASSIGN FUN IN LET WHILE DO IF THEN ELSE REF NOT TYPE ARROW UMINUS CALL PROJECT

%token <<EOF>> END 
 
/*program = program_instance;*/  
/* %type <foo> bar */ 

/* precedence directives */

/* #i, REF, function call, !, unary minus */


/* let in */ 

/* %left LET IN  */
/* %nonassoc LET, IN */ 

/* %left LET IN */ 
/* let in */

%nonassoc LET IN  
%left ';'

/*do while */  
%nonassoc THEN IF DO WHILE
%nonassoc ELSE


/* if */  
%right ARROW 
%left ASSIGN 
%left ':' 
%left '&' OR 
%nonassoc NOT 
%left '=' '<' 
%left '+' '-' 
%left '*' 

/* #i, ref, function call, !, -*/

/* UNARY MINUS!! */  
%left PROJECT '(' REF '!' UMINUS CALL
%%

/* Fill in grammar rules and semantic actions here */

/* tp */ 
tp : ID {$$ = new IntTypeAST(SRC_LOC);} 
	| format_tp_list {$$ = $1;} 
	| tp ARROW tp {$$ = new FunTypeAST(SRC_LOC, $1, $3);} 
	| tp REF {$$ = new RefTypeAST(SRC_LOC, $1);}
	| '('tp')'{$$ = $2;} 
	; 

format_tp_list : '<''>' {TypeASTList* empty_list = new TypeASTList(); 
				$$ = new TupleTypeAST(SRC_LOC, empty_list);} 
		| '<' tp_list '>' {$$ = new TupleTypeAST(SRC_LOC, $2);}
		; 
tp_list : tp_list ',' tp {($1)->append($3); $$ = ($1); } 
	| tp {TypeASTList* aid_tp_list = new TypeASTList(); (aid_tp_list)->append($1); $$ = aid_tp_list; } 
	; 


/* exp */ 
exp : '(' exp ')' {$$ = $2;} 
	| ID {$$ = new IdExpAST(SRC_LOC, $1);}
	| NUM {$$ = new IntExpAST(SRC_LOC, $1);}
	| exp ';'exp {$$ = new SeqExpAST(SRC_LOC, $1, $3);} 
	/* un exp */ 
	| '-' exp %prec UMINUS {$$ = new UnExpAST(OP_UMinus, SRC_LOC, $2);}
	| NOT exp {$$ = new UnExpAST(OP_Not, SRC_LOC, $2);}
	| '!' exp {$$ = new UnExpAST(OP_Get, SRC_LOC, $2);}
	| '#' NUM exp %prec PROJECT {$$ = new ProjExpAST(SRC_LOC, $2, $3); }
	/* end un exp */ 
	/* exp bin exp */ 
	| exp '+' exp {$$ = new BinExpAST(OP_Add, SRC_LOC, $1, $3);}
	| exp '-' exp {$$ = new BinExpAST(OP_Sub, SRC_LOC, $1, $3); }
	| exp '*' exp {$$ = new BinExpAST(OP_Mul, SRC_LOC, $1, $3);}
	| exp '&' exp {$$ = new BinExpAST(OP_And, SRC_LOC, $1, $3);} 
	| exp OR exp {$$ = new BinExpAST(OP_Or, SRC_LOC, $1, $3);}
	| exp '=' exp {$$ = new BinExpAST(OP_Equal, SRC_LOC, $1, $3);} 
	| exp '<' exp {$$ = new BinExpAST(OP_LT, SRC_LOC, $1, $3);}
	| exp ASSIGN exp {$$ = new BinExpAST(OP_Set, SRC_LOC, $1, $3);}
	/* end exp bin exp */   
	| format_exp_list {$$ = $1;} 
	| exp '(' exp ')' %prec CALL {$$ = new CallExpAST(SRC_LOC, $1, $3);}
	| exp ':' tp {$$ = new ConstrainExpAST(SRC_LOC, $1, $3);}
	| IF exp THEN exp ELSE exp { $$ = new IfExpAST(SRC_LOC, $2, $4, $6);}
	| IF exp THEN exp {$$ = new IfExpAST(SRC_LOC, $2, $4, new IdExpAST(SRC_LOC, "<>"));} 
	| WHILE exp DO exp {$$ = new WhileExpAST(SRC_LOC, $2, $4);} 
	| LET ID '=' exp IN exp {$$ = new LetExpAST(SRC_LOC, $2, $4, $6);} 
	| REF exp {$$ = new UnExpAST(OP_RefType, SRC_LOC, $2); }	
	; 	
 
format_exp_list : '<''>' {ExpASTList* empty_list = new ExpASTList();
				$$ = new TupleExpAST(SRC_LOC, empty_list);}
		| '<' exp_list '>' {$$ = new TupleExpAST(SRC_LOC, $2);}
		;  
exp_list : exp_list ',' exp {($1)->append($3); $$ = ($1);}
	 | exp {ExpASTList* aid_exp_list = new ExpASTList(); (aid_exp_list)->append($1); $$ = aid_exp_list;}
	; 

/* fun declaration */ 
fundecl : FUN ID '('ID  ':' tp')'':' tp '=' exp 
	{$$ = new FunDeclAST(SRC_LOC, $2, $4, $6, $9, $11);} 
	;  
  
/* program */ 
program_nonterm : program_nonterm fundecl {($1)->append($2); $$ = $1;} 
		| fundecl {program = new ProgramAST(SRC_LOC); (program)->append($1); $$ = program;}
		; 



%%
