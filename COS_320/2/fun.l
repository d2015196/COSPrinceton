%{
#include "AST/AST.h"
using namespace fun;
#include "fun.tab.hpp"
#include <iostream>

void yyerror(const char *s);
int yycolumn_next = 1;
int yycolumn = 1;

#define YY_USER_ACTION \
  yycolumn = yycolumn_next; \
  yycolumn_next += yyleng; \
  yylval.id = strndup(yytext, yyleng);

int comment_level = 0;

%}

%option yylineno
%option nounput
%option noyywrap

%x COMMENTS

letter      [A-Za-z]
character   [A-Za-z0-9_]
digit       [0-9]
id          {letter}{character}*
num         {digit}+

%%


"/*"              { BEGIN COMMENTS; comment_level++; }
<COMMENTS>"/*"    { comment_level++; }
<COMMENTS>"*/"    { comment_level--; if (comment_level == 0) BEGIN INITIAL; }
<COMMENTS>"\n"    yycolumn_next = 1;
<COMMENTS>.       ;
"\n"              yycolumn_next = 1;

[-!#+*&=<>();,:]  { return *yytext; }

"||"              return OR;
":="              return ASSIGN;
"->"              return ARROW;

"fun"             return FUN;
"in"              return IN;
"let"             return LET;
"while"           return WHILE;
"do"              return DO;
"if"              return IF;
"then"            return THEN;
"else"            return ELSE;
"ref"             return REF;
"not"             return NOT;
"type"            return TYPE;

{id}              { yylval.id = strndup(yytext, yyleng); return ID; }
{num}             { yylval.num = atoi(yytext); return NUM; }
[ \t\r]+          // skip whitespace
.                 yyerror("Invalid character");


%%

void yyerror(const char *s) {
  std::cerr << yylineno << ":" << yycolumn << ": " << s << ": " << yytext
            << "\n";
}