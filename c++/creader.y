/*
 $Id$

 Cassowary Incremental Constraint Solver
 Original Smalltalk Implementation by Alan Borning
 This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
 http://www.cs.washington.edu/homes/gjb
 (C) 1998 Alan Borning and Greg Badros
 See ../COPYRIGHT for legal details regarding this software

 creader.y
 Original implementation contributed by Steve Wolfman
*/


%{
  /* C Declarations */

#include "Cl.h"
#include "creader.h"
#include <string>
#include <map>

string current;  /* Global to help in debugging/error messages */

struct yyarg {
  yyarg(istream &xi, StringToVarMap &mapVars) : _xi(xi), _mapVars(mapVars) {};

  istream & _xi;
  ClConstraint * _pcn;
  StringToVarMap &_mapVars;
};

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm

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
int yylex(YYSTYPE * lvalp, void * YYLEX_PARAM);
void yyerror(const char * s);
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

constraint:     equation           { $$ = $1;
                                     ((yyarg*)YYPARSE_PARAM)->_pcn = $1;  }
              | inequality         { $$ = $1;
                                     ((yyarg*)YYPARSE_PARAM)->_pcn = $1;  }
;

equation:       expr '=' expr       { $$ = new ClLinearEquation(*$1, *$3);   }
;

inequality:     expr GEQ expr  { $$ = new ClLinearInequality(*$1, cnGEQ, *$3);}
              | expr LEQ expr  { $$ = new ClLinearInequality(*$1, cnLEQ, *$3);}
;

expr:           NUM                { $$ = new ClLinearExpression($1);        }
              | VAR                { $$ = new ClLinearExpression(*$1);       }
              | expr '+' expr      { $$ = new ClLinearExpression(*$1 + *$3); }
              | expr '-' expr      { $$ = new ClLinearExpression(*$1 - *$3); }
              | expr '*' expr      { $$ = new ClLinearExpression(*$1 * *$3); }
              | expr '/' expr      { $$ = new ClLinearExpression(*$1 / *$3); }
              | '-' expr %prec NEG { $$ = new ClLinearExpression(-1 * *$2);  }
              | '(' expr ')'       { $$ = $2;                                }
;

%%

/* Additional C Code */
#include <iostream>  /* for yyerror */
#include <ctype.h> /* for testing tokens */
#include <stdlib.h> /* for strtod */

/* Return 0 for EOF or a token number with a value on the stack */
int yylex(YYSTYPE * lvalp, void * YYLEX_PARAM)
{
  istream & yylexIn = ((yyarg*)YYLEX_PARAM)->_xi;
  string token;
  current = "";
  if (yylexIn >> token) {
    current += token + " ";
    if (isdigit(token[0])) { /* NUM */
	 lvalp->num = strtod(token.c_str(), 0);
	 return NUM;
    }
    else if (token == ">=") {
	 return GEQ;
    }
    else if (token == "<=") {
	 return LEQ;
    }
    else if (isalpha(token[0])) { /* VAR */
	 yyarg *parg = ((yyarg*)YYLEX_PARAM);
	 // Lookup the variable name
	 StringToVarMap::iterator it = parg->_mapVars.find(token);
	 if (it != parg->_mapVars.end()) {
 	     lvalp->pclv = it->second;
	     return VAR;
   	 } else {
	     string szErr = "Unrecognized identifier: '";
	     szErr += token;
	     szErr += "'";
	     yyerror(szErr.c_str());
	     return 0;
         }
    }
    else { /* OP or error! */
	 return (int)token[0]; /* Code for one char OP is ASCII code */
    }
  }
  else /* EOF */
    return 0;
}

void yyerror(const char * s)
{
  cerr << s << ": " << current << endl;
  throw s;
}

// xi is the stream from which to read the constraint.
// aVars is an array of variables large enough to account for
// each one that might be mentioned in a constraint
ClConstraint *parseConstraint(istream &xi, StringToVarMap &mapVars)
{
  yyarg arg(xi, mapVars);

  if (yyparse(&arg) == 0) {// success
#ifndef NDEEPDEBUG
    cerr << *arg._pcn << endl;
#endif
    return arg._pcn;
  }
  else {               // failure
    return 0;
  }
}
