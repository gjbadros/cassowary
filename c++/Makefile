# $Id$
#
# Cassowary Incremental Constraint Solver
# Original Smalltalk Implementation by Alan Borning
# This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
# http://www.cs.washington.edu/homes/gjb
# (C) 1998 Alan Borning and Greg Badros
# See ../COPYRIGHT for legal details regarding this software

# Makefile for cassowary

CC = gcc
CPP = g++
CFLAGS = -g -Wall
EGCSDIR = /uns/egcs
CXX = $(CPP)
SWIG = swig
PYTHON_INCLUDE = /uns/include/python1.5

#CPPFLAGS = -O6 -DCL_NO_TRACE -DNO_SOLVER_STATS -DNDEBUG -Wall -pedantic -W
#CPPFLAGS = -O6 -DCL_NO_TRACE -DNO_SOLVER_STATS -Wall -pedantic -W
#CPPFLAGS = -O6 -DNO_REQUIRED_EXCEPTIONS -DCL_NO_TRACE -Wall -pedantic -W
#CPPFLAGS = -O6 -pg -DCL_NO_TRACE -Wall -pedantic -W
CPPFLAGS = -g -DCL_NO_TRACE -DNO_SOLVER_STATS -Wall -pedantic -W
#CPPFLAGS = -g -Wall -pedantic -W

#LIB=cassowary-optimized
#LIB=cassowary-notrace
#LIB=cassowary-trace
LIB=cassowary-debug
#LIB=cassowary
LIB_FILE=lib$(LIB).a

LDLIBS = -l$(LIB)

#LDFLAGS = -L. -pg
LDFLAGS = -L.

CL_LIBRARY = lib$(LIB).a

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
	test-ClBounds.o	           \
	ClTests.o \
	ClTestColumns.o



SOURCES = $(OBJS:.o=.cc)

TEST_SRCS = $(TEST_OBJS:.o=.cc)

NT_SOURCES = $(OBJS:.o=.cpp) $(TEST_OBJS:.o=.cpp)

TESTS = test-ClStrength test-ClVariable test-ClLinearExpression test-ClConstraint \
	test-ClSimplexSolver ClTests

HEADERS = Cassowary.h \
	Cl.h \
	ClAbstractVariable.h \
	ClDummyVariable.h \
	ClObjectiveVariable.h \
	ClSlackVariable.h \
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
	ClVariable.h \
	ClPoint.h

all:	lib ClTests ClTestColumns

lib:    $(CL_LIBRARY)

$(CL_LIBRARY): $(OBJS)
	rm -f $@
	ar ruv $@ $(OBJS)

clean:	
	rm -f $(TESTS) *.o $(CL_LIBRARY)
	@echo "Consider doing a \`make depend'"

TAGS: $(SOURCES) $(HEADERS)
	etags $^

cassowary-tests: $(TESTS)

ClTests: ClTests.o $(LIB_FILE)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

ClTestColumns: ClTestColumns.o $(LIB_FILE)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

test-ClStrength: test-ClStrength.o ClStrength.o ClSymbolicWeight.o $(LIB_FILE)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

test-ClLinearExpression: test-ClLinearExpression.o $(LIB_FILE)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

test-ClVariable: test-ClVariable.o $(LIB_FILE)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

test-ClConstraint: test-ClConstraint.o $(LIB_FILE)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

test-ClBounds: test-ClBounds.o $(LIB_FILE)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

test-ClSimplexSolver: test-ClSimplexSolver.o $(LIB_FILE)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

cassowary-test:	$(TEST_OBJS)

libcassowary.a:	

tags: TAGS

ntsources: $(NT_SOURCES)

# Perl-related targets
.man: .pm
	pod2man $< >$@

.html: .pm
	pod2html $< >$@

.texi: .pm
	modpod2texinfo $< >$@


.PHONY:	all cassowary-tests lib tags ntsources tests depend 

%.cpp: %.cc
	ln -s $< $@

# How to make dependencies:
# -------------------------
# (Can add Makefile.dependencies to the left of below colon
#  to auto-build dependencies)
depend: $(TEST_SRCS) $(SOURCES) $(HEADERS)
	rm -f Makefile.dependencies
	$(CPP) -M $(TEST_SRCS) $(SOURCES) >> Makefile.dependencies

-include Makefile.dependencies
