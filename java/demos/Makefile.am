## $Id$
## Process this file with automake to create Makefile.in

JAVAFILES = DraggableBox.java QuadDemo.java

if HAVE_JAVA
java_JAVA = $(JAVAFILES)
endif

EXTRA_DIST=$(JAVAFILES)

run:
	appletviewer quaddemo.htm
