# $Id$
#
# Cassowary Incremental Constraint Solver
# Original Smalltalk Implementation by Alan Borning
# This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
# http://www.cs.washington.edu/homes/gjb
# (C) 1998, All Rights Reserved.
# Makefile for cassowary

CC = gcc
CPP = egcs-g++
CFLAGS = -g -Wall
CPPFLAGS = -g -Wall -fno-rtti -L/uns/lib/egcs
CXX = $(CPP)
# -falt-external-templates
LDFLAGS = -L/uns/lib/egcs

TEST_OBJS = ClStrength.o ClSymbolicWeight.o ClLinearExpression.o ClVariable.o ClConstraint.o
TEST_SRCS = $(OBJS:.o=.cc)
TESTS = test-ClStrength test-ClVariable test-ClLinearExpression test-ClConstraint

SOURCES = $(wildcard *.cc)

HEADERS = Cassowary.h ClErrors.h \
	  ClVariable.h \
	  ClConstraint.h ClStrength.h

all:	libcassowary.a cassowary-tests

clean:	
	rm -f $(TESTS) *.o

TAGS: $(SOURCES)
	etags $^

cassowary-tests: $(TESTS)

test-ClStrength: test-ClStrength.o ClStrength.o ClSymbolicWeight.o
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClLinearExpression: test-ClLinearExpression.o $(TEST_OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClVariable: test-ClVariable.o $(TEST_OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClConstraint: test-ClConstraint.o $(TEST_OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

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


.PHONY:	tags all cassowary-tests tests
