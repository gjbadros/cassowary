#include "Cl.h"
#include <stdlib.h>


int
main( char **, int  )
{
  try 
    {
    ClSimplexSolver s;
    ClVariable x(1);
    s.addLowerBound(x,0);
    s.addUpperBound(x,10);
    } 
  catch (ExCLError &err)
    {
    cerr << err.description() << endl;
    }
  catch (...) 
    {
    cerr << "Random exception!" << endl;
    }
}
