#
# File:		makefile.unx
# Author:	Julian Smart
# Created:	1993
# Updated:	
# Copyright:	(c) 1993, AIAI, University of Edinburgh
#
# "%W% %G%"
#
# Makefile for minimal example (UNIX).

WXDIR = $(LHOME)/wx
CLDIR = ..

PROGRAM = ClBoundedQuadDemo

EGCSDIR=/uns/egcs
CL_LDFLAGS = -L$(CLDIR) -lcassowary -L$(EGCSDIR)/lib
CL_CPPFLAGS = -I$(CLDIR) -I$(EGCSDIR)/include -DCL_NO_TRACE

# All common UNIX compiler flags and options are now in
# this central makefile.
include $(WXDIR)/src/make.env

OBJECTS = $(OBJDIR)/$(PROGRAM).$(OBJSUFF)

.SUFFIXES:

all:    $(OBJDIR) $(PROGRAM)$(GUISUFFIX)

wx:

motif:
	$(MAKE) -f makefile.unx GUISUFFIX= GUI=-Dwx_motif GUISUFFIX=_motif OPT='$(OPT)' LDLIBS='$(CL_LDFLAGS) $(MOTIFLDLIBS)' WXLIB=$(WXDIR)/lib/libwx_motif.a  OPTIONS='$(CL_CPPFLAGS) $(OPTIONS)' DEBUG='$(DEBUG)' WARN='$(WARN)' XLIB='$(XLIB)' XINCLUDE='$(XINCLUDE)' XVIEW_LINK=



xview:
	cd $(WXDIR)/src/x; $(MAKE) -f makefile.unx xview
	$(MAKE) -f makefile.unx GUI=-Dwx_xview GUISUFFIX=_ol CC=$(CC) OPTIONS='$(OPTIONS)' DEBUG='$(DEBUG)' WARN='$(WARN)' XLIB='$(XLIB)' XINCLUDE='$(XINCLUDE)'

hp:
	cd $(WXDIR)/src/x; $(MAKE) -f makefile.unx hp
	$(MAKE) -f makefile.unx GUI=-Dwx_motif GUISUFFIX=_hp CC=CC DEBUG='$(DEBUG)' WARN='-w' \
         XINCLUDE='$(HPXINCLUDE)' XLIB='$(HPXLIB)' XVIEW_LINK='' LDLIBS='$(HPLDLIBS)'

$(OBJDIR):
	mkdir $(OBJDIR)

$(PROGRAM)$(GUISUFFIX):	$(OBJDIR)/$(PROGRAM).$(OBJSUFF) $(WXLIB)
	$(CC) $(LDFLAGS) -o $(PROGRAM)$(GUISUFFIX) $(OBJDIR)/$(PROGRAM).$(OBJSUFF) $(XVIEW_LINK) $(LDLIBS)
	strip $@

$(OBJDIR)/$(PROGRAM).$(OBJSUFF):	$(PROGRAM).$(SRCSUFF)
	$(CC) -c $(CPPFLAGS) -o $@ $(PROGRAM).$(SRCSUFF)

clean_motif:
	$(MAKE) -f makefile.unx GUISUFFIX=_motif cleanany

clean_ol:
	$(MAKE) -f makefile.unx GUISUFFIX=_ol cleanany

clean_hp:
	$(MAKE) -f makefile.unx GUISUFFIX=_hp cleanany

cleanany:
	rm -f $(OBJECTS) $(PROGRAM)$(GUISUFFIX) core
