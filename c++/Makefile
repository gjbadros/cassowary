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
CPPFLAGS = -g -Wall -L/uns/lib/egcs
CXX = $(CPP)
# -falt-external-templates
LDFLAGS = -L/uns/lib/egcs

TEST_OBJS = ClStrength.o ClSymbolicWeight.o ClLinearExpression.o \
		 ClConstraint.o ClTableau.o ClSimplexSolver.o ClAbstractVariable.o
TEST_SRCS = $(OBJS:.o=.cc)
TESTS = test-ClStrength test-ClVariable test-ClLinearExpression test-ClConstraint \
	test-ClSimplexSolver

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

ClAbstractVariable.o: ClAbstractVariable.h /usr/include/stdio.h
ClAbstractVariable.o: /usr/include/libio.h /usr/include/_G_config.h
ClAbstractVariable.o: /usr/include/sys/cdefs.h Cassowary.h
ClAbstractVariable.o: /usr/include/math.h /usr/include/features.h
ClAbstractVariable.o: /usr/include/huge_val.h /usr/include/endian.h
ClAbstractVariable.o: /usr/include/bytesex.h /usr/include/nan.h
ClAbstractVariable.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
ClAbstractVariable.o: /usr/include/values.h /usr/include/ieee754.h
ClAbstractVariable.o: /usr/include/ieee854.h /usr/include/assert.h
ClConstraint.o: ClConstraint.h Cassowary.h /usr/include/math.h
ClConstraint.o: /usr/include/features.h /usr/include/sys/cdefs.h
ClConstraint.o: /usr/include/huge_val.h /usr/include/endian.h
ClConstraint.o: /usr/include/bytesex.h /usr/include/nan.h
ClConstraint.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
ClConstraint.o: /usr/include/values.h /usr/include/ieee754.h
ClConstraint.o: /usr/include/ieee854.h ClLinearExpression.h debug.h
ClConstraint.o: ClAbstractVariable.h /usr/include/stdio.h
ClConstraint.o: /usr/include/libio.h /usr/include/_G_config.h ClStrength.h
ClConstraint.o: ClSymbolicWeight.h
ClLinearExpression.o: /usr/include/assert.h /usr/include/features.h
ClLinearExpression.o: /usr/include/sys/cdefs.h ClLinearExpression.h debug.h
ClLinearExpression.o: ClAbstractVariable.h /usr/include/stdio.h
ClLinearExpression.o: /usr/include/libio.h /usr/include/_G_config.h
ClLinearExpression.o: Cassowary.h /usr/include/math.h /usr/include/huge_val.h
ClLinearExpression.o: /usr/include/endian.h /usr/include/bytesex.h
ClLinearExpression.o: /usr/include/nan.h
ClLinearExpression.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
ClLinearExpression.o: /usr/include/values.h /usr/include/ieee754.h
ClLinearExpression.o: /usr/include/ieee854.h ClTableau.h ClErrors.h
ClSimplexSolver.o: debug.h ClSimplexSolver.h Cassowary.h /usr/include/math.h
ClSimplexSolver.o: /usr/include/features.h /usr/include/sys/cdefs.h
ClSimplexSolver.o: /usr/include/huge_val.h /usr/include/endian.h
ClSimplexSolver.o: /usr/include/bytesex.h /usr/include/nan.h
ClSimplexSolver.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
ClSimplexSolver.o: /usr/include/values.h /usr/include/ieee754.h
ClSimplexSolver.o: /usr/include/ieee854.h ClTableau.h ClLinearExpression.h
ClSimplexSolver.o: ClAbstractVariable.h /usr/include/stdio.h
ClSimplexSolver.o: /usr/include/libio.h /usr/include/_G_config.h
ClSimplexSolver.o: ClLinearInequality.h ClLinearConstraint.h ClConstraint.h
ClSimplexSolver.o: ClStrength.h ClSymbolicWeight.h ClStayConstraint.h
ClSimplexSolver.o: ClEditOrStayConstraint.h ClObjectiveVariable.h ClErrors.h
ClSimplexSolver.o: ClVariable.h /usr/include/assert.h ClSlackVariable.h
ClSimplexSolver.o: ClDummyVariable.h
ClStrength.o: ClStrength.h Cassowary.h /usr/include/math.h
ClStrength.o: /usr/include/features.h /usr/include/sys/cdefs.h
ClStrength.o: /usr/include/huge_val.h /usr/include/endian.h
ClStrength.o: /usr/include/bytesex.h /usr/include/nan.h
ClStrength.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
ClStrength.o: /usr/include/values.h /usr/include/ieee754.h
ClStrength.o: /usr/include/ieee854.h ClSymbolicWeight.h /usr/include/assert.h
ClSymbolicWeight.o: Cassowary.h /usr/include/math.h /usr/include/features.h
ClSymbolicWeight.o: /usr/include/sys/cdefs.h /usr/include/huge_val.h
ClSymbolicWeight.o: /usr/include/endian.h /usr/include/bytesex.h
ClSymbolicWeight.o: /usr/include/nan.h
ClSymbolicWeight.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
ClSymbolicWeight.o: /usr/include/values.h /usr/include/ieee754.h
ClSymbolicWeight.o: /usr/include/ieee854.h ClSymbolicWeight.h
ClSymbolicWeight.o: /usr/include/assert.h
ClTableau.o: ClTableau.h Cassowary.h /usr/include/math.h
ClTableau.o: /usr/include/features.h /usr/include/sys/cdefs.h
ClTableau.o: /usr/include/huge_val.h /usr/include/endian.h
ClTableau.o: /usr/include/bytesex.h /usr/include/nan.h
ClTableau.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
ClTableau.o: /usr/include/values.h /usr/include/ieee754.h
ClTableau.o: /usr/include/ieee854.h ClLinearExpression.h debug.h
ClTableau.o: ClAbstractVariable.h /usr/include/stdio.h /usr/include/libio.h
ClTableau.o: /usr/include/_G_config.h
ClVariable.o: ClVariable.h /usr/include/stdio.h /usr/include/libio.h
ClVariable.o: /usr/include/_G_config.h /usr/include/sys/cdefs.h
ClVariable.o: /usr/include/assert.h /usr/include/features.h Cassowary.h
ClVariable.o: /usr/include/math.h /usr/include/huge_val.h
ClVariable.o: /usr/include/endian.h /usr/include/bytesex.h /usr/include/nan.h
ClVariable.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
ClVariable.o: /usr/include/values.h /usr/include/ieee754.h
ClVariable.o: /usr/include/ieee854.h ClAbstractVariable.h
test-ClConstraint.o: Cassowary.h /usr/include/math.h /usr/include/features.h
test-ClConstraint.o: /usr/include/sys/cdefs.h /usr/include/huge_val.h
test-ClConstraint.o: /usr/include/endian.h /usr/include/bytesex.h
test-ClConstraint.o: /usr/include/nan.h
test-ClConstraint.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
test-ClConstraint.o: /usr/include/values.h /usr/include/ieee754.h
test-ClConstraint.o: /usr/include/ieee854.h ClVariable.h /usr/include/stdio.h
test-ClConstraint.o: /usr/include/libio.h /usr/include/_G_config.h
test-ClConstraint.o: /usr/include/assert.h ClAbstractVariable.h
test-ClConstraint.o: ClLinearEquation.h ClLinearConstraint.h ClConstraint.h
test-ClConstraint.o: ClLinearExpression.h debug.h ClStrength.h
test-ClConstraint.o: ClSymbolicWeight.h ClLinearInequality.h
test-ClConstraint.o: ClStayConstraint.h ClEditOrStayConstraint.h
test-ClLinearExpression.o: Cassowary.h /usr/include/math.h
test-ClLinearExpression.o: /usr/include/features.h /usr/include/sys/cdefs.h
test-ClLinearExpression.o: /usr/include/huge_val.h /usr/include/endian.h
test-ClLinearExpression.o: /usr/include/bytesex.h /usr/include/nan.h
test-ClLinearExpression.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
test-ClLinearExpression.o: /usr/include/values.h /usr/include/ieee754.h
test-ClLinearExpression.o: /usr/include/ieee854.h ClVariable.h
test-ClLinearExpression.o: /usr/include/stdio.h /usr/include/libio.h
test-ClLinearExpression.o: /usr/include/_G_config.h /usr/include/assert.h
test-ClLinearExpression.o: ClAbstractVariable.h ClLinearExpression.h debug.h
test-ClLinearExpression.o: ClSimplexSolver.h ClTableau.h ClLinearInequality.h
test-ClLinearExpression.o: ClLinearConstraint.h ClConstraint.h ClStrength.h
test-ClLinearExpression.o: ClSymbolicWeight.h ClStayConstraint.h
test-ClLinearExpression.o: ClEditOrStayConstraint.h ClObjectiveVariable.h
test-ClSimplexSolver.o: ClErrors.h Cassowary.h /usr/include/math.h
test-ClSimplexSolver.o: /usr/include/features.h /usr/include/sys/cdefs.h
test-ClSimplexSolver.o: /usr/include/huge_val.h /usr/include/endian.h
test-ClSimplexSolver.o: /usr/include/bytesex.h /usr/include/nan.h
test-ClSimplexSolver.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
test-ClSimplexSolver.o: /usr/include/values.h /usr/include/ieee754.h
test-ClSimplexSolver.o: /usr/include/ieee854.h ClLinearEquation.h
test-ClSimplexSolver.o: ClLinearConstraint.h ClConstraint.h
test-ClSimplexSolver.o: ClLinearExpression.h debug.h ClAbstractVariable.h
test-ClSimplexSolver.o: /usr/include/stdio.h /usr/include/libio.h
test-ClSimplexSolver.o: /usr/include/_G_config.h ClStrength.h
test-ClSimplexSolver.o: ClSymbolicWeight.h ClLinearInequality.h
test-ClSimplexSolver.o: ClEditConstraint.h ClEditOrStayConstraint.h
test-ClSimplexSolver.o: ClStayConstraint.h ClSimplexSolver.h ClTableau.h
test-ClSimplexSolver.o: ClObjectiveVariable.h ClVariable.h
test-ClSimplexSolver.o: /usr/include/assert.h
test-ClStrength.o: Cassowary.h /usr/include/math.h /usr/include/features.h
test-ClStrength.o: /usr/include/sys/cdefs.h /usr/include/huge_val.h
test-ClStrength.o: /usr/include/endian.h /usr/include/bytesex.h
test-ClStrength.o: /usr/include/nan.h
test-ClStrength.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
test-ClStrength.o: /usr/include/values.h /usr/include/ieee754.h
test-ClStrength.o: /usr/include/ieee854.h ClStrength.h ClSymbolicWeight.h
test-ClVariable.o: ClVariable.h /usr/include/stdio.h /usr/include/libio.h
test-ClVariable.o: /usr/include/_G_config.h /usr/include/sys/cdefs.h
test-ClVariable.o: /usr/include/assert.h /usr/include/features.h Cassowary.h
test-ClVariable.o: /usr/include/math.h /usr/include/huge_val.h
test-ClVariable.o: /usr/include/endian.h /usr/include/bytesex.h
test-ClVariable.o: /usr/include/nan.h
test-ClVariable.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/float.h
test-ClVariable.o: /usr/include/values.h /usr/include/ieee754.h
test-ClVariable.o: /usr/include/ieee854.h ClAbstractVariable.h
