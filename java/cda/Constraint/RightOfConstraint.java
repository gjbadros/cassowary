/*
 * RightOf relational constraint.  The target CC (the second) is to be 
 * right of the first (source) CC at all times, with respect to their bounding
 * boxes.
 *
 * $Id$
 *
 */

import java.awt.Graphics;
import java.awt.Color;
import java.util.Vector;
import CDA_G;

public class RightOfConstraint extends AdjacencyConstraint {

  public RightOfConstraint(ClSimplexSolver solver, ConstrComponent srcCC,
    ConstrComponent targetCC) {

    super(solver, srcCC, targetCC);
  }

  // Add constraints to solver.  This entails establishing a constraint on
  // every SelPoint in the target to be right of the rightmost SP in the src.
  public void addConstraints() {
    int a;
    SelPoint sp;
    ConstrComponent srcCC, targetCC;
    double targval;
    ClLinearInequality cli;

    if ( ccList.size() != 2 ) {
      System.out.println("RightOfConstr.addConstr: " + ccList.size() 
        + " CC's, not required 2!");
      return;
    }
    srcCC = (ConstrComponent) ccList.elementAt(0);
    targetCC = (ConstrComponent) ccList.elementAt(1);

    if ( relConstrs.size() != targetCC.selPoints.size() ) {
      // Need to create new constraints
      if ( relConstrs.size() != 0 ) {
        System.out.println("RightOfConstr.addConstr: relConstrs = " 
          + relConstrs + "; should be empty!");
        relConstrs.removeAllElements();
        relConstrs.setSize(targetCC.selPoints.size());
      }
      relConstrs.setSize(targetCC.selPoints.size());
      for ( a = 0; a < targetCC.selPoints.size(); a++ ) {
        sp = (SelPoint) targetCC.selPoints.elementAt(a);
        try {
          cli = new ClLinearInequality(sp.X(), CL.GEQ, srcCC.rightSP.X());
        } catch (ExCLInternalError e) {
          System.out.println("RightOfConstr.constructor: ExCLInternalError"
            + " on #" + a);
          return;
        }
        relConstrs.setElementAt(cli, a);
      }
    } 

    for ( a = 0; a < relConstrs.size(); a++ ) {
      cli = (ClLinearInequality) relConstrs.elementAt(a);
      try { 
        if ( cli != null ) 
          solver.addConstraint(cli);
      } catch (ExCLInternalError e) {
        System.out.println("RightOfConstr.addConstr: ExCLInternalError"
          + " adding #" + a + " = " + cli);
      } catch (ExCLRequiredFailure e) {
        System.out.println("RightOfConstr.addConstr: ExCLRequiredFailure "
          + "adding #" + a + " = " + cli);
      }
    }
    
  }
  
  // Method to convert constraint to a string
  public String toString() {
    StringBuffer sb = new StringBuffer("RightOfConstraint: ");
    ConstrComponent srcCC, targetCC;
    
    if ( ccList.size() != 2 ) {
      sb.append(" ILL-FORMED CONSTRAINT WITH " + ccList.size());
      sb.append(" INSTEAD OF 2 CC's");
    } else {
      srcCC = (ConstrComponent) ccList.elementAt(0);
      targetCC = (ConstrComponent) ccList.elementAt(1);
      sb.append("srcCC = " + srcCC);
      sb.append(", targetCC = " + targetCC);
    }
    return sb.toString();
  }

}

/*
 * $Log$
 * Revision 1.1  1998/05/09 00:11:02  gjb
 * Added
 *
 * Revision 1.2  1998/04/20 09:51:59  Michael
 * Moved draw method into base class
 *
 * Revision 1.1  1998/04/02 06:58:12  Michael
 * Initial check-in
 *
 */
