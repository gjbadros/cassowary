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

#define CLC_IMPL
#include "ClC.h"

#include "Cl.h"
#include <strstream.h>
#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#define boolean int

extern "C" {

typedef StringToVarMap *CL_VarMap;

CL_VarMap varmap = NULL;

/* Functions unique to the C interface */
void
CL_Init()
{
}

void
CL_Shutdown()
{
  /* empty */
}

CL_VarMap CL_VarMapNew()
{
  return new StringToVarMap();
}


CL_VarMap CL_SetVarMap(CL_VarMap varmap_)
{
  if (varmap_ == NULL)
    varmap_ = CL_VarMapNew();
  varmap = varmap_;
  ClVariable::SetVarMap(varmap);
  return varmap;
}

CL_VarMap CL_GetVarMap()
{
  return varmap;
}

boolean
CL_VarMapDelete(const char *sz)
{
  const string s(sz);
  StringToVarMap::iterator it = varmap->find(s);
  varmap->erase(it);
}


/* Return a new ClVariable with name and initial Value */
CLV CL_ClvNew(const char *szName, double Value, CL_SimplexSolver solver)
{
  ClVariable *pclv = new ClVariable(szName,Value);
  if (solver)
    solver->AddStay(*pclv);
#if 0
  fprintf(stderr,"Created var %s @ %p\n",szName,pclv->get_pclv());
#endif
  return pclv;
}

void CL_VariableSetPv(CLV var, void *pv)
{ var->SetPv(pv); }

void *CL_VariablePv(CLV var)
{ return var->Pv(); }

const char *
CL_VariableName(CLV var)
{ return var->Name().c_str(); }

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

/* Print the constraint object out to the given FILE * */
void 
CL_ConstraintPrint(CL_Constraint pcn, FILE *out)
{
  strstream xo;
  xo << *pcn << ends;
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
  solver->AddStay(*var,ClsMedium(),weight);
}



/* Return a clvariable with the given name, or NULL if not found */
CLV CL_ClvLookup(const char *szName)
{
  assert(varmap);
  StringToVarMap::iterator it = varmap->find(szName);
  if (it == varmap->end())
    return NULL;
  ClVariable *pclv = new ClVariable(it->second);
  return pclv;
}

/* Return the Value of clv */
double CL_ClvValue(const CLV var)
{
  return var->Value();
}

boolean
CL_ClvIsNil(const CLV var)
{
  return var->IsNil();
}


/* Return a new constraint (or NULL) from parsing the strings */
CL_Constraint CL_ParseConstraint(const char *szConstraintRule, const char *szConstraintStrength)
{
  try {
    istrstream xiLine(szConstraintRule);
    ClConstraint *pcn = PcnParseConstraint(xiLine,ClVarLookupInMap(varmap,false),
                                           ClsFromSz(szConstraintStrength));
    return pcn;
  } catch (ExCLError &e) {
    fprintf(stderr,"%s\n",e.description().c_str());
    return NULL;
  }
}

boolean CL_FIsSatisfied(ClConstraint *pcn)
{
  return pcn->FIsSatisfied();
}


/* Add a constraint to the solver; return 1 on success, 0 on failure */
boolean CL_AddConstraint(CL_SimplexSolver solver, CL_Constraint cn)
{
  try {
    return (solver->AddConstraintNoException(cn)?1:0);
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
  solver->Resolve();
}


void CL_SimplexSolverSetEditedValue(CL_SimplexSolver solver, CLV var, double n)
{
  solver->SetEditedValue(*var,n);
}

}
