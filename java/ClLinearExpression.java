// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
//
// ClLinearExpression
//

import java.util.*;

class ClLinearExpression extends CL
{

  public ClLinearExpression(double num)
    {
      my_constant = new ClDouble(num);
      my_terms = new Hashtable();
    }

  public ClLinearExpression()
    {
      my_constant = new ClDouble(0);
      my_terms = new Hashtable();
    }


  public ClLinearExpression(ClAbstractVariable clv, double value, double constant)
    {
      my_constant = new ClDouble(constant);
      my_terms = new Hashtable();
      my_terms.put(clv,new ClDouble(value));
    }

  public ClLinearExpression(ClAbstractVariable clv, double value)
    {
      my_constant = new ClDouble(0);
      my_terms = new Hashtable();
      my_terms.put(clv,new ClDouble(value));
    }

  public ClLinearExpression(ClAbstractVariable clv)
    {
      my_constant = new ClDouble(0);
      my_terms = new Hashtable();
      my_terms.put(clv,new ClDouble(1));
    }


  // for use by the clone method
  protected ClLinearExpression(ClDouble constant, Hashtable terms)
    {
      my_constant = (ClDouble) constant.clone();
      my_terms = new Hashtable();
      // need to unalias the ClDouble-s that we clone (do a deep clone)
      for (Enumeration e = terms.keys(); e.hasMoreElements() ; ) {
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	my_terms.put(clv,((ClDouble) terms.get(clv)).clone());
      }
    }


  public ClLinearExpression multiplyMe(double x)
    {
      if (safe)
	my_constant = new ClDouble(my_constant.doubleValue() * x);
      else
	my_constant.setValue(my_constant.doubleValue() * x);
      
      for (Enumeration e = my_terms.keys() ; e.hasMoreElements(); ) {
        ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	ClDouble cld = (ClDouble) my_terms.get(clv);
	if (safe)
	  my_terms.put(clv,new ClDouble(cld.doubleValue() * x));
	else
	  cld.setValue(cld.doubleValue() * x);
      } 
      return  this;
    }

  public final Object clone()
    {
      return new ClLinearExpression(my_constant,my_terms);
    }

  public final ClLinearExpression times(double x)
    {
      return ((ClLinearExpression) clone()).multiplyMe(x);
    }

  public final ClLinearExpression times(ClLinearExpression expr) 
       throws ExCLNonlinearExpression
    {
      if (isConstant())
	{
	return expr.times(my_constant.doubleValue());
	}
      else if (!expr.isConstant())
	{
	throw new ExCLNonlinearExpression();
	}	
      return times(expr.my_constant.doubleValue());
    }

  public final ClLinearExpression plus(ClLinearExpression expr)
    {
      return ((ClLinearExpression) clone()).addExpression(expr,1.0);
    }

  public final ClLinearExpression plus(ClVariable var) 
    throws ExCLNonlinearExpression
    { 
      return ((ClLinearExpression) clone()).addVariable(var,1.0);
    }

  public final ClLinearExpression minus(ClLinearExpression expr)
    {
      return ((ClLinearExpression) clone()).addExpression(expr,-1.0);
    }

  public final ClLinearExpression minus(ClVariable var) 
       throws ExCLNonlinearExpression
    { 
      return ((ClLinearExpression) clone()).addVariable(var,-1.0);
    }


  public final ClLinearExpression divide(double x)
    throws ExCLNonlinearExpression
    {
      if (CL.approx(x,0.0))
	{
	throw new ExCLNonlinearExpression();
	}
      return times(1.0/x);
    }

  public final ClLinearExpression divide(ClLinearExpression expr)
    throws ExCLNonlinearExpression
    {
      if (!expr.isConstant())
	{
	throw new ExCLNonlinearExpression();
	}
      return divide(expr.my_constant.doubleValue());
    }

  public final ClLinearExpression divFrom(ClLinearExpression expr) 
    throws ExCLNonlinearExpression
    {
      if (!isConstant() || CL.approx(my_constant.doubleValue(),0.0))
	{
	throw new ExCLNonlinearExpression();
	}
      return expr.divide(my_constant.doubleValue());
    }

  public final ClLinearExpression subtractFrom(ClLinearExpression expr)
    { return expr.minus( this); }

  public final ClLinearExpression addExpression(ClLinearExpression expr, double n,
						ClAbstractVariable subject, 
						ClTableau solver)
    {
      incrementConstant(n * expr.constant());
      
      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
 	double coeff = ((ClDouble) expr.terms().get(clv)).doubleValue();
 	addVariable(clv,coeff*n, subject, solver);
      }
      return this;
    }

  public final ClLinearExpression addExpression(ClLinearExpression expr, double n)
    {
      incrementConstant(n * expr.constant());
      
      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
        ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	double coeff = ((ClDouble) expr.terms().get(clv)).doubleValue();
	addVariable(clv,coeff*n);
      }
      return this;
    }

  public final ClLinearExpression addExpression(ClLinearExpression expr)
    {
      return addExpression(expr,1.0);
    }

  public final ClLinearExpression addVariable(ClAbstractVariable v, double c)
    { // body largely duplicated below
      fnenterprint("addVariable:" + v + ", " + c);

      ClDouble coeff = (ClDouble) my_terms.get(v);
      if (coeff != null) {
	double new_coefficient = coeff.doubleValue() + c;
	if (CL.approx(new_coefficient,0.0)) {
	  my_terms.remove(v);
	}
	else {
	  if (safe)
	    my_terms.put(v,new ClDouble(new_coefficient));
	  else
	    coeff.setValue(new_coefficient);
	}
      } else {
	if (!CL.approx(c,0.0)) {
	  my_terms.put(v,new ClDouble(c));
	}
      }
      return this;
    }

  public final ClLinearExpression addVariable(ClAbstractVariable v)
  { return addVariable(v,1.0); }


  public final ClLinearExpression setVariable(ClAbstractVariable v, double c)
    { 
      //assert(c != 0.0);  
      ClDouble coeff = (ClDouble) my_terms.get(v);
      if (coeff != null && !safe) 
	coeff.setValue(c);
      else
	my_terms.put(v,new ClDouble(c)); 
      return this;
    }

  public final ClLinearExpression addVariable(ClAbstractVariable v, double c,
 					ClAbstractVariable subject, ClTableau solver)
     { // body largely duplicated above
       fnenterprint("addVariable:" + v + ", " + c + ", " + subject + ", ...");

       ClDouble coeff = (ClDouble) my_terms.get(v);
       if (coeff != null) {
	 double new_coefficient = coeff.doubleValue() + c;
	 if (CL.approx(new_coefficient,0.0)) {
	   solver.noteRemovedVariable(v,subject);
	   my_terms.remove(v);
	 } else { 
	   if (safe)
	     my_terms.put(v,new ClDouble(new_coefficient));
	   else
	     coeff.setValue(new_coefficient);
	 }
       } else {
	 if (!CL.approx(c,0.0)) {
	   my_terms.put(v,new ClDouble(c));
	   solver.noteAddedVariable(v,subject);
	 }
       }
       return this;
     }
  
  public final ClAbstractVariable anyVariable() throws ExCLInternalError
    {
      if (isConstant())
	{
	throw new ExCLInternalError();
	}
      return (ClAbstractVariable) my_terms.keys().nextElement();
    }

  public final void substituteOut(ClAbstractVariable var, ClLinearExpression expr, 
 			    ClAbstractVariable subject, ClTableau solver)
  {
    fnenterprint("CLE:substituteOut: " + var + ", " + expr + ", " + subject + ", ...");
    traceprint("this = " + this);

    double multiplier = ((ClDouble) my_terms.remove(var)).doubleValue();
    incrementConstant(multiplier * expr.constant());
    
    for (Enumeration e = expr.terms().keys(); e.hasMoreElements(); ) {
      ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
      double coeff = ((ClDouble) expr.terms().get(clv)).doubleValue();
      ClDouble d_old_coeff = (ClDouble) my_terms.get(clv);
      if (d_old_coeff != null) {
	double old_coeff = d_old_coeff.doubleValue();
	double newCoeff = old_coeff + multiplier * coeff;
	if (CL.approx(newCoeff,0.0)) {
	  solver.noteRemovedVariable(clv,subject);
	  my_terms.remove(clv);
	} else {
	  if (safe)
	    my_terms.put(clv,new ClDouble(newCoeff));
	  else
	    d_old_coeff.setValue(newCoeff);
	}
      } else {
	// did not have that variable already
	my_terms.put(clv,new ClDouble(multiplier * coeff));
	solver.noteAddedVariable(clv,subject);
      }
    }
    traceprint("Now this is " + this);
  }
  
  public final void changeSubject(ClAbstractVariable old_subject, ClAbstractVariable new_subject)
    {
      ClDouble cld = (ClDouble) my_terms.get(old_subject);
      if (cld != null && !safe)
	cld.setValue(newSubject(new_subject));
      else
	my_terms.put(old_subject,new ClDouble(newSubject(new_subject)));
    }
  
  public final double newSubject(ClAbstractVariable subject)
    {
      fnenterprint("newSubject:" + subject);
      ClDouble coeff = (ClDouble) my_terms.remove(subject);
      double reciprocal = 1.0 / coeff.doubleValue();
      multiplyMe(-reciprocal);
      return reciprocal;
    }

  public final double coefficientFor(ClAbstractVariable var)
    { 
      ClDouble coeff = (ClDouble) my_terms.get(var);
      if (coeff != null)
	return coeff.doubleValue();
      else
	return 0.0;
    }

  public final double constant()
    { return my_constant.doubleValue(); }

  public final void set_constant(double c)
    { 
      if (safe) 
	my_constant = new ClDouble(c);
      else 
	my_constant.setValue(c);
    }

  public final Hashtable terms()
    { return my_terms; }

  public final void incrementConstant(double c)
    { 
      if (safe)
	set_constant(my_constant.doubleValue() + c);
      else  
	my_constant.setValue(my_constant.doubleValue() + c);
    }

  public final boolean isConstant()
    { return my_terms.size() == 0; }

  public final String toString()
    {
      StringBuffer bstr = new StringBuffer();
      Enumeration e = my_terms.keys();

      if (!CL.approx(my_constant.doubleValue(),0.0) || my_terms.size() == 0) 
	{
	bstr.append(my_constant.toString());
	}
      else
	{
	if (my_terms.size() == 0)
	  {
	  return bstr.toString();
	  }
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	ClDouble coeff = (ClDouble) my_terms.get(clv);
	bstr.append(coeff.toString() + "*" + clv.toString());
	}
      for (; e.hasMoreElements(); )
	{
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	ClDouble coeff = (ClDouble) my_terms.get(clv);
	bstr.append(" + " + coeff.toString() + "*" + clv.toString());
	}
      return bstr.toString();
    }

  public final static ClLinearExpression Plus(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.plus(e2); }

  public final static ClLinearExpression Minus(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.minus(e2); }

  public final static ClLinearExpression Times(ClLinearExpression e1, ClLinearExpression e2) 
    throws ExCLNonlinearExpression
    { return e1.times(e2); }

  public final static ClLinearExpression Divide(ClLinearExpression e1, ClLinearExpression e2)
    throws ExCLNonlinearExpression
    { return e1.divide(e2); }

  public final static boolean FEquals(ClLinearExpression e1, ClLinearExpression e2)
    { return e1 == e2; }

  public final boolean safe = false;
  private ClDouble my_constant;
  private Hashtable my_terms; // from ClVariable to ClDouble

}
