%{
  /* C Declarations */

#include "Cl.h"
#include <string>
#include <map>

template class map<string, ClVariable*>;
typedef map<string, ClVariable*> VarMap;

class istream;

struct yyarg {
  yyarg(istream & in, VarMap & map) : _in(in), _varMap(map) {};

  istream & _in;
  ClConstraint * _clcons;
  VarMap & _varMap;
};

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm

int yylex(YYSTYPE * lvalp, void * YYLEX_PARAM);
void yyerror(char * s);

%}


/* Bison Declarations */

%pure_parser

%union {
  double num;
  ClVariable * clvar;
  ClLinearExpression * clexpr;
  ClConstraint * clcons;
}

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

equation:       VAR '=' expr       { $$ = new ClLinearEquation(*$1, *$3);
//                                     delete $3;                              }
;

inequality:     VAR GEQ expr  { $$ = new ClLinearInequality(*$1, cnGEQ, *$3);
//                                     delete $3;                              }
              | VAR LEQ expr  { $$ = new ClLinearInequality(*$1, cnLEQ, *$3);
//                                     delete $3;                              }
;

expr:           NUM                { $$ = new ClLinearExpression($1);        }
              | VAR                { $$ = new ClLinearExpression(*$1);       }
              | expr '+' expr      { $$ = new ClLinearExpression(*$1 + *$3); 
//                                     delete $1; delete $3;                   }
              | expr '-' expr      { $$ = new ClLinearExpression(*$1 - *$3); 
//                                     delete $1; delete $3;                   }
              | expr '*' expr      { $$ = new ClLinearExpression(*$1 * *$3); 
//                                     delete $1; delete $3;                   }
              | expr '/' expr      { $$ = new ClLinearExpression(*$1 / *$3); 
//                                     delete $1; delete $3;                   }
              | '(' expr ')'       { $$ = $2;                                }
;

%%

/* Additional C Code */
#include <iostream.h>  /* for main and yyerror */
//#include <strstream.h> /* for main */

#if 0
/*
 * I'll leave this around for reference purposes.
 *

int main()
{
  string s;

  while (getline(cin, s)) {
    istrstream in(s.c_str());
    yyarg parm(in);

    if (yyparse((void*)&parm) == 0) { /* success! */
	 ClConstraint *clc = parm._clcons;

	 cout << *clc << endl;
    }
    else /* failure! */
	 cout << "Boo! Hiss!" << endl;
  }
}

 */
#endif

#include <ctype.h> /* for testing tokens */
#include <stdlib.h> /* for strtod */

/* Return 0 for EOF or a token number with a value on the stack */
int yylex(YYSTYPE * lvalp, void * YYLEX_PARAM)
{
  istream & yylexIn = ((yyarg*)YYLEX_PARAM)->_in;
  string token;
  if (yylexIn >> token) {
    if (isdigit(token[0])) { /* NUM */
	 lvalp->num = strtod(token.c_str(), 0);
	 return NUM;
    }
    else if (isalpha(token[0])) { /* VAR */
	 // Check for the variable in the varmap if it's there, don't recreate it!
	 lvalp->clvar = ((yyarg*)YYLEX_PARAM)->_varMap[token];
	 if (lvalp->clvar == 0) {
	   lvalp->clvar = new ClVariable(token);
	   ((yyarg*)YYLEX_PARAM)->_varMap[token] = lvalp->clvar;
	 }

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
  cerr << s << endl;
  exit(1);
}

ClConstraint * parseConstraint(istream & in)
{
  static VarMap map;
  yyarg arg(in, map);

  if (yyparse() == 0) // success
    return arg._clcons;
  else                // failure
    return 0;
}
