/* $Id$
 * Cassowary Incremental Constraint Solver
 * Original Smalltalk Implementation by Alan Borning
 * This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
 * http://www.cs.washington.edu/homes/gjb
 * (C) 1998, 1999 Alan Borning and Greg Badros
 * See ../LICENSE for legal details regarding this software
 *
 * ClC.h
 * Top-level include file for C clients
 */

#ifndef _CLC_H__
#define _CLC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define boolean int

struct ClVariable;

typedef struct ClVariable *CL_Variable;

typedef CL_Variable CLV; /* synonym, for brevity */

struct ClSimplexSolver;
typedef struct ClSimplexSolver *CL_SimplexSolver;

struct ClConstraint;
typedef struct ClConstraint *CL_Constraint;

/* Must call this before any others */
void CL_Init();

/* Must call this when done */
void CL_Shutdown();

/* Return a new ClVariable with name and initial value
   Also adds a stay for the newly-created variable in solver if
   solver is non null */
CLV CL_ClvNew(const char *szName, double value, CL_SimplexSolver solver);

void CL_VariableSetPv(CLV var, void *pv);

void *CL_VariablePv(CLV var);

/* Return a new ClSimplexSolver object */
CL_SimplexSolver CL_SimplexSolverNew();

/* Print the ClSimplexSolver object out to the given FILE * */
void CL_SimplexSolverPrint(CL_SimplexSolver solver, FILE *out);

  /* FIXGJB: do not use ClVariable -- use ClAbstractVariable, perhaps? */
typedef void (*PfnChangeClvCallback)(CLV clv, CL_SimplexSolver solver);

void CL_SimplexSolverSetChangeClvCallback(CL_SimplexSolver solver, PfnChangeClvCallback pfn);

void CL_SimplexSolverAddStrongStay(CL_SimplexSolver solver, CLV var, double weight);

/* Return a clvariable with the given name, or NULL if not found */
CLV CL_ClvLookup(const char *szName);

/* Return the value of clv */
double CL_ClvValue(const CLV clv);

boolean CL_ClvIsNil(const CLV clv);

/* Return a new constraint from parsing the strings */
CL_Constraint CL_ParseConstraint(const char *szConstraintRule, const char *szConstraintStrength);

/* Add a constraint to the solver */
int CL_AddConstraint(CL_SimplexSolver solver, CL_Constraint cn);

void CL_Solve(CL_SimplexSolver solver);

void CL_Resolve(CL_SimplexSolver solver);

void CL_SimplexSolverSetEditedValue(CL_SimplexSolver solver, CLV var, double n);

#undef boolean

#ifdef __cplusplus
}
#endif


#endif
