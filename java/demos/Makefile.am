## $Id$
## Process this file with automake to create Makefile.in

JAVAFILES = DraggableBox.java QuadDemo.java

if HAVE_JAVA
java_JAVA = $(JAVAFILES)
else
classjava.stamp:
	echo timestamp > classjava.stamp
endif

EXTRA_DIST=$(JAVAFILES)

run:
	appletviewer quaddemo.htm
