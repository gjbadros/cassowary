/* $Id$
 * File:	ClBoundedQuadDemo.cpp
 */

/* static const char sccsid[] = "%W% %G%"; */

#include "Cl.h"

#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx_prec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx.h"
#endif

// Define a new application type
class MyApp: public wxApp
{ public:
    wxFrame *OnInit(void);
};

// Define a new frame type
class MyFrame: public wxFrame
{ public:
    MyFrame(wxFrame *frame, char *title, int x, int y, int w, int h);
    void OnMenuCommand(int id);
    Bool OnClose(void) { return TRUE; }
};

// ID for the menu quit command
#define MINIMAL_QUIT 1

// This statement initializes the whole application and calls OnInit
MyApp myApp;

// A macro needed for some compilers (AIX) that need 'main' to be defined
// in the application itself.
IMPLEMENT_WXWIN_MAIN

ClVariable xStart, yStart;
ClVariable xEnd, yEnd;
ClSimplexSolver solver;

class MyCanvas: public wxCanvas
{
public:
  MyCanvas(wxWindow *window, int x = -1, int y = -1, int width = -1, int height = -1,
	   long style = wxRETAINED, char *Name = "mycanvas") :
    wxCanvas(window,x,y,width,height,style,Name)
    { }
  
  void OnEvent(wxMouseEvent &event)
    { 
      float x, y;
      vector<Number> v;
      event.Position(&x,&y); // get the position
      v.push_back(x);
      v.push_back(y);
      solver.Resolve(v);
      Clear();
      OnPaint(); // repaint
    }

  // Called when canvas needs to be repainted.
  void OnPaint(void)
    {
      // Speeds up drawing under Windows.
      GetDC()->BeginDrawing();
      wxCanvasDC *canvdc = GetDC();

#if 0      
      // Find Out where the window is scrolled to
      int vbX,vbY;                     // Top left corner of client
      ViewStart(&vbX,&vbY);
      
      int vX,vY,vW,vH;                 // Dimensions of client area in pixels
      wxUpdateIterator      upd(this); // get the update rect list
      
      while (upd)
	{
	vX = upd.GetX();
	vY = upd.GetY();
	vW = upd.GetW();
	vH = upd.GetH();
	
	// Alternatively we can do this:
	// wxRectangle rect;
	// upd.GetRect(&rect);
	
	

	upd ++ ;
	}
#endif
      IntDrawLine(xStart.Value(),yStart.Value(),xEnd.Value(),yEnd.Value());
      GetDC()->EndDrawing();
    }
};
// `Main program' equivalent, creating windows and returning main app frame
wxFrame *MyApp::OnInit(void)
{
  // Create the main frame window
  MyFrame *frame = new MyFrame(NULL, "Minimal wxWindows App", 50, 50, 400, 300);

  // Give it an icon
#ifdef wx_msw
  frame->SetIcon(new wxIcon("mondrian"));
#endif
#ifdef wx_x
  frame->SetIcon(new wxIcon("aiai.xbm"));
#endif

  // Make a menubar
  wxMenu *file_menu = new wxMenu;

  file_menu->Append(MINIMAL_QUIT, "E&xit");
  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu, "File");
  frame->SetMenuBar(menu_bar);

  // Make a panel with a message
  wxCanvas *pcanvas = new MyCanvas(frame);

  // Show the frame
  frame->Show(TRUE);

  xEnd.SetValue(100.0);
  yEnd.SetValue(100.0);

  solver
    .AddStay(xStart)
    .AddStay(yStart)
    .AddStay(xEnd)
    .AddStay(yEnd);

  solver
    .AddConstraint(ClLinearEquation(xEnd,xStart+200.0))
    .AddConstraint(ClLinearEquation(yEnd,xStart*2.0))
    .AddConstraint(ClEditConstraint(xStart))
    .AddConstraint(ClEditConstraint(yStart));

  // Return the main frame window
  return frame;
}

// My frame constructor
MyFrame::MyFrame(wxFrame *frame, char *title, int x, int y, int w, int h):
  wxFrame(frame, title, x, y, w, h)
{}

// Intercept menu commands
void MyFrame::OnMenuCommand(int id)
{
  switch (id) {
    case MINIMAL_QUIT:
      delete this;
    break;
  }
}

