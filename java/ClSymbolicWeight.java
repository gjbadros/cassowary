
/*
 * class ClSymbolicWeight
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClSymbolicWeight
{

/**
* ClSymbolicWeight
*/
public
ClSymbolicWeight()
{ }

/**
* ClSymbolicWeight

public


/**
* ClSymbolicWeight

public


/**
* ClSymbolicWeight
* @param weights
*/
public
ClSymbolicWeight(>& weights)
{
 my_values = weights;
	 }

/**
* zero
* @return ClSymbolicWeight&
*/
public
ClSymbolicWeight& zero()
{
  static ClSymbolicWeight Zero(0.0, 0.0, 0.0);
  return Zero;
}

/**
* times
* @param n
* @return ClSymbolicWeight
*/
public
ClSymbolicWeight times(Number n)
{
  ClSymbolicWeight clsw(0);
  vector<double>::const_iterator i = my_values.begin();
  for (; i != my_values.end(); ++i)
    {
/* @c2j++: "clsw.push_back(*i * n);" replacement: * to " " */
/* @c2j++: "clsw.push_back( i * n);" replacement: * to " " */
    clsw.push_back( i   n);
    }
  return clsw;
}

/**
* divideBy
* @param n
* @return ClSymbolicWeight
*/
public
ClSymbolicWeight divideBy(Number n)
{
  assert(n!=0);
  ClSymbolicWeight clsw(0);
  vector<double>::const_iterator i = my_values.begin();
  for (; i != my_values.end(); ++i)
    {
/* @c2j++: "clsw.push_back(*i / n);" replacement: * to " " */
    clsw.push_back( i / n);
    }
  return clsw;
}

/**
* add
* @param cl
* @return ClSymbolicWeight
*/
public
ClSymbolicWeight add(ClSymbolicWeight& cl)
{
  assert(cl.cLevels() == cLevels());

  ClSymbolicWeight clsw(0);
  vector<double>::const_iterator i1 = my_values.begin();
  vector<double>::const_iterator i2 = cl.my_values.begin();
  for (; i1 != my_values.end(); ++i1, ++i2)
    {
/* @c2j++: "clsw.push_back(*i1 + *i2);" replacement: * to " " */
/* @c2j++: "clsw.push_back( i1 + *i2);" replacement: * to " " */
    clsw.push_back( i1 +  i2);
    }
  return clsw;
}

/**
* subtract
* @param cl
* @return ClSymbolicWeight
*/
public
ClSymbolicWeight subtract(ClSymbolicWeight& cl)
{
  assert(cl.cLevels() == cLevels());

  ClSymbolicWeight clsw(0);
  vector<double>::const_iterator i1 = my_values.begin();
  vector<double>::const_iterator i2 = cl.my_values.begin();
  for (; i1 != my_values.end(); ++i1, ++i2)
    {
/* @c2j++: "clsw.push_back(*i1 - *i2);" replacement: * to " " */
/* @c2j++: "clsw.push_back( i1 - *i2);" replacement: * to " " */
    clsw.push_back( i1 -  i2);
    }
  return clsw;
}

/**
* lessThan
* @param cl
* @return bool
*/
public
bool lessThan(ClSymbolicWeight& cl)
{
  return my_values < cl.my_values;
}

/**
* lessThanOrEqual
* @param cl
* @return bool
*/
public
bool lessThanOrEqual(ClSymbolicWeight& cl)
{
  return my_values <= cl.my_values;
}

/**
* equal
* @param cl
* @return bool
*/
public
bool equal(ClSymbolicWeight& cl)
{
  return my_values == cl.my_values;
}

/**
* greaterThan
* @param cl
* @return bool
*/
public
bool greaterThan(ClSymbolicWeight& cl)
{
  return my_values > cl.my_values;
}

/**
* greaterThanOrEqual
* @param cl
* @return bool
*/
public
bool greaterThanOrEqual(ClSymbolicWeight& cl)
{
  return my_values >= cl.my_values;
}

/**
* isNegative
* @return bool
*/
public
bool isNegative()
{
  return my_values < zero().my_values;
}

/**
* =
* @param cl1
* @param cl2
* @return =
*/
public
= =(ClSymbolicWeight& cl1, ClSymbolicWeight& cl2)
{ return cl1.equal(cl2); }

/**
* =
* @param cl1
* @param cl2
* @return =
*/
public
= =(ClSymbolicWeight& cl1, ClSymbolicWeight& cl2)
{ return cl1.equal(cl2); }

/**
* <
* @param cl1
* @param cl2
* @return operator
*/
public
operator <(ClSymbolicWeight& cl1, ClSymbolicWeight& cl2)
{ return cl1.lessThan(cl2); }

/**
* >
* @param cl1
* @param cl2
* @return operator
*/
public
operator >(ClSymbolicWeight& cl1, ClSymbolicWeight& cl2)
{ return (cl2 < cl1); }

/**
* asDouble
* @return double
*/
public
double asDouble()
{
    vector<double>::const_reverse_iterator i = my_values.rbegin();
    if (i == my_values.rend())
      return 0;
/* @c2j++: "double sum  = *i;" replacement: * to " " */
    double sum  =  i;
    double factor = 1;
    double multiplier = 1000;
    for (++i; i != my_values.rend(); ++i) 
      {
/* @c2j++: "sum += *i * factor;" replacement: * to " " */
/* @c2j++: "sum +=  i * factor;" replacement: * to " " */
      sum +=  i   factor;
/* @c2j++: "factor *= multiplier;" replacement: * to " " */
      factor  = multiplier;
      }
    return sum;
    }

/**
* printOn
* @param xo
* @return ostream &
*/
public
/* @c2j++: "ostream & printOn(ostream& xo)" replacement:  &  to " " */
ostream printOn(ostream& xo)
{ 
    vector<double>::const_iterator i = my_values.begin();
    if (i == my_values.end())
      return xo;

/* @c2j++: "xo << *i;" replacement: * to " " */
    xo <<  i;
    for (++i; i != my_values.end(); ++i) 
      {
/* @c2j++: "xo << "," << *i;" replacement: * to " " */
      xo << "," <<  i;
      }
    return xo;
    }

/**
* cLevels
* @return int
*/
public
int cLevels()
{ return my_values.size(); }

/**
* <
* @param xos
* @param clsw
* @return <
*/
public
< <(ostream& xos, ClSymbolicWeight& clsw)
{ clsw.printOn(xos); return xos; }
> my_values;

/**
* push_back
* @param d
*/
private
void push_back(double d)
{ my_values.push_back(d); }

}
