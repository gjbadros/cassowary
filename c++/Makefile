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
EGCSDIR = /uns/egcs
CPPFLAGS = -O6 -DNO_TRACE -Wall -L$(EGCSDIR)/lib -I$(EGCSDIR)/include
CXX = $(CPP)
# -falt-external-templates
LDFLAGS = -L/uns/lib/egcs
CL_LIBRARY = libCl.a

OBJS = 	ClAbstractVariable.o \
	ClConstraint.o \
	ClLinearExpression.o \
	ClSimplexSolver.o \
	ClStrength.o \
	ClSymbolicWeight.o \
	ClTableau.o


TEST_OBJS = test-ClConstraint.o   \
	test-ClLinearExpression.o  \
	test-ClSimplexSolver.o     \
	test-ClStrength.o          \
	test-ClVariable.o	   \
	ClTests.o



SOURCES = $(OBJS:.o=.cc)

TEST_SRCS = $(TEST_OBJS:.o=.cc)

TESTS = test-ClStrength test-ClVariable test-ClLinearExpression test-ClConstraint \
	test-ClSimplexSolver ClTests

HEADERS = Cassowary.h \
	Cl.h \
	ClAbstractVariable.h \
	ClDummyVariable.h \
	ClObjectiveVariable.h \
	ClSlackVariable.h \
	ClEditVariable.h \
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

all:	libcassowary.a cassowary-tests $(CL_LIBRARY)

$(CL_LIBRARY): $(OBJS)
	ar ruv $@ $(OBJS)

clean:	
	rm -f $(TESTS) *.o $(CL_LIBRARY)

TAGS: $(SOURCES)
	etags $^

cassowary-tests: $(TESTS)

ClTests: ClTests.o $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClStrength: test-ClStrength.o ClStrength.o ClSymbolicWeight.o
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClLinearExpression: test-ClLinearExpression.o $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClVariable: test-ClVariable.o $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClConstraint: test-ClConstraint.o $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

test-ClSimplexSolver: test-ClSimplexSolver.o $(OBJS)
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
