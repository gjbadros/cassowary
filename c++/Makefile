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
# Below line is for egcs-1.0
#CPPFLAGS = -g -Wall -L/uns/lib/egcs
# Below line is for egcs-1.01 -- this makes exceptions work for me!
CPPFLAGS = -O6 -DNO_TRACE -Wall -L/uns/egcs-1.01/lib -I/uns/egcs-1.01/include
CXX = $(CPP)
# -falt-external-templates
LDFLAGS = -L/uns/lib/egcs

TEST_OBJS = ClStrength.o ClSymbolicWeight.o ClLinearExpression.o \
		 ClConstraint.o ClTableau.o ClSimplexSolver.o ClAbstractVariable.o
TEST_SRCS = $(OBJS:.o=.cc)
TESTS = test-ClStrength test-ClVariable test-ClLinearExpression test-ClConstraint \
	test-ClSimplexSolver ClTests

SOURCES = $(wildcard *.cc)

HEADERS = Cassowary.h \
	ClConstraint.h \
	ClEditConstraint.h \
	ClEditOrStayConstraint.h \
	ClErrors.h \
	ClLinearConstraint.h \
	ClLinearEquation.h \
	ClLinearExpression.h \
	ClLinearInequality.h \
	ClSimplexSolver.h \
	ClStayConstraint.h \
	ClStrength.h \
	ClSymbolicWeight.h \
	ClTableau.h \
	ClVariable.h

all:	libcassowary.a cassowary-tests

clean:	
	rm -f $(TESTS) *.o

TAGS: $(SOURCES)
	etags $^

cassowary-tests: $(TESTS)

ClTests: ClTests.o $(TEST_OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClStrength: test-ClStrength.o ClStrength.o ClSymbolicWeight.o
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClLinearExpression: test-ClLinearExpression.o $(TEST_OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClVariable: test-ClVariable.o $(TEST_OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClConstraint: test-ClConstraint.o $(TEST_OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClSimplexSolver: test-ClSimplexSolver.o $(TEST_OBJS)
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


.PHONY:	tags all cassowary-tests tests depend



# How to make dependencies:
# -------------------------
depend: $(TEST_SRCS) $(HEADERS)
	makedepend *.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
