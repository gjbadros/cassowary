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
// ClEditOrStayConstraint
//

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
* @return ClVariable &
*/
public
/* @c2j++: "ClVariable & variable()" replacement:  &  to " " */
ClVariable variable()
/* @c2j++: "{ return *my_pvariable; }" replacement: * to " " */
{ return  my_pvariable; }

/**
* expression
* @return ClLinearExpression
*/
public
ClLinearExpression expression()
/* @c2j++: "{ return ClLinearExpression(*my_pvariable,-1,my_pvariable->value()); }" replacement: * to " " */
/* @c2j++: "{ return ClLinearExpression( my_pvariable,-1,my_pvariable->value()); }" replacement: -> to . */
{ return ClLinearExpression( my_pvariable,-1,my_pvariable.value()); }

/**
* setVariable
* @param v
*/
private
void setVariable(ClVariable& v)
{ my_pvariable = &v; }
/* @c2j++: "ClVariable * my_pvariable;" replacement: * to " " */
ClVariable   my_pvariable;

}
