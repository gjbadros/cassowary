## $Id$
## Process this file with automake to create Makefile.in

if HAVE_PYTHON
PYTHON_SUBDIRS = wrappers
else
PYTHON_SUBDIRS =
endif

if HAVE_JAVA
JAVA_SUBDIRS = java
else
JAVA_SUBDIRS =
endif

if HAVE_GUILE
GUILE_SUBDIRS = guile
else
GUILE_SUBDIRS =
endif

if HAVE_CPP
CPP_SUBDIRS = c++
else
CPP_SUBDIRS =
endif

SUBDIRS = \
 $(CPP_SUBDIRS) \
 $(JAVA_SUBDIRS) \
 $(GUILE_SUBDIRS) \
 $(PYTHON_SUBDIRS) \
 docs

EXTRA_DIST = README LICENSE AUTHORS THANKS IMPORTANT NEWS
