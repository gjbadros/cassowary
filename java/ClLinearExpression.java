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

class ClLinearExpression
{

  public ClLinearExpression(double num)
    {
      my_constant = new Double(num);
      my_terms = new Hashtable();
    }

  public ClLinearExpression(ClAbstractVariable clv, double value, double constant)
    {
      my_constant = new Double(constant);
      my_terms = new Hashtable();
      my_terms.put(clv,new Double(value));
    }

  public ClLinearExpression(Double constant, Hashtable terms)
    {
      my_constant = constant;
      my_terms = terms;
    }


  public ClLinearExpression multiplyMe(double x)
    {
      my_constant = new Double(my_constant.doubleValue() * x);
      
      for (Enumeration e = my_terms.keys() ; e.hasMoreElements(); ) {
        ClVariable clv = (ClVariable) e.nextElement();
	double coeff = ((Double) my_terms.get(clv)).doubleValue();
	my_terms.put( clv, new Double((coeff * x)));
      } 
      return  this;
    }

  public Object clone()
    {
      return new ClLinearExpression(my_constant,my_terms);
    }

  public ClLinearExpression times(double x)
    {
      ClLinearExpression result = (ClLinearExpression) clone();
      return result.multiplyMe(x);
    }

  public ClLinearExpression times(ClLinearExpression expr) throws ExCLNonlinearExpression
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

  public ClLinearExpression plus(ClLinearExpression expr)
    {
      ClLinearExpression result =  this;
      result.addExpression(expr,1.0);
      return result;
    }

  public ClLinearExpression minus(ClLinearExpression expr)
    {
      ClLinearExpression result =  this;
      result.addExpression(expr,-1.0);
      return result;
    }

  public ClLinearExpression divide(double x) throws ExCLNonlinearExpression
    {
      if (ClVariable.clApprox(x,0.0))
	{
	throw new ExCLNonlinearExpression();
	}
      return times(1.0/x);
    }

  public ClLinearExpression divide(ClLinearExpression expr) throws ExCLNonlinearExpression
    {
      if (!expr.isConstant())
	{
	throw new ExCLNonlinearExpression();
	}
      return divide(expr.my_constant.doubleValue());
    }

  public ClLinearExpression divFrom(ClLinearExpression expr) throws ExCLNonlinearExpression
    {
      if (!isConstant() || ClVariable.clApprox(my_constant.doubleValue(),0.0))
	{
	throw new ExCLNonlinearExpression();
	}
      return expr.divide(my_constant.doubleValue());
    }

  public ClLinearExpression subtractFrom(ClLinearExpression expr)
    { return expr.minus( this); }

  // FIXGJB
//   public ClLinearExpression addExpression(ClLinearExpression expr, double n,
// 					  ClAbstractVariable subject, 
// 					  ClTableau solver)
//     {
//       incrementConstant(n * expr.constant());
      
//       for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
//         ClVariable clv = (ClVariable) e.nextElement();
// 	double coeff = ((Double) expr.terms().get(clv)).doubleValue();
// 	addVariable(clv,coeff*n, subject, solver);
//       }
//       return this;
//     }

  public ClLinearExpression addExpression(ClLinearExpression expr, double n)
    {
      incrementConstant(n * expr.constant());
      
      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
        ClVariable clv = (ClVariable) e.nextElement();
	double coeff = ((Double) expr.terms().get(clv)).doubleValue();
	addVariable(clv,coeff*n);
      }
      return this;
    }


  public ClLinearExpression addVariable(ClAbstractVariable v, double c)
    { // body largely duplicated below
      //#ifndef CL_NO_TRACE
      //Tracer TRACER(__FUNCTION__);
      //System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(c) + ")");
      //#endif

      Double coeff = (Double) my_terms.get(v);
      if (coeff != null) 
	{
	double new_coefficient = coeff.doubleValue() + c;
	if (ClVariable.clApprox(new_coefficient,0))
	  {
	  my_terms.remove(v);
	  }
	else 
	  {
	  my_terms.put(v,new Double(new_coefficient));
	  }
	}
      else
	{
	if (!ClVariable.clApprox(c,0.0))
	  {
	  my_terms.put(v,new Double(c));
	  }
	}
      return this;
    }

  public ClLinearExpression setVariable(ClAbstractVariable v, double c)
    { 
      //assert(c != 0.0);  
      my_terms.put(v,new Double(c)); return this;
    }

  // FIXGJB  
//   public ClLinearExpression addVariable(ClAbstractVariable v, double c,
// 					ClAbstractVariable subject, ClTableau solver)
//     { // body largely duplicated above
//       //#ifndef CL_NO_TRACE
//       // Tracer TRACER(__FUNCTION__);
//       // System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(c) + ", " + String.valueOf(subject) + ", ...)");
//       // #endif
//       Double coeff = (Double) my_terms.get(v);
//       if (coeff != null) 
// 	{
// 	double new_coefficient = coeff.doubleValue() + c;
// 	if (ClVariable.clApprox(new_coefficient,0))
// 	  {
// 	  solver.noteRemovedVariable(v,subject);
// 	  my_terms.remove(v);
// 	  }
// 	else 
// 	  {
// 	  my_terms.put(v, new Double( new_coefficient));
// 	  }
// 	}
//       else
// 	{
// 	if (!ClVariable.clApprox(c,0.0))
// 	  {
// 	  my_terms.put(v,new Double(c));
// 	  solver.noteAddedVariable(v,subject);
// 	  }
// 	}
//       return this;
//     }
  
  public ClAbstractVariable  anyVariable() throws ExCLInternalError
    {
      if (isConstant())
	{
	throw new ExCLInternalError();
	}
      return (ClAbstractVariable) my_terms.keys().nextElement();
    }

  // FIXGJB  
  //  public void substituteOut(ClAbstractVariable var, ClLinearExpression expr, 
  //			    ClAbstractVariable subject, ClTableau solver)
    {
      //#ifndef CL_NO_TRACE
      //System.err.print("* ClLinearExpression::");
      //Tracer TRACER(__FUNCTION__);
      //System.err.print("(" + String.valueOf(var) + ", " + String.valueOf(expr) + ", " + String.valueOf(subject) + ", ");
      //System.err.println("*this == " + String.valueOf(*this));
      //#endif


      // FIXGJB write this
    }
  
  public void changeSubject(ClAbstractVariable old_subject, ClAbstractVariable new_subject)
    {
      my_terms.put(old_subject,new Double(newSubject(new_subject)));
    }
  
  public double newSubject(ClAbstractVariable subject)
    {
      //#ifndef CL_NO_TRACE
      //Tracer TRACER(__FUNCTION__);
      //System.err.println("(" + String.valueOf(subject) + ")");
      //#endif
      Double coeff = (Double) my_terms.remove(subject);
      // assert(coeff != null);
      double reciprocal = 1.0 / coeff.doubleValue();
      multiplyMe(-reciprocal);
      return reciprocal;
    }

  public double coefficientFor(ClAbstractVariable var)
    { 
      Double coeff = (Double) my_terms.get(var);
      if (coeff != null)
	return coeff.doubleValue();
      else
	return 0.0;
    }

  public double constant()
    { return my_constant.doubleValue(); }

  public void set_constant(double c)
    { my_constant = new Double(c); }

  public Hashtable terms()
    { return my_terms; }

  public void incrementConstant(double c)
    { my_constant = new Double(my_constant.doubleValue() + c); }

  public boolean isConstant()
    { return my_terms.size() == 0; }

  public String toString()
    {
      // FIXGJB: write this
      return "FIXGJB";
    }

  public ClLinearExpression Plus(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.plus(e2); }

  public ClLinearExpression Minus(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.minus(e2); }

  public ClLinearExpression Times(ClLinearExpression e1, ClLinearExpression e2) 
    throws ExCLNonlinearExpression
    { return e1.times(e2); }

  public ClLinearExpression Divide(ClLinearExpression e1, ClLinearExpression e2)
    throws ExCLNonlinearExpression
    { return e1.divide(e2); }

  public boolean FEquals(ClLinearExpression e1, ClLinearExpression e2)
    { return e1 == e2; }

  private Double my_constant;
  private Hashtable my_terms;
  
}
