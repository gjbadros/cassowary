#include "Cl.h"

void main()
{
  ClVariable *var = new ClVariable();
  ClSimplexSolver *solver = new ClSimplexSolver();
  ClStayConstraint *stcn = new ClStayConstraint(*var,clsWeak(),1.0);

  cout << *solver;
  solver->addConstraint(*stcn);
  cout << *solver;
  solver->removeConstraint(*stcn);
  cout << *solver;
}
/*
The result is a segmentation fault when the constraint is removed.  I
don't understand why.

Anthony Beurive'" <beurive@labri.u-bordeaux.fr>
*/

