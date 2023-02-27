/*
 * interp_lex.l 
I*
 * lex definitions for a modified version of a simple interpreter
 * shown in "A Compact Guide to Lex & Yacc" by Thomas Niemann (see
 * http://epaperpress.com/lexandyacc).
 */

%option noinput
%option nounput

%{
#include <stdlib.h>
#include <string.h>

#include "interp_ex.h"
#include "interp_yacc.h"

int yyerror(const char *, ...);
%}

%%

[a-z]       { 
                yylval.sIndex = *yytext - 'a';
                return VARIABLE;
            }

[0-9]+      {
                yylval.iValue = atoi(yytext);
                return INTEGER;
            }

[-()<>=+*/;{},&%] {
                return *yytext;
             }

\"[^"\n]*["\n] {
                 return STRING;
               }
                  

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"uprint"        return UPRINT;
"for"           return FOR;
"free"          return FREE;
"nil"           return NIL;
"show"          return SHOW;
"rand"          return RAND;
"time"          return TIME;
"create"        return CREATE;
"find"          return FIND;
"insert"        return INSERT;
"remove"        return REMOVE;
"clear"         return CLEAR;
"destroy"       return DESTROY;
"depth"         return DEPTH;
"size"          return SIZE;
"first"         return FIRST;
"last"          return LAST;
"min"           return MIN;
"max"           return MAX;
"next"          return NEXT;
"previous"      return PREVIOUS;
"upper"         return UPPER;
"lower"         return LOWER;
"check"         return CHECK;
"walk"          return WALK;
"sort"          return SORT;
"balance"       return BALANCE;
"pop"           return POP;
"push"          return PUSH;
"peek"          return PEEK;
"copy"          return COPY;
"exit"          return EXIT;

[ \t\n]+        ; /* ignore whitespace */

#.*             ; /* comment */
.               yyerror("Unknown character");
%%
int yywrap(void) 
{
    return 1;
}
