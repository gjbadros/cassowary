

Application create: #ClDemos with:
    (#( ClKernel)
        collect: [:each | Smalltalk at: each ifAbsent: [
            Application errorPrerequisite: #ClDemos missing: each]])!

ClDemos becomeDefault!
Object subclass: #ClDemoWindow
    instanceVariableNames: 'shell main form da gc gcw buffer grabPoint dragPoint cs errorPixel editX editY '
    classVariableNames: ''
    poolDictionaries: 'CwConstants CgConstants ClConstants '!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClBinaryTreeDemo
    instanceVariableNames: 'nodes lines height '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClBoundedLineDemo
    instanceVariableNames: 'p1 p2 '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClBoundedLineDemo subclass: #ClElasticBoundedLineDemo
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClBoxesDemo
    instanceVariableNames: 'p1 p2 p3 p4 p5 '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClCrossedQuadDemo
    instanceVariableNames: 'p1 p2 p3 p4 m1 m2 m3 m4 center '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClLineDemo
    instanceVariableNames: 'p1 p2 '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClMidpointDemo
    instanceVariableNames: 'p1 p2 m1 '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClQuadDemo
    instanceVariableNames: 'p1 p2 p3 p4 m1 m2 m3 m4 '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClQuadDemo subclass: #ClBoundedQuadDemo
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClBoundedQuadDemo subclass: #ClElasticQuadDemo
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClSequenceDiagramDemo1
    instanceVariableNames: 'l1 l2 l3 l4 m1 m2 m3 m4 m5 m6 l12 l21 l23 l32 l34 l43 l15 l51 l56 l65 '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClTreeDemo1
    instanceVariableNames: 'nodes lines '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
ClDemoWindow subclass: #ClTreeDemo2
    instanceVariableNames: 'nodes lines '
    classVariableNames: ''
    poolDictionaries: ''!

ClDemos becomeDefault!
Application subclass: #ClDemos
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''!


ClDemos becomeDefault!

!ClBinaryTreeDemo class publicMethods !

height: h
		| demo |
	demo := self basicNew.
	demo height: h.
	demo initialize.
	^demo
! !

!ClBinaryTreeDemo publicMethods !

childrenAndParentsDo: block
	"evaluate a block for all children and parents, starting with leaves and going up the tree"
		| parentcount left right parent |
	height to: 2 by: -1 do: [:h |
		parentcount := 2 raisedTo: h-2.
		1 to: parentcount do: [:i | 
			left := nodes at: (100*h)+(2*i)-1.
			right := nodes at: (100*h)+(2*i).
			parent := nodes at: (100*(h-1))+i.
			block value: left value: right value: parent]]
!

constraintPointsDo: aBlock

	nodes do: [:each | aBlock value: each]!

createConstraints
		| sorted inset spacing winWidth winHeight root |

	"Add stays on each node.  Accumulate the nodes children first, so that the weights
		on the parents are stronger than those on the children."
	sorted := OrderedCollection new.
	self childrenAndParentsDo: [:lc :rc :p |
		(sorted includes: lc) ifFalse: [sorted add: lc].
		(sorted includes: rc) ifFalse: [sorted add: lc].
		root := p  "we need this at the end"].
	sorted add: root.
	cs addPointStays: sorted.

	inset := self inset.
	spacing := self spacing.
	winWidth := self initialExtent x.
	winHeight := self initialExtent y.
	self childrenAndParentsDo: [:lc :rc :p |
		"we need the root at the end"   root := p.
		"children are at equal height"
		cs addConstraint: (lc y cnEqual: rc y).
		"children are below parents"
		cs addConstraint: (p y + spacing cnLEQ: lc y).
		cs addConstraint: (p y + spacing cnLEQ: rc y).
		"parent is centered in the x coordinate between children"
		cs addConstraint: ( (0.5*lc x) + (0.5*rc x) cnEqual: p x).
		"each node lies within the window"
		cs addBounds: lc x lowerBound: inset upperBound: winWidth-inset.
		cs addBounds: rc x lowerBound: inset upperBound: winWidth-inset.
		cs addBounds: lc y lowerBound: inset upperBound: winHeight-inset.
		cs addBounds: rc y lowerBound: inset upperBound: winHeight-inset].
	cs addBounds: root x lowerBound: inset upperBound: winWidth-inset.
	cs addBounds: root y lowerBound: inset upperBound: winHeight-inset.
!

drawArea

	self setForegroundColor: 0.
	lines do: [:each |
		buffer
			drawLine: gc
			x1: each first x value rounded
			y1: each first y value rounded
			x2: each last x value rounded
			y2: each last y value rounded].
	self setForegroundColor: 5.
	nodes do: [:each |
		buffer
			fillRectangle: gc
			x: (each x value rounded - 5)
			y: (each y value rounded - 5)
			width: 10
			height: 10]!

height: h
	height := h!

initialExtent
	"ACTION
		Return the initial size for the overall window.
	"

	^500 @ 300!

initialize
		| xspace yspace index nleaves nodecount left right px |
	xspace :=20.  
	yspace := 20.

	super initialize.
	nodes := LookupTable new.
	
	"create the leaves"
	nleaves := 2 raisedTo: height-1.
	1 to: nleaves do: [:i | index := 100*height + i.  nodes at: index put: 
			((ClVariable newWith: (xspace*i) asFloat name: 'n' , index printString , 'x') @ 
			  (ClVariable newWith: (yspace*height) asFloat name: 'n' , index printString , 'y'))].

	"create the other nodes"
	height-1 to: 1 by: -1 do: [:h |
		nodecount := 2 raisedTo: h-1.
		1 to: nodecount do: [:i | 
			left := nodes at: (100*(h+1))+(2*i)-1.  
			right := nodes at: (100*(h+1))+(2*i).
			px := (left x value + right x value) * 0.5.
			index := 100*h+ i.  nodes at: index put: 
			((ClVariable newWith: px name: 'n' , index printString , 'x') @ 
			  (ClVariable newWith: yspace*h name: 'n' , index printString , 'y'))]].

	lines := OrderedCollection new.
	self childrenAndParentsDo: [:lc :rc :p | 
		lines add: (Array with: p with: lc).
		lines add: (Array with: p with: rc)].

	self createConstraints.
!

inset
	^10.0
!

spacing
	^10.0
!

title

	^'Binary Tree Demo'! !

!ClBoundedLineDemo publicMethods !

constraintPointsDo: aBlock

	aBlock value: p1.
	aBlock value: p2!

drawArea

	self setForegroundColor: 0.
	buffer
		drawLine: gc
		x1: p1 x value rounded
		y1: p1 y value rounded
		x2: p2 x value rounded
		y2: p2 y value rounded!

initialize
		| offset box left right top bottom |

	super initialize.
	p1 := (ClVariable newWith: 10.0 name: 'p1x') @ (ClVariable newWith: 20.0 name: 'p1y').
	p2 := (ClVariable newWith: 110.0 name: 'p2x') @ (ClVariable newWith: 220.0 name: 'p2y').

	offset := 10.0.
	box := 0 @ 0 corner: 300 @ 300.
	left := box origin x asFloat + offset.
	right := box corner x asFloat - offset.
	top := box origin y asFloat + offset.
	bottom := box corner y asFloat - offset.

	cs addStay: p1 x.
	cs addStay: p1 y.
	cs addStay: p2 x.
	cs addStay: p2 y.

	cs addConstraint: (p1 x cnGEQ: left).  cs addConstraint: (p1 x cnLEQ: right).
	cs addConstraint: (p1 y cnGEQ: top).  cs addConstraint: (p1 y cnLEQ: bottom).
	cs addConstraint: (p2 x cnGEQ: left).  cs addConstraint: (p2 x cnLEQ: right).
	cs addConstraint: (p2 y cnGEQ: top).  cs addConstraint: (p2 y cnLEQ: bottom).
!

title

	^'Bounded Line Demo'! !

!ClBoundedQuadDemo publicMethods !

addAdditionalConstraints

	| offset box left right top bottom |

	offset := 2.0.
	box := 0 @ 0 corner: 300 @ 300.
	left := box origin x asFloat + offset.
	right := box corner x asFloat - offset.
	top := box origin y asFloat + offset.
	bottom := box corner y asFloat - offset.

	cs addConstraint: (p1 x cnGEQ: left).  cs addConstraint: (p1 x cnLEQ: right).
	cs addConstraint: (p1 y cnGEQ: top).  cs addConstraint: (p1 y cnLEQ: bottom).
	cs addConstraint: (p2 x cnGEQ: left).  cs addConstraint: (p2 x cnLEQ: right).
	cs addConstraint: (p2 y cnGEQ: top).  cs addConstraint: (p2 y cnLEQ: bottom).
	cs addConstraint: (p3 x cnGEQ: left).  cs addConstraint: (p3 x cnLEQ: right).
	cs addConstraint: (p3 y cnGEQ: top).  cs addConstraint: (p3 y cnLEQ: bottom).
	cs addConstraint: (p4 x cnGEQ: left).  cs addConstraint: (p4 x cnLEQ: right).
	cs addConstraint: (p4 y cnGEQ: top).  cs addConstraint: (p4 y cnLEQ: bottom).
!

title

	^'Bounded Quadrilateral Demo'! !

!ClBoxesDemo publicMethods !

constraintPointsDo: aBlock

	aBlock value: p1.
	aBlock value: p2.
	aBlock value: p3.
	aBlock value: p4.
	aBlock value: p5!

drawArea

	self setForegroundColor: 12.
	buffer fillRectangle: gc x: p1 x value rounded - 20 y: 160 width: 40 height: 40.
	buffer fillRectangle: gc x: p2 x value rounded - 20 y: 160 width: 40 height: 40.
	buffer fillRectangle: gc x: p3 x value rounded - 20 y: 160 width: 40 height: 40.
	buffer fillRectangle: gc x: p4 x value rounded - 20 y: 160 width: 40 height: 40.
	buffer fillRectangle: gc x: p5 x value rounded - 20 y: 160 width: 40 height: 40!

initialExtent
	"ACTION
		Return the initial size for the overall window.
	"

	^600 @ 300!

initialize

	super initialize.
	p1 := (ClVariable newWith: 50.0) @ (ClVariable newWith: 200.0).
	p2 := (ClVariable newWith: 100.0) @ (ClVariable newWith: 200.0).
	p3 := (ClVariable newWith: 150.0) @ (ClVariable newWith: 200.0).
	p4 := (ClVariable newWith: 200.0) @ (ClVariable newWith: 200.0).
	p5 := (ClVariable newWith: 250.0) @ (ClVariable newWith: 200.0).

	p1 x name: 'p1x'.
	p1 y name: 'p1y'.
	p2 x name: 'p2x'.
	p2 y name: 'p2y'.
	p3 x name: 'p3x'.
	p3 y name: 'p3y'.
	p4 x name: 'p4x'.
	p4 y name: 'p4y'.
	p5 x name: 'p5x'.
	p5 y name: 'p5y'.

	cs addStay: p1 x.
	cs addStay: p2 x.
	cs addStay: p3 x.
	cs addStay: p4 x.
	cs addStay: p5 x.

	cs addStay: p1 y strength: ClStrength required.
	cs addStay: p2 y strength: ClStrength required.
	cs addStay: p3 y strength: ClStrength required.
	cs addStay: p4 y strength: ClStrength required.
	cs addStay: p5 y strength: ClStrength required.

	cs addConstraint: (p1 x cnGEQ: 24.0).
	cs addConstraint: (p5 x cnLEQ: 576.0).

	cs addConstraint: (p1 x + 44.0 cnLEQ: p2 x).
	cs addConstraint: (p2 x + 44.0 cnLEQ: p3 x).
	cs addConstraint: (p3 x + 44.0 cnLEQ: p4 x).
	cs addConstraint: (p4 x + 44.0 cnLEQ: p5 x).

!

title

	^'Boxcars Demo'! !

!ClCrossedQuadDemo publicMethods !

addAdditionalConstraints
	"no additional constraints for this version"
!

constraintPointsDo: aBlock

	aBlock value: p1.
	aBlock value: p2.
	aBlock value: p3.
	aBlock value: p4.

	aBlock value: m1.
	aBlock value: m2.
	aBlock value: m3.
	aBlock value: m4.

	aBlock value: center!

drawArea

	self setForegroundColor: 0.

	"draw the four sides of the quadrilateral"
	buffer
		drawLine: gc
		x1: p1 x value rounded
		y1: p1 y value rounded
		x2: p2 x value rounded
		y2: p2 y value rounded.
	buffer
		drawLine: gc
		x1: p2 x value rounded
		y1: p2 y value rounded
		x2: p3 x value rounded
		y2: p3 y value rounded.
	buffer
		drawLine: gc
		x1: p3 x value rounded
		y1: p3 y value rounded
		x2: p4 x value rounded
		y2: p4 y value rounded.
	buffer
		drawLine: gc
		x1: p4 x value rounded
		y1: p4 y value rounded
		x2: p1 x value rounded
		y2: p1 y value rounded.

	"draw the lines between the midpoints"
	buffer
		drawLine: gc
		x1: m1 x value rounded
		y1: m1 y value rounded
		x2: m2 x value rounded
		y2: m2 y value rounded.
	buffer
		drawLine: gc
		x1: m2 x value rounded
		y1: m2 y value rounded
		x2: m3 x value rounded
		y2: m3 y value rounded.
	buffer
		drawLine: gc
		x1: m3 x value rounded
		y1: m3 y value rounded
		x2: m4 x value rounded
		y2: m4 y value rounded.
	buffer
		drawLine: gc
		x1: m4 x value rounded
		y1: m4 y value rounded
		x2: m1 x value rounded
		y2: m1 y value rounded.

	"draw the cross lines"
	buffer
		drawLine: gc
		x1: m1 x value rounded
		y1: m1 y value rounded
		x2: m3 x value rounded
		y2: m3 y value rounded.
	buffer
		drawLine: gc
		x1: m2 x value rounded
		y1: m2 y value rounded
		x2: m4 x value rounded
		y2: m4 y value rounded!

initialize

	super initialize.
	p1 := (ClVariable newWith: 110.0) @ (ClVariable newWith: 10.0).
	p2 := (ClVariable newWith: 210.0) @ (ClVariable newWith: 110.0).
	p3 := (ClVariable newWith: 110.0) @ (ClVariable newWith: 210.0).
	p4 := (ClVariable newWith: 10.0) @ (ClVariable newWith: 110.0).

	m1 := (ClVariable newWith: 160.0) @ (ClVariable newWith: 60.0).
	m2 := (ClVariable newWith: 160.0) @ (ClVariable newWith: 160.0).
	m3 := (ClVariable newWith: 60.0) @ (ClVariable newWith: 160.0).
	m4 := (ClVariable newWith: 60.0) @ (ClVariable newWith: 60.0).

	center := (ClVariable newWith: 110.0) @ (ClVariable newWith: 110.0).

	p1 x name: 'p1x'.
	p1 y name: 'p1y'.
	p2 x name: 'p2x'.
	p2 y name: 'p2y'.
	p3 x name: 'p3x'.
	p3 y name: 'p3y'.
	p4 x name: 'p4x'.
	p4 y name: 'p4y'.

	m1 x name: 'm1x'.
	m1 y name: 'm1y'.
	m2 x name: 'm2x'.
	m2 y name: 'm2y'.
	m3 x name: 'm3x'.
	m3 y name: 'm3y'.
	m4 x name: 'm4x'.
	m4 y name: 'm4y'.

	center x name: 'cx'.
	center y name: 'cy'.

	"add stays on the corner points"
	cs addPointStays: (Array with: p1 with: p2 with: p3 with: p4).

	"set up the midpoint constraints"
	cs addConstraint: (p1 x + p2 x cnEqual: 2.0 * m1 x).
	cs addConstraint: (p2 x + p3 x cnEqual: 2.0 * m2 x).
	cs addConstraint: (p3 x + p4 x cnEqual: 2.0 * m3 x).
	cs addConstraint: (p4 x + p1 x cnEqual: 2.0 * m4 x).
	cs addConstraint: (p1 y + p2 y cnEqual: 2.0 * m1 y).
	cs addConstraint: (p2 y + p3 y cnEqual: 2.0 * m2 y).
	cs addConstraint: (p3 y + p4 y cnEqual: 2.0 * m3 y).
	cs addConstraint: (p4 y + p1 y cnEqual: 2.0 * m4 y).

	"set up the centerpoint constraints"
	cs addConstraint: (m1 x + m3 x cnEqual: 2.0 * center x).
	cs addConstraint: (m1 y + m3 y cnEqual: 2.0 * center y).
	cs addConstraint: (m2 x + m4 x cnEqual: 2.0 * center x).
	cs addConstraint: (m2 y + m4 y cnEqual: 2.0 * center y).

	self addAdditionalConstraints.
!

title

	^'Crossed Quadrilateral Demo'! !

!ClDemoWindow class publicMethods !

makeDemos

	"statements to build various demos"

	ClBoundedLineDemo new open.
	ClElasticBoundedLineDemo new open.
	ClQuadDemo new open.
	ClMidpointDemo new open.
	ClBoundedQuadDemo new open.
	ClCrossedQuadDemo new open.
	ClElasticQuadDemo new open.
	ClTreeDemo1 new open.
	ClTreeDemo2 new open.
	ClBoxesDemo new open.
	ClSequenceDiagramDemo1 new open.
	(ClBinaryTreeDemo height: 4) open.
!

new

	^super new initialize! !

!ClDemoWindow publicMethods !

beginManipulation
	"ACTION
		This method is called when a constrainable point is starting to
		be manipulated, i.e., upon mouse down.  The constrainable
		point is stored in the 'dragPoint' instance variable, and the
		corresponding plan for moving that point is stored in the 'plan'
		instance variable.

		The order of messages is:
			1. #beginManipulation
			2-N. #manipulationMoveTo:
			N+1. #endManipulation
	"

	self constraintPointsDo: [:p |
		dragPoint == p ifTrue: [
			editX := ClEditConstraint variable: p x strength: ClStrength strong.
			editY := ClEditConstraint variable: p y strength: ClStrength strong.
			cs addConstraint: editX.
			cs addConstraint: editY.
			self redraw.
			^nil]].

	 "drag point not found"
	self error 
!

button1Motion: aWidget clientData: clientData callData: callData

	dragPoint isNil ifTrue: [^self].
	self manipulationMoveTo: callData point!

buttonPress: aWidget clientData: clientData callData: callData

	grabPoint isNil ifTrue: [^self].
	self drawGrabBox.
	dragPoint := grabPoint.
	grabPoint := nil.

	self beginManipulation!

buttonRelease: aWidget clientData: clientData callData: callData

	dragPoint isNil ifTrue: [^self].
	dragPoint := nil.

	self endManipulation!

constraintPointsDo: aBlock
	"ACTION
		Evaluate this block for each of the constrainable points in this demo.
		Constrainable points are those that can be manipulated by the mouse.
		A constraint point is a Point object whose x and y instance variables
		hold instances of ClVariable.

	PARAMETERS
		aBlock <[:<Point of <ClVariable>> | ...]>
	"
!

createForm

	da := form createDrawingArea: 'da' argBlock: nil.
	da manageChild.
	da setValuesBlock: [:w |
		w
			bottomAttachment: XmATTACHFORM;
			topAttachment: XmATTACHFORM;
			rightAttachment: XmATTACHFORM;
			leftAttachment: XmATTACHFORM].
	da
		addCallback: XmNexposeCallback
			receiver: self
			selector: #exposeArea:clientData:callData:
			clientData: nil;
		addCallback: XmNresizeCallback
			receiver: self
			selector: #resizeArea:clientData:callData:
			clientData: nil;
		addEventHandler: Button1MotionMask
			receiver: self
			selector: #button1Motion:clientData:callData:
			clientData: nil;
		addEventHandler: ButtonPressMask
			receiver: self
			selector: #buttonPress:clientData:callData:
			clientData: nil;
		addEventHandler: ButtonReleaseMask
			receiver: self
			selector: #buttonRelease:clientData:callData:
			clientData: nil;
		addEventHandler: PointerMotionMask
			receiver: self
			selector: #pointerMotion:clientData:callData:
			clientData: nil!

createWindow

	shell :=
		CwTopLevelShell
			createApplicationShell: 'shell'
			argBlock: [:w |
				w
					title: self title;
					width: self initialExtent x;
					height: self initialExtent y].
	main := shell createMainWindow: 'main' argBlock: nil.
	main manageChild.
	form := main createForm: 'form' argBlock: nil.
	form manageChild.
	shell
		addCallback: XmNdestroyCallback
		receiver: self
		selector: #destroyWindow:clientData:callData:
		clientData: nil.
	self createForm!

destroy
	"ACTION
		This method is called when the window is closing.
	"
!

destroyWindow: aWidget clientData: clientData callData: callData

	gc isNil
		ifFalse: [
			gc freeGC.
			gc := nil].
	gcw isNil
		ifFalse: [
			gcw freeGC.
			gcw := nil].
	buffer isNil
		ifFalse: [
			buffer freePixmap.
			buffer := nil].
	self destroy!

drawArea
	"ACTION
		Draw the demo.
	"
!

drawBackground
	"ACTION
		Draw the background of the demo.
	"

	gc setForeground: 15.
	buffer fillRectangle: gc x: 0 y: 0 width: buffer width height: buffer height!

drawGrabBox

	| r pv |

	pv := grabPoint x value rounded @ grabPoint y value rounded.
	gcw
		setFunction: GXhighlight;
		setLineAttributes: 3 lineStyle: LineSolid capStyle: CapButt joinStyle: JoinMiter;
		setForeground: 0.
	r := (pv - self grabThreshold) corner: (pv + self grabThreshold).
	da window drawRectangle: gcw x: r origin x y: r origin y width: r width height: r height.
	gcw setFunction: GXcopy!

endManipulation
	"ACTION
		This method is called when a constrainable point is ending
		being manipulated, i.e., upon mouse up.  The constrainable
		point is stored in the 'dragPoint' instance variable, and the
		corresponding plan for moving that point is stored in the 'plan'
		instance variable.

		The order of messages is:
			1. #beginManipulation
			2-N. #manipulationMoveTo:
			N+1. #endManipulation
	"
	cs removeConstraint: editX.
	cs removeConstraint: editY.
	self redraw.
!

exposeArea: aWidget clientData: clientData callData: callData

	buffer isNil ifTrue: [self resizeArea: aWidget clientData: clientData callData: nil].
	buffer
		copyArea: aWidget window
		gc: gcw
		srcX: 0
		srcY: 0
		width: buffer width
		height: buffer height
		destX: 0
		destY: 0.
	grabPoint := nil!

grabThreshold

	^7 @ 7!

initialExtent
	"ACTION
		Return the initial size for the overall window.
	"

	^300 @ 300!

initialize
	"ACTION
		Initialize the demo structures before the widgets are created.
	"

	cs := ClSimplexSolver new.
	errorPixel := nil

!

manipulationMoveTo: aPoint
	"ACTION
		This method is called when a constrainable point is manipulated
		using the mouse, i.e., during mouse motion.  aPoint is the
		location of the mouse and thus the new requested location of
		the constrainable point.  The constrainable
		point is stored in the 'dragPoint' instance variable, and the
		corresponding plan for moving that point is stored in the 'plan'
		instance variable.

		The order of messages is:
			1. #beginManipulation
			2-N. #manipulationMoveTo:
			N+1. #endManipulation

	PARAMETERS
		aPoint <Point of <Number>>
	"
	cs resolve: (Array with: aPoint x asFloat with: aPoint y asFloat).
	self redraw
!

open

	self createWindow.
	shell realizeWidget!

pointerMotion: aWidget clientData: clientData callData: callData

	| r |

	dragPoint isNil
		ifTrue: [
			self constraintPointsDo: [:p | | pv th |
				th := self grabThreshold * 2.
				pv := p x value rounded @ p y value rounded.
				(((pv - th) corner: (pv + th)) containsPoint: callData point)
					ifTrue: [
						grabPoint == p ifTrue: [^self].
						grabPoint notNil ifTrue: [self drawGrabBox].
						grabPoint := p.
						self drawGrabBox.
						^self].
				grabPoint notNil ifTrue: [self drawGrabBox].
				grabPoint := nil]]!

redraw

	self drawBackground.
	self drawArea.
	self exposeArea: da clientData: nil callData: nil!

resizeArea: aWidget clientData: clientData callData: callData

	buffer isNil
		ifFalse: [
			buffer freePixmap.
			buffer := nil].
	gc isNil
		ifFalse: [
			gc freeGC.
			gc := nil].
	gcw isNil ifTrue: [gcw := aWidget window createGC: None values: nil].
	buffer :=
		aWidget window createPixmap: aWidget width height: aWidget height depth: aWidget window depth.
	gc := buffer createGC: None values: nil.
	self drawBackground.
	self drawArea!

setForegroundColor: c

	gc setForeground: (errorPixel isNil ifTrue: [c] ifFalse: [errorPixel])

!

title
	"ACTION
		Return the title for the overall window.
	"

	^'demo'! !

!ClElasticBoundedLineDemo publicMethods !

initialize

	super initialize.
	cs addConstraint: (p2 y - p1 y cnEqual: 200.0 strength: ClStrength medium).
! !

!ClElasticQuadDemo publicMethods !

addAdditionalConstraints

	super addAdditionalConstraints.
	"add constraints to keep it from turning inside out"
	cs addConstraint: (p4 x+10 cnLEQ: p3 x).
	cs addConstraint: (p4 x+10 cnLEQ: p1 x).
	cs addConstraint: (p3 x+10 cnLEQ: p2 x).
	cs addConstraint: (p1 x+10 cnLEQ: p2 x).

	cs addConstraint: (p1 y+10 cnLEQ: p2 y).
	cs addConstraint: (p1 y+10 cnLEQ: p4 y).
	cs addConstraint: (p4 y+10 cnLEQ: p3 y).
	cs addConstraint: (p2 y+10 cnLEQ: p3 y).

	"add medium constraints to keep the quadrilateral's original shape"
	cs addConstraint: (p2 y cnEqual: p4 y strength: ClStrength medium).
	cs addConstraint: (p2 x - p4 x cnEqual: 200.0 strength: ClStrength medium).
	cs addConstraint: (p1 x cnEqual: p3 x strength: ClStrength medium).
	cs addConstraint: (p3 y - p1 y cnEqual: 200.0 strength: ClStrength medium).
!

title

	^'Elastic Quadrilateral Demo'! !

!ClLineDemo publicMethods !

constraintPointsDo: aBlock

	aBlock value: p1.
	aBlock value: p2!

drawArea

	self setForegroundColor: 0.
	buffer
		drawLine: gc
		x1: p1 x value rounded
		y1: p1 y value rounded
		x2: p2 x value rounded
		y2: p2 y value rounded!

initialize

	super initialize.
	p1 := (ClVariable newWith: 10.0 name: 'p1x') @ (ClVariable newWith: 20.0 name: 'p1y').
	p2 := (ClVariable newWith: 110.0 name: 'p2x') @ (ClVariable newWith: 220.0 name: 'p2y').
	cs addStay: p1 x.
	cs addStay: p1 y.
	cs addStay: p2 x.
	cs addStay: p2 y.
!

title

	^'Line Demo'! !

!ClMidpointDemo publicMethods !

constraintPointsDo: aBlock

	aBlock value: p1.
	aBlock value: p2.
	aBlock value: m1.
!

drawArea

	self setForegroundColor: 0.

	buffer
		drawLine: gc
		x1: p1 x value rounded
		y1: p1 y value rounded
		x2: p2 x value rounded
		y2: p2 y value rounded.
	buffer
		drawLine: gc
		x1: m1 x value rounded
		y1: m1 y value rounded -1
		x2: m1 x value rounded
		y2: m1 y value rounded +1.!

initialize

	super initialize.
	p1 := (ClVariable newWith: 100.0) @ (ClVariable newWith: 100.0).
	p2 := (ClVariable newWith: 200.0) @ (ClVariable newWith: 100.0).
	m1 := (ClVariable newWith: 150.0) @ (ClVariable newWith: 100.0).

	p1 x name: 'p1x'.
	p1 y name: 'p1y'.
	p2 x name: 'p2x'.
	p2 y name: 'p2y'.
	m1 x name: 'm1x'.
	m1 y name: 'm1y'.

	"add stays on the corner points"
	cs addPointStays: (Array with: p1 with: p2).

	"set up the midpoint constraints"
	cs addConstraint: (p1 x + p2 x cnEqual: 2.0 * m1 x).
	cs addConstraint: (p1 y + p2 y cnEqual: 2.0 * m1 y).!

title

	^'Midpoint Demo'! !

!ClQuadDemo publicMethods !

addAdditionalConstraints
	"no additional constraints for this version"
!

constraintPointsDo: aBlock

	aBlock value: p1.
	aBlock value: p2.
	aBlock value: p3.
	aBlock value: p4.

	aBlock value: m1.
	aBlock value: m2.
	aBlock value: m3.
	aBlock value: m4!

drawArea

	self setForegroundColor: 0.

	"draw the four sides of the quadrilateral"
	buffer
		drawLine: gc
		x1: p1 x value rounded
		y1: p1 y value rounded
		x2: p2 x value rounded
		y2: p2 y value rounded.
	buffer
		drawLine: gc
		x1: p2 x value rounded
		y1: p2 y value rounded
		x2: p3 x value rounded
		y2: p3 y value rounded.
	buffer
		drawLine: gc
		x1: p3 x value rounded
		y1: p3 y value rounded
		x2: p4 x value rounded
		y2: p4 y value rounded.
	buffer
		drawLine: gc
		x1: p4 x value rounded
		y1: p4 y value rounded
		x2: p1 x value rounded
		y2: p1 y value rounded.

	"draw the lines between the midpoints"
	buffer
		drawLine: gc
		x1: m1 x value rounded
		y1: m1 y value rounded
		x2: m2 x value rounded
		y2: m2 y value rounded.
	buffer
		drawLine: gc
		x1: m2 x value rounded
		y1: m2 y value rounded
		x2: m3 x value rounded
		y2: m3 y value rounded.
	buffer
		drawLine: gc
		x1: m3 x value rounded
		y1: m3 y value rounded
		x2: m4 x value rounded
		y2: m4 y value rounded.
	buffer
		drawLine: gc
		x1: m4 x value rounded
		y1: m4 y value rounded
		x2: m1 x value rounded
		y2: m1 y value rounded


!

initialize

	super initialize.
	p1 := (ClVariable newWith: 110.0) @ (ClVariable newWith: 10.0).
	p2 := (ClVariable newWith: 210.0) @ (ClVariable newWith: 110.0).
	p3 := (ClVariable newWith: 110.0) @ (ClVariable newWith: 210.0).
	p4 := (ClVariable newWith: 10.0) @ (ClVariable newWith: 110.0).

	m1 := (ClVariable newWith: 160.0) @ (ClVariable newWith: 60.0).
	m2 := (ClVariable newWith: 160.0) @ (ClVariable newWith: 160.0).
	m3 := (ClVariable newWith: 60.0) @ (ClVariable newWith: 160.0).
	m4 := (ClVariable newWith: 60.0) @ (ClVariable newWith: 60.0).

	p1 x name: 'p1x'.
	p1 y name: 'p1y'.
	p2 x name: 'p2x'.
	p2 y name: 'p2y'.
	p3 x name: 'p3x'.
	p3 y name: 'p3y'.
	p4 x name: 'p4x'.
	p4 y name: 'p4y'.

	m1 x name: 'm1x'.
	m1 y name: 'm1y'.
	m2 x name: 'm2x'.
	m2 y name: 'm2y'.
	m3 x name: 'm3x'.
	m3 y name: 'm3y'.
	m4 x name: 'm4x'.
	m4 y name: 'm4y'.

	"add stays on the corner points"
	cs addPointStays: (Array with: p1 with: p2 with: p3 with: p4).

	"set up the midpoint constraints"
	cs addConstraint: (p1 x + p2 x cnEqual: 2.0 * m1 x).
	cs addConstraint: (p2 x + p3 x cnEqual: 2.0 * m2 x).
	cs addConstraint: (p3 x + p4 x cnEqual: 2.0 * m3 x).
	cs addConstraint: (p4 x + p1 x cnEqual: 2.0 * m4 x).
	cs addConstraint: (p1 y + p2 y cnEqual: 2.0 * m1 y).
	cs addConstraint: (p2 y + p3 y cnEqual: 2.0 * m2 y).
	cs addConstraint: (p3 y + p4 y cnEqual: 2.0 * m3 y).
	cs addConstraint: (p4 y + p1 y cnEqual: 2.0 * m4 y).

	self addAdditionalConstraints.
!

title

	^'Quadrilateral Demo'! !

!ClSequenceDiagramDemo1 publicMethods !

constraintPointsDo: aBlock

	aBlock value: l1.
	aBlock value: l2.
	aBlock value: l3.
	aBlock value: l4.

	m1 do: [:e | aBlock value: e].
	m2 do: [:e | aBlock value: e].
	m3 do: [:e | aBlock value: e].
	m4 do: [:e | aBlock value: e].
	m5 do: [:e | aBlock value: e].
	m6 do: [:e | aBlock value: e].
!

createConstraints

		| spacing block |

	spacing := 5.0.

	"add stays on each node"
	self constraintPointsDo: [:p |
		cs addStay: p x strength: ClStrength weak.
		cs addStay: p y strength: ClStrength weak].

	"- - -  Left bar fixed"
	cs addStay: l1 x strength: ClStrength required.

	"- - -  Bar tops fixed"
	cs addStay: l1 y strength: ClStrength required.
	cs addStay: l2 y strength: ClStrength required.
	cs addStay: l3 y strength: ClStrength required.
	cs addStay: l4 y strength: ClStrength required.

	"- - -  Bars left and right of each other"
	cs addConstraint: (l1 x+spacing cnLEQ: l2 x).
	cs addConstraint: (l2 x+spacing cnLEQ: l3 x).
	cs addConstraint: (l3 x+spacing cnLEQ: l4 x).

	"- - -  Methods on bars"
	cs addConstraint: (l1 x cnEqual: m1 first x).
	cs addConstraint: (l1 x cnEqual: m1 last x).
	cs addConstraint: (l2 x cnEqual: m2 first x).
	cs addConstraint: (l2 x cnEqual: m2 last x).
	cs addConstraint: (l3 x cnEqual: m3 first x).
	cs addConstraint: (l3 x cnEqual: m3 last x).
	cs addConstraint: (l4 x cnEqual: m4 first x).
	cs addConstraint: (l4 x cnEqual: m4 last x).
	cs addConstraint: (l2 x cnEqual: m5 first x).
	cs addConstraint: (l2 x cnEqual: m5 last x).
	cs addConstraint: (l3 x cnEqual: m6 first x).
	cs addConstraint: (l3 x cnEqual: m6 last x).

	"- - -  Methods above and below"
	cs addConstraint: (m2 last y+spacing cnLEQ: m5 first y).
	cs addConstraint: (m3 last y+spacing cnLEQ: m6 first y).

	"- - -  Methods sizing"
	cs addConstraint: (m1 first y+spacing cnLEQ: m1 last y).
	cs addConstraint: (m2 first y+spacing cnLEQ: m2 last y).
	cs addConstraint: (m3 first y+spacing cnLEQ: m3 last y).
	cs addConstraint: (m4 first y+spacing cnLEQ: m4 last y).
	cs addConstraint: (m5 first y+spacing cnLEQ: m5 last y).
	cs addConstraint: (m6 first y+spacing cnLEQ: m6 last y).

	"- - -  Bottom line alignment"
	block := [:ln :mth1 :mth2 |
		cs addConstraint: (ln first x cnEqual: mth1 first x).
		cs addConstraint: (ln last x cnEqual: mth2 first x).
		cs addConstraint: (ln first y cnEqual: ln last y).
		cs addConstraint: (ln last y cnEqual: mth2 last y).
		cs addConstraint: (ln first y+spacing cnLEQ: mth1 last y).
		cs addConstraint: (mth1 first y+spacing cnLEQ: ln first y)].
	block value: l21 value: m1 value: m2.
	block value: l32 value: m2 value: m3.
	block value: l43 value: m3 value: m4.
	block value: l51 value: m1 value: m5.
	block value: l65 value: m5 value: m6.

	"- - -  Top line alignment"
	block := [:ln :mth1 :mth2 |
		cs addConstraint: (ln first x cnEqual: mth1 first x).
		cs addConstraint: (ln last x cnEqual: mth2 first x).
		cs addConstraint: (ln first y+spacing cnLEQ: ln last y).
		cs addConstraint: (ln last y cnEqual: mth2 first y).
		cs addConstraint: (ln first y+spacing cnLEQ: mth1 last y).
		cs addConstraint: (mth1 first y+spacing cnLEQ: ln first y)].
	block value: l12 value: m1 value: m2.
	block value: l23 value: m2 value: m3.
	block value: l34 value: m3 value: m4.
	block value: l15 value: m1 value: m5.
	block value: l56 value: m5 value: m6.
!

drawArea

	| idx |
	self setForegroundColor: 0.
	(Array with: l1 with: l2 with: l3 with: l4) do: [:each |
		buffer
			drawLine: gc
			x1: each x value rounded
			y1: 0
			x2: each x value rounded
			y2: buffer height].
	idx := 8.
	(Array with: m1 with: m2 with: m3) do: [:each |
		idx := idx - 1.
		self setForegroundColor: idx.
		buffer
			fillRectangle: gc
			x: (each first x value rounded - 5)
			y: (each first y value rounded)
			width: 10
			height: ((each last y value rounded) - (each first y value rounded))].
	(Array with: m4 with: m5 with: m6) do: [:each |
		idx := idx - 1.
		self setForegroundColor: idx.
		buffer
			fillRectangle: gc
			x: (each first x value rounded - 5)
			y: (each first y value rounded)
			width: 10
			height: ((each last y value rounded) - (each first y value rounded))].
	self setForegroundColor: 0.
	(Array with: l12 with: l23 with: l34) do: [:each |
		buffer
			drawLine: gc
			x1: each first x value rounded + 5
			y1: each first y value rounded
			x2: each last x value rounded
			y2: each last y value rounded].
	(Array with: l15 with: l56) do: [:each |
		buffer
			drawLine: gc
			x1: each first x value rounded + 5
			y1: each first y value rounded
			x2: each last x value rounded
			y2: each last y value rounded].
	(Array with: l21 with: l32 with: l43) do: [:each |
		buffer
			drawLine: gc
			x1: each first x value rounded + 5
			y1: each first y value rounded
			x2: each last x value rounded
			y2: each last y value rounded].
	(Array with: l51 with: l65) do: [:each |
		buffer
			drawLine: gc
			x1: each first x value rounded + 5
			y1: each first y value rounded
			x2: each last x value rounded
			y2: each last y value rounded].
!

initialize

	super initialize.

	l1 := ((ClVariable newWith: 10 name: 'l1x') @ (ClVariable newWith: 10 name: 'l1y')).
	l2 := ((ClVariable newWith: 60 name: 'l1x') @ (ClVariable newWith: 10 name: 'l1y')).
	l3 := ((ClVariable newWith: 110 name: 'l1x') @ (ClVariable newWith: 10 name: 'l1y')).
	l4 := ((ClVariable newWith: 160 name: 'l1x') @ (ClVariable newWith: 10 name: 'l1y')).

	m1 := Array 
		with: ((ClVariable newWith: 10 name: 'm1ax') @ (ClVariable newWith: 20 name: 'm1ay'))
		with: ((ClVariable newWith: 10 name: 'm1bx') @ (ClVariable newWith: 240 name: 'm1by')).
	m2 := Array 
		with: ((ClVariable newWith: 60 name: 'm2ax') @ (ClVariable newWith: 40 name: 'm2ay'))
		with: ((ClVariable newWith: 60 name: 'm2bx') @ (ClVariable newWith: 140 name: 'm2by')).
	m3 := Array 
		with: ((ClVariable newWith: 110 name: 'm3ax') @ (ClVariable newWith: 60 name: 'm3ay'))
		with: ((ClVariable newWith: 110 name: 'm3bx') @ (ClVariable newWith: 120 name: 'm3by')).
	m4 := Array 
		with: ((ClVariable newWith: 160 name: 'm4ax') @ (ClVariable newWith: 80 name: 'm4ay'))
		with: ((ClVariable newWith: 160 name: 'm4bx') @ (ClVariable newWith: 100 name: 'm4by')).
	m5 := Array 
		with: ((ClVariable newWith: 60 name: 'm5ax') @ (ClVariable newWith: 160 name: 'm5ay'))
		with: ((ClVariable newWith: 60 name: 'm5bx') @ (ClVariable newWith: 220 name: 'm5by')).
	m6 := Array 
		with: ((ClVariable newWith: 110 name: 'm6ax') @ (ClVariable newWith: 180 name: 'm6ay'))
		with: ((ClVariable newWith: 110 name: 'm6bx') @ (ClVariable newWith: 200 name: 'm6by')).

	l12 := Array 
		with: ((ClVariable newWith: 10 name: 'l12ax') @ (ClVariable newWith: 40 name: 'l12ay'))
		with: ((ClVariable newWith: 60 name: 'l12bx') @ (ClVariable newWith: 40 name: 'l12by')).
	l21 := Array 
		with: ((ClVariable newWith: 60 name: 'l21ax') @ (ClVariable newWith: 140 name: 'l21ay'))
		with: ((ClVariable newWith: 10 name: 'l21bx') @ (ClVariable newWith: 140 name: 'l21by')).
	l23 := Array 
		with: ((ClVariable newWith: 60 name: 'l23ax') @ (ClVariable newWith: 60 name: 'l23ay'))
		with: ((ClVariable newWith: 110 name: 'l23bx') @ (ClVariable newWith: 60 name: 'l23by')).
	l32 := Array 
		with: ((ClVariable newWith: 110 name: 'l32ax') @ (ClVariable newWith: 120 name: 'l32ay'))
		with: ((ClVariable newWith: 60 name: 'l32bx') @ (ClVariable newWith: 120 name: 'l32by')).
	l34 := Array 
		with: ((ClVariable newWith: 110 name: 'l34ax') @ (ClVariable newWith: 80 name: 'l34ay'))
		with: ((ClVariable newWith: 160 name: 'l34bx') @ (ClVariable newWith: 80 name: 'l34by')).
	l43 := Array 
		with: ((ClVariable newWith: 160 name: 'l43ax') @ (ClVariable newWith: 100 name: 'l43ay'))
		with: ((ClVariable newWith: 110 name: 'l43bx') @ (ClVariable newWith: 100 name: 'l43by')).
	l15 := Array 
		with: ((ClVariable newWith: 10 name: 'l15ax') @ (ClVariable newWith: 160 name: 'l15ay'))
		with: ((ClVariable newWith: 60 name: 'l15bx') @ (ClVariable newWith: 160 name: 'l15by')).
	l51 := Array 
		with: ((ClVariable newWith: 60 name: 'l51ax') @ (ClVariable newWith: 220 name: 'l51ay'))
		with: ((ClVariable newWith: 10 name: 'l51bx') @ (ClVariable newWith: 220 name: 'l51by')).
	l56 := Array 
		with: ((ClVariable newWith: 60 name: 'l56ax') @ (ClVariable newWith: 180 name: 'l56ay'))
		with: ((ClVariable newWith: 110 name: 'l56bx') @ (ClVariable newWith: 180 name: 'l56by')).
	l65 := Array 
		with: ((ClVariable newWith: 110 name: 'l65ax') @ (ClVariable newWith: 200 name: 'l65ay'))
		with: ((ClVariable newWith: 60 name: 'l65bx') @ (ClVariable newWith: 200 name: 'l65by')).

	self createConstraints.
!

title

	^'Sequence Diagram Demo'! !

!ClTreeDemo1 publicMethods !

constraintPointsDo: aBlock

	nodes do: [:each | aBlock value: each]!

createConstraints
		| spacing |
	spacing := 10.0.

	"add stays on each node"
	self constraintPointsDo: [:p |
		cs addStay: p x strength: ClStrength weak.
		cs addStay: p y strength: ClStrength weak].

	cs addConstraint: ((nodes at: 21) y cnEqual: (nodes at: 22) y).

	cs addConstraint: ((nodes at: 31) y cnEqual: (nodes at: 32) y).
	cs addConstraint: ((nodes at: 32) y cnEqual: (nodes at: 33) y).
	cs addConstraint: ((nodes at: 33) y cnEqual: (nodes at: 34) y).
	cs addConstraint: ((nodes at: 34) y cnEqual: (nodes at: 35) y).
	cs addConstraint: ((nodes at: 35) y cnEqual: (nodes at: 36) y).

	cs addConstraint: ((nodes at: 41) y cnEqual: (nodes at: 42) y).
	cs addConstraint: ((nodes at: 42) y cnEqual: (nodes at: 43) y).
	cs addConstraint: ((nodes at: 43) y cnEqual: (nodes at: 44) y).
	cs addConstraint: ((nodes at: 44) y cnEqual: (nodes at: 45) y).
	cs addConstraint: ((nodes at: 45) y cnEqual: (nodes at: 46) y).


	cs addConstraint: ((nodes at: 11) y+spacing cnLEQ: (nodes at: 21) y).
	cs addConstraint: ((nodes at: 11) y+spacing cnLEQ: (nodes at: 22) y).

	cs addConstraint: ((nodes at: 21) y+spacing cnLEQ: (nodes at: 31) y).
	cs addConstraint: ((nodes at: 21) y+spacing cnLEQ: (nodes at: 32) y).
	cs addConstraint: ((nodes at: 21) y+spacing cnLEQ: (nodes at: 33) y).

	cs addConstraint: ((nodes at: 22) y+spacing cnLEQ: (nodes at: 34) y).
	cs addConstraint: ((nodes at: 22) y+spacing cnLEQ: (nodes at: 35) y).
	cs addConstraint: ((nodes at: 22) y+spacing cnLEQ: (nodes at: 36) y).

	cs addConstraint: ((nodes at: 32) y+spacing cnLEQ: (nodes at: 41) y).
	cs addConstraint: ((nodes at: 32) y+spacing cnLEQ: (nodes at: 42) y).

	cs addConstraint: ((nodes at: 34) y+spacing cnLEQ: (nodes at: 43) y).
	cs addConstraint: ((nodes at: 34) y+spacing cnLEQ: (nodes at: 44) y).
	cs addConstraint: ((nodes at: 34) y+spacing cnLEQ: (nodes at: 45) y).

	cs addConstraint: ((nodes at: 36) y+spacing cnLEQ: (nodes at: 46) y)!

drawArea

	self setForegroundColor: 0.
	lines do: [:each |
		buffer
			drawLine: gc
			x1: each first x value rounded
			y1: each first y value rounded
			x2: each last x value rounded
			y2: each last y value rounded].
	self setForegroundColor: 5.
	nodes do: [:each |
		buffer
			fillRectangle: gc
			x: (each x value rounded - 5)
			y: (each y value rounded - 5)
			width: 10
			height: 10]!

initialize

	super initialize.
	nodes := LookupTable new.
	nodes at: 11 put: ((ClVariable newWith: 50 name: 'n11x') @ (ClVariable newWith: 10 name: 'n11y')).
	nodes at: 21 put: ((ClVariable newWith: 25 name: 'n21x') @ (ClVariable newWith: 60 name: 'n21y')).
	nodes at: 22 put: ((ClVariable newWith: 75 name: 'n22x') @ (ClVariable newWith: 60 name: 'n22y')).
	nodes at: 31 put: ((ClVariable newWith: 15 name: 'n31x') @ (ClVariable newWith: 110 name: 'n31y')).
	nodes at: 32 put: ((ClVariable newWith: 30 name: 'n32x') @ (ClVariable newWith: 110 name: 'n32y')).
	nodes at: 33 put: ((ClVariable newWith: 45 name: 'n33x') @ (ClVariable newWith: 110 name: 'n33y')).
	nodes at: 34 put: ((ClVariable newWith: 60 name: 'n34x') @ (ClVariable newWith: 110 name: 'n34y')).
	nodes at: 35 put: ((ClVariable newWith: 75 name: 'n35x') @ (ClVariable newWith: 110 name: 'n35y')).
	nodes at: 36 put: ((ClVariable newWith: 90 name: 'n36x') @ (ClVariable newWith: 110 name: 'n36y')).
	nodes at: 41 put: ((ClVariable newWith: 20 name: 'n41x') @ (ClVariable newWith: 160 name: 'n41y')).
	nodes at: 42 put: ((ClVariable newWith: 35 name: 'n42x') @ (ClVariable newWith: 160 name: 'n42y')).
	nodes at: 43 put: ((ClVariable newWith: 55 name: 'n43x') @ (ClVariable newWith: 160 name: 'n43y')).
	nodes at: 44 put: ((ClVariable newWith: 70 name: 'n44x') @ (ClVariable newWith: 160 name: 'n44y')).
	nodes at: 45 put: ((ClVariable newWith: 85 name: 'n45x') @ (ClVariable newWith: 160 name: 'n45y')).
	nodes at: 46 put: ((ClVariable newWith: 100 name: 'n46x') @ (ClVariable newWith: 160 name: 'n46y')).

	lines := OrderedCollection new.
	lines add: (Array with: (nodes at: 11) with: (nodes at: 21)).
	lines add: (Array with: (nodes at: 11) with: (nodes at: 22)).
	lines add: (Array with: (nodes at: 21) with: (nodes at: 31)).
	lines add: (Array with: (nodes at: 21) with: (nodes at: 32)).
	lines add: (Array with: (nodes at: 21) with: (nodes at: 33)).
	lines add: (Array with: (nodes at: 22) with: (nodes at: 34)).
	lines add: (Array with: (nodes at: 22) with: (nodes at: 35)).
	lines add: (Array with: (nodes at: 22) with: (nodes at: 36)).
	lines add: (Array with: (nodes at: 32) with: (nodes at: 41)).
	lines add: (Array with: (nodes at: 32) with: (nodes at: 42)).
	lines add: (Array with: (nodes at: 34) with: (nodes at: 43)).
	lines add: (Array with: (nodes at: 34) with: (nodes at: 44)).
	lines add: (Array with: (nodes at: 34) with: (nodes at: 45)).
	lines add: (Array with: (nodes at: 36) with: (nodes at: 46)).

	self createConstraints.
!

title

	^'Tree Demo'! !

!ClTreeDemo2 publicMethods !

constraintPointsDo: aBlock

	nodes do: [:each | aBlock value: each]!

createConstraints
		| spacing |
	spacing := 10.0.

	"add stays on each node"
	self constraintPointsDo: [:p |
		cs addStay: p x strength: ClStrength weak.
		cs addStay: p y strength: ClStrength weak].

	cs addConstraint: ((nodes at: 21) y cnEqual: (nodes at: 22) y).

	cs addConstraint: ((nodes at: 31) y cnEqual: (nodes at: 32) y).
	cs addConstraint: ((nodes at: 32) y cnEqual: (nodes at: 33) y).
	cs addConstraint: ((nodes at: 34) y cnEqual: (nodes at: 35) y).
	cs addConstraint: ((nodes at: 35) y cnEqual: (nodes at: 36) y).

	cs addConstraint: ((nodes at: 41) y cnEqual: (nodes at: 42) y).
	cs addConstraint: ((nodes at: 43) y cnEqual: (nodes at: 44) y).
	cs addConstraint: ((nodes at: 44) y cnEqual: (nodes at: 45) y).


	cs addConstraint: ((nodes at: 11) y+spacing cnLEQ: (nodes at: 21) y).

	cs addConstraint: ((nodes at: 21) y+spacing cnLEQ: (nodes at: 31) y).
	cs addConstraint: ((nodes at: 22) y+spacing cnLEQ: (nodes at: 34) y).

	cs addConstraint: ((nodes at: 32) y+spacing cnLEQ: (nodes at: 41) y).
	cs addConstraint: ((nodes at: 34) y+spacing cnLEQ: (nodes at: 43) y).
	cs addConstraint: ((nodes at: 36) y+spacing cnLEQ: (nodes at: 46) y)!

drawArea

	self setForegroundColor: 0.
	lines do: [:each |
		buffer
			drawLine: gc
			x1: each first x value rounded
			y1: each first y value rounded
			x2: each last x value rounded
			y2: each last y value rounded].
	self setForegroundColor: 5.
	nodes do: [:each |
		buffer
			fillRectangle: gc
			x: (each x value rounded - 5)
			y: (each y value rounded - 5)
			width: 10
			height: 10]!

initialize

	super initialize.
	nodes := LookupTable new.
	nodes at: 11 put: ((ClVariable newWith: 50 name: 'n11x') @ (ClVariable newWith: 10 name: 'n11y')).
	nodes at: 21 put: ((ClVariable newWith: 25 name: 'n21x') @ (ClVariable newWith: 60 name: 'n21y')).
	nodes at: 22 put: ((ClVariable newWith: 75 name: 'n22x') @ (ClVariable newWith: 60 name: 'n22y')).
	nodes at: 31 put: ((ClVariable newWith: 15 name: 'n31x') @ (ClVariable newWith: 110 name: 'n31y')).
	nodes at: 32 put: ((ClVariable newWith: 30 name: 'n32x') @ (ClVariable newWith: 110 name: 'n32y')).
	nodes at: 33 put: ((ClVariable newWith: 45 name: 'n33x') @ (ClVariable newWith: 110 name: 'n33y')).
	nodes at: 34 put: ((ClVariable newWith: 60 name: 'n34x') @ (ClVariable newWith: 110 name: 'n34y')).
	nodes at: 35 put: ((ClVariable newWith: 75 name: 'n35x') @ (ClVariable newWith: 110 name: 'n35y')).
	nodes at: 36 put: ((ClVariable newWith: 90 name: 'n36x') @ (ClVariable newWith: 110 name: 'n36y')).
	nodes at: 41 put: ((ClVariable newWith: 20 name: 'n41x') @ (ClVariable newWith: 160 name: 'n41y')).
	nodes at: 42 put: ((ClVariable newWith: 35 name: 'n42x') @ (ClVariable newWith: 160 name: 'n42y')).
	nodes at: 43 put: ((ClVariable newWith: 55 name: 'n43x') @ (ClVariable newWith: 160 name: 'n43y')).
	nodes at: 44 put: ((ClVariable newWith: 70 name: 'n44x') @ (ClVariable newWith: 160 name: 'n44y')).
	nodes at: 45 put: ((ClVariable newWith: 85 name: 'n45x') @ (ClVariable newWith: 160 name: 'n45y')).
	nodes at: 46 put: ((ClVariable newWith: 100 name: 'n46x') @ (ClVariable newWith: 160 name: 'n46y')).

	lines := OrderedCollection new.
	lines add: (Array with: (nodes at: 11) with: (nodes at: 21)).
	lines add: (Array with: (nodes at: 11) with: (nodes at: 22)).
	lines add: (Array with: (nodes at: 21) with: (nodes at: 31)).
	lines add: (Array with: (nodes at: 21) with: (nodes at: 32)).
	lines add: (Array with: (nodes at: 21) with: (nodes at: 33)).
	lines add: (Array with: (nodes at: 22) with: (nodes at: 34)).
	lines add: (Array with: (nodes at: 22) with: (nodes at: 35)).
	lines add: (Array with: (nodes at: 22) with: (nodes at: 36)).
	lines add: (Array with: (nodes at: 32) with: (nodes at: 41)).
	lines add: (Array with: (nodes at: 32) with: (nodes at: 42)).
	lines add: (Array with: (nodes at: 34) with: (nodes at: 43)).
	lines add: (Array with: (nodes at: 34) with: (nodes at: 44)).
	lines add: (Array with: (nodes at: 34) with: (nodes at: 45)).
	lines add: (Array with: (nodes at: 36) with: (nodes at: 46)).

	self createConstraints.

	^self!

title

	^'Tree Demo'! !

ClDemoWindow initializeAfterLoad!
ClBinaryTreeDemo initializeAfterLoad!
ClBoundedLineDemo initializeAfterLoad!
ClElasticBoundedLineDemo initializeAfterLoad!
ClBoxesDemo initializeAfterLoad!
ClCrossedQuadDemo initializeAfterLoad!
ClLineDemo initializeAfterLoad!
ClMidpointDemo initializeAfterLoad!
ClQuadDemo initializeAfterLoad!
ClBoundedQuadDemo initializeAfterLoad!
ClElasticQuadDemo initializeAfterLoad!
ClSequenceDiagramDemo1 initializeAfterLoad!
ClTreeDemo1 initializeAfterLoad!
ClTreeDemo2 initializeAfterLoad!
ClDemos initializeAfterLoad!

ClDemos loaded!