#include <string>
#include "Cassowary.h"
#include "ClLinearConstraint.h"

int
main( char **argv, int argc )
{
  ClVariable a("a");
  ClVariable b("b");
  ClVariable c("c");
  ClVariable w("w");
  ClVariable x("x");
  ClVariable y("y");

  ClLinearExpression cle = a * 3 + b * 2 + c;
  ClLinearConstraint constraint(cle,1.0);
  cout << constraint << endl;
}
