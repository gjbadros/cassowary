

Application create: #ClTests with:
    (#( BuckyTestLauncher ClKernel)
        collect: [:each | Smalltalk at: each ifAbsent: [
            Application errorPrerequisite: #ClTests missing: each]])!

ClTests becomeDefault!
Object subclass: #ClTestsRoot
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: 'ClConstants '!

ClTests becomeDefault!
ClTestsRoot subclass: #ClCassowaryTests
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''!

ClTests becomeDefault!
Object subclass: #ClTimingTests
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: 'ClConstants '!

ClTests becomeDefault!
RbftAbstractTestsLauncher subclass: #ClTestsLauncher
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''!

ClTests becomeDefault!
Application subclass: #ClTests
    instanceVariableNames: ''
    classVariableNames: ''
    poolDictionaries: ''!


ClTests becomeDefault!

!ClCassowaryTests class publicMethods !

addDelete1: aLauncher
	"add/delete #1"

		| x solver result c10 c10again c20 |

	x := ClVariable new name: 'x'.

	solver := ClSimplexSolver new.
	solver addConstraint: (x cnEqual: 100.0 strength: ClStrength weak).
	c10 := x cnLEQ: 10.0.
	c20 := x cnLEQ: 20.0.
	solver addConstraint: c10; addConstraint: c20.
	result := (x value clApprox: 10.0).
	solver removeConstraint: c10.
	result := result & (x value clApprox: 20.0).
	solver removeConstraint: c20.
	result := result & (x value clApprox: 100.0).
	c10again := x cnLEQ: 10.0.
	solver addConstraint: c10; addConstraint: c10again.
	result := result & (x value clApprox: 10.0).
	solver removeConstraint: c10.
	result := result & (x value clApprox: 10.0).
	solver removeConstraint: c10again.
	result := result & (x value clApprox: 100.0).

	^result!

addDelete2: aLauncher
	"add/delete #2"

		| x y solver result c10 c10again c20 cxy |

	x := ClVariable new name: 'x'.
	y := ClVariable new name: 'y'.

	solver := ClSimplexSolver new.
	solver addConstraint: (x cnEqual: 100.0 strength: ClStrength weak).
	solver addConstraint: (y cnEqual: 120.0 strength: ClStrength strong).
	c10 := x cnLEQ: 10.0.
	c20 := x cnLEQ: 20.0.
	solver addConstraint: c10; addConstraint: c20.
	result := (x value clApprox: 10.0) & (y value clApprox: 120.0).
	solver removeConstraint: c10.
	result := result & (x value clApprox: 20.0) & (y value clApprox: 120.0).
	cxy := 2*x cnEqual: y.
	solver addConstraint: cxy.
	result := result & (x value clApprox: 20.0) & (y value clApprox: 40.0).
	solver removeConstraint: c20.
	result := result & (x value clApprox: 60.0) & (y value clApprox: 120.0).
	solver removeConstraint: cxy.
	result := result & (x value clApprox: 100.0) & (y value clApprox: 120.0).

	^result!

casso1: aLauncher
	"cassowary #1"
	"
		req'd	x<=y
		req'd	y=x+3
 		weak	x=10
		weak	y=10

		The solution is x=7, y=10  or x=10, y=13 "

		| x y solver result |

	x := ClVariable new name: 'x'.
	y := ClVariable new name: 'y'.

	solver := ClSimplexSolver new.
	solver addConstraint: (x cnLEQ: y).
	solver addConstraint: (y cnEqual: x+3.0).
	solver addConstraint: (x cnEqual: 10.0 strength: ClStrength weak).
	solver addConstraint: (y cnEqual: 10.0 strength: ClStrength weak).

	result :=
		(x value clApprox: 10.0) & (y value clApprox: 13.0)
			or: [	(x value clApprox: 7.0) & (y value clApprox: 10.0) ].

	^result!

inconsistent1: aLauncher
	"inconsistent constraints #1"
	"
		req'd	x=10
		req'd	x=5
	"

		| x solver signalled |

	x := ClVariable new name: 'x'.

	solver := ClSimplexSolver new.
	solver addConstraint: (x cnEqual: 10.0).
	signalled := false.
	[solver addConstraint: (x cnEqual: 5.0)] when: ExCLRequiredFailure do: [:signal | signalled := true. signal exitWith: false].
	^signalled
!

inconsistent2: aLauncher
	"inconsistent constraints #2"
	"
		req'd	x>=10
		req'd	x<=5
	"

		| x solver signalled |

	x := ClVariable new name: 'x'.

	solver := ClSimplexSolver new.
	solver addConstraint: (x cnGEQ: 10.0).
	signalled := false.
	[solver addConstraint: (x cnLEQ: 5.0)] when: ExCLRequiredFailure do: [:signal | signalled := true. signal exitWith: false].
	^signalled
!

title

	^'Alan''s simplex solver tests'! !

!ClTestsLauncher class publicMethods !

open
	"ClTestsLauncher open"
	^self new open
! !

!ClTestsLauncher publicMethods !

rootTestClass

	^ClTestsRoot!

testCleanup
	"who knows what this should do ... just override it ..."!

title

	^'Constraint Library Tests'! !

!ClTimingTests class publicMethods !

addDel: nCns nVars: nVars nResolves: nResolves
	"self addDel: 300 nVars: 300 nResolves: 1000"

		| ineqProb maxVars vars solver rand nvs cn cns exp coeff v e1Index e2Index edit1 edit2 start end |

	"probability of an inequality constraint"
	ineqProb := 0.12.
	"maximum number of variables per constraint"
	maxVars := 3.

	Transcript cr; show: 'starting timing test.  nCns=' , nCns printString , 
		' nVars=' , nVars printString , ' nResolves=' , nResolves printString; cr.

	solver := ClSimplexSolver new.
	rand := EsRandom new.

	"generate variables and stays"
	vars := Array new: nVars.
	1 to: nVars do: [:i | 
		vars at: i put: (ClVariable newWith: 0.0 name: 'x', i printString).
		solver addStay: (vars at: i)].

	cns := Array new: nCns.
	"generate constraints"
	1 to: nCns do: [:j |
		"number of variables in this constraint"
		nvs := (rand next*maxVars) truncated + 1.
		exp := rand next*20.0 - 10.0.
		1 to: nvs do: [:k | 
			coeff := rand next*10.0 - 5.0.
			v := vars at: ((rand next*nVars) truncated + 1).
			exp := v*coeff + exp].	
		rand next < ineqProb ifTrue: [cn := exp cnGEQ: 0.0] ifFalse: [cn := exp cnEqual: 0.0].
		cns at: j put: cn].

	start := Time now.
	1 to: nCns do: [:i |
		"add the constraint.  If it's incompatible just ignore it"
		[solver addConstraint: (cns at: i)] when: ExCLRequiredFailure do: [:signal | cns at: i put: nil.  signal exitWith: false]].
	end := Time now.
	Transcript show: 'total add time: ' , (end subtractTime: start) asSeconds printString , ' seconds'; cr.
	Transcript show: 'add time per cn: ' , ((end subtractTime: start) asSeconds *1000.0 / nCns) printString , ' msecs'; cr.

	e1Index := (rand next*nVars) truncated + 1.
	e2Index := (rand next*nVars) truncated + 1.

	edit1 := ClEditConstraint variable: (vars at: e1Index) strength: ClStrength strong.
	edit2 := ClEditConstraint variable: (vars at: e1Index) strength: ClStrength strong.
	solver addConstraint: edit1.
	solver addConstraint: edit2.

	Transcript show: 'done creating constraints -- about to start resolves'; cr.
	start := Time now.
	1 to: nResolves do: [:i |
		solver resolve: (Array with: edit1 variable value*1.001 with: edit2 variable value*1.001)].
	end := Time now.
	Transcript show: 'run time: ' , (end subtractTime: start) asSeconds printString , ' seconds'; cr.

	start := Time now.
	1 to: nCns do: [:i |
		"remove each constraint"
		cn := cns at: i.
		cn notNil ifTrue: [solver removeConstraint: cn]].
	end := Time now.
	Transcript show: 'total remove time: ' , (end subtractTime: start) asSeconds printString , ' seconds'; cr.
	Transcript show: 'remove time per cn: ' , ((end subtractTime: start) asSeconds *1000.0 / nCns) printString , ' msecs'; cr.
!

addDelEdit: nCns nVars: nVars nResolves: nResolves
	"self addDelEdit: 30 nVars: 30 nResolves: 1000"

		| ineqProb maxVars vars solver rand nvs cn cns exp coeff v e1Index e2Index edit1 edit2 t
			addTime removeTime |

	"probability of an inequality constraint"
	ineqProb := 0.12.
	"maximum number of variables per constraint"
	maxVars := 3.

	Transcript cr; show: 'starting timing test.  nCns=' , nCns printString , 
		' nVars=' , nVars printString , ' nResolves=' , nResolves printString; cr.

	solver := ClSimplexSolver new.
	rand := EsRandom new.

	"generate variables and stays"
	vars := Array new: nVars.
	1 to: nVars do: [:i | 
		vars at: i put: (ClVariable newWith: 0.0 name: 'x', i printString).
		solver addStay: (vars at: i)].

	cns := Array new: nCns.
	"generate constraints"
	1 to: nCns do: [:j |
		"number of variables in this constraint"
		nvs := (rand next*maxVars) truncated + 1.
		exp := rand next*20.0 - 10.0.
		1 to: nvs do: [:k | 
			coeff := rand next*10.0 - 5.0.
			v := vars at: ((rand next*nVars) truncated + 1).
			exp := v*coeff + exp].	
		rand next < ineqProb ifTrue: [cn := exp cnGEQ: 0.0] ifFalse: [cn := exp cnEqual: 0.0].
		cns at: j put: cn].

	t := Time microsecondsToRun: [
		1 to: nCns do: [:i |
			"add the constraint.  If it's incompatible just ignore it"
			[solver addConstraint: (cns at: i)] when: ExCLRequiredFailure do: 
					[:signal | cns at: i put: nil.  signal exitWith: false]]].
	Transcript show: 'total add time: ' , (t/1.0e6) printString , ' seconds'; cr.
	Transcript show: 'add time per cn: ' , ((t/1000.0) / nCns) printString , ' msecs'; cr.

	e1Index := (rand next*nVars) truncated + 1.
	e2Index := (rand next*nVars) truncated + 1.

	edit1 := ClEditConstraint variable: (vars at: e1Index) strength: ClStrength strong.
	edit2 := ClEditConstraint variable: (vars at: e1Index) strength: ClStrength strong.
	addTime := Time microsecondsToRun: [solver addConstraint: edit1. solver addConstraint: edit2].
	removeTime := Time microsecondsToRun: [solver removeConstraint: edit1. solver removeConstraint: edit2].

	Transcript show: 'add time per edit: ' , (addTime/2000.0) printString , ' msecs'; cr.
	Transcript show: 'remove time per edit: ' , (removeTime/2000.0) printString , ' msecs'; cr.
!

random: nCns nVars: nVars nResolves: nResolves
	"self random: 300 nVars: 300 nResolves: 1000"

		| ineqProb maxVars vars solver rand nvs cn exp coeff v e1Index e2Index edit1 edit2 start end |

	"probability of an inequality constraint"
	ineqProb := 0.12.
	"maximum number of variables per constraint"
	maxVars := 3.

	Transcript cr; show: 'starting timing test.  nCns=' , nCns printString , 
		' nVars=' , nVars printString , ' nResolves=' , nResolves printString; cr.

	solver := ClSimplexSolver new.
	rand := EsRandom new.

	"generate variables and stays"
	vars := Array new: nVars.
	1 to: nVars do: [:i | 
		vars at: i put: (ClVariable newWith: 0.0 name: 'x', i printString).
		solver addStay: (vars at: i)].

	"generate constraints"
	1 to: nCns do: [:j |
		"number of variables in this constraint"
		nvs := (rand next*maxVars) truncated + 1.
		exp := rand next*20.0 - 10.0.
		1 to: nvs do: [:k | 
			coeff := rand next*10.0 - 5.0.
			v := vars at: ((rand next*nVars) truncated + 1).
			exp := v*coeff + exp].	
		rand next < ineqProb ifTrue: [cn := exp cnGEQ: 0.0] ifFalse: [cn := exp cnEqual: 0.0].
		"add the constraint.  If it's incompatible just ignore it"
		[solver addConstraint: cn] when: ExCLRequiredFailure do: [:signal | signal exitWith: false]].

	e1Index := (rand next*nVars) truncated + 1.
	e2Index := (rand next*nVars) truncated + 1.

	edit1 := ClEditConstraint variable: (vars at: e1Index) strength: ClStrength strong.
	edit2 := ClEditConstraint variable: (vars at: e1Index) strength: ClStrength strong.
	solver addConstraint: edit1.
	solver addConstraint: edit2.

	Transcript show: 'done creating constraints -- about to start resolves'; cr.
	start := Time now.
	1 to: nResolves do: [:i |
		solver resolve: (Array with: edit1 variable value*1.001 with: edit2 variable value*1.001)].
	end := Time now.
	Transcript show: 'run time: ' , (end subtractTime: start) asSeconds printString , ' seconds'; cr.
! !

ClTestsRoot initializeAfterLoad!
ClCassowaryTests initializeAfterLoad!
ClTimingTests initializeAfterLoad!
ClTestsLauncher initializeAfterLoad!
ClTests initializeAfterLoad!

ClTests loaded!