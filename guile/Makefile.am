## $Id$
## Process this file with automake to create Makefile.in

## The module's name is (app cassowary constraints) so put it in
## ./cassowary of somewhere off of the GUILE_LOAD_PATH
cassoguiledir = @GUILE_PKGDATA_DIR@/site/cassowary

# This gets put in with libcassowary.a
lib_LIBRARIES = libconstraints.a

# While this gets put where guile will look for it
cassoguile_LTLIBRARIES = libconstraints.la

GUILE_INCLUDES = @GUILE_INCLUDES@

INCLUDES = -I. -I$(top_srcdir)/c++ -I$(prefix)/include -I$(exec_prefix)/include $(GUILE_INCLUDES)

libconstraints_a_SOURCES = cassowary_scm.cc

libconstraints_la_SOURCES = cassowary_scm.cc

libconstraints_a_LIBADD = $(top_builddir)/c++/libcassowary.a

# Below does not -- get error:
# libtool: link: libtool library `libconstraints.la' may not depend on
# uninstalled libraries: ../c++/libcassowary.la
#libconstraints_la_LIBADD = $(top_builddir)/c++/libcassowary.la

# This works, but may require doing a make install from the ../c++ first
# also, guile doesn't load the module properly when I do this
#libconstraints_la_LIBADD = -lcassowary

# The horrid ugly hack that seems to work
# It'd be nice to be able to mention the name of 
# a library from the directory instead of this
libconstraints_la_LIBADD = $(top_builddir)/c++/*.lo

include_HEADERS = \
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
