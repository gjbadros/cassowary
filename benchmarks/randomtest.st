

!ClTimingTests class publicMethods !

addEdit: fileStream nCns: nCns nSolvers: nSolvers nResolves: nResolves testNo: testNo
	"Helper method for randomTests: method.  Do one test.  For each test 
		we generate nCns random constraints on nCns vars (same number of variables and constraints
		in this version).  We measure the time to add the constraints, then to incrementally add and 
		delete the two edit constraints.  Also we make nSolvers copies of the tableau, to get better
		timings for adding and deleting the edits."

		| ineqProb maxVars nVars grain vars solvers rand nvs cn cns exp coeff v nAdded nUsed e1Index e2Index edit1 edit2 
				cnAddTime resolveTime editAddTime editDeleteTime r |

	"probability of an inequality constraint"
	ineqProb := 0.12.
	"maximum number of variables per constraint"
	maxVars := 3.
	"grain size for coefficients.  (The coefficients in the constraints are restricted to be integral 
		multiples of the given grain size.)"
	grain := 1.0e-4.
	"number of variables is the same as the number of constraints"
	nVars := nCns.


	solvers := Array new: nSolvers.
	1 to: nSolvers do: [:i | solvers at: i put: ClSimplexSolver new].
	rand := EsRandom new.

	"generate variables and stays"
	vars := Array new: nVars.
	1 to: nVars do: [:i | 
		vars at: i put: (ClVariable newWith: 0.0 name: 'x', i printString).
		1 to: nSolvers do: [:s | (solvers at: s) addStay: (vars at: i)]].

	 "make 2 times as many constraints as needed, since some will be unsatisfiable and we won't count those"
	cns := Array new: 2*nCns.
	"generate constraints"
	1 to: cns size do: [:j |
		"number of variables in this constraint"
		nvs := (rand next*maxVars) truncated + 1.
		"round the constant to grain size, to prevent numerical problems"
		r := (rand next/grain) truncated * grain.
		exp := r*20.0 - 10.0.
		1 to: nvs do: [:k | 
			"round the coefficient to grain size, to prevent numerical problems"
			r := (rand next/grain) truncated * grain.
			coeff := r*10.0 - 5.0.
			v := vars at: ((rand next*nCns) truncated + 1).
			exp := v*coeff + exp].	
		rand next < ineqProb ifTrue: [cn := exp cnGEQ: 0.0] ifFalse: [cn := exp cnEqual: 0.0].
		cns at: j put: cn].

	cnAddTime := Time microsecondsToRun: [
		1 to: nSolvers do: [:s |
			nUsed := 0.  nAdded := 0.
			[nAdded < nCns] whileTrue: [
				nUsed := nUsed+1.  nAdded := nAdded+1.
				"add the constraint.  If it's incompatible just ignore it (and don't count it toward the total)"
				[(solvers at: s) addConstraint: (cns at: nUsed)] when: ExCLRequiredFailure do: 
					[:signal | nAdded := nAdded-1.  signal exitWith: false]]]].

	e1Index := (rand next*nVars) truncated + 1.
	e2Index := (rand next*nVars) truncated + 1.

	edit1 := ClEditConstraint variable: (vars at: e1Index) strength: ClStrength strong.
	edit2 := ClEditConstraint variable: (vars at: e1Index) strength: ClStrength strong.
	editAddTime := Time microsecondsToRun: 
		[1 to: nSolvers do: [:s | (solvers at: s) addConstraint: edit1.  (solvers at: s) addConstraint: edit2]].

	resolveTime := Time microsecondsToRun: [
		1 to: nSolvers do: [:s |
			1 to: nResolves do: [:i |
				(solvers at: s) resolve: (Array with: edit1 variable value*1.001 with: edit2 variable value*1.001)]]].

	editDeleteTime := Time microsecondsToRun: 
		[1 to: nSolvers do: [:s | (solvers at: s) removeConstraint: edit1.  (solvers at: s) removeConstraint: edit2]].

	"print out the row to the output file in comma-separated value format"
	nCns printOn: fileStream.  fileStream nextPutAll: ','.
	nSolvers printOn: fileStream.  fileStream nextPutAll: ','.
	nResolves printOn: fileStream.  fileStream nextPutAll: ','.
	testNo printOn: fileStream.  fileStream nextPutAll: ','.
	"Print out the totals.  Convert all times to milliseconds"
	(cnAddTime asFloat / 1000.0)  printOn: fileStream.  fileStream nextPutAll: ','.
	(editAddTime asFloat / 1000.0)  printOn: fileStream.  fileStream nextPutAll: ','.
	(resolveTime asFloat / 1000.0)  printOn: fileStream.  fileStream nextPutAll: ','.
	(editDeleteTime asFloat / 1000.0)  printOn: fileStream.  fileStream nextPutAll: ','.
	"now print the times per constraint or edit"
	(cnAddTime asFloat /  (1000.0 * nCns * nSolvers ))  printOn: fileStream.  fileStream nextPutAll: ','.
	"there are 2 edit constraints, hence the 2 in the following expression:"
	(editAddTime asFloat / (1000.0 * 2 * nSolvers))  printOn: fileStream.  fileStream nextPutAll: ','.
	(resolveTime asFloat / (1000.0 * nResolves * nSolvers ))  printOn: fileStream.  fileStream nextPutAll: ','.
	(editDeleteTime asFloat / (1000.0 * 2 * nSolvers))  printOn: fileStream.  fileStream cr.

	
! !