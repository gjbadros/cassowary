
/*
 * class ClEditConstraint
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClEditConstraint extends ClEditOrStayConstraint
{

/**
* ClEditConstraint
*/
public
ClEditConstraint()
{ }

/**
* isEditConstraint
* @return bool
*/
public
bool isEditConstraint()
{ return true; }

/**
* printOn
* @param xo
* @return ostream &
*/
public
/* @c2j++: "ostream & printOn(ostream& xo)" replacement:  &  to " " */
ostream printOn(ostream& xo)
{ xo << "edit" << variable(); return xo; }

}
