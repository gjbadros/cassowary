#!/uns/bin/python

from timer import Timer
from cassowary import *
import rand, sys

def UniformRandom():
	return rand.rand()/32768.0

def addDel(nCns = 900, nVars = 900, nResolves = 10000):
	timer = Timer()
	ineqProb = 0.12
	maxVars = 3

	print "starting timing test. nCns = %i, nVars = %i, nResolves = %i" %\
		(nCns, nVars, nResolves)

	timer.Start()
	solver = ClSimplexSolver()

	rgpclv = map(lambda i: ClVariable('x', i), range(nVars))
	for p in rgpclv:
		solver.AddStay(p)

	# Create a list of length nCns, to be filled in below
	rgpcns = map(None, range(nCns))
	for j in range(nCns):
		nvs = UniformRandom()*maxVars + 1
		expr = ClLinearExpression(UniformRandom()*20.0 - 10.0)
		for k in range(nvs):
			coeff = UniformRandom()*10-5
			e1 = rgpclv[int(UniformRandom()*nVars)] * coeff
			expr.AddExpression(e1)
		if UniformRandom() < ineqProb:
			rgpcns[j] = ClLinearInequality(expr, cnLEQ, 0)
		else:
			rgpcns[j] = ClLinearEquation(expr)
	
	print "done building data structures"
	print "time = %f" % timer.ElapsedTime()
	print

	timer.Start()
	cExceptions = 0
	for c in range(len(rgpcns)):
		try:
			solver.AddConstraint(rgpcns[c])
		except:
			# Assume it's an ExCLError, though it could be something
			# internal to SWIG...
			cExceptions = cExceptions + 1
			rgpcns[c] = None
	
	print "done adding constraints [%i exceptions]" % cExceptions
	print "time = %f" % timer.ElapsedTime()
	print
	timer.Start()

	e1Index = int(UniformRandom()*nVars)
	e2Index = int(UniformRandom()*nVars)

	edit1 = ClEditConstraint(rgpclv[e1Index], ClsStrong())
	edit2 = ClEditConstraint(rgpclv[e2Index], ClsStrong())
	solver.AddConstraint(edit1)
	solver.AddConstraint(edit2)

	print "done creating edit constraints -- about to start resolves"
	print "time = %f" % timer.ElapsedTime()
	print
	timer.Start()

	for m in range(nResolves):
		val1 = rgpclv[e1Index].Value() * 1.001
		val2 = rgpclv[e2Index].Value() * 1.001
		solver.Resolve(val1, val2)
	
	print "done resolves -- now removing constraints"
	print "time = %f" % timer.ElapsedTime()
	print
	solver.RemoveConstraint(edit1)
	solver.RemoveConstraint(edit2)

	timer.Start()

	for j in range(nCns):
		if rgpcns[j]:
			solver.RemoveConstraint(rgpcns[j])
#		print "removed number %d" % j

	print "done removing constraints and addDel timing test"
	print "time = %f" % timer.ElapsedTime()
	print

	return 1

def main():
	fAllOkResult = 1
	for test in ["addDel"]:
		print test + ":"
		fResult = eval("%s()" % test)
		fAllOkResult = fAllOkResult and fResult
		if not fResult:
			print "Failed!"
	
	if fAllOkResult:
		sys.exit(0)
	else:
		sys.exit(255)

if __name__ == '__main__':
	main()
