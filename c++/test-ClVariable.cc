#include "ClVariable.h"

int
main( char **argv, int argc )
{
  ClVariable clv1("clv1");
  ClVariable clv2("clv2",CLVar,2.4);
  cout << clv1 << endl;
  cout << clv2 << endl;
}
