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
  CLV frameWidth;
  CLV frameHeight;
  CLV x;
  CLV y;
  CL_Constraint cn;

  CL_Init();

  solver = CL_SimplexSolverNew();
  frameWidth = CL_ClvNew("frameWidth",577,solver);
  frameHeight = CL_ClvNew("frameHeight",651,solver);
  CL_SimplexSolverAddStrongStay(solver,frameWidth,10);
  CL_SimplexSolverAddStrongStay(solver,frameHeight,10);
  x = CL_ClvNew("x",0,solver);

  printf("\"x\" has Value %g\n", CL_ClvValue(CL_ClvLookup("x")));
 
  printf("x = %g, frameWidth = %g\n",CL_ClvValue(x),CL_ClvValue(frameWidth));

  cn = CL_ParseConstraint("x = frameWidth/3", "strong");
  printf("x = %g, frameWidth = %g\n",CL_ClvValue(x),CL_ClvValue(frameWidth));

  CL_AddConstraint(solver,cn);
  printf("x = %g, frameWidth = %g\n",CL_ClvValue(x),CL_ClvValue(frameWidth));

  CL_Solve(solver);
  printf("x = %g, frameWidth = %g\n",CL_ClvValue(x),CL_ClvValue(frameWidth));

  printf("\"x\" has Value %g\n", CL_ClvValue(CL_ClvLookup("x")));

  CL_SimplexSolverPrint(solver,stderr);

  CL_SimplexSolverSetEditedValue(solver,frameWidth,620);
  printf("x = %g, frameWidth = %g\n",CL_ClvValue(x),CL_ClvValue(frameWidth));

  CL_SimplexSolverPrint(solver,stderr);

  printf("x = %g, frameWidth = %g\n",CL_ClvValue(x),CL_ClvValue(frameWidth));

  CL_SimplexSolverSetEditedValue(solver,frameWidth,700);
  printf("x = %g, frameWidth = %g\n",CL_ClvValue(x),CL_ClvValue(frameWidth));

  CL_TableauPrintExternalVariables(solver,stderr);
  CL_Shutdown();
  return 0;
}
