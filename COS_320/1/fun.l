%{
#include "tokens.h"
#include <iostream>

void yyerror(const char *s);
int yycolumn_next = 1;
int yycolumn = 1;

 /* for nested commenting purposes */ 

 /* number of open comments*/ 
int open_count = 0; 

 /* number of close comments */ 
int close_count = 0; 

#define YY_USER_ACTION \
  yycolumn = yycolumn_next; \
  yycolumn_next += yyleng; \
  yylval.id = strndup(yytext, yyleng);

%}

%option yylineno
%option nounput
%option noyywrap

 /* nested comments feature */ 
%x IN_COMMENT

/* TODO Fill in definitions here */

%%

 /* TODO Fill in rules here (In the rules section a comment can't start from the
    beginning of a line) */

 /* Reserved keywords */ 
"fun" { 
	return FUN; 
}

"in" { 
	return IN; 
}

"let" { 
	return LET; 
}

"while" { 
	return WHILE; 
}

"do" { 
	return DO; 
}

"if" { 
	return IF; 
}

"then" { 
	return THEN; 
}

"else" { 
	return ELSE; 
}

"ref" { 
	return REF; 
}

"not" { 
	return NOT; 
}

"type" { 
	return TYPE; 
}

 /* or */ 
"||" { 
	return OR; 
}

 /* assign */
":=" { 
	return ASSIGN; 
}

 /* arrow */ 
"->" { 
	return ARROW; 
}

 /* commenting nested */

 /* count open comment signs*/ 
 
"/*" { 
	open_count = open_count + 1; 
	BEGIN IN_COMMENT; 
}

<IN_COMMENT>"/*" { 
	open_count = open_count + 1; 
}

 /* consume non commenting characters (i.e everything but open and close comment signs */ 
<IN_COMMENT>[^"/*""*/"] { 
	; 
}

 /* count close comment signs */ 
<IN_COMMENT>"*/" { 

	close_count = close_count + 1;

	/* exit IN_COMMENT state if there is equal number of open and close. */ 
	if (close_count == open_count) { 
		BEGIN INITIAL; 
	}
}

 /* invalid comment if input file reaches EOF in state <IN_COMMENT> */ 
<IN_COMMENT><<EOF>> {
	/* to avoid infinite looping set state back to INITIAL */ 
	BEGIN INITIAL;  
	yyerror("invalid comment"); 	
}
	

 /* space */ 
[ \r\t\n]+ { 
	; 
}
 /* ID - sequence of letters, digits and underscores, starting with a letter */ 
[A-Za-z][A-Za-z0-9|"_"]* { 
	
	yylval.id = strdup(yytext);
	return ID; 
}

 /* number */ 
[0-9]+ { 
	
	yylval.num = atoi(yytext); 
	return NUM; 
}

 /*valid single characters */
[";"|":"|","|"+"|"-"|"-"|*"|"!"|"<"|">"|"="|"&"|"#"|"("|")"] { 
	return yytext[0]; 
} 

 /* invalid single characters */ 

. { 
	yyerror("invalid character"); 
}

%%

void yyerror(const char *s) {
  std::cerr << yylineno << ":" << yycolumn << ": " << s << ": " << yytext
            << "\n";
}
