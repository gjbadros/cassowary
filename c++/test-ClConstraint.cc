#include <string>
#include "Cassowary.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"

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

}
