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

TESTS = test-ClStrength test-ClVariable test-ClLinearExpression test-ClConstraint \
	test-ClSimplexSolver ClTests

JAVAFILES = \
	ClAbstractVariable.java \
	ClDummyVariable.java \
	ClObjectiveVariable.java \
	ClSlackVariable.java \
	ClConstraint.java \
	ClEditConstraint.java \
	ClEditOrStayConstraint.java \
	ClErrors.java \
	ClLinearConstraint.java \
	ClLinearEquation.java \
	ClLinearExpression.java \
	ClLinearInequality.java \
	ClSimplexSolver.java \
	ClStayConstraint.java \
	ClStrength.java \
	ClSymbolicWeight.java \
	ClTableau.java \
	ClVariable.java \
	ClPoint.java \
	ClTests.java \
	Set.java

CLASSFILES = $(JAVAFILES:.java=.class)

%.class: %.java
	$(JAVAC) $<

all:	$(CLASSFILES)

clean:	
	rm -f *.class

TAGS: $(JAVAFILES)
	jtags $^

tags: TAGS

.PHONY:	all cassowary-tests lib tags ntsources tests depend
