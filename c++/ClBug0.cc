// $Id$

#include "Cl.h"

/* This bug fixed --02/15/99 gjb
   Replaced the parallel vectors for edit constraints
   (the errorPlus..., errorMinus..., prevEditConstants vectors)
   with a ClEditInfo class that is the value of the _editVarMap map.
*/

int main()
{
  ClSimplexSolver solver;

  ClVariable x("x",7);
  ClVariable y("y",8);
  ClVariable z("z",9);

  solver
    .addStay(x)
    .addStay(y)
    .addStay(z);

  try {
    solver.addEditVar(x);
    solver.addEditVar(y);
    solver.addEditVar(z);
    solver.beginEdit();
    
    solver.suggestValue(x,1);
    solver.suggestValue(z,2);
    
    solver.removeEditVar(y);

    solver.suggestValue(x,3);
    solver.suggestValue(z,4);

    solver.endEdit();
    
  } catch (ExCLError &e) {
    cerr << e.description() << endl;
  }

  cout << x << endl << y << endl << z <<endl;

}

#if 0 /* Message below */
 From: "Michael Kaufmann" <Michael.Kaufmann@ubs.com> 
 To: <noth@cs.washington.edu> 
 Subject: bugreport 
 Date: Thu, 1 Oct 1998 11:40:55 +0200 
 Message-Id: <000001bded1f$973a2060$230e1fac@itc_mk.sbcs.swissbank.com> 
 Mime-Version: 1.0 
 Content-Type: text/plain; 
       charset="iso-8859-1" 
 Content-Transfer-Encoding: 7bit 
 X-Priority: 3 (Normal) 
 X-Msmail-Priority: Normal 
 X-Mailer: Microsoft Outlook 8.5, Build 4.71.2173.0 
 Importance: Normal 
 X-Mimeole: Produced By Microsoft MimeOLE V4.72.2106.4 
  
 Dear Mr Noth, 
  
 I am currently working with the Java implementation of Cassowary and found 
 the following bug: 
  
 If I add several editConstraints, remove some of them again later and 
 perform a 'ClSimplexSolver.suggestValue()', the indices of 
 'ClConstraintAndIndex' in the variable 'cai' are sometimes wrong (see 
 ClSimplexSolver.suggestValue(ClVariable v, double x), the 3rd line). This is 
 because if you remove an element from a 'java.util.Vector', and the element 
 is somewhere in the middle of the Vector, the indices of the Vector change. 
 (see java.util.Vector.removeElementAt(int index): 
  
     public final synchronized void removeElementAt(int index) { 
       if (index >= elementCount) { 
           throw new ArrayIndexOutOfBoundsException(index + " >= " + 
                                                    elementCount); 
       } 
       else if (index < 0) { 
           throw new ArrayIndexOutOfBoundsException(index); 
       } 
       int j = elementCount - index - 1; 
       if (j > 0) { 
           System.arraycopy(elementData, index + 1, elementData, index, j); 
       } 
       elementCount--; 
       elementData[elementCount] = null; /* to let gc do its work */ 
     } 
  
  
 My workaround now is, that everytime when I remove an EditVariable from the 
 Solver, I have to remove all the EditVariables and add then the ones again, 
 that I do not want to remove. 
  
#endif