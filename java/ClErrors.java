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

class ExCLError extends Exception
{

  public String description()
    { return "(ExCLError) An error has occured in CL"; }
  
}

class ExCLInternalError extends ExCLError
{
  public String description()
    { return "(ExCLInternalError) An internal error has occurred"; }
}

class ExCLTooDifficult extends ExCLError
{
  public String description()
    { return "(ExCLTooDifficult) The constraints are too difficult to solve"; }
}

class ExCLRequiredFailure extends ExCLError
{
  public String description()
    { return "(ExCLRequiredFailure) A required constraint cannot be satisfied"; }
}


class ExCLNotEnoughtStays extends ExCLError
{
  public String description()
    { return "(ExCLNotEnoughtStays) There are not enough stays to give specific values to every variable"; }
}

class ExCLNonlinearExpression extends ExCLError
{
  public String description()
    { return "(ExCLNonlinearExpression) The resulting expression would be nonlinear"; }
}

class ExCLConstraintNotFound extends ExCLError
{
  public String description()
    { return "(ExCLConstraintNotFound) Tried to remove a constraint never added to the tableu"; }
}
