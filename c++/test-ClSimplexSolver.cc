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

  ClLinearExpression b_minus_a = 0- a /* = */ + 2 * b + 1;
  cerr << b_minus_a << endl;
  ClLinearEquation a_equals_2b(b_minus_a);
  ClEditConstraint edit_a(a);
  ClEditConstraint edit_b(b);
  
  ClSimplexSolver solver;

  cerr << "Starting addConstraint-s" << endl;
  solver.addConstraint(a_equals_2b);
  solver.addStay(a);
  solver.addStay(b);
  solver.addConstraint(edit_b);
  
  
  vector<double> rgedits;
  rgedits.push_back(6.0);
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
