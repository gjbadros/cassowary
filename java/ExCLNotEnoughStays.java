// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ExCLNotEnoughStays
//

class ExCLNotEnoughStays extends ExCLError
{
  public String description()
    { return "(ExCLNotEnoughStays) There are not enough stays to give specific values to every variable"; }
}
