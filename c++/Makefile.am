## $Id$
## Process this file with automake to create Makefile.in

lib_LIBRARIES = libcassowary.a
lib_LTLIBRARIES = libcassowary.la

INCLUDES = @GUILE_INCLUDES@
LDADD = libcassowary.a -L@prefix@/lib -lGTL
ClCTest_LDADD = $(LDADD) -lstdc++

benchdir=$(libdir)/cassowary/bench
bugsdir=$(libdir)/cassowary/bugs
testdir=$(libdir)/cassowary/tests

bench_PROGRAMS = ClTests QocaBench

test_PROGRAMS = \
	ClParseTest \
	ClSubgraphTest \
	ClCTest \
	ClLeakTest \
	ClFDTests \
	test-ClConstraint \
	test-ClLinearExpression \
	test-ClSimplexSolver \
	test-ClStrength \
	test-ClVariable



#bugs_PROGRAMS = \
#	ClBug0 \
#	ClBug1 \
#	ClBug2

#ClBug0_SOURCES = ClBug0.cc

#ClBug1_SOURCES = ClBug1.cc

#ClBug2_SOURCES = ClBug2.cc


libcassowary_a_SOURCES = \
	ClAbstractVariable.cc \
	ClConstraint.cc \
	ClLinearExpression.cc \
	ClSolver.cc \
	ClSimplexSolver.cc \
	ClFDSolver.cc \
	ClStrength.cc \
	ClSymbolicWeight.cc \
	ClTableau.cc \
	ClVariable.cc \
	ClFDVariable.cc \
	ClFDBinaryOneWayConstraint.cc \
	ClFloatVariable.cc \
	ClSlackVariable.cc \
	ClDummyVariable.cc \
	ClReader-lex.cc \
	ClReader.cc \
	ClC.cc

libcassowary_la_SOURCES = \
	ClAbstractVariable.cc \
	ClConstraint.cc \
	ClLinearExpression.cc \
	ClSolver.cc \
	ClSimplexSolver.cc \
	ClFDSolver.cc \
	ClStrength.cc \
	ClSymbolicWeight.cc \
	ClTableau.cc \
	ClVariable.cc \
	ClFDVariable.cc \
	ClFDBinaryOneWayConstraint.cc \
	ClFloatVariable.cc \
	ClSlackVariable.cc \
	ClDummyVariable.cc \
	ClReader-lex.cc \
	ClReader.cc \
	ClC.cc

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
	ClFDSolver.h \
	ClFDBinaryOneWayConstraint.h \
	ClFDConstraint.h \
	ClStayConstraint.h \
	ClStrength.h \
	ClSymbolicWeight.h \
	ClTableau.h \
	ClFDVariable.h \
	ClFloatVariable.h \
	ClVariable.h \
	ClReader.h \
	ClTypedefs.h \
	ClMap.h \
	ClSet.h \
	ClPoint.h \
	auto_ptr.h \
	config-inline.h \
	debug.h


ClTests_SOURCES = ClTests.cc

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


ClReader-lex.cc: ClReader.l ClReader.h
	$(LEX) -Pcl -o$@ $<

ClReader-lex.cc: ClReader.cc.h

ClReader.cc: ClReader.y ClReader.h
	$(YACC) -d -pcl --output-file $@ $<

ClReader.cc.h: ClReader.cc


config-inline.h: config.h
	grep '#define CL_' $^ > $@ || true

