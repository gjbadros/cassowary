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
// ClPoint
//

class ClPoint
{

/**
* ClPoint
* @param x
* @param y
*/
public
ClPoint(Number x, Number y)
{
 clv_x = x;
	clv_y = y;
	 }

/**
* ClPoint
*/
public
ClPoint()
{ }

/**
* X
* @return ClVariable &
*/
public
/* @c2j++: "ClVariable & X()" replacement:  &  to " " */
ClVariable X()
{ return clv_x; }

/**
* Y
* @return ClVariable &
*/
public
/* @c2j++: "ClVariable & Y()" replacement:  &  to " " */
ClVariable Y()
{ return clv_y; }

/**
* X
* @return ClVariable &
*/
public
/* @c2j++: "ClVariable & X()" replacement:  &  to " " */
ClVariable X()
{ return clv_x; }

/**
* Y
* @return ClVariable &
*/
public
/* @c2j++: "ClVariable & Y()" replacement:  &  to " " */
ClVariable Y()
{ return clv_y; }

/**
* SetXY
* @param x
* @param y
*/
public
void SetXY(Number x, Number y)
{ clv_x = x; clv_y = y; }

/**
* Xvalue
* @return Number
*/
public
Number Xvalue()
{ return X().value(); }

/**
* Yvalue
* @return Number
*/
public
Number Yvalue()
{ return Y().value(); }
ClVariable clv_x;
ClVariable clv_y;

/**
* <

private


/**
* @param xo
* @param clp
* @return <
*/
static < <(ostream& xo, ClPoint& clp)
{
  xo << "(" << clp.clv_x << ", " << clp.clv_y << ")";
  return xo;
}
}
