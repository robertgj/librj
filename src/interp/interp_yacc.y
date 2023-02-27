/*
 * interp_yacc.y 
I*
 * yacc definitions for a modified version of a simple interpreter
 * shown in "A Compact Guide to Lex & Yacc" by Thomas Niemann (see
 * http://epaperpress.com/lexandyacc).
 */
                                                             %{
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>

#include "interp_ex.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE 1
#define YYINCLUDED_STDLIB_H

  /* prototypes */
  extern int yylex_destroy(void);
  extern int yyerror(const char *format, ...);
  extern int yylex(void);

  /* Functions to initialise nodeType so it can be interpreted */
  nodeType *opr(data_t oper, data_t nops, ...);
  nodeType *id(data_t i);
  nodeType *con(data_t value);
  nodeType *def(data_t value);
  nodeType *str(data_t value);
  /* Free the contents of a node */
  void freeNode(nodeType *p);

  extern int yy_flex_debug;

  data_t sym[26];                /* symbol table (variables a-z) */
  %}

%union {
  data_t iValue;                 /* integer value */
  char sIndex;                   /* symbol table index */
  char *sPtr;                    /* string */
  nodeType *nPtr;                /* node pointer */
};

%token <iValue> NIL
%token <iValue> INTEGER
%token <sIndex> VARIABLE
%token <sIndex> STRING
%token WHILE IF PRINT UPRINT FOR FREE EXIT
%token CREATE FIND INSERT REMOVE CLEAR DESTROY DEPTH SIZE 
%token MIN MAX FIRST LAST NEXT PREVIOUS UPPER LOWER CHECK WALK SORT 
%token SHOW BALANCE POP PUSH PEEK COPY RAND TIME 

%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc DEREFERENCE
%nonassoc ADDRESSOF
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list

%%

program:
body                               { return 0; } 
;

/* 
 * Given a node interpret the expressions then free the node.
 * Unfortunately, with this approach if an error is found in the input
 * there is no way to 'unwind' the tree of nodes, deallocating 
 * memory as we go.
 */
body:   body stmt                  { ex($2); freeNode($2); }
| /* NULL */
;  

/* Create a tree of nodes representing expressions. Return the nodeType* */
stmt:   ';'                        { $$ = opr(';', 2, NULL, NULL); }
| expr ';'                         { $$ = $1; }
| PRINT expr ';'                   { $$ = opr(PRINT, 1, $2); }
| UPRINT expr ';'                  { $$ = opr(UPRINT, 1, $2); }
| VARIABLE '=' expr ';'            { $$ = opr('=', 2, id($1), $3); }
| WHILE '(' expr ')' stmt          { $$ = opr(WHILE, 2, $3, $5); }
| FOR '(' stmt stmt stmt ')' stmt  { $$ = opr(FOR, 4, $3, $4, $5, $7); }
| FREE '(' expr ')'                { $$ = opr(FREE, 1, $3); }
| IF '(' expr ')' stmt %prec IFX   { $$ = opr(IF, 2, $3, $5); }
| IF '(' expr ')' stmt ELSE stmt   { $$ = opr(IF, 3, $3, $5, $7); }
| '{' stmt_list '}'                { $$ = $2; }
| EXIT ';'                         { return 0; }
| error ';'                        { yyerror("Bailing out!"); }
;

stmt_list:
stmt                               { $$ = $1; }
| stmt_list stmt                   { $$ = opr(';', 2, $1, $2); }
;

expr:
INTEGER                            { $$ = con($1); }
| VARIABLE                         { $$ = id($1); }
| STRING                           { $$ = str($1); }
| '-' expr %prec UMINUS            { $$ = opr(UMINUS, 1, $2); }
| expr '+' expr                    { $$ = opr('+', 2, $1, $3); }
| expr '-' expr                    { $$ = opr('-', 2, $1, $3); }
| expr '*' expr                    { $$ = opr('*', 2, $1, $3); }
| expr '/' expr                    { $$ = opr('/', 2, $1, $3); } 
| expr '%' expr                    { $$ = opr('%', 2, $1, $3); }
| expr '<' expr                    { $$ = opr('<', 2, $1, $3); }
| expr '>' expr                    { $$ = opr('>', 2, $1, $3); }
| expr GE expr                     { $$ = opr(GE, 2, $1, $3); }
| expr LE expr                     { $$ = opr(LE, 2, $1, $3); }
| expr NE expr                     { $$ = opr(NE, 2, $1, $3); }
| expr EQ expr                     { $$ = opr(EQ, 2, $1, $3); }
| '*' expr %prec DEREFERENCE       { $$ = opr(DEREFERENCE, 1, $2); }
| '&' expr %prec ADDRESSOF         { $$ = opr(ADDRESSOF, 1, $2); }
| '(' expr ')'                     { $$ = $2; }
| NIL                              { $$ = def(NIL); }
| SHOW                             { $$ = def(SHOW); }
| RAND '(' expr ')'                { $$ = opr(RAND, 1, $3); }
| TIME '(' expr ')'                { $$ = opr(TIME, 1, $3); }
| CREATE '(' ')'                   { $$ = opr(CREATE, 0); }
| FIND '(' expr ',' expr ')'       { $$ = opr(FIND, 2, $3, $5); }
| INSERT '(' expr ',' expr ')'     { $$ = opr(INSERT, 2, $3, $5); }
| REMOVE '(' expr ',' expr ')'     { $$ = opr(REMOVE, 2, $3, $5); }
| CLEAR '(' expr ')'               { $$ = opr(CLEAR, 1, $3); }
| DESTROY '(' expr ')'             { $$ = opr(DESTROY, 1, $3); }
| DEPTH '(' expr ')'               { $$ = opr(DEPTH, 1, $3); }
| SIZE '(' expr ')'                { $$ = opr(SIZE, 1, $3); }
| FIRST '(' expr ')'               { $$ = opr(FIRST, 1, $3); }
| LAST '(' expr ')'                { $$ = opr(LAST, 1, $3); }
| MIN '(' expr ')'                 { $$ = opr(MIN, 1, $3); }
| MAX '(' expr ')'                 { $$ = opr(MAX, 1, $3); }
| NEXT '(' expr ',' expr ')'       { $$ = opr(NEXT, 2, $3, $5); }
| PREVIOUS '(' expr ',' expr ')'   { $$ = opr(PREVIOUS, 2, $3, $5); }
| UPPER '(' expr ',' expr ')'      { $$ = opr(UPPER, 2, $3, $5); }
| LOWER '(' expr ',' expr ')'      { $$ = opr(LOWER, 2, $3, $5); }
| CHECK '(' expr ')'               { $$ = opr(CHECK, 1, $3); }
| WALK '(' expr ',' expr ')'       { $$ = opr(WALK, 2, $3, $5); }
| SORT '(' expr ')'                { $$ = opr(SORT, 1, $3); }
| BALANCE '(' expr ')'             { $$ = opr(BALANCE, 1, $3); }
| POP '(' expr ')'                 { $$ = opr(POP, 1, $3); }
| PUSH '(' expr ',' expr ')'       { $$ = opr(PUSH, 2, $3, $5); }
| PEEK '(' expr ')'                { $$ = opr(PEEK, 1, $3); }
| COPY '(' expr ',' expr ')'       { $$ = opr(COPY, 2, $3, $5); }
;

%%

/* Make node a numeric constant */
nodeType* con(data_t value) 
{
  nodeType *p;
  size_t nodeSize;

  /* allocate node */
  nodeSize = sizeof(nodeType);
  if ((p = (nodeType *)malloc(nodeSize)) == NULL) {
    yyerror("out of memory");
    return NULL;
  }

  /* copy information */
  p->type = typeCon;
  p->con.value = value;

  return p;
}

/* Make node a predefined symbol */
nodeType *def(data_t value)
{
  nodeType *p;
  size_t nodeSize;

  /* allocate node */
  nodeSize = sizeof(nodeType);
  if ((p = (nodeType *)malloc(nodeSize)) == NULL) {
    yyerror("out of memory");
    return NULL;
  }

  /* copy information */
  p->type = typeDef;
  p->def.value = value;

  return p;
}

/* Make node a variable symbol (one of a-z) */
nodeType *id(data_t i) {
  nodeType *p;
  size_t nodeSize;

  /* allocate node */
  nodeSize = sizeof(nodeType);
  if ((p = (nodeType *)malloc(nodeSize)) == NULL) {
    yyerror("out of memory");
    return NULL;
  }

  /* copy information */
  p->type = typeId;
  p->id.i = i;

  return p;
}

extern int yyleng;
extern const char * yytext;

/* Make node a string */
nodeType *str(data_t i)
{
  nodeType *p = NULL;
  size_t nodeSize = 0;

  (void)i;

  /* Sanity check */
  if ((yyleng<2) || (yytext[0] != '"') || (yytext[yyleng-1] != '"'))
    {
      yyerror("Unterminated string!");
      return NULL;
    }

  /* allocate node */
  nodeSize = sizeof(nodeType);
  if ((p = (nodeType *)malloc(nodeSize+(size_t)(yyleng-1))) == NULL)
    {
      yyerror("Out of memory");
      return NULL;
    }

  char *s = p->str.str;
  strncpy(s, yytext+1, (size_t)(yyleng-2));
  s[yyleng-2] = '\0';

  /* copy information */
  p->type = typeStr;

  return p; 
}

/* Make node a predefined operator */
nodeType *opr(data_t oper, data_t nops, ...)
{
  va_list ap;
  nodeType *p = NULL;
  size_t nodeSize = 0;

  /* allocate node */
  if (nops < 0) {
        yyerror("nops < 0");
        return NULL;
    }
  nodeSize = sizeof(nodeType)+((size_t)nops*sizeof(nodeType *));
  if ((p = (nodeType *)malloc(nodeSize)) == NULL) {
      yyerror("out of memory");
      return NULL;
    }

  /* copy information */
  p->type = typeOpr;
  p->opr.oper = oper;
  p->opr.nops = nops;
  va_start(ap, nops);
  for (data_t i = 0; i < nops; i++) {
      p->opr.op[i] = va_arg(ap, nodeType*);
    }
  va_end(ap);
  return p;
}

/* Recursively free a node after it has been interpreted. */
void freeNode(nodeType *p) 
{
  if (!p)
    return;
  
  if (p->type == typeOpr) {
    for (data_t i = 0; i < p->opr.nops; i++) {
      freeNode(p->opr.op[i]);
    }
  }

  free (p);
}

/* 
 * Exit on error. 
 * Unfortunately, doesn't dealloc. interpreter tree. For that we 
 * need a different, stack-based scheme.
 */
int yyerror(const char *format, ...) 
{
  va_list ap;

  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  fprintf(stderr, "\n");

  yylex_destroy();
  exit(-1);
}

extern void compile_time_checks(void);

int main(int argc, char *argv[]) 
{
  (void)argc;
  (void)argv;
  
#ifdef YACC_DEBUGGING
  yydebug = 1;
#endif

  compile_time_checks();

  while(yyparse());

  /* flex cleanup */
  yylex_destroy();
  
  return 0;
}
