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
// ClLinearInequality
//

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
* ClLinearInequality
*/
public
ClLinearInequality()
{ }

/**
* isInequality
* @return boolean
*/
public
boolean isInequality()
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
