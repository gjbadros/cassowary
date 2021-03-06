## $Id$
## Process this file with automake to create Makefile.in

lib_LTLIBRARIES = libcassowary.la

libcassowary_la_LDFLAGS = -version-info 0:1:0

INCLUDES = @GUILE_INCLUDES@ @GTL_INCLUDES@
LDADD = -L@prefix@/lib @GTL_LIB@ libcassowary.la

benchdir=$(libdir)/cassowary/bench
bugsdir=$(libdir)/cassowary/bugs
testdir=$(libdir)/cassowary/tests
moretestdir=$(libdir)/cassowary/tests

AM_CPPFLAGS = $(CPPEXTRAFLAGS)

EXTRA_PROGRAMS = $(test_programs) $(bench_programs) $(bugs_programs)

EXTRA_DIST = ClReader.l ClReader.y

## Only get built if --enable-cxx-tests is given
bench_programs = QocaBench

if CL_BUILD_FD_SOLVER
test_fd_solver = ClFDTests
else
test_fd_solver = 
endif

if CL_BUILD_FD_SOLVER
sources_for_fd_solver = \
	ClFDSolver.cc \
	ClFDVariable.cc \
	ClFDConnectorVariable.cc \
	ClFDBinaryOneWayConstraint.cc
headers_for_fd_solver = ClFDBinaryOneWayConstraint.h ClFDConstraint.h ClFDSolver.h
else
sources_for_fd_solver =
headers_for_fd_solver =
endif

## Only get built if --enable-cxx-tests is given
test_programs = \
	ClParseTest \
	ClSubgraphTest \
	ClCTest \
	ClLeakTest \
	test-ClConstraint \
	test-ClLinearExpression \
	test-ClSimplexSolver \
	test-ClStrength \
	test-ClVariable \
	$(test_fd_solver)

test_PROGRAMS = ClTests

ClCTest_LDADD = $(LDADD) -lstdc++

ClTests_SOURCES = ClTests.cc
ClTests_LDADD = $(LDADD) -lstdc++

if CL_BUILD_TESTS
moretest_PROGRAMS = @TEST_PROGRAMS@
bench_PROGRAMS = @BENCH_PROGRAMS@
endif

bugs_programs = \
	ClBug0 \
	ClBug1 \
	ClBug2

ClBug0_SOURCES = ClBug0.cc

ClBug1_SOURCES = ClBug1.cc

ClBug2_SOURCES = ClBug2.cc

sources = \
	ClAbstractVariable.cc \
	ClConstraint.cc \
	ClLinearExpression.cc \
	ClSolver.cc \
	ClSimplexSolver.cc \
	ClStrength.cc \
	ClSymbolicWeight.cc \
	ClTableau.cc \
	ClVariable.cc \
	ClFloatVariable.cc \
	ClSlackVariable.cc \
	ClDummyVariable.cc \
	ClReader-lex.cc \
	ClReader.cc \
	ClC.cc

libcassowary_la_SOURCES = \
	$(sources) \
	$(sources_for_fd_solver)

include_HEADERS = Cl.h ClC.h

pkginclude_HEADERS = \
	Cassowary.h \
	ClAbstractVariable.h \
	ClDummyVariable.h \
	ClObjectiveVariable.h \
	ClSlackVariable.h \
	ClConstraint.h \
	ClConstraintHash.h \
	ClEditConstraint.h \
	ClEditOrStayConstraint.h \
	ClErrors.h \
	ClLinearConstraint.h \
	ClLinearEquation.h \
	ClLinearExpression.h \
	ClLinearExpression_fwd.h \
	ClLinearInequality.h \
	ClSolver.h \
	ClSimplexSolver.h \
	ClTimedSimplexSolver.h \
	ClStayConstraint.h \
	ClStrength.h \
	ClSymbolicWeight.h \
	ClTableau.h \
	ClFDVariable.h \
	ClFDConnectorVariable.h \
	ClFloatVariable.h \
	ClVariable.h \
	ClReader.h \
	ClTypedefs.h \
	ClMap.h \
	ClSet.h \
	ClPoint.h \
	cl_auto_ptr.h \
	config-inline.h \
	debug.h \
	timer.h \
	$(headers_for_fd_solver)



if CL_BUILD_TESTS
ClFDTests_SOURCES = ClFDTests.cc

QocaBench_SOURCES = QocaBench.cc

ClParseTest_SOURCES = ClParseTest.cc

ClSubgraphTest_SOURCES = ClSubgraphTest.cc

ClLeakTest_SOURCES = ClLeakTest.cc

test_ClConstraint_SOURCES = test-ClConstraint.cc

test_ClLinearExpression_SOURCES = test-ClLinearExpression.cc

test_ClSimplexSolver_SOURCES = test-ClSimplexSolver.cc

test_ClStrength_SOURCES = test-ClStrength.cc

test_ClVariable_SOURCES = test-ClVariable.cc
endif

ClReader-lex.cc: ClReader.l ClReader.h ClReader.hh
	$(LEX) -Pcl -o$@ $<

ClReader.cc ClReader.hh: ClReader.y ClReader.h
	$(YACC) -d -pcl --output-file ClReader.cc $<

cassowary/config-inline.h: cassowary
