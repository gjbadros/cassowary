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
      my_terms.put(clv,value);
    }


  public ClLinearExpression times(double x)
    {
      ClLinearExpression result = (ClLinearExpression) clone();
      return result.multiplyMe(x);
    }

  public ClLinearExpression times(ClLinearExpression expr)
    {
      if (isConstant())
	{
	return expr.times(my_constant);
	}
      else if (!expr.isConstant())
	{
	throw ExCLNonlinearExpression();
	}	
      return times(expr.my_constant);
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

  public ClLinearExpression divide(Number x)
    {
      if (clApprox(x,0.0))
	{
	throw ExCLNonlinearExpression();
	}
      return times(1.0/x);
    }

  public ClLinearExpression divide(ClLinearExpression expr)
    {
      if (!expr.isConstant())
	{
	throw ExCLNonlinearExpression();
	}
      return divide(expr.my_constant);
    }

  public ClLinearExpression divFrom(ClLinearExpression expr)
    {
      if (!isConstant() || clApprox(my_constant,0.0))
	{
	throw ExCLNonlinearExpression();
	}
      return expr.divide(my_constant);
    }

  public ClLinearExpression subtractFrom(ClLinearExpression expr)
  /* @c2j++: "{ return expr.minus(*this); }" replacement: * to " " */
    { return expr.minus( this); }

  public ClLinearExpression addExpression(ClLinearExpression expr, Number n,
					  ClAbstractVariable subject, 
					  ClTableau solver)
    {
      incrementConstant(n expr.constant());
      
      ClVarToNumberMap::const_iterator i = expr.my_terms.begin();
      for ( ; i != expr.my_terms.end(); ++i)
	{
	addVariable( (( i).first), n   ( i).second, subject, solver);
	}
      return  this;
    }

  public ClLinearExpression addVariable(ClAbstractVariable v, Number c)
    { // body largely duplicated below
#ifndef CL_NO_TRACE
      Tracer TRACER(__FUNCTION__);
      System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(c) + ")");
#endif
      ClVarToNumberMap::iterator i = my_terms.find(v);
      if (i != my_terms.end())
	{
	Number new_coefficient = 0;
	/* @c2j++: "new_coefficient = (*i).second + c;" replacement: * to " " */
	new_coefficient = ( i).second + c;
	if (clApprox(new_coefficient,0.0))
	  {
	  my_terms.erase(i);
	  }
	else
	  {
	  ( i).second = new_coefficient;
	  }
	}
      else // expression did not contain that variable
	{
	if (!clApprox(c,0.0))
	  {
	  my_terms[v] = c;
	  }
	}
      return  this;
    }

  public ClLinearExpression setVariable(ClAbstractVariable v, Number c)
    { 
      //assert(c != 0.0);  
      my_terms[v] = c; return  this; 
    }
  
  public ClLinearExpression addVariable(ClAbstractVariable v, Number c,
					ClAbstractVariable subject, ClTableau solver)
    { // body largely duplicated above
#ifndef CL_NO_TRACE
      Tracer TRACER(__FUNCTION__);
      /** @c2j++ Replacement from cerr << "(" << v << ", " << c << ", " << subject << ", ...)" << endl; */
      System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(c) + ", " + String.valueOf(subject) + ", ...)");
#endif
      ClVarToNumberMap::iterator i = my_terms.find(&v);
      if (i != my_terms.end())
	{
	Number new_coefficient = 0;
	/* @c2j++: "new_coefficient = (*i).second + c;" replacement: * to " " */
	new_coefficient = ( i).second + c;
	if (clApprox(new_coefficient,0.0))
	  {
	  /* @c2j++: "solver.noteRemovedVariable(*((*i).first),subject);" replacement: * to " " */
	  /* @c2j++: "solver.noteRemovedVariable( ((*i).first),subject);" replacement: * to " " */
	  solver.noteRemovedVariable( (( i).first),subject);
	  my_terms.erase(i);
	  }
	else
	  {
	  /* @c2j++: "(*i).second = new_coefficient;" replacement: * to " " */
	  ( i).second = new_coefficient;
	  }
	}
      else // expression did not contain that variable
	{
	if (!clApprox(c,0.0))
	  {
	  my_terms[&v] = c;
	  solver.noteAddedVariable(v,subject);
	  }
	}
#ifndef CL_NO_TRACE
      /** @c2j++ Replacement from cerr << "Now *this == " << *this << endl; */
      System.err.println("Now *this == " + String.valueOf(*this));
#endif
      /* @c2j++: "return *this;" replacement: * to " " */
      return  this;
    }
  
  public ClAbstractVariable  anyVariable()
    {
      if (isConstant())
	{
	throw ExCLInternalError();
	}
      return ( my_terms.begin()).first;
    }
  
  public void substituteOut(ClAbstractVariable var, ClLinearExpression expr, 
			    ClAbstractVariable subject, ClTableau solver)
    {
#ifndef CL_NO_TRACE
      System.err.print("* ClLinearExpression::");
      Tracer TRACER(__FUNCTION__);
      System.err.print("(" + String.valueOf(var) + ", " + String.valueOf(expr) + ", " + String.valueOf(subject) + ", ");
      System.err.println("*this == " + String.valueOf(*this));
#endif
      ClVarToNumberMap::iterator pv = my_terms.find(&var);
      // assert(pv != my_terms.end());

      Number multiplier = ( pv).second;
      my_terms.erase(pv);
      /* @c2j++: "incrementConstant(multiplier * expr.my_constant);" replacement: * to " " */
      incrementConstant(multiplier   expr.my_constant);
      ClVarToNumberMap::const_iterator i = expr.my_terms.begin();
      for ( ; i != expr.my_terms.end(); ++i)
	{
	/* @c2j++: "const ClAbstractVariable *pv = (*i).first;" replacement: * to " " */
	/* @c2j++: "const ClAbstractVariable  pv = (*i).first;" replacement: * to " " */
	/* @c2j++: "const ClAbstractVariable  pv = ( i).first;" replacement: const  to static final  */
	static final ClAbstractVariable  pv = ( i).first;
/* @c2j++: "Number c = (*i).second;" replacement: * to " " */
	Number c = ( i).second;
	ClVarToNumberMap::iterator poc = my_terms.find(pv);
	if (poc != my_terms.end())
	  { // if oldCoeff is not nil
#ifndef CL_NO_TRACE
	  /** @c2j++ Replacement from cerr << "Considering (*poc) == " << (*poc).second << "*" << *(*poc).first << endl; */
	  System.err.println("Considering (*poc) == " + String.valueOf((*poc).second) + "*" + String.valueOf(*(*poc).first));
#endif
	  /* @c2j++: "Number newCoeff = (*poc).second + (multiplier*c);" replacement: * to " " */
	  /* @c2j++: "Number newCoeff = ( poc).second + (multiplier*c);" replacement: * to " " */
	  Number newCoeff = ( poc).second + (multiplier c);
	  if (clApprox(newCoeff,0.0))
	    {
	    /* @c2j++: "solver.noteRemovedVariable(*((*poc).first),subject);" replacement: * to " " */
	    /* @c2j++: "solver.noteRemovedVariable( ((*poc).first),subject);" replacement: * to " " */
	    solver.noteRemovedVariable( (( poc).first),subject);
	    my_terms.erase(poc);
	    }
	  else
	    {
	    /* @c2j++: "(*poc).second = newCoeff;" replacement: * to " " */
	    ( poc).second = newCoeff;
	    }
	  }
	else
	  { // did not have that variable already (oldCoeff == nil)
#ifndef CL_NO_TRACE
	  /** @c2j++ Replacement from cerr << "Adding (*i) == " << (*i).second << "*" << *(*i).first << endl; */
	  System.err.println("Adding (*i) == " + String.valueOf((*i).second) + "*" + String.valueOf(*(*i).first));
#endif
	  /* @c2j++: "my_terms[pv] = multiplier * c;" replacement: * to " " */
	  my_terms[pv] = multiplier   c;
	  /* @c2j++: "solver.noteAddedVariable(*pv,subject);" replacement: * to " " */
	  solver.noteAddedVariable( pv,subject);
	  }
	}
#ifndef CL_NO_TRACE
      /** @c2j++ Replacement from cerr << "Now (*this) is " << *this << endl; */
      System.err.println("Now (*this) is " + String.valueOf(*this));
#endif
    }
  
  public void changeSubject(ClAbstractVariable old_subject, ClAbstractVariable new_subject)
    {
      my_terms[&old_subject] = newSubject(new_subject);
    }
  
  public Number newSubject(ClAbstractVariable subject)
    {
#ifndef CL_NO_TRACE
      Tracer TRACER(__FUNCTION__);
      System.err.println("(" + String.valueOf(subject) + ")");
#endif
      ClVarToNumberMap::iterator pnewSubject = my_terms.find(&subject);
      // assert(pnewSubject != my_terms.end());
      Number reciprocal = 1.0 / ( pnewSubject).second;
      my_terms.erase(pnewSubject);
      multiplyMe(-reciprocal);
      return reciprocal;
    }

  public Number coefficientFor(ClAbstractVariable var)
    { 
      /* @c2j++: "map<const ClAbstractVariable *, Number>::const_iterator it = my_terms.find(&var);" replacement: * to " " */
      /* @c2j++: "map<const ClAbstractVariable  , Number>::const_iterator it = my_terms.find(&var);" replacement: const  to static final  */
      map<static final ClAbstractVariable  , Number>::const_iterator it = my_terms.find(&var);
      if (it != my_terms.end())
	/* @c2j++: "return (*it).second;" replacement: * to " " */
	return ( it).second;
      return 0.0;
    }

  public Number constant()
    { return my_constant; }

  public void set_constant(Number c)
    { my_constant = c; }

  public ClVarToNumberMap terms()
    { return my_terms; }

  public ClVarToNumberMap terms()
    { return my_terms; }

  public void incrementConstant(Number c)
    { my_constant += c; }

  public boolean isConstant()
    { return my_terms.size() == 0; }

  public String toString()
    {
      ClVarToNumberMap::const_iterator i = my_terms.begin();
      
      if (!clApprox(my_constant,0.0) || i == my_terms.end())
	{
	xo << my_constant;
	}
      else
	{
	if (i == my_terms.end())
	  return xo;
	/* @c2j++: "xo << (*i).second << "*" << *((*i).first);" replacement: * to " " */
	/* @c2j++: "xo << ( i).second << "*" << *((*i).first);" replacement: * to " " */
	/* @c2j++: "xo << ( i).second << " " << *((*i).first);" replacement: * to " " */
	/* @c2j++: "xo << ( i).second << " " <<  ((*i).first);" replacement: * to " " */
	xo << ( i).second << " " <<  (( i).first);
	++i;
	}
      for ( ; i != my_terms.end(); ++i)
	{
	/* @c2j++: "xo << " + " << (*i).second << "*" << *((*i).first);" replacement: * to " " */
	/* @c2j++: "xo << " + " << ( i).second << "*" << *((*i).first);" replacement: * to " " */
	/* @c2j++: "xo << " + " << ( i).second << " " << *((*i).first);" replacement: * to " " */
	/* @c2j++: "xo << " + " << ( i).second << " " <<  ((*i).first);" replacement: * to " " */
	xo << " + " << ( i).second << " " <<  (( i).first);
	}
      return xo;
    }

  public ClLinearExpression Plus(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.plus(e2); }

  public ClLinearExpression Minus(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.minus(e2); }

  public ClLinearExpression Times(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.times(e2); }

  public ClLinearExpression Divide(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.divide(e2); }

  public boolean FEquals(ClLinearExpression e1, ClLinearExpression e2)
    { return &e1 == &e2; }

  public ClLinearExpression multiplyMe(Number x)
    {
      /* @c2j++: "my_constant *= x;" replacement: * to " " */
      my_constant  = x;
      
      ClVarToNumberMap::const_iterator i = my_terms.begin();
      for ( ; i != my_terms.end(); ++i)
	{
	/* @c2j++: "my_terms[(*i).first] = (*i).second * x;" replacement: * to " " */
	/* @c2j++: "my_terms[( i).first] = (*i).second * x;" replacement: * to " " */
	/* @c2j++: "my_terms[( i).first] = ( i).second * x;" replacement: * to " " */
	my_terms[( i).first] = ( i).second   x;
	}
      /* @c2j++: "return *this;" replacement: * to " " */
      return  this;
    }

  private Number my_constant;
  private ClVarToNumberMap my_terms;
  
}
