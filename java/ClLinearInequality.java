
/*
 * class ClLinearInequality
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClLinearInequality extends ClLinearConstraint
{
ClLinearConstraint super;

/**
* ClLinearInequality
*/
public
ClLinearInequality()
{ }

/**
* ClLinearInequality
*/
public
ClLinearInequality()
{ }

/**
* isInequality
* @return bool
*/
public
bool isInequality()
{ return true; }

/**
* printOn
* @param xo
* @return ostream &
*/
public
/* @c2j++: "ostream & printOn(ostream& xo)" replacement:  &  to " " */
ostream printOn(ostream& xo)
{  super::printOn(xo); xo << " >= 0 )"; return xo; }

}
