####### This section was automatically generated from
#######    /scratch/gjb/qt-1.32/Makefile for building on
#######    linux-gcc-shared on Fri Jan 23 16:00:50 PST 1998

EGCSDIR=/uns/egcs

INCDIR = /scratch/gjb/qt-1.32/include
CFLAGS = -pg -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I.. -DCL_NO_TRACE
LIBCFLAGS = -fPIC
YACCCFLAGS = -Wno-unused -Wno-parentheses
LFLAGS = -pg -L$(EGCSDIR)/lib -L/scratch/gjb/qt-1.32/lib -lqt -L.. -lcassowary
CC = g++
MOC = /scratch/gjb/qt-1.32/bin/moc
SHELL =	/bin/sh

####### End of automatically generated section
#
# $Source$
#

#############################################################################
# Makefile for building hello
# Generated by tmake at 12:24, 1997/03/25
#     Project: hello.pro
#    Template: /home/hanord/qt/tmake/linux-gcc/example.t
#############################################################################

####### Files

HEADERS =	hello.h
SOURCES =	hello.cpp \
		main.cpp
OBJECTS =	hello.o \
		main.o
SRCMOC	=	moc_hello.cpp
OBJMOC	=	moc_hello.o
TARGET	=	hello

####### Implicit rules

.SUFFIXES: .cpp .c

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) -o $@ $<

####### Build rules

all: $(TARGET) 

$(TARGET): $(OBJECTS) $(OBJMOC)
	$(CC) $(OBJECTS) $(OBJMOC) -o $(TARGET) $(LFLAGS) 

moc: $(SRCMOC)

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC) $(TARGET)

####### Compile

hello.o: hello.cpp \
		hello.h

main.o: main.cpp \
		hello.h

moc_hello.o: moc_hello.cpp \
		hello.h

moc_hello.cpp: hello.h
	$(MOC) hello.h -o moc_hello.cpp
