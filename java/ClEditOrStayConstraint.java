
/*
 * class ClEditOrStayConstraint
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClEditOrStayConstraint extends ClConstraint
{

/**
* ClEditOrStayConstraint
*/
public
ClEditOrStayConstraint()
{ }

/**
* variable
* @return ClAbstractVariable &
*/
public
/* @c2j++: "ClAbstractVariable & variable()" replacement:  &  to " " */
ClAbstractVariable variable()
/* @c2j++: "{ return *my_pvariable; }" replacement: * to " " */
{ return  my_pvariable; }

/**
* expression
* @return ClLinearExpression
*/
public
ClLinearExpression expression()
/* @c2j++: "{ return ClLinearExpression(*my_pvariable,-1); }" replacement: * to " " */
{ return ClLinearExpression( my_pvariable,-1); }

/**
* setVariable
* @param v
*/
private
void setVariable(ClAbstractVariable& v)
{ my_pvariable = &v; }
/* @c2j++: "ClAbstractVariable * my_pvariable;" replacement: * to " " */
ClAbstractVariable   my_pvariable;

}
