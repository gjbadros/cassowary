//
// File: main.cpp
//
// A small Qt example application written by Troll Tech,
// modified to link with libcassowary and do a simple
// numerical constraint solution. --01/31/98 gjb
//
// It displays a text in a window and quits when you click
// the mouse in the window.
//

#include "hello.h"
#include <qapp.h>
#include "Cl.h"


/*
  The program starts here. It parses the command line and builds a message
  string to be displayed by the Hello widget.
*/

bool
addDelete1()
{
 try 
   {
   bool fOkResult = true; 
   ClVariable x("x");
   ClSimplexSolver solver;

   solver.AddConstraint( ClLinearEquation( x, 100, ClsWeak() ) );
    
   ClLinearInequality c10(x,cnLEQ,10.0);
   ClLinearInequality c20(x,cnLEQ,20.0);
    
   solver
     .AddConstraint(c10)
     .AddConstraint(c20);

   fOkResult = fOkResult && ClApprox(x,10.0);
   cout << "x == " << x.Value() << endl;

   solver.RemoveConstraint(c10);
   fOkResult = fOkResult && ClApprox(x,20.0);
   cout << "x == " << x.Value() << endl;

   solver.RemoveConstraint(c20);
   fOkResult = fOkResult && ClApprox(x,100.0);
   cout << "x == " << x.Value() << endl;

   ClLinearInequality c10again(x,cnLEQ,10.0);

   solver
     .AddConstraint(c10)
     .AddConstraint(c10again);

   fOkResult = fOkResult && ClApprox(x,10.0);
   cout << "x == " << x.Value() << endl;
    
   solver.RemoveConstraint(c10);
   fOkResult = fOkResult && ClApprox(x,10.0);
   cout << "x == " << x.Value() << endl;

   solver.RemoveConstraint(c10again);
   fOkResult = fOkResult && ClApprox(x,100.0);
   cout << "x == " << x.Value() << endl;

   return(fOkResult);
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

int main( int argc, char **argv )
{
    QApplication a(argc,argv);
    QString s;
    addDelete1();
    for ( int i=1; i<argc; i++ ) {
	s += argv[i];
	if ( i<argc-1 )
	    s += " ";
    }
    if ( s.isEmpty() )
	s = "Hello, World";
    Hello h( s );
    h.setCaption( "Qt says hello" );
    QObject::connect( &h, SIGNAL(clicked()), &a, SLOT(quit()) );
    h.setFont( QFont("times",32,QFont::Bold) );		// default font
    h.setBackgroundColor( white );			// default bg color
    a.setMainWidget( &h );
    h.show();
    return a.exec();
}
