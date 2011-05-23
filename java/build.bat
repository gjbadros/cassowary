javac -d classes -source 1.3 CL.java ClTableau.java ClAbstractVariable.java ClTestColumns.java ClConstraint.java ClTests.java ClDouble.java ClVariable.java ClDummyVariable.java ExCLConstraintNotFound.java ClEditConstraint.java ExCLError.java ClEditInfo.java ExCLInternalError.java ClEditOrStayConstraint.java ExCLNonlinearExpression.java ClLinearConstraint.java ExCLNotEnoughStays.java ClLinearEquation.java ExCLRequiredFailure.java ClLinearExpression.java ExCLTooDifficult.java ClLinearInequality.java ClObjectiveVariable.java Set.java sym.java ClPoint.java testClLinearExpression.java ClSimplexSolver.java testClStrength.java ClSlackVariable.java Timer.java ClStayConstraint.java ClStrength.java ClSymbolicWeight.java
cd demos
javac -d ../classes -cp ../classes -source 1.3 DraggableBox.java QuadDemo.java
cd ..

java -classpath classes EDU.Washington.grad.gjb.cassowary.ClTests
@rem Below does not work -- needs appletviewer quaddemo.htm, but that fails to find the class
@rem java -classpath classes EDU.Washington.grad.gjb.cassowary_demos.QuadDemo 
