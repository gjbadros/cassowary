
/*
 * class ClLinearConstraint
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ClLinearConstraint extends ClConstraint
{
ClLinearConstraint super;

/**
* ClLinearConstraint
*/
public
ClLinearConstraint()
{ }

/**
* expression
* @return ClLinearExpression
*/
public
ClLinearExpression expression()
{ return my_expression; }
ClLinearExpression my_expression;

/**
* setExpression
* @param expr
*/
protected
void setExpression(ClLinearExpression& expr)
{ my_expression = expr; }

}
