%module cassowary
%include typemaps.i
%{
#include "Cl.h"
#include "ClPoint.h"
%}


%typemap(python,in) vector<double> &{
	if (PyList_Check($source)) {
		int size = PyList_Size($source);
		int i;

		$target = new vector<double>;
		for (i=0; i<size; i++) {
			PyObject *o = PyList_GetItem($source, i);
			if (PyFloat_Check(o)) {
				$target->push_back(PyFloat_AsDouble(o));
			} else if (PyInt_Check(o)) {
				$target->push_back(PyInt_AsLong(o));
			} else {
				PyErr_SetString(PyExc_TypeError,
						"list must contain numbers");
				delete $target;
				return NULL;
			}
		}
	} else {
		PyErr_SetString(PyExc_TypeError, "expected list");
		return NULL;
	}
}

%typemap(python, freearg) vector<double> & {
	delete $source;
}

typedef double Number;
%apply double { Number };

class ClVariable {
public:

  ClVariable(Number value);
  %name(ClVariableNamed) ClVariable(char *name = "", Number value = 0.0);
  %name(ClVariablePrefix) ClVariable(
  	long number, char *prefix, Number value = 0.0);


  // Return the current value I hold.
  Number value() const
    { return my_value; }

  void set_value(Number value)
    { my_value = value; }
};

class ClPoint {
 public:
  ClPoint();
  %name(ClPointXY) ClPoint(Number x, Number y)
    : clv_x(x), clv_y(y)
    { }

  ClVariable X()
    { return clv_x; }

  ClVariable Y()
    { return clv_y; }

  Number Xvalue() const
    { return X().value(); }

  Number Yvalue() const
    { return Y().value(); }

};

class ClStrength {
};

const ClStrength &clsRequired();
const ClStrength &clsStrong();
const ClStrength &clsMedium();
const ClStrength &clsWeak();

%except(python) {
	try{
		$function
	} catch (ExCLError &err) {
		PyErr_SetString(PyExc_RuntimeError, err.description().c_str());
		return NULL;
	}
}

class ClConstraint {
};

class ClLinearEquation : public ClConstraint {

 public:
  // Constructor
 ClLinearEquation(const ClLinearExpression &cle,
		  const ClStrength strength,
		  double weight = 1.0);
 %name(ClLinearEquation1) ClLinearEquation(ClVariable clv,
		  const ClLinearExpression &cle,
		  const ClStrength strength,
		  double weight = 1.0);
 %name(ClLinearEquation2) ClLinearEquation(const ClLinearExpression &cle,
 		ClVariable clv, const ClStrength strength,
		double weight = 1.0);
};

enum ClInequalityOperator { cnLEQ, cnGEQ };

class ClLinearInequality : public ClConstraint {

 public:
 // Constructor
 ClLinearInequality(const ClLinearExpression &cle,
 		    const ClStrength strength, double weight = 1.0);
 %name(ClLinearInequality1) ClLinearInequality(ClVariable clv,
  		    ClInequalityOperator op,
		    const ClLinearExpression &cle,
		    const ClStrength strength,
		    double weight = 1.0);
 %name(ClLinearInequality2) ClLinearInequality(const ClLinearExpression &cle1,
		    ClInequalityOperator op,
		    const ClLinearExpression &cle2,
		    const ClStrength strength,
		    double weight = 1.0);
};

class ClLinearExpression  {
 public:

  // Convert from ClVariable to a ClLinearExpression
  // this replaces ClVariable::asLinearExpression
  ClLinearExpression(ClVariable clv, Number value = 1.0,
	  Number constant = 0.0);
  %name(ClLinearExpressionNum) ClLinearExpression(Number num = 0.0);

  virtual ~ClLinearExpression();

  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  ClLinearExpression times(Number x) const;

  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  %name(timesE) ClLinearExpression times(const ClLinearExpression &expr)
	const;

  // Return a new linear expression formed by adding x to self.
  ClLinearExpression plus(const ClLinearExpression &expr) const;

  // Return a new linear expression formed by subtracting x from self.
  ClLinearExpression minus(const ClLinearExpression &expr) const;

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  ClLinearExpression divide(Number x) const;

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  %name(divideE) ClLinearExpression divide(const ClLinearExpression &expr)
	const;

  // Return a new linear expression (aNumber/this).  Since the result
  // must be linear, this is permissible only if 'this' is a constant.
  ClLinearExpression divFrom(const ClLinearExpression &expr) const;

  // Return a new linear expression (aNumber-this).
  ClLinearExpression subtractFrom(const ClLinearExpression &expr) const
  { return expr.minus(*this); }

  // Add n*expr to this expression for another expression expr.
  ClLinearExpression &addExpression(const ClLinearExpression &expr, 
				    Number n = 1.0);

  // Add n*expr to this expression for another expression expr.
  // Notify the solver if a variable is added or deleted from this
  // expression.
  %name(addExpression1) ClLinearExpression
  	&addExpression(const ClLinearExpression &expr, Number n,
				    ClVariable subject,
				    ClSimplexSolver &solver);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  ClLinearExpression &addVariable(ClVariable v, Number c);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.  Notify the
  // solver if v appears or disappears from this expression.
  %name(addVariable1) ClLinearExpression &addVariable(ClVariable v,
				  Number c,
				  ClVariable subject,
				  ClSimplexSolver &solver);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  ClLinearExpression &setVariable(ClVariable v, Number c)
    {assert(c != 0.0);  my_terms[&v] = c; return *this; }

  // Return a variable in this expression.  (It is an error if this
  // expression is constant -- signal ExCLInternalError in that case).
  ClVariable anyPivotableVariable() const;
};

class ClSimplexSolver {

 public:

  // Constructor
  ClSimplexSolver();

  ~ClSimplexSolver();
  
  // Add constraints so that lower<=var<=upper.  (nil means no  bound.)
  ClSimplexSolver &addLowerBound(ClVariable v, Number lower);

  ClSimplexSolver &addUpperBound(ClVariable v, Number upper);

  ClSimplexSolver &addBounds(ClVariable v, Number lower,
  	Number upper);

  // Add the constraint cn to the tableau
  ClSimplexSolver &addConstraint(const ClConstraint &cn);

  // Add weak stays to the x and y parts of each point. These have
  // increasing weights so that the solver will try to satisfy the x
  // and y stays on the same point, rather than the x stay on one and
  // the y stay on another.
  // !!! Find some way to turn a Python list into a vector !!!
  ClSimplexSolver &addPointStays(const vector<const ClPoint *> &listOfPoints);

  ClSimplexSolver &addPointStay(const ClPoint &clp, Number weight);

  %name(addPointStayXY) ClSimplexSolver &addPointStay(ClVariable vx,
  	ClVariable vy, Number weight);

  // Add a stay of the given strength (default to weak) of v to the tableau
  ClSimplexSolver &addStay(ClVariable v,
			   const ClStrength &strength, Number weight =
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
//  void resolve(vector<double> &newEditConstants);
  void resolve(vector<double> &INPUT);

  %name(resolveXY) void resolve(Number x, Number y);

};

class ClEditConstraint : public ClConstraint {
 public:
  
  ClEditConstraint(ClVariable var,
		   const ClStrength &strength, Number weight = 1.0 );
};

