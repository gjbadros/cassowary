/* $Id$
 * Cassowary Incremental Constraint Solver
 * Original Smalltalk Implementation by Alan Borning
 * This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
 * http://www.cs.washington.edu/homes/gjb
 * (C) 1998, 1999 Alan Borning and Greg Badros
 * See ../LICENSE for legal details regarding this software
 *
 * ClCTest.h
 */

#include "ClC.h"
#include <stdio.h>

int main(int argc, char *argv[] )
{
  CL_SimplexSolver solver;
  CLV x;
  CLV y;
  CL_Constraint cn;

  CL_Init();

  solver = CL_SimplexSolverNew();
  x = CL_ClvNew("x",5,solver);
  y = CL_ClvNew("y",-2,solver);

  printf("\"x\" has value %g\n", CL_ClvValue(CL_ClvLookup("x")));
 
  printf("x = %g, y = %g\n",CL_ClvValue(x),CL_ClvValue(y));

  cn = CL_ParseConstraint("x = y", "required");
  printf("x = %g, y = %g\n",CL_ClvValue(x),CL_ClvValue(y));

  CL_AddConstraint(solver,cn);
  printf("x = %g, y = %g\n",CL_ClvValue(x),CL_ClvValue(y));

  CL_Solve(solver);
  printf("x = %g, y = %g\n",CL_ClvValue(x),CL_ClvValue(y));

  printf("\"x\" has value %g\n", CL_ClvValue(CL_ClvLookup("x")));
  
  CL_Shutdown();
  return 0;
}
