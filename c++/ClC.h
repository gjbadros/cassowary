/* $Id$
 * Cassowary Incremental Constraint Solver
 * Original Smalltalk Implementation by Alan Borning
 * This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
 * http://www.cs.washington.edu/homes/gjb
 * (C) 1998, 1999 Greg J. Badros and Alan Borning
 * See ../LICENSE for legal details regarding this software
 *
 * ClC.h
 * Top-level include file for C clients
 */

#ifndef _CLC_H__
#define _CLC_H__

#if defined(HAVE_CONFIG_H) && !defined(CONFIG_H_INCLUDED) && !defined(CONFIG_INLINE_H_INCLUDED)
#include "cassowary/config-inline.h"
#define CONFIG_INLINE_H_INCLUDED
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define boolean int

struct ClVariable;

typedef struct ClVariable *CL_Variable;

typedef CL_Variable CLV; /* synonym, for brevity */

struct ClSolver;
typedef struct ClSolver *CL_Solver;

struct ClSimplexSolver;
typedef struct ClSimplexSolver *CL_SimplexSolver;

struct ClFDSolver;
typedef struct ClFDSolver *CL_FDSolver;

struct ClTableau;
typedef struct ClSimplexSolver *CL_Tableau; // ClSimplexSolver * on purpose

struct ClConstraint;
typedef struct ClConstraint *CL_Constraint;

#ifndef CLC_IMPL
struct StringToVarMap;
typedef struct StringToVarMap *CL_VarMap;


CL_VarMap CL_SetVarMap(CL_VarMap varmap_);

CL_VarMap CL_GetVarMap();

CL_VarMap CL_VarMapNew();

#endif

/* return true iff the variable did exist
   in the current global varmap, else return false */
boolean CL_VarMapDelete(const char *sz);

/* Must call this before any others */
void CL_Init();

/* Must call this when done */
void CL_Shutdown();

/* Return a new ClVariable with name and initial Value
   Also adds a stay for the newly-created variable in solver if
   solver is non null */
CLV CL_ClvNew(const char *szName, double Value, CL_SimplexSolver solver);

void CL_VariableSetPv(CLV var, void *pv);

void *CL_VariablePv(CLV var);

const char *CL_VariableName(CLV var);


/* Return a new ClFDSolver object */
CL_FDSolver CL_FDSolverNew();

/* Return a new ClSimplexSolver object */
CL_SimplexSolver CL_SimplexSolverNew();

/* Print the ClSolver object out to the given FILE * */
void CL_SolverPrint(CL_Solver solver, FILE *out);

void CL_ConstraintPrint(CL_Constraint pcn, FILE *out);

  /* FIXGJB: do not use ClVariable -- use ClAbstractVariable, perhaps? */
typedef void (*PfnChangeClvCallback)(CLV clv, CL_SimplexSolver solver);

void CL_SimplexSolverSetChangeClvCallback(CL_SimplexSolver solver, PfnChangeClvCallback pfn);

void CL_SimplexSolverAddStrongStay(CL_SimplexSolver solver, CLV var, double weight);

void CL_SimplexSolverAddStay(CL_SimplexSolver solver, CLV var, double weight);

void CL_TableauPrintExternalVariables(CL_Tableau tableau, FILE *out);

/* Return a clvariable with the given name, or NULL if not found */
CLV CL_ClvLookup(const char *szName);

CLV CL_ClvLookupTrim(const char *szNameConst);


/* Return the Value of clv */
double CL_ClvValue(const CLV clv);

boolean CL_ClvIsNil(const CLV clv);

/* Return a new constraint from parsing the strings */
CL_Constraint CL_ParseConstraint(const char *szConstraintRule, const char *szConstraintStrength);

boolean CL_FIsSatisfied(CL_Constraint pcn);

/* Add a constraint to the solver */
int CL_AddConstraint(CL_Solver solver, CL_Constraint cn);

int CL_RemoveConstraint(CL_Solver solver, CL_Constraint cn);

void CL_Solve(CL_Solver solver);

void CL_Resolve(CL_Solver solver);

void CL_SimplexSolverSetEditedValue(CL_SimplexSolver solver, CLV var, double n);

#undef boolean

#ifdef __cplusplus
}
#endif


#endif
