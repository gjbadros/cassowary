
/*
 * class ClConstraint
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClConstraint
{

/**
* ClConstraint
*/
public
ClConstraint()
{ }

/**
* ClConstraint
*/
public
ClConstraint()
{ }

/**
* expression

public


/**
* isEditConstraint
* @return bool
*/
public
bool isEditConstraint()
{ return false; }

/**
* isInequality
* @return bool
*/
public
bool isInequality()
{ return false; }

/**
* isRequired
* @return bool
*/
public
bool isRequired()
{ return my_strength.isRequired(); }

/**
* isStayConstraint
* @return bool
*/
public
bool isStayConstraint()
{ return false; }

/**
* strength
* @return ClStrength
*/
public
ClStrength strength()
{ return my_strength; }

/**
* weight
* @return double
*/
public
double weight()
{ return my_weight; }

/**
* printOn
* @param xo
* @return ostream&
*/
public
ostream& printOn(ostream& xo)
{
  xo << strength() << " {" << weight() << "} (" << expression();
  return xo;
}

/**
* <
* @param xos
* @param constraint
* @return <
*/
public
< <(ostream& xos, ClConstraint& constraint)
{ constraint.printOn(xos); return xos; }

/**
* setStrength
* @param strength
*/
private
void setStrength(ClStrength& strength)
{ my_strength = strength; }

/**
* setWeight
* @param weight
*/
private
void setWeight(double weight)
{ my_weight = weight; }
ClStrength my_strength;
double my_weight;

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef ClConstraint * PClConstraint ;" replacement: * to " " */
typedef ClConstraint   PClConstraint ; 

}
