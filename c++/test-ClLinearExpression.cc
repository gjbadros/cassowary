#include <string>
#include "Cassowary.h"
#include "ClAbstractVariable.h"
#include "ClLinearExpression.h"

int
main( char **argv, int argc )
{
  ClAbstractVariable a(CLVar,"a");
  ClLinearExpression cle(a);
  cout << cle << endl;
  ClAbstractVariable b(CLVar,"b");
  cle.addVariable(b,2);
  cout << cle << endl;
  cout << cle.times(2) << endl;
  cout << (cle.times(2).addVariable(ClAbstractVariable(CLVar,"c"),3)).times(-1) << endl;
  cle = cle + 8;
  cout << cle << endl;
  cle.changeSubject(a,b);
  cout << cle << endl;
  cle.addExpression(cle,-1);
  cout << cle << endl;
  cout << b << endl;
}
