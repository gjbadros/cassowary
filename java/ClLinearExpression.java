// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClLinearExpression
//

import java.util.*;

class ClLinearExpression extends CL
{

  public ClLinearExpression(ClAbstractVariable clv, double value, double constant)
    {
      if (CL.fGC)
	System.err.println("new ClLinearExpression");

      _constant = new ClDouble(constant);
      _terms = new Hashtable(1);
      if (clv != null)
	_terms.put(clv,new ClDouble(value));
    }

  public ClLinearExpression(double num)
    {
      this(null,0,num);
    }

  public ClLinearExpression()
    {
      this(0);
    }

  public ClLinearExpression(ClAbstractVariable clv, double value)
    {
      this(clv,value,0.0);
    }

  public ClLinearExpression(ClAbstractVariable clv)
    {
      this(clv,1,0);
    }


  // for use by the clone method
  protected ClLinearExpression(ClDouble constant, Hashtable terms)
    {
      if (CL.fGC)
	System.err.println("clone ClLinearExpression");
      _constant = (ClDouble) constant.clone();
      _terms = new Hashtable();
      // need to unalias the ClDouble-s that we clone (do a deep clone)
      for (Enumeration e = terms.keys(); e.hasMoreElements() ; ) {
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	_terms.put(clv,((ClDouble) terms.get(clv)).clone());
      }
    }


  public ClLinearExpression multiplyMe(double x)
    {
      _constant.setValue(_constant.doubleValue() * x);
      
      for (Enumeration e = _terms.keys() ; e.hasMoreElements(); ) {
        ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	ClDouble cld = (ClDouble) _terms.get(clv);
	cld.setValue(cld.doubleValue() * x);
      } 
      return  this;
    }

  public final Object clone()
    {
      return new ClLinearExpression(_constant,_terms);
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
	return expr.times(_constant.doubleValue());
	}
      else if (!expr.isConstant())
	{
	throw new ExCLNonlinearExpression();
	}	
      return times(expr._constant.doubleValue());
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
      return divide(expr._constant.doubleValue());
    }

  public final ClLinearExpression divFrom(ClLinearExpression expr) 
    throws ExCLNonlinearExpression
    {
      if (!isConstant() || CL.approx(_constant.doubleValue(),0.0))
	{
	throw new ExCLNonlinearExpression();
	}
      return expr.divide(_constant.doubleValue());
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
      if (fTraceOn) fnenterprint("addVariable:" + v + ", " + c);

      ClDouble coeff = (ClDouble) _terms.get(v);
      if (coeff != null) {
	double new_coefficient = coeff.doubleValue() + c;
	if (CL.approx(new_coefficient,0.0)) {
	  _terms.remove(v);
	}
	else {
	  coeff.setValue(new_coefficient);
	}
      } else {
	if (!CL.approx(c,0.0)) {
	  _terms.put(v,new ClDouble(c));
	}
      }
      return this;
    }

  public final ClLinearExpression addVariable(ClAbstractVariable v)
  { return addVariable(v,1.0); }


  public final ClLinearExpression setVariable(ClAbstractVariable v, double c)
    { 
      //assert(c != 0.0);  
      ClDouble coeff = (ClDouble) _terms.get(v);
      if (coeff != null) 
	coeff.setValue(c);
      else
	_terms.put(v,new ClDouble(c)); 
      return this;
    }

  public final ClLinearExpression addVariable(ClAbstractVariable v, double c,
 					ClAbstractVariable subject, ClTableau solver)
     { // body largely duplicated above
       if (fTraceOn) fnenterprint("addVariable:" + v + ", " + c + ", " + subject + ", ...");

       ClDouble coeff = (ClDouble) _terms.get(v);
       if (coeff != null) {
	 double new_coefficient = coeff.doubleValue() + c;
	 if (CL.approx(new_coefficient,0.0)) {
	   solver.noteRemovedVariable(v,subject);
	   _terms.remove(v);
	 } else { 
	   coeff.setValue(new_coefficient);
	 }
       } else {
	 if (!CL.approx(c,0.0)) {
	   _terms.put(v,new ClDouble(c));
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
      return (ClAbstractVariable) _terms.keys().nextElement();
    }

  public final void substituteOut(ClAbstractVariable var, ClLinearExpression expr, 
 			    ClAbstractVariable subject, ClTableau solver)
  {
    if (fTraceOn) fnenterprint("CLE:substituteOut: " + var + ", " + expr + ", " + subject + ", ...");
    if (fTraceOn) traceprint("this = " + this);

    double multiplier = ((ClDouble) _terms.remove(var)).doubleValue();
    incrementConstant(multiplier * expr.constant());
    
    for (Enumeration e = expr.terms().keys(); e.hasMoreElements(); ) {
      ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
      double coeff = ((ClDouble) expr.terms().get(clv)).doubleValue();
      ClDouble d_old_coeff = (ClDouble) _terms.get(clv);
      if (d_old_coeff != null) {
	double old_coeff = d_old_coeff.doubleValue();
	double newCoeff = old_coeff + multiplier * coeff;
	if (CL.approx(newCoeff,0.0)) {
	  solver.noteRemovedVariable(clv,subject);
	  _terms.remove(clv);
	} else {
	  d_old_coeff.setValue(newCoeff);
	}
      } else {
	// did not have that variable already
	_terms.put(clv,new ClDouble(multiplier * coeff));
	solver.noteAddedVariable(clv,subject);
      }
    }
    if (fTraceOn) traceprint("Now this is " + this);
  }
  
  public final void changeSubject(ClAbstractVariable old_subject, ClAbstractVariable new_subject)
    {
      ClDouble cld = (ClDouble) _terms.get(old_subject);
      if (cld != null)
	cld.setValue(newSubject(new_subject));
      else
	_terms.put(old_subject,new ClDouble(newSubject(new_subject)));
    }
  
  public final double newSubject(ClAbstractVariable subject)
    {
      if (fTraceOn) fnenterprint("newSubject:" + subject);
      ClDouble coeff = (ClDouble) _terms.remove(subject);
      double reciprocal = 1.0 / coeff.doubleValue();
      multiplyMe(-reciprocal);
      return reciprocal;
    }

  public final double coefficientFor(ClAbstractVariable var)
    { 
      ClDouble coeff = (ClDouble) _terms.get(var);
      if (coeff != null)
	return coeff.doubleValue();
      else
	return 0.0;
    }

  public final double constant()
    { return _constant.doubleValue(); }

  public final void set_constant(double c)
    { 
      _constant.setValue(c);
    }

  public final Hashtable terms()
    { return _terms; }

  public final void incrementConstant(double c)
    { 
      _constant.setValue(_constant.doubleValue() + c);
    }

  public final boolean isConstant()
    { return _terms.size() == 0; }

  public final String toString()
    {
      StringBuffer bstr = new StringBuffer();
      Enumeration e = _terms.keys();

      if (!CL.approx(_constant.doubleValue(),0.0) || _terms.size() == 0) 
	{
	bstr.append(_constant.toString());
	}
      else
	{
	if (_terms.size() == 0)
	  {
	  return bstr.toString();
	  }
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	ClDouble coeff = (ClDouble) _terms.get(clv);
	bstr.append(coeff.toString() + "*" + clv.toString());
	}
      for (; e.hasMoreElements(); )
	{
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	ClDouble coeff = (ClDouble) _terms.get(clv);
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

  private ClDouble _constant;
  private Hashtable _terms; // from ClVariable to ClDouble

}
