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
// ExCLError
//

class ExCLError extends exception
{

/**
* description
* @return char *
*/
public
/* @c2j++: "char * description()" replacement: char *  to String  */
String description()
{ return "(ExCLError) An error has occured in CL"; }

}

/*
 * class ExCLInternalError
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ExCLInternalError extends ExCLError
{

/**
* description
* @return char *
*/
public
/* @c2j++: "char * description()" replacement: char *  to String  */
String description()
{ return "(ExCLInternalError) An internal error has occurred"; }
}

/*
 * class ExCLTooDifficult
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ExCLTooDifficult extends ExCLError
{

/**
* description
* @return char *
*/
public
/* @c2j++: "char * description()" replacement: char *  to String  */
String description()
{ return "(ExCLTooDifficult) The constraints are too difficult to solve"; }
}

/*
 * class ExCLRequiredFailure
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ExCLRequiredFailure extends ExCLError
{

/**
* description
* @return char *
*/
public
/* @c2j++: "char * description()" replacement: char *  to String  */
String description()
{ return "(ExCLRequiredFailure) A required constraint cannot be satisfied"; }
}

/*
 * class ExCLNotEnoughtStays
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ExCLNotEnoughtStays extends ExCLError
{

/**
* description
* @return char *
*/
public
/* @c2j++: "char * description()" replacement: char *  to String  */
String description()
{ return "(ExCLNotEnoughtStays) There are not enough stays to give specific values to every variable"; }
}

/*
 * class ExCLNonlinearExpression
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ExCLNonlinearExpression extends ExCLError
{

/**
* description
* @return char *
*/
public
/* @c2j++: "char * description()" replacement: char *  to String  */
String description()
{ return "(ExCLNonlinearExpression) The resulting expression would be nonlinear"; }
}

/*
 * class ExCLConstraintNotFound
 * 
 * This code has been generated using C2J++
 * C2J++ is based on Chris Laffra's C2J (laffra@ms.com)
 * Read general disclaimer distributed with C2J++ before using this code
 * For information about C2J++, send mail to Ilya_Tilevich@ibi.com
 */

class ExCLConstraintNotFound extends ExCLError
{

/**
* description
* @return char *
*/
public
/* @c2j++: "char * description()" replacement: char *  to String  */
String description()
{ return "(ExCLConstraintNotFound) Tried to remove a constraint never added to the tableu"; }
}
