%module cassowary
%{
#include "Cl.h"
#include "ClPoint.h"
%}

class ClVariable {
public:

  ClVariable(char *name, double value);

  // Return the current value I hold.
  double value() const
    { return my_value; }

  void set_value(double value)
    { my_value = value; }

};

class ClPoint {
 public:
  ClPoint(double x, double y)
    : clv_x(x), clv_y(y)
    { }

  ClVariable &X()
    { return clv_x; }

  ClVariable &Y()
    { return clv_y; }

  double Xvalue() const
    { return X().value(); }

  double Yvalue() const
    { return Y().value(); }

};

class ClStrength {
};

ClStrength &clsRequired();
ClStrength &clsStrong();
ClStrength &clsMedium();
ClStrength &clsWeak();

%except(python) {
	try{
		$function
	} catch (ExCLError &err) {
		PyErr_SetString(PyExc_RuntimeError, err.description());
		return NULL;
	}
}

class ClConstraint {
};

class ClLinearEquation : public ClConstraint {

 public:
  // Constructor
 ClLinearEquation(const ClVariable &clv,
		  const ClLinearExpression &cle,
		  const ClStrength strength,
		  double weight = 1.0);
};

enum ClInequalityOperator { cnLEQ, cnGEQ };

class ClLinearInequality : public ClConstraint {

 public:
 // Constructor
 ClLinearInequality(const ClLinearExpression &cle1,
		    ClInequalityOperator op,
		    const ClLinearExpression &cle2,
		    const ClStrength strength,
		    double weight = 1.0);
};

class ClSimplexSolver {

 public:

  // Constructor
  ClSimplexSolver();

  ~ClSimplexSolver();
  
  // Add constraints so that lower<=var<=upper.  (nil means no  bound.)
  ClSimplexSolver &addLowerBound(const ClVariable &v, double lower);

  ClSimplexSolver &addUpperBound(const ClVariable &v, double upper);

  ClSimplexSolver &addBounds(const ClVariable &v, double lower,
  	double upper);

  // Add the constraint cn to the tableau
  ClSimplexSolver &addConstraint(const ClConstraint &cn);

  // Add weak stays to the x and y parts of each point. These have
  // increasing weights so that the solver will try to satisfy the x
  // and y stays on the same point, rather than the x stay on one and
  // the y stay on another.
  // !!! Find some way to turn a Python list into a vector !!!
  ClSimplexSolver &addPointStays(const vector<const ClPoint *> &listOfPoints);

  ClSimplexSolver &addPointStay(const ClPoint &clp, double weight);

  %name(addPointStayXY) ClSimplexSolver &addPointStay(const ClVariable &vx,
  	const ClVariable &vy, double weight);

  // Add a stay of the given strength (default to weak) of v to the tableau
  ClSimplexSolver &addStay(const ClVariable &v,
			   const ClStrength &strength, double weight =
			   1.0 );

  // Remove the constraint cn from the tableau
  // Also remove any error variable associated with cn
  ClSimplexSolver &removeConstraint(const ClConstraint &pcn);

  // Re-initialize this solver from the original constraints, thus
  // getting rid of any accumulated numerical problems.  (Actually,
  // Alan hasn't observed any such problems yet, but here's the method
  // anyway.)
  void reset();

  // Re-solve the current collection of constraints for new values for
  // the constants of the edit variables.
  void resolve(const vector<double> &newEditConstants);

  %name(resolveXY) void resolve(double x, double y);

};

class ClLinearExpression  {
 public:

  // Convert from ClVariable to a ClLinearExpression
  // this replaces ClVariable::asLinearExpression
  ClLinearExpression(const ClVariable &clv, double value = 1.0,
	  double constant = 0.0);

  %name(ClLinearExpressionNum) ClLinearExpression(double num = 0.0);

  virtual ~ClLinearExpression();

  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  ClLinearExpression times(double x) const;

  // Return a new linear expression formed by adding x to self.
  ClLinearExpression plus(const ClLinearExpression &expr) const;

  // Return a new linear expression formed by subtracting x from self.
  ClLinearExpression minus(const ClLinearExpression &expr) const;

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  ClLinearExpression divide(double x) const;

  // Python operator overloads
  %addmethods {
  	ClLinearExpression __add__(ClLinearExpression &expr) {
	   return self->plus(expr);
	}
  }
};

class ClEditConstraint : public ClConstraint {
 public:
  
  ClEditConstraint(const ClVariable &var,
		   const ClStrength &strength, double weight = 1.0 );
};

