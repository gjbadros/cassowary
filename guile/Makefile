# $Id$
# Makefile for cassowary_scm
# Guile wrappers for Cassowary Constraint Solving Toolkit
#

CPP = g++
CFLAGS = -Wall -g

INCLUDES = -I. -I../c++ -I/uns/share/include -I/uns/include

cassowary_scm.o: cassowary_scm.cc cassowary_scm.x
	$(CPP) $(CFLAGS) $(INCLUDES) -c $<

cassowary_scm.x: cassowary_scm.cc
	CC=$(CPP) guile-snarf $(INCLUDES)  $<  >$@
