# $Id$
# Makefile for Constraint-Based Drawing Application
# By Michael Noth
# Modified for Cassowary distribution by Greg J. Badros

CDA_HOME = /scratch/gjb/cassowary/java/cda

CLASSES = ../classes
#CLASSES = .


MAIN_FILES = \
  Main.java \
  MainComponentListener.java

MAIN_CLASSES = $(MAIN_FILES:%.java=$(CLASSES)/%.class)
ALL_MAIN_FILES = $(patsubst %.java,Main/%.java,$(MAIN_FILES))

INTERFACE_FILES = \
  MainPanel.java \
  MainPanelComponentListener.java \
  PlaceBtns.java \
  PlaceBtnsActionListener.java \
  ConstraintBtns.java \
  ConstraintBtnsActionListener.java \
  DrawPanel.java \
  DPKeyListener.java \
  DPMouseMotionListener.java \
  ImageButton.java \
  ImageButtonImageObserver.java

INTERFACE_CLASSES = $(INTERFACE_FILES:%.java=$(CLASSES)/%.class)
ALL_INTERFACE_FILES = $(patsubst %.java,Interface/%.java,$(INTERFACE_FILES))

COMPONENT_FILES = \
  SelPoint.java \
  EditConstantList.java \
  ConstrComponent.java \
  LineCC.java \
  MidpointLineCC.java \
  RectangleCC.java \
  CircleCC.java \
  CDA_G.java 

COMPONENT_CLASSES = $(COMPONENT_FILES:%.java=$(CLASSES)/%.class)
ALL_COMPONENT_FILES = $(patsubst %.java,Component/%.java,$(COMPONENT_FILES))

CONSTRAINT_FILES = \
  Constraint.java \
  AnchorConstraint.java \
  ColocationConstraint.java \
  AdjacencyConstraint.java \
  AboveConstraint.java \
  BelowConstraint.java \
  LeftOfConstraint.java \
  RightOfConstraint.java \
  AlignmentConstraint.java

CONSTRAINT_CLASSES = $(CONSTRAINT_FILES:%.java=$(CLASSES)/%.class)
ALL_CONSTRAINT_FILES = \
  $(patsubst %.java,Constraint/%.java,$(CONSTRAINT_FILES))

ALL_JAVA_FILES = \
	$(ALL_CONSTRAINT_FILES) \
	$(ALL_COMPONENT_FILES) \
	$(ALL_INTERFACE_FILES) \
	$(ALL_MAIN_FILES)

ALL_CLASSES = \
  $(COMPONENT_CLASSES) \
  $(CONSTRAINT_CLASSES) \
  $(INTERFACE_CLASSES) \
  $(MAIN_CLASSES) 

ICON_BMPS = \
  LineCC.bmp \
  MidLineCC.bmp \
  RectangleCC.bmp \
  CircleCC.bmp \
  Anchor.bmp \
  Colocation.bmp \
  LeftOf.bmp \
  RightOf.bmp \
  Above.bmp \
  Below.bmp \
  LeftAlign.bmp \
  RightAlign.bmp \
  TopAlign.bmp \
  BottomAlign.bmp \
  Blank.bmp 

ALL_ICON_FILES = $(patsubst %.bmp,Icons/%.bmp,$(ICON_BMPS))
ICON_GIFS = $(ICON_BMPS:%.bmp=classes/%.gif)

JFLAGS = -g -d $(CLASSES)
# -classpath $(CLASSES)\;e:/jdk1.1.5/lib/classes.zip

default:  all

all: .built-classes

.built-classes: $(ALL_JAVA_FILES) $(ICON_GIFS)
	javac $(JFLAGS) $(ALL_JAVA_FILES)
	touch $@

#FIXGJB: make this work for Unices
$(ICON_GIFS): classes/%.gif: Icons/%.bmp
	e:\\imgalch\\alchlong.exe ---t 255 255 255 -o -g $< $@

clean:
	rm -rf $(ALL_CLASSES)
	rm -rf $(ICON_GIFS)

run: all
	appletviewer classes/run.html

tar:
	rm -f cda.tar
	tar -cf cda.tar $(ALL_MAIN_FILES) $(ALL_INTERFACE_FILES) $(ALL_COMPONENT_FILES) $(ALL_CONSTRAINT_FILES) $(ALL_ICON_FILES) makefile todo classes/*.htm*

icons: $(ICON_GIFS)

demo: all
	tar -cf cdademo.tar $(ALL_CLASSES) $(ICON_GIFS) classes/*.htm*

.PHONY: all run tar clean icons demo
