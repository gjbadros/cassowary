#include <string>
#include "ClErrors.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"
#include "ClEditConstraint.h"
#include "ClStayConstraint.h"
#include "ClSimplexSolver.h"
#include "ClVariable.h"

int
main( char **argv, int argc )
{
  ClVariable a("a",0.0);
  ClVariable b("b",0.0);

  ClLinearExpression b_minus_a = b - a;
  ClLinearEquation a_equals_b(b_minus_a);
  ClStayConstraint stay_a(a);
  ClStayConstraint stay_b(b);
  ClEditConstraint edit_a(a);
  
  ClSimplexSolver solver;

  cerr << "Starting addConstraint-s" << endl;
  solver.addConstraint(a_equals_b);
  solver.addConstraint(stay_a);
  solver.addConstraint(stay_b);
  solver.addConstraint(edit_a);
  
  
  vector<double> rgedits;
  rgedits.push_back(1.0);
  try 
    {
    solver.resolve(rgedits);
    cerr << "a = " << a << endl;
    cerr << "b = " << b << endl;
    }
  catch (const ExCLError &error) 
    {
    cerr << "Exception " << error.description() << endl;
    exit(0);
    }
}
