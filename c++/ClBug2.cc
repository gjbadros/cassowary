/* $Id$

From: "Anthony Beurive'" <beurive@labri.u-bordeaux.fr> 
Subject: cassowary 
To: gjb@cs.washington.edu 
Date: Tue, 9 Mar 1999 12:42:24 +0100 (CET) 

I believe there's a bug in cassowary.  It seems to be related to the 
previous one I encountered a while ago, concerning the removal of 
constraints. 
 
The three following examples may help you to track the bug, I hope. 
 
-------------------------------------------------------------------------------- 
#include "Cl.h" 
 
void main() 
{ 
  ClSimplexSolver *solver = new ClSimplexSolver(); 
  ClVariable *var = new ClVariable(); 
  ClStayConstraint *stcn = new ClStayConstraint(*var,clsWeak(),1.0); 
 
  solver->addConstraint(*stcn); 
  cout << *solver; 
  solver->removeConstraint(*stcn); 
  cout << *solver; 
} 
-------------------------------------------------------------------------------- 
This works fine. 
 
 
Now, the factor of the stay constraint is changed. 
-------------------------------------------------------------------------------- 
#include "Cl.h" 
 
void main() 
{ 
  ClSimplexSolver *solver = new ClSimplexSolver(); 
  ClVariable *var = new ClVariable(); 
  ClStayConstraint *stcn = new ClStayConstraint(*var,clsWeak(),2.0); 
 
  solver->addConstraint(*stcn); 
  cout << *solver; 
  solver->removeConstraint(*stcn); 
  cout << *solver; 
} 
-------------------------------------------------------------------------------- 
The result is: 
test2: ClSimplexSolver.cc:1199: void ClSimplexSolver::optimize(class ClVariable): Assertion \
`pzRow != __null' failed. 
Aborted 
 
 
Now, the solver is created after the variable. 
-------------------------------------------------------------------------------- 
#include "Cl.h" 
 
void main() 
{ 
  ClVariable *var = new ClVariable(); 
  ClSimplexSolver *solver = new ClSimplexSolver(); 
  ClStayConstraint *stcn = new ClStayConstraint(*var,clsWeak(),2.0); 
 
  solver->addConstraint(*stcn); 
  cout << *solver; 
  solver->removeConstraint(*stcn); 
  cout << *solver; 
} 
-------------------------------------------------------------------------------- 
This works again. 
 
 
Can you reproduce the same results?  Maybe the cause is my c++ 
compiler (egcs-2.90.29 980515 (egcs-1.0.3 release)).  I don't know. 

*/

#include "Cl.h"

void foo1() 
{ 
  ClSimplexSolver *solver = new ClSimplexSolver(); 
  ClVariable *var = new ClVariable(); 
  ClStayConstraint *stcn = new ClStayConstraint(*var,clsWeak(),1.0); 
 
  solver->addConstraint(*stcn); 
  cout << *solver; 
  solver->removeConstraint(*stcn); 
  cout << *solver; 
} 


void foo2() 
{ 
  ClSimplexSolver *solver = new ClSimplexSolver(); 
  ClVariable *var = new ClVariable(); 
  ClStayConstraint *stcn = new ClStayConstraint(*var,clsWeak(),2.0); 
 
  solver->addConstraint(*stcn); 
  cout << *solver; 
  solver->removeConstraint(*stcn); 
  cout << *solver; 
} 


void foo3() 
{ 
  ClVariable *var = new ClVariable(); 
  ClSimplexSolver *solver = new ClSimplexSolver(); 
  ClStayConstraint *stcn = new ClStayConstraint(*var,clsWeak(),2.0); 
 
  solver->addConstraint(*stcn); 
  cout << *solver; 
  solver->removeConstraint(*stcn); 
  cout << *solver; 
} 


int main()
{
  cerr << "Test1: " << endl;
  foo1();

  cerr << "\nTest2: " << endl;
  foo2();

  cerr << "\nTest3: " << endl;
  foo3();

}