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
  ClVariable c("c",0.0);

  ClLinearEquation a_b( 0-a /* = */ + 2 * b + 1);
  ClLinearEquation a_c( 0-c /* = */ + b - 1);
  ClEditConstraint edit_a(a);
  ClEditConstraint edit_b(b);
  ClEditConstraint edit_c(c);
  
  ClSimplexSolver solver;

  cerr << "Starting addConstraint-s" << endl;
  solver.addConstraint(a_b);
  solver.addConstraint(a_c);
  solver.addStay(a);
  solver.addStay(b);
  solver.addStay(c);
  solver.addConstraint(edit_b);
  
  vector<double> rgedits;

  cout << a_b << endl;
  cout << a_c << endl;
  
  rgedits.push_back(6.0);
  try 
    {
    solver.resolve(rgedits);
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    }
  catch (const ExCLError &error) 
    {
    cerr << "Exception " << error.description() << endl;
    exit(0);
    }
}
