%{
  /* C Declarations */

#include "Cl.h"
#include <string>

class istream;

string current;  /* Global to help in debugging/error messages */

struct yyarg {
  yyarg(istream & in, ClVariable * aVars) : _in(in), _aVars(aVars) {};

  istream & _in;
  ClConstraint * _clcons;
  ClVariable * _aVars;
};

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm

%}


/* Bison Declarations */

%pure_parser

%union {
  double num;
  ClVariable * clvar;
  ClLinearExpression * clexpr;
  ClConstraint * clcons;
}

%{
int yylex(YYSTYPE * lvalp, void * YYLEX_PARAM);
void yyerror(char * s);
%}

%token <num> NUM
%token <clvar> VAR

%token GEQ
%token LEQ

%type <clexpr> expr
%type <clcons> constraint equation inequality

%left '-' '+'
%left '*' '/'
%left NEG

%%
/* Grammar Rules */

constraint:     equation           { $$ = $1;
                                     ((yyarg*)YYPARSE_PARAM)->_clcons = $1;  }
              | inequality         { $$ = $1;
                                     ((yyarg*)YYPARSE_PARAM)->_clcons = $1;  }
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
#include <iostream.h>  /* for yyerror */
#include <ctype.h> /* for testing tokens */
#include <stdlib.h> /* for strtod */

/* Return 0 for EOF or a token number with a value on the stack */
int yylex(YYSTYPE * lvalp, void * YYLEX_PARAM)
{
  istream & yylexIn = ((yyarg*)YYLEX_PARAM)->_in;
  string token;
  current = "";
  if (yylexIn >> token) {
    current += token + " ";
    if (isdigit(token[0])) { /* NUM */
	 lvalp->num = strtod(token.c_str(), 0);
	 return NUM;
    }
    else if (token[0] == 'r') { /* VAR */
	 // Pull the variable from the argument:
	 int index = atoi(token.substr(1).c_str());
	 assert(index > 0);

	 lvalp->clvar = &((yyarg*)YYLEX_PARAM)->_aVars[index - 1];
	 
	 return VAR;
    }
    else if (token == ">=") {
	 return GEQ;
    }
    else if (token == "<=") {
	 return LEQ;
    }
    else { /* OP or error! */
	 return (int)token[0]; /* Code for one char OP is ASCII code */
    }
  }
  else /* EOF */
    return 0;
}

void yyerror(char * s)
{
  cerr << s << ": " << current << endl;
  exit(1);
}

// in is the stream from which to read the constraint.
// aVars is an array of variables large enough to account for
// each one that might be mentioned in a constraint (as tXX where
// XX is some number).
ClConstraint * parseConstraint(istream & in, ClVariable * aVars)
{
  yyarg arg(in, aVars);

  if (yyparse(&arg) == 0) {// success
#ifndef NDEEPDEBUG
    cerr << *arg._clcons << endl;
#endif
    return arg._clcons;
  }
  else {               // failure
    return 0;
  }
}
