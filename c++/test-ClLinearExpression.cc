#include <string>
#include "Cassowary.h"
#include "ClLinearExpression.h"

int
main( char **argv, int argc )
{
  ClVariable a("a");
  ClLinearExpression cle(a);
  cout << cle << endl;
  ClVariable b("b");
  cle.addVariable(b,2);
  cout << cle << endl;
  cout << cle.times(2) << endl;
}
