# $Id$
#
# Cassowary Incremental Constraint Solver
# Original Smalltalk Implementation by Alan Borning
# This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
# http://www.cs.washington.edu/homes/gjb
# (C) 1998, All Rights Reserved.
#
# (C) 1998 Alan Borning and Greg Badros.  This code is provided for use by
# students for course projects in the course CSE 595 in the Department of
# Computer Science and Engineering, University of Washington, during winter
# quarter 1998.  Any other use requires written permission from the copyright
# holders.

# Makefile for cassowary, java version

JAVAC = javac
#JAVAC_FLAGS = -g
JAVAC_FLAGS = -O

TESTS = test-ClStrength test-ClVariable test-ClLinearExpression test-ClConstraint \
	test-ClSimplexSolver ClTests

# For java, keep ClErrors.java first, since it defines multiple
# classes that are used by the others (javac does not get
# dependences right if multiple classes are in one file)
JAVAFILES = \
	ClAbstractVariable.java \
	ClDummyVariable.java \
	ClObjectiveVariable.java \
	ClSlackVariable.java \
	ClConstraint.java \
	ClEditConstraint.java \
	ClEditOrStayConstraint.java \
	ClLinearConstraint.java \
	ClLinearEquation.java \
	ClLinearExpression.java \
	ClLinearInequality.java \
	ClStayConstraint.java \
	ClStrength.java \
	ClSymbolicWeight.java \
	ClVariable.java \
	ClPoint.java \
	ClDouble.java \
	ClTableau.java \
	ClSimplexSolver.java \
	ExCLConstraintNotFound.java \
	ExCLError.java \
	ExCLInternalError.java \
	ExCLNonlinearExpression.java \
	ExCLNotEnoughStays.java \
	ExCLRequiredFailure.java \
	ExCLTooDifficult.java \
	CL.java \
	Set.java \
	Timer.java

TESTFILES = \
	testClStrength.java \
	testClLinearExpression.java \
	ClTests.java

CLASSFILES = $(JAVAFILES:.java=.class)

TESTCLASSFILES = $(TESTFILES:.java=.class)

%.class: $(JAVAFILES) $(TESTFILES)
	$(JAVAC) $(JAVAC_FLAGS) $(JAVAFILES) $(TESTFILES)

all:	$(CLASSFILES)

test:	$(TESTCLASSFILES)

clean:	
	rm -f *.class

TAGS: $(JAVAFILES)
	jtags $^

tags: TAGS

.PHONY:	all cassowary-tests lib tags ntsources tests depend
