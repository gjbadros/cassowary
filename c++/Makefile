# $Id$
#
# Cassowary Incremental Constraint Solver
# Original Smalltalk Implementation by Alan Borning
# This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
# http://www.cs.washington.edu/homes/gjb
# (C) 1998, 1999 Alan Borning and Greg Badros
# See ../LICENSE for legal details regarding this software

# Makefile for cassowary

CC = gcc
CPP = g++
CPP_SO_LINK = g++ -shared
CFLAGS = -g -Wall
EGCSDIR = /uns/egcs
CXX = $(CPP)
FLEX = flex
BISON = bison
SWIG = swig
PYTHON_INCLUDE = /uns/include/python1.5

#FLEX_FLAGS = -d
FLEX_FLAGS = 


## NOTE: the egcs-1.1b release's g++ will get a parse error
## with -pedantic -- just remove that option for compilation
## with egcs-1.1b, until they fix the problem (bug was reported --09/06/98 gjb)

CPPFLAGS = -Wall -pedantic -W

CPPFLAGS += -DCL_SOLVER_CHECK_INTEGRITY

#CPPFLAGS += -O6 -NDEBUG
#CPPFLAGS += -O6
#CPPFLAGS += -O2
CPPFLAGS += -g

#CPPFLAGS += -DCL_USE_HASH_MAP_AND_SET

#CPPFLAGS += -DCL_TRACE

#CPPFLAGS += -DCL_SOLVER_STATS

#CPPFLAGS += -DCL_NO_IO

#OTHER_LIBS = 
OTHER_LIBS = ../../gc/gc.a

#LIB=cassowary-optimized
LIB=cassowary-notrace
#LIB=cassowary-trace
#LIB=cassowary-debug
#LIB=cassowary
LIB_FILE=lib$(LIB).a

LDLIBS = -l$(LIB)

#LDFLAGS = -L. -pg
LDFLAGS = -L.

CL_LIBRARY = lib$(LIB).a
CL_SHARED_LIBRARY = lib$(LIB).so

OBJS = 	ClAbstractVariable.o \
	ClConstraint.o \
	ClLinearExpression.o \
	ClSimplexSolver.o \
	ClStrength.o \
	ClSymbolicWeight.o \
	ClTableau.o \
	ClVariable.o \
	ClSlackVariable.o \
	ClDummyVariable.o


TEST_OBJS = test-ClConstraint.o    \
	test-ClLinearExpression.o  \
	test-ClSimplexSolver.o     \
	test-ClStrength.o          \
	test-ClVariable.o	   \
	test-ClBounds.o	           \
	ClTests.o                  \
	ClBug0.o                  \
	ClBug1.o                  \
	ClLeakTest.o		   \
	ClParseTest.o              \
	ClSubgraphTest.o           \
	ClTestColumns.o



SOURCES = $(OBJS:.o=.cc)

TEST_SRCS = $(TEST_OBJS:.o=.cc)

NT_SOURCES = $(OBJS:.o=.cpp) $(TEST_OBJS:.o=.cpp)

TESTS = test-ClStrength test-ClVariable test-ClLinearExpression test-ClConstraint \
	test-ClSimplexSolver ClTests ClBug0 ClBug1 ClLeakTest ClSubgraphTest ClParseTest

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

all:	lib ClTests ClLeakTest ClTestColumns ClSubgraphTest $(bugtests)

bugtests: ClBug0 ClBug1

lib:    $(CL_LIBRARY)

shared_lib:	$(CL_SHARED_LIBRARY)

$(CL_LIBRARY): $(OBJS)
	rm -f $@
	ar ruv $@ $(OBJS)

$(CL_SHARED_LIBRARY): $(OBJS)
	rm -f $@
	$(CPP_SO_LINK) -o $@ $(OBJS)

clean:	
	rm -f $(TESTS) *.o
	@echo "Consider doing a \`make depend'"

TAGS: $(SOURCES) $(HEADERS)
	etags $^

cassowary-tests: $(TESTS)

%: %.o $(LIB_FILE) $(OTHER_LIBS)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS)

cassowary-test:	$(TEST_OBJS)

creader.cc: creader.y
	$(BISON) -d -pcl --output-file $@ $^

creader-lex.cc: creader.l
	$(FLEX) $(FLEX_FLAGS) -Pcl -o$@ $^ 

ClParseTest: $(LIB_FILE) $(OTHER_LIBS) ClParseTest.o creader-lex.o creader.o
	$(CPP) $(LDFLAGS) -o $@ $^ $(LDLIBS) -lfl

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


.PHONY:	all bugtests cassowary-tests lib shared_lib tags ntsources tests depend 

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
