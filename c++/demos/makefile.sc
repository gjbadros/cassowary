# Symantec C++ makefile for minimal example
# NOTE that peripheral libraries are now dealt in main wxWindows makefile.

WXDIR = $(WXWIN)
WXLIB = $(WXDIR)\lib\wx.lib
INCDIR = $(WXDIR)\include
MSWINC = $(INCDIR)\msw
BASEINC = $(INCDIR)\base
INCLUDE=$(BASEINC);$(MSWINC);$(WXDIR)\contrib\fafa

include $(WXDIR)\src\makesc.env

minimal.exe: minimal.obj $(DEFFILE) minimal.res
	*$(CC) $(LDFLAGS) -o$@ $** $(LIBS)

clean:
        -del *.obj
	-del *.exe
	-del *.res
	-del *.map
	-del *.rws
