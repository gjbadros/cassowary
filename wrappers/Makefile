# $Id$
#
# Makefile for cassowary/wrappers
#
#
# Cassowary Incremental Constraint Solver
# Original Smalltalk Implementation by Alan Borning
# This Makefile by Greg J. Badros, <gjb@cs.washington.edu>
# http://www.cs.washington.edu/homes/gjb
# (C) 1998 Alan Borning and Greg Badros
# See ../COPYRIGHT for legal details regarding this software
#
# Python wrapper by Tessa Lau <tlau@cs.washington.edu>

CC = gcc
CPP = g++
CFLAGS = -g -Wall -I../c++
EGCSDIR = /uns/egcs
CXX = $(CPP)
SWIG = swig
#PYTHON_INCLUDE = /uns/share/include/python1.5 -I/uns/include/python1.5
PYTHON_INCLUDE = /usr/include/python1.4

LIB=cassowary
LIB_FILE=../c++/lib$(LIB).a

LDLIBS = -l$(LIB)

#LDFLAGS = -L. -pg
LDFLAGS = -L../c++

CL_LIBRARY = ../c++/lib$(LIB).a


# swig-python, swig-guile, and module are phony targets

all: module

swig-python: cassowary.py cassowary_wrap.c cassowary_wrap.doc

cassowary.py cassowary_wrap.c cassowary_wrap.doc: cassowary.i
	$(SWIG) -c++ -python -shadow cassowary.i

cassowary_wrap.o: cassowary_wrap.c
	$(CPP) $(CFLAGS) -I$(PYTHON_INCLUDE) -fpic -c cassowary_wrap.c

#swig-guile: cassowary.i
#	$(SWIG) -c++ -guile cassowary.i

module: cassowarycmodule.so

cassowarycmodule.so: cassowary_wrap.o $(CL_LIBRARY)
	$(CPP) $(CFLAGS) -shared -o $@ $^

clean: 
	rm cassowary.py cassowary_wrap.c cassowary_wrap.doc cassowary_wrap.o cassowarycmodule.so

.PHONY: all clean module swig-python
