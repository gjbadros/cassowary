// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
//
// Demonstration that the polygon inscribed in the midpoints
// of an arbitrary quadrilateral is a parallelogram.
//
// QuadDemo.cc

#include "QuadDemoWindow.h"
#include <qapp.h>
#include "Cl.h"

int main( int argc, char **argv )
{
  try
    {
    QApplication a(argc,argv);
    QuadDemoWindow qdw;
    qdw.setCaption( "QuadDemoWindow" );
    a.setMainWidget( &qdw );
    qdw.show();
    return a.exec();
    }
  catch (ExCLError &error) 
    {
    cerr << "Exception! " << error.description() << endl;
    return(false);
    }
  catch (...) 
    {
    cerr << "Unknown exception" << endl;
    return(false);
    }
}
