## $Id$
## Process this file with automake to create Makefile.in

lib_LIBRARIES = libcassowary.a
LDADD = libcassowary.a

benchdir=$(libdir)/bench
bugsdir=$(libdir)/bench
testdir=$(libdir)/test

bench_PROGRAMS = ClTests QocaBench

test_PROGRAMS = \
	ClParseTest \
	ClSubgraphTest \
	ClLeakTest \
	test-ClConstraint \
	test-ClLinearExpression \
	test-ClSimplexSolver \
	test-ClStrength \
	test-ClVariable



bugs_PROGRAMS = \
	ClBug0 \
	ClBug1 \
	ClBug2



libcassowary_a_SOURCES = \
	ClAbstractVariable.cc \
	ClConstraint.cc \
	ClLinearExpression.cc \
	ClSimplexSolver.cc \
	ClStrength.cc \
	ClSymbolicWeight.cc \
	ClTableau.cc \
	ClVariable.cc \
	ClFloatVariable.cc \
	ClSlackVariable.cc \
	ClDummyVariable.cc \
	ClReader-lex.cc \
	ClReader.cc


cassoincludedir = $(includedir)/cassowary

pkginclude_HEADERS = Cl.h ClC.h

cassoinclude_HEADERS = \
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
	ClSimplexSolver.h \
	ClStayConstraint.h \
	ClStrength.h \
	ClSymbolicWeight.h \
	ClTableau.h \
	ClFloatVariable.h \
	ClVariable.h \
	ClReader.h \
	ClTypedefs.h \
	ClMap.h \
	ClSet.h \
	ClPoint.h \
	auto_ptr.h \
	debug.h \


ClTests_SOURCES = ClTests.cc

QocaBench_SOURCES = QocaBench.cc

ClParseTest_SOURCES = ClParseTest.cc

ClSubgraphTest_SOURCES = ClSubgraphTest.cc

ClLeakTest_SOURCES = ClLeakTest.cc

test_ClConstraint_SOURCES = test-ClConstraint.cc

test_ClLinearExpression_SOURCES = test-ClLinearExpression.cc

test_ClSimplexSolver_SOURCES = test-ClSimplexSolver.cc

test_ClStrength_SOURCES = test-ClStrength.cc

test_ClVariable_SOURCES = test-ClVariable.cc


ClBug0_SOURCES = ClBug0.cc

ClBug1_SOURCES = ClBug1.cc

ClBug2_SOURCES = ClBug2.cc

