#include <string>
#include "Cassowary.h"
#include "ClStrength.h"
#include "debug.h"

int
main( char **argv, int argc )
{
  ClSymbolicWeight clsw(0.0,1.0,0.0);
  ClSymbolicWeight clsw2(2.0,0.5,0.5);
  ClSymbolicWeight clsw3(2.0,0.5,0.5);
  ClSymbolicWeight clsw4(2.0,0.4,0.5);

  ClStrength cls(String("cls"),clsw);
  cout << cls << endl;

  ClStrength cls2(String("cls2"),clsw.times(2));
  cout << cls2 << endl;

  ClStrength cls3(String("cls3"),clsw.plus(clsw2));
  cout << cls3 << endl;

  ClStrength cls4(String("cls4"),clsw.minus(clsw2));
  cout << cls4 << endl;

  ClStrength cls5(String("cls5"),clsw.divideBy(2));
  cout << cls5 << endl;

  ClStrength a(clsRequired());
  cout << "a is " << a.isRequired() << endl;

  cout << clsw << (clsw == clsw2? "==" : "!=") << clsw2 << endl;
  cout << clsw2 << (clsw2 == clsw3? "==" : "!=") << clsw3 << endl;
  cout << clsw3 << (clsw3 == clsw4? "==" : "!=") << clsw4 << endl;

  cout << clsw << (clsw < clsw2? "<" : "!<") << clsw2 << endl;
  cout << clsw2 << (clsw2 < clsw3? "<" : "!<") << clsw3 << endl;
  cout << clsw3 << (clsw3 < clsw4? "<" : "!<") << clsw4 << endl;

  cout << clsw << (clsw > clsw2? ">" : "!>") << clsw2 << endl;
  cout << clsw2 << (clsw2 > clsw3? ">" : "!>") << clsw3 << endl;
  cout << clsw3 << (clsw3 > clsw4? ">" : "!>") << clsw4 << endl;

}
