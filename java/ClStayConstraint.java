
/*
 * class ClStayConstraint
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClStayConstraint extends ClEditOrStayConstraint
{

/**
* ClStayConstraint
*/
public
ClStayConstraint()
{ }

/**
* isStayConstraint
* @return bool
*/
public
bool isStayConstraint()
{ return true; }

/**
* printOn
* @param xo
* @return ostream &
*/
public
/* @c2j++: "ostream & printOn(ostream& xo)" replacement:  &  to " " */
ostream printOn(ostream& xo)
{ xo << "stay" << variable(); return xo; }

}
