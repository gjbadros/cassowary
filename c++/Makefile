# $Id$
#
# Cassowary Incremental Constraint Solver
# Original Smalltalk Implementation by Alan Borning
# This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
# http://www.cs.washington.edu/homes/gjb
# (C) 1998, All Rights Reserved.
# Makefile for cassowary

CC = gcc
CFLAGS = -g

TEST_OBJS = 
TEST_SRCS = $(OBJS:.o=.cc)

HEADERS = Cassowary.h ClErrors.h \
	  ClAbstractVariable.h ClDummyVariable.h ClObjectVariable.h \
	  ClSlackVariable.h ClVariable.h

TAGS: $(SOURCES)
	etags $^

all:	libcassowary.a cassowary-test

cassowary-test:	$(TEST_OBJS)

libcassowary.a:	

tags: TAGS


# Perl-related targets
.man: .pm
	pod2man $< >$@

.html: .pm
	pod2html $< >$@

.texi: .pm
	modpod2texinfo $< >$@


.PHONY:	tags all
