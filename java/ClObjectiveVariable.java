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
// ClObjectiveVariable
//

class ClObjectiveVariable extends ClAbstractVariable
{
friend ClTableau;
friend ClSimplexSolver;

/**
* ClObjectiveVariable
*/
protected
ClObjectiveVariable()
{
 super(name);
	 }

/**
* ClObjectiveVariable
* @param number
* @param prefix
*/
protected
/* @c2j++: "ClObjectiveVariable(long number, char* prefix)" replacement: char*  to String  */
ClObjectiveVariable(long number, String prefix)
{
 super(number,prefix);
	 }

/**
* printOn
* @param xo
* @return ostream &
*/
protected
/* @c2j++: "ostream & printOn(ostream& xo)" replacement:  &  to " " */
ostream printOn(ostream& xo)
{  
    xo << "[" << name() << ":obj]";
    return xo;
  }

/**
* isExternal
* @return boolean
*/
protected
boolean isExternal()
{ return false; }

/**
* isPivotable
* @return boolean
*/
protected
boolean isPivotable()
{ return false; }

/**
* isRestricted
* @return boolean
*/
protected
boolean isRestricted()
{ return false; }

}
