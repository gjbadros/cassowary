// $Id$
// 
// QuadDemo.cc
// Demonstration that the polygon inscribed in the midpoints
// of an arbitrary quadrilateral is a parallelogram.
//

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
    qdw.setBackgroundColor( white );
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
