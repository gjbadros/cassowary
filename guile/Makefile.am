## $Id$
## Process this file with automake to create Makefile.in

lib_LIBRARIES = libcassowaryguile.a

GUILE_INCLUDES = @GUILE_INCLUDES@

INCLUDES = -I. -I../c++ -I$(prefix)/include -I$(exec_prefix)/include $(GUILE_INCLUDES)

libcassowaryguile_a_SOURCES = \
	cassowary_scm.cc

noinst_HEADERS = \
	cassowary_scm.h \
	cassowary_scm.hpp \
	cl-snarf.h

guile_snarfs=cassowary_scm.x

BUILT_SOURCES=$(guile_snarfs)

# just use `make CC=g++' and the revised guile-snarf which uses ${CC} will work
# for C++ prototype extraction
%.x: %.c
	guile-snarf $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $< > $@

%.x: %.cc
	CC=$(CXX) \
	guile-snarf $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CXXFLAGS) $< > $@
