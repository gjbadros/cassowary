# $Id$
# Makefile for cassowary_scm
# Guile wrappers for Cassowary Constraint Solving Toolkit
#

CPP = g++
CFLAGS = -Wall -g

# See SCWM distribution for the extract-docs script
# http://web.mit.edu/mstachow/www/scwm.html

EXTRACT_DOCS = ../../scwm/utilities/dev/extract-docs

INCLUDES = -I. -I../c++ -I/uns/share/include -I/uns/include

cassowary_scm.o: cassowary_scm.cc cassowary_scm.x
	$(CPP) $(CFLAGS) $(INCLUDES) -c $<

cassowary_scm.x: cassowary_scm.cc
	CC=$(CPP) guile-snarf $(INCLUDES)  $<  >$@

cassowary_scm.sgml: cassowary_scm.cc
	 $(EXTRACT_DOCS) $^ > $@

clean:
	rm -f cassowary_scm.o cassowary_scm.x cassowary_scm.sgml

.PHONY: clean
