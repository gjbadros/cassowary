
/*
 * class ClLinearExpression
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClLinearExpression
{

/**
* ClLinearExpression

public


/**
* ClLinearExpression

public


/**
* ClLinearExpression
* @param expr
*/
public
ClLinearExpression(ClLinearExpression& expr)
{
 my_constant = expr.my_constant;
	my_terms = expr.my_terms;
	 }

/**
* ClLinearExpression

public


/**
* times
* @param x
* @return ClLinearExpression
*/
public
ClLinearExpression times(Number x)
{
/* @c2j++: "ClLinearExpression result = *this;" replacement: * to " " */
  ClLinearExpression result =  this;
  return result.multiplyMe(x);
}

/**
* times
* @param expr
* @return ClLinearExpression
*/
public
ClLinearExpression times(ClLinearExpression& expr)
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

/**
* plus
* @param expr
* @return ClLinearExpression
*/
public
ClLinearExpression plus(ClLinearExpression& expr)
{
/* @c2j++: "ClLinearExpression result = *this;" replacement: * to " " */
  ClLinearExpression result =  this;
  result.addExpression(expr,1.0);
  return result;
}

/**
* minus
* @param expr
* @return ClLinearExpression
*/
public
ClLinearExpression minus(ClLinearExpression& expr)
{
/* @c2j++: "ClLinearExpression result = *this;" replacement: * to " " */
  ClLinearExpression result =  this;
  result.addExpression(expr,-1.0);
  return result;
}

/**
* divide
* @param x
* @return ClLinearExpression
*/
public
ClLinearExpression divide(Number x)
{
  if (clApprox(x,0.0))
    {
    throw ExCLNonlinearExpression();
    }
  return times(1.0/x);
}

/**
* divide
* @param expr
* @return ClLinearExpression
*/
public
ClLinearExpression divide(ClLinearExpression& expr)
{
  if (!expr.isConstant())
    {
    throw ExCLNonlinearExpression();
    }
  return divide(expr.my_constant);
}

/**
* divFrom
* @param expr
* @return ClLinearExpression
*/
public
ClLinearExpression divFrom(ClLinearExpression& expr)
{
  if (!isConstant() || clApprox(my_constant,0.0))
    {
    throw ExCLNonlinearExpression();
    }
  return expr.divide(my_constant);
}

/**
* subtractFrom
* @param expr
* @return ClLinearExpression
*/
public
ClLinearExpression subtractFrom(ClLinearExpression& expr)
/* @c2j++: "{ return expr.minus(*this); }" replacement: * to " " */
{ return expr.minus( this); }

/**
* addExpression

public


/**
* addExpression
* @param expr
* @param n
* @param subject
* @param solver
* @return ClLinearExpression&
*/
public
ClLinearExpression& addExpression(ClLinearExpression& expr, Number n, ClAbstractVariable& subject, ClTableau& solver)
{
/* @c2j++: "incrementConstant(n*expr.constant());" replacement: * to " " */
  incrementConstant(n expr.constant());

  ClVarToNumberMap::const_iterator i = expr.my_terms.begin();
  for ( ; i != expr.my_terms.end(); ++i)
    {
/* @c2j++: "addVariable(*((*i).first), n * (*i).second, subject, solver);" replacement: * to " " */
/* @c2j++: "addVariable( ((*i).first), n * (*i).second, subject, solver);" replacement: * to " " */
/* @c2j++: "addVariable( (( i).first), n * (*i).second, subject, solver);" replacement: * to " " */
/* @c2j++: "addVariable( (( i).first), n   (*i).second, subject, solver);" replacement: * to " " */
    addVariable( (( i).first), n   ( i).second, subject, solver);
    }
/* @c2j++: "return *this;" replacement: * to " " */
  return  this;
}

/**
* addVariable
* @param v
* @param c
* @return ClLinearExpression&
*/
public
ClLinearExpression& addVariable(ClAbstractVariable& v, Number c)
{ // body largely duplicated below
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << v << ", " << c << ")" << endl; */
System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(c) + ")");
#endif
  ClVarToNumberMap::iterator i = my_terms.find(&v);
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
/* @c2j++: "(*i).second = new_coefficient;" replacement: * to " " */
      ( i).second = new_coefficient;
      }
    }
  else // expression did not contain that variable
    {
    if (!clApprox(c,0.0))
      {
      my_terms[&v] = c;
      }
    }
/* @c2j++: "return *this;" replacement: * to " " */
  return  this;
}

/**
* setVariable
* @param v
* @param c
* @return ClLinearExpression &
*/
public
/* @c2j++: "ClLinearExpression & setVariable(ClAbstractVariable& v, Number c)" replacement:  &  to " " */
ClLinearExpression setVariable(ClAbstractVariable& v, Number c)
/* @c2j++: "{assert(c != 0.0);  my_terms[&v] = c; return *this; }" replacement: * to " " */
{assert(c != 0.0);  my_terms[&v] = c; return  this; }

/**
* addVariable
* @param v
* @param c
* @param subject
* @param solver
* @return ClLinearExpression&
*/
public
ClLinearExpression& addVariable(ClAbstractVariable& v, Number c, ClAbstractVariable& subject, ClTableau& solver)
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

/**
* anyVariable
* @return ClAbstractVariable*
*/
public
/* @c2j++: "ClAbstractVariable* anyVariable()" replacement: * to " " */
ClAbstractVariable  anyVariable()
{
  if (isConstant())
    {
    throw ExCLInternalError();
    }
/* @c2j++: "return (*my_terms.begin()).first;" replacement: * to " " */
  return ( my_terms.begin()).first;
}

/**
* substituteOut
* @param var
* @param expr
* @param subject
* @param solver
*/
public
void substituteOut(ClAbstractVariable& var, ClLinearExpression& expr, ClAbstractVariable& subject, ClTableau& solver)
{
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "* ClLinearExpression::"; */
System.err.print("* ClLinearExpression::");
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << var << ", " << expr << ", " << subject << ", " */
System.err.print("(" + String.valueOf(var) + ", " + String.valueOf(expr) + ", " + String.valueOf(subject) + ", ");
       << solver << ")" << endl;
/** @c2j++ Replacement from cerr << "*this == " << *this << endl; */
System.err.println("*this == " + String.valueOf(*this));
#endif
  ClVarToNumberMap::iterator pv = my_terms.find(&var);
  assert(pv != my_terms.end());

/* @c2j++: "Number multiplier = (*pv).second;" replacement: * to " " */
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

/**
* changeSubject
* @param old_subject
* @param new_subject
*/
public
void changeSubject(ClAbstractVariable& old_subject, ClAbstractVariable& new_subject)
{
  my_terms[&old_subject] = newSubject(new_subject);
}

/**
* newSubject
* @param subject
* @return Number
*/
public
Number newSubject(ClAbstractVariable& subject)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << subject << ")" << endl; */
System.err.println("(" + String.valueOf(subject) + ")");
#endif
  ClVarToNumberMap::iterator pnewSubject = my_terms.find(&subject);
  assert(pnewSubject != my_terms.end());
/* @c2j++: "Number reciprocal = 1.0 / (*pnewSubject).second;" replacement: * to " " */
  Number reciprocal = 1.0 / ( pnewSubject).second;
  my_terms.erase(pnewSubject);
  multiplyMe(-reciprocal);
  return reciprocal;
}

/**
* coefficientFor
* @param var
* @return Number
*/
public
Number coefficientFor(ClAbstractVariable& var)
{ 
/* @c2j++: "map<const ClAbstractVariable *, Number>::const_iterator it = my_terms.find(&var);" replacement: * to " " */
/* @c2j++: "map<const ClAbstractVariable  , Number>::const_iterator it = my_terms.find(&var);" replacement: const  to static final  */
    map<static final ClAbstractVariable  , Number>::const_iterator it = my_terms.find(&var);
    if (it != my_terms.end())
/* @c2j++: "return (*it).second;" replacement: * to " " */
      return ( it).second;
    return 0.0;
    }

/**
* constant
* @return Number
*/
public
Number constant()
{ return my_constant; }

/**
* set_constant
* @param c
*/
public
void set_constant(Number c)
{ my_constant = c; }

/**
* terms
* @return ClVarToNumberMap &
*/
public
/* @c2j++: "ClVarToNumberMap & terms()" replacement:  &  to " " */
ClVarToNumberMap terms()
{ return my_terms; }

/**
* terms
* @return ClVarToNumberMap &
*/
public
/* @c2j++: "ClVarToNumberMap & terms()" replacement:  &  to " " */
ClVarToNumberMap terms()
{ return my_terms; }

/**
* incrementConstant
* @param c
*/
public
void incrementConstant(Number c)
{ my_constant += c; }

/**
* isConstant
* @return bool
*/
public
bool isConstant()
{ return my_terms.size() == 0; }

/**
* printOn
* @param xo
* @return ostream&
*/
public
ostream& printOn(ostream& xo)
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

/**
* <
* @param xo
* @param cle
* @return <
*/
public
< <(ostream& xo, ClLinearExpression& cle)
{ return cle.printOn(xo); }

/**
* +
* @param e1
* @param e2
* @return operator
*/
public
operator +(ClLinearExpression& e1, ClLinearExpression& e2)
{ return e1.plus(e2); }

/**
* -
* @param e1
* @param e2
* @return operator
*/
public
operator -(ClLinearExpression& e1, ClLinearExpression& e2)
{ return e1.minus(e2); }

/**
* *
* @param e1
* @param e2
* @return operator
*/
public
/* @c2j++: "operator *(ClLinearExpression& e1, ClLinearExpression& e2)" replacement: * to " " */
operator  (ClLinearExpression& e1, ClLinearExpression& e2)
{ return e1.times(e2); }
/* @c2j++: "ClLinearExpression & e1;" replacement:  &  to " " */
ClLinearExpression e1;

/**
* =
* @param e1
* @param e2
* @return =
*/
public
= =(ClLinearExpression& e1, ClLinearExpression& e2)
{ return &e1 == &e2; }

/**
* multiplyMe
* @param x
* @return ClLinearExpression&
*/
public
ClLinearExpression& multiplyMe(Number x)
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
Number my_constant;
ClVarToNumberMap my_terms;

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef map < static final ClAbstractVariable * , Number > ClVarToNumberMap ;" replacement: * to " " */
typedef map < static final ClAbstractVariable   , Number > ClVarToNumberMap ; 

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef ClLinearExpression * PClLinearExpression ;" replacement: * to " " */
typedef ClLinearExpression   PClLinearExpression ; 

}
