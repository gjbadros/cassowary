#include <string>
#include "ClErrors.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"
#include "ClEditConstraint.h"
#include "ClStayConstraint.h"
#include "ClSimplexSolver.h"

int
main( char **argv, int argc )
{
  ClVariable a("a");
  ClVariable b("b");
  ClVariable c("c");
  ClVariable w("w");
  ClVariable x("x");
  ClVariable y("y");

  ClLinearExpression cle = a * 3 + b * 2 + c + 5;
  ClLinearEquation constraint(cle);
  ClLinearExpression cle2 = cle / 2 + 1;
  ClLinearInequality constraint2(cle2);
  ClStayConstraint cn(w);
  ClEditConstraint cnEdit(a);
  
  ClSimplexSolver solver;
  
  solver.addConstraint(cn);
  solver.addConstraint(cnEdit);
  solver.addConstraint(constraint2);
  
  
  vector<double> rgedits;
  rgedits.push_back(1.0);
  try 
    {
    solver.resolve(rgedits);
    }
  catch (const ExCLError &error) 
    {
    //    cerr << "Exception " << error.description() << endl;
    exit(0);
    }
}
