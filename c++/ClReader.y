/*
 $Id$

 Cassowary Incremental Constraint Solver
 Original Smalltalk Implementation by Alan Borning
 This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
 http://www.cs.washington.edu/homes/gjb
 (C) 1998, 1999 Alan Borning and Greg Badros
 See ../LICENSE for legal details regarding this software

 ClReader.y
 Original implementation contributed by Steve Wolfman
 Subsequently largely revised by Greg J. Badros
*/


%{
  /* C Declarations */

#include "Cl.h"
#include <string>
#include <map>

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#ifdef USE_CRUMMY_LEXER
string current;  /* Global to help in debugging/error messages */
#endif

/* Get yyparse, yylex to have an extra argument (type void *) */
#define YYPARSE_PARAM cl_parse_data
#define YYLEX_PARAM cl_parse_data

%}


/* Bison Declarations */

%pure_parser

%union {
  double num;
  const ClVariable *pclv;
  ClLinearExpression *pcle;
  ClConstraint *pcn;
}

%{
int yylex(YYSTYPE *lvalp, void *YYLEX_PARAM);
void yyerror(const char *sz);
%}

%token <num> NUM
%token <pclv> VAR

%token GEQ
%token LEQ

%type <pcle> expr
%type <pcn> constraint equation inequality

%left '-' '+'
%left '*' '/'
%left NEG

%%
/* Grammar Rules */

constraint:     equation  { $$ = $1; ((ClParseData*)YYPARSE_PARAM)->_pcn = $1; }
            | inequality  { $$ = $1; ((ClParseData*)YYPARSE_PARAM)->_pcn = $1; }
;

equation:  expr '=' expr  { $$ = new ClLinearEquation(*$1, *$3);   }
;

inequality: expr GEQ expr { $$ = new ClLinearInequality(*$1, cnGEQ, *$3); }
          | expr LEQ expr { $$ = new ClLinearInequality(*$1, cnLEQ, *$3); }
;

expr:     NUM                { $$ = new ClLinearExpression($1);        }
	| VAR                { $$ = new ClLinearExpression(*$1);       }
	| expr '+' expr      { $$ = new ClLinearExpression(*$1 + *$3); }
	| expr '-' expr      { $$ = new ClLinearExpression(*$1 - *$3); }
	| expr '*' expr      { $$ = new ClLinearExpression(*$1 * *$3); }
	| expr '/' expr      { $$ = new ClLinearExpression(*$1 / *$3); }
	| '-' expr %prec NEG { $$ = new ClLinearExpression(-1 * *$2);  }
	| '(' expr ')'       { $$ = $2;                                }
;

%%

void clerror(const char *sz)
{
  throw ExCLParseErrorMisc(sz);
}

extern istream *pxi_lexer;

// xi is the stream from which to read the constraint.
// aVars is an array of variables large enough to account for
// each one that might be mentioned in a constraint
ClConstraint *PcnParseConstraint(istream &xi, const ClVarLookupFunction &lookup_func, 
	const ClStrength &strength)
{
  ClParseData cl_parse_data(xi, lookup_func);
  pxi_lexer = &xi;
  if (yyparse(&cl_parse_data) == 0) { // success
#ifndef NO_DEBUG_PARSER
    cerr << *cl_parse_data.Pcn() << endl;
#endif
    cl_parse_data.Pcn()->ChangeStrength(strength);
    return cl_parse_data.Pcn();
  }
  else { // failed
    return 0;
  }
}
