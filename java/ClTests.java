// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
// 
// ClTests.java

class ClTests {
  public static boolean justStay1()
  {
    try
      {
      boolean fOkResult = true;
      ClVariable x = new ClVariable(5);
      ClVariable y = new ClVariable(10);
      ClSimplexSolver solver = new ClSimplexSolver();
      
      solver.addStay(x);
      solver.addStay(y);
      fOkResult = fOkResult && ClVariable.clApprox(x,5);
      fOkResult = fOkResult && ClVariable.clApprox(y,10);
      System.out.println("x == " + x.value());
      System.out.println("y == " + y.value());
      return(fOkResult);
      } 
    catch (ExCLError error) 
      {
      System.out.println("Exception! " + error.description());
      return(false);
      }
  }
  
  public static boolean addDelete1()
  {
    try 
      {
      boolean fOkResult = true; 
      ClVariable x = new ClVariable("x");
      ClSimplexSolver solver = new ClSimplexSolver();
      
      solver.addConstraint( new ClLinearEquation( x, 100, ClStrength.clsWeak ) );
      
      ClLinearInequality c10 = new ClLinearInequality(x,CN.LEQ,10.0);
      ClLinearInequality c20 = new ClLinearInequality(x,CN.LEQ,20.0);
      
      solver
	.addConstraint(c10)
	.addConstraint(c20);
      
      fOkResult = fOkResult && ClVariable.clApprox(x,10.0);
      System.out.println("x == " + x.value());
      
      solver.removeConstraint(c10);
      fOkResult = fOkResult && ClVariable.clApprox(x,20.0);
      System.out.println("x == " + x.value());

      solver.removeConstraint(c20);
      fOkResult = fOkResult && ClVariable.clApprox(x,100.0);
      System.out.println("x == " + x.value());

      ClLinearInequality c10again = new ClLinearInequality(x,CN.LEQ,10.0);

      solver
	.addConstraint(c10)
	.addConstraint(c10again);
      
      fOkResult = fOkResult && ClVariable.clApprox(x,10.0);
      System.out.println("x == " + x.value());
    
      solver.removeConstraint(c10);
      fOkResult = fOkResult && ClVariable.clApprox(x,10.0);
      System.out.println("x == " + x.value());

      solver.removeConstraint(c10again);
      fOkResult = fOkResult && ClVariable.clApprox(x,100.0);
      System.out.println("x == " + x.value());

      return(fOkResult);
      } 
    catch (ExCLError error) 
      {
      System.out.println("Exception! " + error.description());
      return(false);
      }
  } 

  public static boolean addDelete2()
  {
    try 
      {
      boolean fOkResult = true; 
      ClVariable x = new ClVariable("x");
      ClVariable y = new ClVariable("y");
      ClSimplexSolver solver = new ClSimplexSolver();

      solver
	.addConstraint( new ClLinearEquation(x, 100.0, ClStrength.clsWeak))
	.addConstraint( new ClLinearEquation(y, 120.0, ClStrength.clsStrong));
      
      ClLinearInequality c10 = new ClLinearInequality(x,CN.LEQ,10.0);
      ClLinearInequality c20 = new ClLinearInequality(x,CN.LEQ,20.0);
      
      solver
	.addConstraint(c10)
	.addConstraint(c20);
      fOkResult = fOkResult && ClVariable.clApprox(x,10.0) && ClVariable.clApprox(y,120.0);
      System.out.println("x == " + x.value() + ", y == " + y.value());

      solver.removeConstraint(c10);
      fOkResult = fOkResult && ClVariable.clApprox(x,20.0) && ClVariable.clApprox(y,120.0);
      System.out.println("x == " + x.value() + ", y == " + y.value());
   
      ClLinearEquation cxy = new ClLinearEquation( CL.Times(2.0,x), y);
      solver.addConstraint(cxy);
      fOkResult = fOkResult && ClVariable.clApprox(x,20.0) && ClVariable.clApprox(y,40.0);
      System.out.println("x == " + x.value() + ", y == " + y.value());

      solver.removeConstraint(c20);
      fOkResult = fOkResult && ClVariable.clApprox(x,60.0) && ClVariable.clApprox(y,120.0);
      System.out.println("x == " + x.value() + ", y == " + y.value());

      solver.removeConstraint(cxy);
      fOkResult = fOkResult && ClVariable.clApprox(x,100.0) && ClVariable.clApprox(y,120.0);
      System.out.println("x == " + x.value() + ", y == " + y.value());
      
      return(fOkResult);
      } 
    catch (ExCLError error) 
      {
      System.out.println("Exception! " + error.description());
      return(false);
      }
  }

  public static boolean casso1()
  {
    try 
      {
      boolean fOkResult = true; 
      ClVariable x = new ClVariable("x");
      ClVariable y = new ClVariable("y");
      ClSimplexSolver solver = new ClSimplexSolver();

      solver
	.addConstraint( new ClLinearInequality(x,CN.LEQ,y) )
	.addConstraint( new ClLinearEquation(y, CL.Plus(x,3.0)) )
	.addConstraint( new ClLinearEquation(x,10.0,ClStrength.clsWeak) )
	.addConstraint( new ClLinearEquation(y,10.0,ClStrength.clsWeak) )
	;
   
      fOkResult = fOkResult && 
	( ClVariable.clApprox(x,10.0) && ClVariable.clApprox(y,13.0) ||
	  ClVariable.clApprox(x,7.0) && ClVariable.clApprox(y,10.0) );
      
      System.out.println("x == " + x.value() + ", y == " + y.value());
      return(fOkResult);
      } 
    catch (ExCLError error) 
      {
      System.out.println("Exception! " + error.description());
      return(false);
      }
    }

  public static boolean inconsistent1()
  {
    try 
      {
      ClVariable x = new ClVariable("x");
      ClSimplexSolver solver = new ClSimplexSolver();
      
      solver
	.addConstraint( new ClLinearEquation(x,10.0) )
	.addConstraint( new ClLinearEquation(x, 5.0) );
      
      // no exception, we failed!
      return(false);
      } 
    catch (ExCLRequiredFailure err)
      {
      // we want this exception to get thrown
      System.out.println("Success -- got the exception");
      return(true);
      }
    catch (ExCLError error) 
      {
      System.out.println("Exception! " + error.description());
      return(false);
      }
  }

  public static boolean inconsistent2()
  {
    try 
      {
      ClVariable x = new ClVariable("x");
      ClSimplexSolver solver = new ClSimplexSolver();
      
      solver
	.addConstraint( new ClLinearInequality(x,CN.GEQ,10.0) )
	.addConstraint( new ClLinearInequality(x,CN.LEQ, 5.0) );

      // no exception, we failed!
      return(false);
      } 
    catch (ExCLRequiredFailure err)
      {
      // we want this exception to get thrown
      System.out.println("Success -- got the exception");
      return(true);
      }
    catch (ExCLError error) 
      {
      System.out.println("Exception! " + error.description());
      return(false);
      }
  }

//   boolean addDel(int nCns, int nVars, int nResolves)
//   {
//     //Timer timer;
//   // FIXGJB: from where did .12 come?
//   final double ineqProb = 0.12;
//   final int maxVars = 3;

//   cout << "starting timing test. nCns = " << nCns
//        << ", nVars = " << nVars << ", nResolves = " << nResolves << endl;

//   timer.Start();
//   ClSimplexSolver solver;

//   ClVariable **rgpclv = new PClVariable[nVars];
//   for (int i = 0; i < nVars; i++)
//     {
//     rgpclv[i] = new ClVariable(i,"x");
//     solver.addStay(*rgpclv[i]);
//     }

//   ClConstraint **rgpcns = new PClConstraint[nCns];
//   int nvs = 0;
//   int k;
//   int j;
//   double coeff;
//   for (j = 0; j < nCns; j++)
//     {
//     // number of variables in this constraint
//     nvs = int(Math.random()*maxVars) + 1;
//     ClLinearExpression expr = Math.random()*20.0 - 10.0;
//     for (k = 0; k < nvs; k++)
//        {
//        coeff = UniformRandom()*10 - 5;
//        expr.addExpression(*(rgpclv[int(UniformRandom()*nVars)]) * coeff);
//        }
//     if (UniformRandom() < ineqProb)
//        {
//        rgpcns[j] = new ClLinearInequality(expr);
//        }
//     else
//        {  
//        rgpcns[j] = new ClLinearEquation(expr);
//        }
//     }

//   cout << "done building data structures" << endl;
//   cout << "time = " << timer.ElapsedTime() << "\n" << endl;
//   timer.Start();
//   int cExceptions = 0;
//   for (j = 0; j < nCns; j++)
//     {
//     // add the constraint -- if it's incompatible, just ignore it
//     // FIXGJB: exceptions are extra expensive in C++, so this might not
//     // be particularly fair
//     try
//       {
//       solver.addConstraint(*(rgpcns[j]));
//       }
//     catch (ExCLRequiredFailure &)
//       {
//       cExceptions++;
//       rgpcns[j] = NULL;
//       }
//     }
//   // FIXGJB end = Timer.now();
//   cout << "done adding constraints [" << cExceptions << " exceptions]" << endl;
//   cout << "time = " << timer.ElapsedTime() << "\n" << endl;
//   timer.Start();

//   int e1Index = int(UniformRandom()*nVars);
//   int e2Index = int(UniformRandom()*nVars);

//   ClEditConstraint edit1(*(rgpclv[e1Index]),clsStrong());
//   ClEditConstraint edit2(*(rgpclv[e2Index]),clsStrong());

//   solver
//     .addConstraint(edit1)
//     .addConstraint(edit2);

//   cout << "done creating edit constraints -- about to start resolves" << endl;
//   cout << "time = " << timer.ElapsedTime() << "\n" << endl;
//   timer.Start();

//   // FIXGJB start = Timer.now();
//   for (int m = 0; m < nResolves; m++)
//     {
//     vector<Number> vals;
//     vals.push_back(rgpclv[e1Index]->value() * 1.001);
//     vals.push_back(rgpclv[e2Index]->value() * 1.001);
//     solver.resolve(vals);
//     }

//   // cout << "run time: " <<

//   cout << "done resolves -- now removing constraints" << endl;
//   cout << "time = " << timer.ElapsedTime() << "\n" <<endl;
//   solver.removeConstraint(edit1);
//   solver.removeConstraint(edit2);
  
//   timer.Start();

//   for (j = 0; j < nCns; j++)
//     {
//     if (rgpcns[j])
//       {
//       solver.removeConstraint(*(rgpcns[j]));
//       }
//     }

//   // FIXGJB end = Timer.now();
//   // cout << "Total remove time: " 
//   //      << "remove time per cn"
//   cout << "done removing constraints and addDel timing test" << endl;
//   cout << "time = " << timer.ElapsedTime() << "\n" <<endl;

//   timer.Start();

//   return true;
// }


  public static final void main( String[] args )
  {
    try 
      {
      boolean fAllOkResult = true;
      boolean fResult;
      
      System.out.println("justStay1:");
      fResult = justStay1(); fAllOkResult &= fResult;
      if (!fResult) System.out.println("Failed!");
      
      System.out.println("addDelete1:");
      fResult = addDelete1(); fAllOkResult &= fResult;
      if (!fResult) System.out.println("Failed!");
      
      System.out.println("addDelete2:");
      fResult = addDelete2(); fAllOkResult &= fResult;
      if (!fResult) System.out.println("Failed!");
      
      System.out.println("casso1:");
      fResult = casso1(); fAllOkResult &= fResult;
      if (!fResult) System.out.println("Failed!");
      
      System.out.println("inconsistent1:");
      fResult = inconsistent1(); fAllOkResult &= fResult;
      if (!fResult) System.out.println("Failed!");
      
      System.out.println("inconsistent2:");
      fResult = inconsistent2(); fAllOkResult &= fResult;
      if (!fResult) System.out.println("Failed!");
      
      //     System.out.println("addDel:");
      //     fResult = addDel(); fAllOkResult &= fResult;
      //     if (!fResult) System.out.println("Failed!");
      
      } 
    catch (Exception err)
      {
      System.err.println("Random exception!" + err);
      }
  }
}
