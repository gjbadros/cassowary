#include "ClVariable.h"

int
main( char **, int )
{
  ClVariable clv1("clv1");
  ClVariable clv2("clv2",2.4);
  cout << clv1 << endl;
  cout << clv2 << endl;

  return 0;
}
