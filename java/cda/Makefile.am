## $Id$
## Process this file with automake to create Makefile.in

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

COMPONENT_FILES = \
  SelPoint.java \
  EditConstantList.java \
  ConstrComponent.java \
  LineCC.java \
  MidpointLineCC.java \
  RectangleCC.java \
  CircleCC.java \
  CDA_G.java 

java_JAVA = \
  $(INTERFACE_FILES) $(COMPONENT_FILES)

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

demo: all
	tar -cf cdademo.tar $(ALL_CLASSES) $(ICON_GIFS) classes/*.htm*

CDA_HOME = /scratch/gjb/cassowary/java/cda
