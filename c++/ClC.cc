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

/* Return a new ClSimplexSolver object */
CL_SimplexSolver CL_SimplexSolverNew()
{
  ClSimplexSolver *psolver = new ClSimplexSolver();
  return psolver;
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

/* Return a new constraint from parsing the strings */
CL_Constraint CL_ParseConstraint(const char *szConstraintRule, const char *szConstraintStrength)
{
  istrstream xiLine(szConstraintRule);
  const ClStrength *pcls = &clsRequired();
  if (strcmp("required",szConstraintStrength) == 0)
    ; /* initialized to clsRequired, above */
  else if (strcmp("strong",szConstraintStrength) == 0) { pcls = &clsStrong(); }
  else if (strcmp("medium",szConstraintStrength) == 0) { pcls = &clsMedium(); }
  else if (strcmp("weak",szConstraintStrength) == 0) { pcls = &clsWeak(); }
  else fprintf(stderr,"Error parsing strength '%s'\n",szConstraintStrength);

  return PcnParseConstraint(xiLine,ClVarLookupInMap(&mapVars,false));
}

/* Add a constraint to the solver; return 1 on success, 0 on failure */
int CL_AddConstraint(CL_SimplexSolver solver, CL_Constraint cn)
{
  return (solver->addConstraintNoException(cn)?1:0);
}

void CL_Solve(CL_SimplexSolver solver)
{
  solver->solve();
}

void CL_Resolve(CL_SimplexSolver solver)
{
  solver->resolve();
}

