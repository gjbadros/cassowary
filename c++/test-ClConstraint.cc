#include <string>
#include "Cassowary.h"
#include "ClVariable.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"
#include "ClStayConstraint.h"

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
  cout << constraint << endl;
  ClLinearExpression cle2 = cle / 2 + 1;
  ClLinearInequality constraint2(cle2);
  cout << constraint2 << endl;

  cout << constraint2.expression() << endl;

  ClStayConstraint cn(w);
  cout << cn << endl;

  cerr << (cn.variable() == w? "yup" : "nope") << endl;
  cerr << (cn.variable() == a? "yup" : "nope") << endl;

  cerr << "constraint is " << constraint.isRequired() << endl;

  return 0;
}
