## $Id$
## Process this file with automake to create Makefile.in



INTERFACE_FILES = \
  Interface/MainPanel.java \
  Interface/MainPanelComponentListener.java \
  Interface/PlaceBtns.java \
  Interface/PlaceBtnsActionListener.java \
  Interface/ConstraintBtns.java \
  Interface/ConstraintBtnsActionListener.java \
  Interface/DrawPanel.java \
  Interface/DPKeyListener.java \
  Interface/DPMouseMotionListener.java \
  Interface/ImageButton.java \
  Interface/ImageButtonImageObserver.java

COMPONENT_FILES = \
  Component/SelPoint.java \
  Component/EditConstantList.java \
  Component/ConstrComponent.java \
  Component/LineCC.java \
  Component/MidpointLineCC.java \
  Component/RectangleCC.java \
  Component/CircleCC.java \
  Component/CDA_G.java 

if HAVE_JAVA
java_JAVA = \
  $(INTERFACE_FILES) $(COMPONENT_FILES)
else
classjava.stamp:
	echo timestamp > classjava.stamp
endif

ICON_BMPS = \
  Icons/LineCC.bmp \
  Icons/MidLineCC.bmp \
  Icons/RectangleCC.bmp \
  Icons/CircleCC.bmp \
  Icons/Anchor.bmp \
  Icons/Colocation.bmp \
  Icons/LeftOf.bmp \
  Icons/RightOf.bmp \
  Icons/Above.bmp \
  Icons/Below.bmp \
  Icons/LeftAlign.bmp \
  Icons/RightAlign.bmp \
  Icons/TopAlign.bmp \
  Icons/BottomAlign.bmp \
  Icons/Blank.bmp 

EXTRA_DIST = $(INTERFACE_FILES) $(COMPONENT_FILES) $(ICON_BMPS)

ALL_ICON_FILES = $(patsubst %.bmp,Icons/%.bmp,$(ICON_BMPS))

ICON_GIFS = $(ICON_BMPS:%.bmp=classes/%.gif)

demo: all
	tar -cf cdademo.tar $(ALL_CLASSES) $(ICON_GIFS) classes/*.htm*
