/* $Id$
 * Cassowary Incremental Constraint Solver
 * Original Smalltalk Implementation by Alan Borning
 * This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
 * http://www.cs.washington.edu/homes/gjb
 * (C) 1998, 1999 Alan Borning and Greg Badros
 * See ../LICENSE for legal details regarding this software
 *
 * ClC.c
 * C wrapper for most important Cassowary functionality
 * (Originally developed for the Amaya CCSS extensions)
 */

#include "ClC.h"

#include "Cl.h"
#include <strstream.h>
#include <stdio.h>

static StringToVarMap mapVars;


/* Functions unique to the C interface */
void
CL_Init()
{
  ClVariable::SetVarMap(&mapVars);
}

void
CL_Shutdown()
{
  /* empty */
}


/* Return a new ClVariable with name and initial value */
CLV CL_ClvNew(const char *szName, double value, CL_SimplexSolver solver)
{
  ClVariable *pclv = new ClVariable(szName,value);
  if (solver)
    solver->addStay(*pclv);
  return pclv;
}

void CL_VariableSetPv(CLV var, void *pv)
{ var->setPv(pv); }

void *CL_VariablePv(CLV var)
{ return var->Pv(); }


/* Return a new ClSimplexSolver object */
CL_SimplexSolver CL_SimplexSolverNew()
{
  ClSimplexSolver *psolver = new ClSimplexSolver();
  return psolver;
}


/* Print the ClSimplexSolver object out to the given FILE * */
void 
CL_SimplexSolverPrint(CL_SimplexSolver solver, FILE *out)
{
  strstream xo;
  xo << *solver << ends;
  fprintf(out,"%s",xo.str());
}

void 
CL_TableauPrintExternalVariables(CL_Tableau tableau, FILE *out)
{
  strstream xo;
  tableau->printExternalVariablesTo(xo);
  xo << ends;
  fprintf(out,"%s",xo.str());
}


void 
CL_SimplexSolverSetChangeClvCallback(CL_SimplexSolver solver, PfnChangeClvCallback pfn)
{
  solver->SetChangeClvCallback(pfn);
}

void 
CL_SimplexSolverAddStrongStay(CL_SimplexSolver solver, CLV var, double weight)
{
  solver->addStay(*var,clsMedium(),weight);
}



/* Return a clvariable with the given name, or NULL if not found */
CLV CL_ClvLookup(const char *szName)
{
  ClVariable *pclv = new ClVariable(mapVars[szName]);
  return pclv;
}

/* Return the value of clv */
double CL_ClvValue(const CLV var)
{
  return var->value();
}

int
CL_ClvIsNil(const CLV var)
{
  return var->isNil();
}


/* Return a new constraint from parsing the strings */
CL_Constraint CL_ParseConstraint(const char *szConstraintRule, const char *szConstraintStrength)
{
  istrstream xiLine(szConstraintRule);
  ClConstraint *pcn = PcnParseConstraint(xiLine,ClVarLookupInMap(&mapVars,false),
                                         ClsFromSz(szConstraintStrength));
  return pcn;
}

/* Add a constraint to the solver; return 1 on success, 0 on failure */
int CL_AddConstraint(CL_SimplexSolver solver, CL_Constraint cn)
{
  try {
    return (solver->addConstraintNoException(cn)?1:0);
  } catch (...) {
    return 0;
  }
}

void CL_Solve(CL_SimplexSolver solver)
{
  solver->solve();
}

void CL_Resolve(CL_SimplexSolver solver)
{
  solver->resolve();
}


void CL_SimplexSolverSetEditedValue(CL_SimplexSolver solver, CLV var, double n)
{
  solver->setEditedValue(*var,n);
}

