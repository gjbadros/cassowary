/* $Id$
 Cassowary Incremental Constraint Solver
 Original Smalltalk Implementation by Alan Borning
 This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
 http://www.cs.washington.edu/homes/gjb
 (C) 1998, 1999 Alan Borning and Greg Badros
 See ../LICENSE for legal details regarding this software

 ClReader.l - Scanner for constraint parsing.
 By Greg J. Badros
 */

%{
/* Get the token numbers that bison created for us
   (uses the -d option of bison) */

#include "ClReader.h"
#include "ClReader.cc.h"

/* global variable for the istream we are reading from;
   gets set by PcnParseConstraint */
istream *pxi_lexer;

/* Pass in an extra variable (ClParseData *) to cllex so that
   it can look up variable names */
#define YY_DECL int cllex(YYSTYPE *lvalp, void *YYLEX_PARAM)

/* Make lexer reader from the global variable */
#define YY_INPUT(buf,result,max_size) \
	 do { if (pxi_lexer->get(buf[0])) result = 1; \
		  else result = YY_NULL; } while (0)

%}

%option noyywrap

DIGIT [0-9]
ALPHA [A-Za-z]
ALPHANUM [A-Za-z0-9]
ID_OK_PUNC [-\[\]]
ID {ALPHA}({ALPHANUM}|{ID_OK_PUNC})*
NUMID "{"{DIGIT}+"}"
ws [ \t\n]+

%%
{ws}			/* skip whitespace */
\n|";"			{ return 0; }
">="			{ return GEQ; }
"<=" 			{ return LEQ; }
"==" 			{ return '='; }
"="|"-"|"+"|"*"|"/"|"("|")" 	{ return yytext[0]; }

{DIGIT}+("."{DIGIT}*)? |
"."{DIGIT}+		{ lvalp->num = strtod(yytext,0); return NUM; }

{ID} 			{       /* Lookup the variable name */
      ClParseData *pclpd = ((ClParseData *) YYLEX_PARAM);
      const string str(yytext);
      ClVariable *pclv = pclpd->_lookup_func(str);
      if (!pclv->isNil()) {
        lvalp->pclv = pclv;
        return VAR;
      } else {
	pxi_lexer = NULL;
	throw ExCLParseErrorBadIdentifier(str);
        return 0;
      }
   }

.     {	pxi_lexer = NULL; throw ExCLParseErrorMisc("Unrecognized character"); }
