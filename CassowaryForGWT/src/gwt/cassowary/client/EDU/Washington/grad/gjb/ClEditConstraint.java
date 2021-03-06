// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// ClEditConstraint
//

package gwt.cassowary.client.EDU.Washington.grad.gjb;

public class ClEditConstraint extends ClEditOrStayConstraint {

   public ClEditConstraint(ClVariable clv, ClStrength strength, double weight) {
      super(clv, strength, weight);
   }

   public ClEditConstraint(ClVariable clv, ClStrength strength) {
      super(clv, strength);
   }

   public ClEditConstraint(ClVariable clv) {
      super(clv);
   }

   public boolean isEditConstraint() {
      return true;
   }

   public String toString() {
      return "edit" + super.toString();
   }

}
