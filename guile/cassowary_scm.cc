// $Id$
// By Greg J. Badros -- 18 July 1998
//
// A Guile Scheme wrapper for the Cassowary Constraint Solving Toolkit
//
// TODO
// Catch internal errors throughout
// convenience wrappers (e.g., cl-half)
// expression-side-effecting
// standard strength variables
// extra strength args to add-constraint, add-editvar
// cl-begin-edit, cl-end-edit, cl-suggest-value

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define CASSOWARY_SCM_IMPLEMENTATION
#include "cassowary_scm.h"
#include "cassowary_scm.hpp"

#include <guile/gh.h>

#include <iostream.h>
#include <assert.h>
#include <strstream.h>
#include "scwm-snarf.h"

#include <guile/gh.h>

#include "ClVariable.h"
#include "ClSymbolicWeight.h"
#include "ClLinearExpression.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"
#include "ClSimplexSolver.h"
#include "ClErrors.h"

#include <vector>

// C++ version of this macro -- uses true and false, not True and False
#define COPY_BOOL_OR_ERROR_DEFAULT_FALSE(var,flag,pos,func) \
  do { \
  if (flag == SCM_BOOL_T) var = true; \
  else if (flag == SCM_BOOL_F || flag == SCM_UNDEFINED) var = false; \
  else scm_wrong_type_arg(func,pos,flag); \
  } while (0)


#define MAKE_SMOBFUNS(T) \
static scm_smobfuns T ## _smobfuns = { \
  &mark_ ## T, \
  &free_ ## T, \
  &print_ ## T,  0 }

#define REGISTER_SMOBFUNS(T) scm_tc16_ ## T = scm_newsmob(& T ## _smobfuns)



extern "C" {

#define NEW(x) ((x *) safemalloc(sizeof(x)))
#define NEWC(c,x) ((x *) safemalloc((c)*sizeof(x)))
#define FREE(x) free(x)

inline SCM SCM_BOOL_FromF(bool f) { return (f? SCM_BOOL_T: SCM_BOOL_F); }
inline bool FUnsetSCM(SCM scm) { return (scm == SCM_UNDEFINED || scm == SCM_BOOL_F); }


//// ClVariable wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_variable

long SCMTYPEID;

SCM mark_cl_variable(SCM scm)
{
  SCM_SETGC8MARK(scm);
  return SCM_BOOL_F;
}

size_t free_cl_variable(SCM scm)
{
  //  ClVariable *pclv = PclvFromScm(scm);
  //  delete pclv;  // FIXGJB: don't delete until memory audit
  return 0;
}

int print_cl_variable(SCM scm, SCM port, scm_print_state *pstate)
{
  strstream ss;
  ClVariable *pclv = PclvFromScm(scm);
  ss << "#<cl_variable " << *pclv << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}

SCWM_PROC(cl_variable_p, "cl-variable?", 1, 0, 0,
           (SCM obj))
  /** Return #t if OBJ is a constraint variable, #f otherwise. */
#define FUNC_NAME s_cl_variable_p
{
  return SCM_BOOL_FromF(FIsClVariableScm(obj));
}
#undef FUNC_NAME

SCWM_PROC(make_cl_variable, "make-cl-variable", 0, 2, 0,
           (SCM name, SCM value))
  /** Return a newly-created constraint variable.
The created variable has the name NAME, and an initial
value of VALUE.  Be sure to add a stay constraint on the
variable if you want its value to not change. */
#define FUNC_NAME s_make_cl_variable
{
  char *szName = NULL;
  double n = 0;
  int iarg = 1;

  if (!FUnsetSCM(name)) {
    if (!gh_string_p(name)) {
      scm_wrong_type_arg(FUNC_NAME, iarg++, name);
    } else {
      szName = gh_scm2newstr(name,NULL);
    }
  }

  if (!FUnsetSCM(value)) {
    if (!gh_number_p(value)) {
      scm_wrong_type_arg(FUNC_NAME, iarg++, value);
    } else {
      n = gh_scm2double(value);
    }
  }

  ClVariable *pclv = NULL;

  if (szName) {
    pclv = new ClVariable(szName, n);
    FREE(szName);
  } else {
    pclv = new ClVariable(n);
  }

  return ScmMakeClVariable(pclv);
}
#undef FUNC_NAME



SCWM_PROC(cl_value, "cl-value", 1, 0, 0,
           (SCM clv))
  /** Return the current value of the constraint variable CLV.
The value is a double.  Use `cl-int-value' to return an integer. */
#define FUNC_NAME s_cl_value
{
  int iarg = 1;
  if (!FIsClVariableScm(clv)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, clv);
  }

  ClVariable *pclv = PclvFromScm(clv);
  return gh_double2scm(pclv->value());
}
#undef FUNC_NAME

SCWM_PROC(cl_int_value, "cl-int-value", 1, 0, 0,
           (SCM clv))
  /** Return the value of the constraint variable CLV as an integer.
Internally, the value is a double.  This rounds that real number to
an integer before returning the value. */
#define FUNC_NAME s_cl_int_value
{
  int iarg = 1;
  if (!FIsClVariableScm(clv)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, clv);
  }

  ClVariable *pclv = PclvFromScm(clv);
  return gh_double2scm(pclv->intValue());
}
#undef FUNC_NAME



//// ClSymbolicWeight wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_weight

long SCMTYPEID;

SCM
mark_cl_weight(SCM scm)
{
  SCM_SETGC8MARK(scm);
  return SCM_BOOL_F;
}

size_t
free_cl_weight(SCM scm)
{
  ClSymbolicWeight *pclsw = PclswFromScm(scm);
  delete pclsw;
  return 0;
}

int
print_cl_weight(SCM scm, SCM port, scm_print_state *pstate)
{
  strstream ss;
  ClSymbolicWeight *pclsw = PclswFromScm(scm);
  ss << "#<cl-weight " << *pclsw << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}

SCWM_PROC(cl_weight_p, "cl-weight?", 1, 0, 0,
           (SCM obj))
  /** Return #t if OBJ is a constraint weight object, #f otherwise. */
#define FUNC_NAME s_cl_weight_p
{
  return SCM_BOOL_FromF(FIsClSymbolicWeightScm(obj));
}
#undef FUNC_NAME

SCWM_PROC(make_cl_weight, "make-cl-weight", 3, 0, 0,
           (SCM w1, SCM w2, SCM w3))
  /** Return a newly-created constraint symbolic weight object.
Symbolic weights are sequences of real numbers that are sorted
in lexicographical order (e.g., 1,0,0 > 0,999,999).  cl-weight
objects are used when creating cl-strength objects.  By convention,
the sequences are three elements long: W1, W2, and W3.  Longer
cl-weight objects could be supported in the future.  See also
`make-cl-strength-3' which is a convenience procedure for building a
cl-strength object given the sequence of numbers directly. */
#define FUNC_NAME s_make_cl_weight
{
  int iarg = 1;

  if (!gh_number_p(w1)) scm_wrong_type_arg(FUNC_NAME, iarg++, w1);
  if (!gh_number_p(w2)) scm_wrong_type_arg(FUNC_NAME, iarg++, w2);
  if (!gh_number_p(w3)) scm_wrong_type_arg(FUNC_NAME, iarg++, w3);

  ClSymbolicWeight *pclsw = new ClSymbolicWeight(gh_scm2double(w1),
                                                 gh_scm2double(w2),
                                                 gh_scm2double(w3));

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pclsw);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME


//// ClStrength wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_strength

long SCMTYPEID;

SCM
mark_cl_strength(SCM scm)
{
  SCM_SETGC8MARK(scm);
  return SCM_BOOL_F;
}

size_t
free_cl_strength(SCM scm)
{
  ClStrength *pcls = PclsFromScm(scm);
  delete pcls;
  return 0;
}

int
print_cl_strength(SCM scm, SCM port, scm_print_state *pstate)
{
  strstream ss;
  ClStrength *pcls = PclsFromScm(scm);
  ss << "#<cl-strength " << *pcls << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}

SCWM_PROC(cl_strength_p, "cl-strength?", 1, 0, 0,
           (SCM obj))
  /** Return #t if OBJ is a constraint strength, #f otherwise. */
#define FUNC_NAME s_cl_strength_p
{
  return SCM_BOOL_FromF(FIsClStrengthScm(obj));
}
#undef FUNC_NAME

static SCM
ScmMakeClStrength(const ClStrength *pcls)
{
  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pcls);
  SCM_ALLOW_INTS;

  return answer;
}

SCWM_PROC(make_cl_strength, "make-cl-strength", 2, 0, 0,
           (SCM name, SCM weight))
  /** Return a newly-created constraint strength object.
The object is given the name NAME, a string, and the symbolic
strengh WEIGHT, a cl-weight.  Constraint strength objects
are used when specifying the strength of a constraint.  There are
several predefined cl-strenght objects: "cls-required", "cls-strong",
"cls-medium", and "cls-weak".  These four constraint strengths
are often enough for a suitably expressive constraint-hierarchy,
but new strengths can be introduced if necessary. */
#define FUNC_NAME s_make_cl_strength
{
  int iarg = 1;

  if (!gh_string_p(name))
    scm_wrong_type_arg(FUNC_NAME,iarg++,name);

  if (!FIsClSymbolicWeightScm(weight))
    scm_wrong_type_arg(FUNC_NAME,iarg++,weight);
  
  char *szName = gh_scm2newstr(name,NULL);
  ClSymbolicWeight *pclsw = PclswFromScm(weight);
  ClStrength *pcls = new ClStrength(szName,*pclsw);
  delete szName;

  return ScmMakeClStrength(pcls);
}
#undef FUNC_NAME


SCWM_PROC(make_cl_strength_3, "make-cl-strength-3", 4, 0, 0,
           (SCM name, SCM w1, SCM w2, SCM w3))
  /** Return a newly-created constraint strength object.
The object is given the name NAME, a string, and a symbolic weight of
length three with the sequence of values: W1, W2, W3.  This is a
convenience procedure that has the same semantics as building a
cl-weight object with the values and using that to build a cl-strength 
object. */
#define FUNC_NAME s_make_cl_strength_3
{
  int iarg = 1;

  if (!gh_string_p(name)) scm_wrong_type_arg(FUNC_NAME,iarg++,name);
  if (!gh_number_p(w1)) scm_wrong_type_arg(FUNC_NAME, iarg++, w1);
  if (!gh_number_p(w2)) scm_wrong_type_arg(FUNC_NAME, iarg++, w2);
  if (!gh_number_p(w3)) scm_wrong_type_arg(FUNC_NAME, iarg++, w3);

  char *szName = gh_scm2newstr(name,NULL);
  ClStrength *pcls = new ClStrength(szName,
                                    gh_scm2double(w1),
                                    gh_scm2double(w2),
                                    gh_scm2double(w3));
  delete szName;

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pcls);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME



//// ClLinearExpression wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_expression


long SCMTYPEID;

SCM
mark_cl_expression(SCM scm)
{
  SCM_SETGC8MARK(scm);
  return SCM_BOOL_F;
}

size_t
free_cl_expression(SCM scm)
{
  ClLinearExpression *pexpr = PexprFromScm(scm);
  delete pexpr;
  return 0;
}

int
print_cl_expression(SCM scm, SCM port, scm_print_state *pstate)
{
  strstream ss;
  ClLinearExpression *pexpr = PexprFromScm(scm);
  ss << "#<cl_expression " << *pexpr << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}

SCWM_PROC(cl_expression_p, "cl-expression?", 1, 0, 0,
           (SCM obj))
  /** Return #t if OBJ is a constraint expression, #f otherwise. */
#define FUNC_NAME s_cl_expression_p
{
  return SCM_BOOL_FromF(FIsClLinearExpressionScm(obj));
}
#undef FUNC_NAME

SCWM_PROC(make_cl_expression, "make-cl-expression", 1, 0, 0,
           (SCM clv))
  /** Return a newly-create constraint expression object.
The expression is contains just the value CLV.  Often constraint
variables can be used interchangeably with constraint expressions, but 
this procedure can be used to force building a simple expression. */
#define FUNC_NAME s_make_cl_expression
{
  int iarg = 1;
  ClLinearExpression *pexpr = NULL;

  if (!FIsClVariableScm(clv) && !gh_number_p(clv)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, clv);
  } 
  if (FIsClVariableScm(clv)) {
    pexpr = new ClLinearExpression(*PclvFromScm(clv));
  } else {
    pexpr = new ClLinearExpression(gh_scm2double(clv));
  }


  return ScmMakeClLinearExpression(pexpr);
}
#undef FUNC_NAME

static ClLinearExpression *
PexprNewConvertSCM(SCM scm)
{
  if (gh_number_p(scm)) {
    return new ClLinearExpression(gh_scm2double(scm));
  }
  if (FIsClVariableScm(scm)) {
    return new ClLinearExpression(*PclvFromScm(scm));
  }
  if (FIsClLinearExpressionScm(scm)) {
    return new ClLinearExpression(*PexprFromScm(scm));
  }
  return NULL;
}


SCWM_PROC(cl_plus, "cl-plus", 2, 0, 0,
           (SCM exprA, SCM exprB))
  /** Return the cl-expression that is the sum of EXPRA and EXPRB. 
Both EXPRA and EXPRB may be cl-expression objects or cl-variable
objects. */
#define FUNC_NAME s_cl_plus
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;
  
  if (NULL == (pexprA = PexprNewConvertSCM(exprA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(exprB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprB);
  }
  
  pexprA->addExpression(*pexprB);
  delete pexprB;

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pexprA);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME


SCWM_PROC(cl_minus, "cl-minus", 2, 0, 0,
           (SCM exprA, SCM exprB))
  /** Return the cl-expression that is the difference of EXPRA and EXPRB.
Both EXPRA and EXPRB may be cl-expression objects or cl-variable
objects. */
#define FUNC_NAME s_cl_minus
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(exprA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(exprB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprB);
  }
  
  pexprA->addExpression(*pexprB,-1);
  delete pexprB;

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pexprA);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME


SCWM_PROC(cl_times, "cl-times", 2, 0, 0,
           (SCM exprA, SCM exprB))
  /** Return the cl-expression that is the product of EXPRA and EXPRB.
Both EXPRA and EXPRB may be cl-expression objects or cl-variable
objects.  Note that the product may not introduce non-linear terms--
if it does (e.g., if EXPRA and EXPRB both contain the same
cl-variable), an error will result. */
#define FUNC_NAME s_cl_times
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(exprA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(exprB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprB);
  }

  try {
    ClLinearExpression *pexpr = p_Times(*pexprA,*pexprB);
    delete pexprA;
    delete pexprB;
    SCM answer;
    
    SCM_DEFER_INTS;
    SCM_NEWCELL(answer);
    SCM_SETCAR(answer, (SCM) SCMTYPEID);
    SCM_SETCDR(answer, (SCM) pexpr);
    SCM_ALLOW_INTS;
    
    return answer;
  } catch (const ExCLNonlinearExpression &e) {
    delete pexprA;
    delete pexprB;
    scm_misc_error(FUNC_NAME, "NonlinearExpression exception", SCM_EOL);
  }

}
#undef FUNC_NAME


SCWM_PROC(cl_divide, "cl-divide", 2, 0, 0,
           (SCM exprA, SCM exprB))
  /** Return the cl-expression that is the quotient of EXPRA and EXPRB.
Both EXPRA and EXPRB may be cl-expression objects or cl-variable
objects. Note that the product may not introduce non-linear terms--
if it does (e.g., if EXPRA and EXPRB both contain the same
cl-variable), an error will result. */
#define FUNC_NAME s_cl_divide
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(exprA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(exprB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprB);
  }

  try {
    ClLinearExpression *pexpr = p_Divide(*pexprA,*pexprB);
    delete pexprA;
    delete pexprB;
    SCM answer;
    
    SCM_DEFER_INTS;
    SCM_NEWCELL(answer);
    SCM_SETCAR(answer, (SCM) SCMTYPEID);
    SCM_SETCDR(answer, (SCM) pexpr);
    SCM_ALLOW_INTS;
    
    return answer;
  } catch (const ExCLNonlinearExpression &e) {
    delete pexprA;
    delete pexprB;
    scm_misc_error(FUNC_NAME, e.description(), SCM_EOL);
  }
}
#undef FUNC_NAME



//// ClLinearEquation wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_equation

long SCMTYPEID;

SCM
mark_cl_equation(SCM scm)
{
  SCM_SETGC8MARK(scm);
  return SCM_BOOL_F;
}

size_t
free_cl_equation(SCM scm)
{
  ClLinearEquation *peq = PeqFromScm(scm);
  delete peq;
  return 0;
}

int
print_cl_equation(SCM scm, SCM port, scm_print_state *pstate)
{
  strstream ss;
  ClLinearEquation *peq = PeqFromScm(scm);
  ss << "#<cl-equation " << *peq << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}

SCWM_PROC(cl_equation_p, "cl-equation?", 1, 0, 0,
           (SCM obj))
  /** Return #t if OBJ is a constraint equation, #f otherwise.
Constraint equations also respond #t to `cl-constraint?' queries,
since they are also constraint objects. */
#define FUNC_NAME s_cl_equation_p
{
  return SCM_BOOL_FromF(FIsClLinearEquationScm(obj));
}
#undef FUNC_NAME

SCWM_PROC(make_cl_equation, "make-cl-equation", 1, 2, 0,
           (SCM expr, SCM strength, SCM factor))
  /** Return a newly-created constraint equation.
The resulting constraint is that EXPR is equal to 0.  EXPR is a
cl-expression or cl-variable object.  The constraint is given strength
STRENGTH, a cl-strength, and has a weight factor of FACTOR, a real
number.  STRENGTH defaults to cls-required, FACTOR defaults to 1. */
#define FUNC_NAME s_make_cl_equation
{
  int iarg = 1;
  ClLinearExpression *pexpr = NULL;

  if (NULL == (pexpr = PexprNewConvertSCM(expr))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, expr);
  }

  ClStrength *pcls = &clsRequired();
  if (FIsClStrengthScm(strength)) {
    pcls = PclsFromScm(strength);
  } else if (!FUnsetSCM(strength)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,strength);
  }

  double nWeight = 1.0;
  if (gh_number_p(factor)) {
    nWeight = gh_scm2double(factor);
  } else if (!FUnsetSCM(factor)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,factor);
  }

  ClLinearEquation *peq = new ClLinearEquation(*pexpr,*pcls,nWeight);

  SCM answer = ScmMakeClLinearEquation(peq);
  peq->setPv(reinterpret_cast<void *>(answer));
  return answer;
}
#undef FUNC_NAME

SCWM_PROC(make_cl_equality, "make-cl-equality", 2, 2, 0,
           (SCM exprA, SCM exprB, SCM strength, SCM factor))
  /** Return a newly-created constraint equation.
The resulting constraint is that EXPRA is equal to EXPRB.  EXPRA and
EXPRB are cl-expression or cl-variable objects.  The constraint is
given strength STRENGTH, a cl-strength, and has a weight factor of
FACTOR, a real number.  STRENGTH defaults to cls-required, FACTOR
defaults to 1.  Consider `make-cl-constraint' as a higher-level
interface to building arbitrary constraints.

This is a convenience procedure that has the same
semantics as `make-cl-equation' with the difference of EXPRA and
EXPRB.  Note that if neither expression contains a variable, an
error will be signalled. */
#define FUNC_NAME s_make_cl_equality
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(exprA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(exprB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprB);
  }
  if (!(FIsClLinearExpressionScm(exprA) || FIsClVariableScm(exprA) ||
        FIsClLinearExpressionScm(exprB) || FIsClVariableScm(exprB))) {
    scm_misc_error(FUNC_NAME,"One of arguments must contain a variable",SCM_EOL);
  }
  ClStrength *pcls = &clsRequired();
  if (FIsClStrengthScm(strength)) {
    pcls = PclsFromScm(strength);
  } else if (!FUnsetSCM(strength)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,strength);
  }

  double nWeight = 1.0;
  if (gh_number_p(factor)) {
    nWeight = gh_scm2double(factor);
  } else if (!FUnsetSCM(factor)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,factor);
  }

  ClLinearEquation *peq = new ClLinearEquation(*pexprA,*pexprB,*pcls,nWeight);

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) peq);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME




//// ClLinearInequality wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_inequality

long SCMTYPEID;

SCM
mark_cl_inequality(SCM scm)
{
  SCM_SETGC8MARK(scm);
  return SCM_BOOL_F;
}

size_t
free_cl_inequality(SCM scm)
{
  ClLinearInequality *pineq = PineqFromScm(scm);
  delete pineq;
  return 0;
}

int
print_cl_inequality(SCM scm, SCM port, scm_print_state *pstate)
{
  strstream ss;
  ClLinearInequality *pineq = PineqFromScm(scm);
  ss << "#<cl-inequality " << *pineq << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}

SCWM_PROC(cl_inequality_p, "cl-inequality?", 1, 0, 0,
           (SCM obj))
  /** Return #t if OBJ is a constraint inequality object, #t otherwise.
Constraint inequalities also respond #t to `cl-constraint?' queries,
since they are also constraint objects. */
#define FUNC_NAME s_cl_inequality_p
{
  return SCM_BOOL_FromF(FIsClLinearInequalityScm(obj));
}
#undef FUNC_NAME

SCWM_PROC(make_cl_inequality, "make-cl-inequality", 3, 2, 0,
           (SCM exprA, SCM op, SCM exprB, SCM strength, SCM factor))
  /** Return a newly-constructed constraint inequality object.

EXPRA and EXPRB are cl-expression or cl-variable objects.  If OP is
`<=' then the constraint is that EXPRA <= EXPRB; if OP is `>=' then
the constraint is that EXPRA >= EXPRB.  Note that the arithmetic
comparison procedure objects are just used for their identity. If OP
is any other object, an error is signalled. The constraint is given
strength STRENGTH, a cl-strength, and has a weight factor of FACTOR, a
real number.  STRENGTH defaults to cls-required, FACTOR defaults to 1.
Consider `make-cl-constraint' as a higher-level interface to building
arbitrary constraints. */
#define FUNC_NAME s_make_cl_inequality
{
  int iarg = 1;

  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(exprA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(exprB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprB);
  }
  if (!(FIsClLinearExpressionScm(exprA) || FIsClVariableScm(exprA) ||
        FIsClLinearExpressionScm(exprB) || FIsClVariableScm(exprB))) {
    scm_misc_error(FUNC_NAME,"One of arguments must contain a variable",SCM_EOL);
  }

  ClStrength *pcls = &clsRequired();
  if (FIsClStrengthScm(strength)) {
    pcls = PclsFromScm(strength);
  } else if (!FUnsetSCM(strength)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,strength);
  }

  double nWeight = 1.0;
  if (gh_number_p(factor)) {
    nWeight = gh_scm2double(factor);
  } else if (!FUnsetSCM(factor)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,factor);
  }

  ClLinearInequality *pineq;
  if (op == gh_lookup("<=")) {
    pineq = new ClLinearInequality(*pexprA,cnLEQ,*pexprB,*pcls,nWeight);
  } else if (op == gh_lookup(">=")) {
    pineq = new ClLinearInequality(*pexprA,cnGEQ,*pexprB,*pcls,nWeight);
  }

  SCM answer = ScmMakeClLinearInequality(pineq);
  pineq->setPv(reinterpret_cast<void *>(answer));
  return answer;
}
#undef FUNC_NAME


//// cl-constraint -- a wrapper for cl-equation and cl-inequality
/// NOT a new SMOB type, just for convenience

SCWM_PROC(cl_constraint_p, "cl-constraint?", 1, 0, 0,
           (SCM obj))
  /** Return #t if OBJ is some kind of constraint object, #f otherwise.
There are at least two kinds of objects for which this procedure will
return #t: "cl-inequality" and "cl-equation" objects.  Since it is
often useful to treat either as a general constraint, this procedure
exists to simplify that test. */
#define FUNC_NAME s_cl_constraint_p
{
  return SCM_BOOL_FromF(FIsClConstraintScm(obj));
}
#undef FUNC_NAME


SCWM_PROC(make_cl_constraint, "make-cl-constraint", 3, 2, 0,
           (SCM exprA, SCM op, SCM exprB, SCM strength, SCM factor))
  /** Return a newly-constructed constraint object.

EXPRA and EXPRB are cl-expression or cl-variable objects.  OP is one
of `=', `<=', or `>=', anything else will signal an error.  Note that
the arithmetic comparison procedures are just used for their identity.
The returned object is a cl-equation constraint object if OP is `=';
it is a cl-inequality constraint object if OP is `<=' or `>='.  The
created constraint is given strength STRENGTH, a cl-strength, and has
a weight factor of FACTOR, a real number.  STRENGTH defaults to
cls-required, FACTOR defaults to 1.  */
#define FUNC_NAME s_make_cl_constraint
{
  int iarg = 1;

  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(exprA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(exprB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, exprB);
  }
  if (!(FIsClLinearExpressionScm(exprA) || FIsClVariableScm(exprA) ||
        FIsClLinearExpressionScm(exprB) || FIsClVariableScm(exprB))) {
    scm_misc_error(FUNC_NAME,"One of arguments must contain a variable",SCM_EOL);
  }

  ClStrength *pcls = &clsRequired();
  if (FIsClStrengthScm(strength)) {
    pcls = PclsFromScm(strength);
  } else if (!FUnsetSCM(strength)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,strength);
  }

  double nWeight = 1.0;
  if (gh_number_p(factor)) {
    nWeight = gh_scm2double(factor);
  } else if (!FUnsetSCM(factor)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,factor);
  }

  ClConstraint *pcn = NULL;
  SCM id = (SCM) scm_tc16_cl_inequality;
  if (op == gh_lookup("<=")) {
    pcn = new ClLinearInequality(*pexprA,cnLEQ,*pexprB,*pcls,nWeight);
  } else if (op == gh_lookup(">=")) {
    pcn = new ClLinearInequality(*pexprA,cnGEQ,*pexprB,*pcls,nWeight);
  } else if (op == gh_lookup("=")) {
    pcn = new ClLinearEquation(*pexprA,*pexprB,*pcls,nWeight);
    id = (SCM) scm_tc16_cl_equation;
  } else {
    scm_wrong_type_arg(FUNC_NAME, 2, op);
  }
 
  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, id);
  SCM_SETCDR(answer, (SCM) pcn);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME



//// ClSimplexSolver wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_solver

long SCMTYPEID;

SCM
mark_cl_solver(SCM scm)
{
  SCM_SETGC8MARK(scm);
  return SCM_BOOL_F;
}

size_t
free_cl_solver(SCM scm)
{
  ClSimplexSolver *psolver = PsolverFromScm(scm);
  delete psolver;
  return 0;
}

int
print_cl_solver(SCM scm, SCM port, scm_print_state *pstate)
{
  strstream ss;
  ClSimplexSolver *psolver = PsolverFromScm(scm);
  ss << "#<cl-solver ";
  psolver->printInternalInfo(ss);
  ss << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}


SCWM_PROC(cl_solver_debug_print, "cl-solver-debug-print", 1, 1, 0,
           (SCM solver, SCM port))
  /** Output a complete dump of SOLVER onto output port PORT.
The output contains the contents of the tableau contained in SOLVER,
as well as the other internal state.  It can be useful for debugging,
as the printable form of constraint solver objects contains only a
brief summary of the contents of the solver. */
#define FUNC_NAME s_cl_solver_debug_print
{
  int iarg = 1;
  if (!FIsClSimplexSolverScm(solver))
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  if (FUnsetSCM(port))
    port = scm_current_output_port();
  if (scm_output_port_p(port) == SCM_BOOL_F)
    scm_wrong_type_arg(FUNC_NAME,iarg++,port);
  
  strstream ss;
  ClSimplexSolver *psolver = PsolverFromScm(solver);
  psolver->printDebugInfo(ss);
  ss << ends;
  scm_puts(ss.str(), port);
  return SCM_UNDEFINED;
}
#undef FUNC_NAME


SCWM_PROC(cl_solver_p, "cl-solver?", 1, 0, 0,
           (SCM obj))
  /** Return #t if OBJ is a constraint solver object, #f otherwise. */
#define FUNC_NAME s_cl_solver_p
{
  return SCM_BOOL_FromF(FIsClSimplexSolverScm(obj));
}
#undef FUNC_NAME

SCWM_PROC(make_cl_solver, "make-cl-solver", 0, 0, 0,
           ())
  /** Return a newly-created constraint solver object.
Often, an application will need only one of these objects,  but
multiple independent solvers may exist as long as they do not
share constraint variable objects. */
#define FUNC_NAME s_make_cl_solver
{
  ClSimplexSolver *psolver = new ClSimplexSolver();

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) psolver);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME

SCWM_PROC(cl_add_constraint, "cl-add-constraint", 1, 0, 1,
           (SCM solver, SCM args))
  /** Add the constraints, ARGS, to the solver SOLVER.
Each constraint given after the SOLVER argument is added to that
solver in turn.  Once one cannot be added (e.g., due to its addition
causing an insoluble system), this procedure will return #f without
trying to add the remaining constraints.  If all constraints are added 
successfully, this procedure will return #t.  (If none of the
constraints have strength "cls-required", then the system will remain
soluble.)  If an error is signalled because one object in the list ARGS
is not a constraint object, the preceding arguments will have already
been added. */
#define FUNC_NAME s_cl_add_constraint
{
  int iarg = 1;

  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
    
  ClSimplexSolver *psolver = PsolverFromScm(solver);

  try {
    for (int i = 0; SCM_NNULLP (args); args = SCM_CDR (args), ++i) {
      SCM constraint = SCM_CAR(args);
      if (!FIsClConstraintScm(constraint)) {
        scm_wrong_type_arg(FUNC_NAME,iarg,args);
      }
      ClConstraint *pconstraint = PcnFromScm(constraint);
      psolver->addConstraint(*pconstraint);
      scm_protect_object(constraint);
      pconstraint->setPv(reinterpret_cast<void *>(constraint));
    }
  } catch (const ExCLRequiredFailure &e) {
    // scm_misc_error(FUNC_NAME,e.description, SCM_EOL);
    return SCM_BOOL_F;
  }

  return SCM_BOOL_T;
}
#undef FUNC_NAME

SCWM_PROC(cl_remove_constraint, "cl-remove-constraint", 1, 0, 1,
           (SCM solver, SCM args))
  /** Remove the constraints, ARGS, from the solver SOLVER.
Each constraint given after the SOLVER argument is removed from that
solver in turn.  Once one cannot be removed (e.g., due to its never
having been added to SOLVER), this procedure will return #f without
trying to remove the remaining constraints.  If all constraints are
removed successfully, this procedure will return #t.  If an error is
signalled because one object in the list ARGS is not a constraint
object, the preceding arguments will have already been removed. */
#define FUNC_NAME s_cl_remove_constraint
{
  int iarg = 1;

  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }

  ClSimplexSolver *psolver = PsolverFromScm(solver);

  try {
    for (int i = 0; SCM_NNULLP (args); args = SCM_CDR (args), ++i) {
      SCM constraint = SCM_CAR(args);
      if (!FIsClConstraintScm(constraint)) {
        scm_wrong_type_arg(FUNC_NAME,iarg,args);
      }
      ClConstraint *pconstraint = PcnFromScm(constraint);
      psolver->removeConstraint(*pconstraint);
      pconstraint->setPv(0);
      scm_unprotect_object(constraint);  
    }
  } catch (const ExCLConstraintNotFound &e) {
    // scm_misc_error(FUNC_NAME,e.description(),SCM_EOL);
    return SCM_BOOL_F;
  }

  return SCM_BOOL_T;
}
#undef FUNC_NAME


// FIXGJB: add strength argument
SCWM_PROC(cl_add_editvar, "cl-add-editvar", 1, 0, 1,
           (SCM solver, SCM args))
  /** Add edit constraints on variables in ARGS to SOLVER.
ARGS are cl-variable objects that you wish to permit to change under
the solver's control.  An edit-constraint for each cl-variable object
is added in turn.  If any of ARGS is not a cl-variable, an error is
thrown (after the preceding objects have been handled).  After
selecting the edit variables with this procedure, you must call
`cl-begin-edit' before using `cl-suggest-value'.  To remove the edit
variables, use `cl-end-edit' when done changing the variables'
values. */
#define FUNC_NAME s_cl_add_editvar
{
  int iarg = 1;

  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  ClSimplexSolver *psolver = PsolverFromScm(solver);

  try {
    for (int i = 0; SCM_NNULLP (args); args = SCM_CDR (args), ++i) {
      SCM var = SCM_CAR(args);
      if (!FIsClVariableScm(var)) {
        scm_wrong_type_arg(FUNC_NAME,iarg,args);
      }
      ClVariable *pclv = PclvFromScm(var);
      psolver->addEditVar(*pclv);
    }
  } catch (const ExCLEditMisuse &e) {
    scm_misc_error(FUNC_NAME, e.description(), SCM_EOL);
  }

  return SCM_UNDEFINED;
}
#undef FUNC_NAME


SCWM_PROC(cl_add_stay, "cl-add-stay", 1, 0, 1,
           (SCM solver, SCM args))
  /** Add stay constraints on variables in ARGS to SOLVER.

ARGS are cl-variable objects that you wish to remain (i.e., stay) at
their current values unless another constraint forces them to change.
In normal uses of the solver, all variables should have stay
constraints added on them before they are used in a constraint added
to the solver.  Future versions of the solver may add the stay
constraint implicitly upon a variable's first use.  Until then,
though, be sure to add stay constraints on all the cl-variable objects
you intend to use with the given SOLVER. */
#define FUNC_NAME s_cl_add_stay
{
  int iarg = 1;

  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  ClSimplexSolver *psolver = PsolverFromScm(solver);

  try {
    for (int i = 0; SCM_NNULLP (args); args = SCM_CDR (args), ++i) {
      SCM var = SCM_CAR(args);
      if (!FIsClVariableScm(var)) {
        scm_wrong_type_arg(FUNC_NAME,iarg,args);
      }
      ClVariable *pclv = PclvFromScm(var);
      psolver->addStay(*pclv);
    }
  } catch (const ExCLError &e) {
    scm_misc_error(FUNC_NAME, e.description(), SCM_EOL);
  }

  return SCM_UNDEFINED;
}
#undef FUNC_NAME



SCWM_PROC(cl_begin_edit, "cl-begin-edit", 1, 0, 0,
           (SCM solver))
  /** Begin changing values of the edit variables of SOLVER.
This procedure must be invoked after adding edit variables to SOLVER
(via `cl-add-editvar') and before using `cl-suggest-values' to change
those edit variables' values.  Every `cl-begin-edit' invocation should 
have a matching `cl-end-edit' call. */
#define FUNC_NAME s_cl_begin_edit
{
  int iarg = 1;
  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  ClSimplexSolver *psolver = PsolverFromScm(solver);

  psolver->beginEdit();

  return SCM_UNDEFINED;
}
#undef FUNC_NAME

SCWM_PROC(cl_end_edit, "cl-end-edit", 1, 0, 0,
           (SCM solver))
  /** Finish changing the values of the edit variables in SOLVER.
This procedure must be invoked once for every invocation of 
`cl-begin-edit'.   It will force a final re-solve before removing
any edit variables that have been added via `cl-add-editvar'. */
#define FUNC_NAME s_cl_end_edit
{
  int iarg = 1;
  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  ClSimplexSolver *psolver = PsolverFromScm(solver);

  psolver->endEdit();

  return SCM_UNDEFINED;
}
#undef FUNC_NAME


SCWM_PROC (cl_is_constraint_satisfied_p, "cl-is-constraint-satisfied?", 2, 0, 0,
           (SCM solver, SCM cn))
  /** Return #t if CN is satisfied in SOLVER, #f otherwise. */
#define FUNC_NAME s_cl_is_constraint_satisfied_p
{
  int iarg = 1;
  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  if (!FIsClConstraintScm(cn)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,cn);
  }
  ClSimplexSolver *psolver = PsolverFromScm(solver);
  ClConstraint *pcn = PcnFromScm(cn);
  return gh_bool2scm(psolver->FIsConstraintSatisfied(*pcn));
}
#undef FUNC_NAME



SCWM_PROC(cl_suggest_value, "cl-suggest-value", 3, 0, 0,
           (SCM solver, SCM var, SCM value))
  /** Try to change VAR's value to VALUE within SOLVER.

You may only call this procedure after using `cl-add-editvar' to add
VAR as an edit variable for SOLVER, and after invoking `cl-begin-edit'
on SOLVER.  After you call `cl-begin-edit', you may invoke this
procedure on all the edit variables.  The changing of the values of
those variables (and the affect on other variables within SOLVER) will
not happen until after invoking `cl-resolve' on SOLVER.  Then the
possibly changed values can be retrieved via `cl-value' or
`cl-int-value'.  Note that the constraints that have been added to
SOLVER may not permit changing VAR to VALUE. */
#define FUNC_NAME s_cl_suggest_value
{
  int iarg = 1;
  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  if (!FIsClVariableScm(var)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,var);
  }
  if (!gh_number_p(value)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,value);
  }
  ClSimplexSolver *psolver = PsolverFromScm(solver);
  ClVariable *pclv = PclvFromScm(var);
  double n = gh_scm2double(value);

  try {
    psolver->suggestValue(*pclv,n);
  } catch (const ExCLError &e) {
    scm_misc_error(FUNC_NAME, e.description(), SCM_EOL);
  }
  return SCM_UNDEFINED;
}
#undef FUNC_NAME


SCWM_PROC(cl_resolve, "cl-resolve", 1, 0, 1,
           (SCM solver, SCM args))
     /** Ask SOLVER to re-solve its system using the suggested values.

You may only call this procedure between invocations of
`cl-begin-edit' and `cl-end-edit'.  After `cl-begin-edit', you may use
`cl-suggest-value' on SOLVER to tell it the desired new values for the
various edit variables you have added using `cl-add-editvar', and then
call this procedure with only the first argument to have SOLVER update
the values of all the variables in the system.

Alternatively, after adding edit-variables in a particular order, you
may call `cl-begin-edit' then this procedure with ARGS set as
suggested values for the edit-variables.  The order of the values in
the `cl-resolve' invocation must match the order of the variables in
the `cl-add-editvar' invocation(s).

In either case, you must invoke `cl-end-edit' to remove the edit
variables from the solver after you are done changing their values. */
#define FUNC_NAME s_cl_resolve
{
  int iarg = 1;
  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  ClSimplexSolver *psolver = PsolverFromScm(solver);

  if (FUnsetSCM(args)) {
    psolver->resolve();
    return SCM_UNDEFINED;
  }

  vector<double> rgval;

  for (int i = 0; SCM_NNULLP (args); args = SCM_CDR (args), ++i) {
    SCM val = SCM_CAR(args);
    if (!gh_number_p(val)) {
      scm_wrong_type_arg(FUNC_NAME,iarg,args);
    }
    rgval.push_back(gh_scm2double(val));
  }

  try {
    if (rgval.size() == 0)
      psolver->resolve(); // No arg version is different fn
    else
      psolver->resolve(rgval);
  } catch (const ExCLBadResolve &e) {
    scm_misc_error(FUNC_NAME, e.description(), SCM_EOL);
  }
  
  return SCM_UNDEFINED;
}
#undef FUNC_NAME


SCWM_PROC (cl_constraint_list, "cl-constraint-list", 1, 1, 0,
           (SCM solver, SCM internal_also_p))
     /** Return the list of constraints in SOLVER.
Each object in the returned list is a cl-constraint object (i.e.,
it is either a cl-linear-equation or a cl-inequality). If
INTERNAL-ALSO? is #f or not given, then only the constraints added via 
the guile interface will be listed.  If INTERNAL-ALSO? is #t, then all 
constraints added to the solver (even those added by primitive code
directly) will be listed. */
#define FUNC_NAME s_cl_constraint_list
{
  SCM list = SCM_EOL;
  int iarg = 1;
  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  bool fInternalAlso;
  COPY_BOOL_OR_ERROR_DEFAULT_FALSE(fInternalAlso,internal_also_p,iarg++,FUNC_NAME);
    
  ClSimplexSolver *psolver = PsolverFromScm(solver);

  const ClConstraintToVarMap mapCns = psolver->ConstraintMap();

  map<const ClConstraint *, const ClAbstractVariable *>::const_iterator 
    it_marker = mapCns.begin();

  for ( ; it_marker != mapCns.end() ; ++it_marker) {
    const ClConstraint *pcn = (*it_marker).first;
    SCM cn = SCM_UNDEFINED;
    void *pv = pcn->Pv();
    if (pv) cn = reinterpret_cast<SCM>(pv);
    else if (fInternalAlso) {
      const ClLinearEquation *peq = 
        dynamic_cast<const ClLinearEquation *>(pcn);
      if (peq) {
        cn = ScmMakeClLinearEquation(peq);
      } else {
        const ClLinearInequality *pineq = 
          dynamic_cast<const ClLinearInequality *>(pcn);
        if (pineq)
          cn = ScmMakeClLinearInequality(pineq);
      }
    }
    if (cn != SCM_UNDEFINED)
      list = gh_cons(cn,list);
  }

  return list;
}
#undef FUNC_NAME




//// cassowary_scm initialization

MAKE_SMOBFUNS(cl_variable);
MAKE_SMOBFUNS(cl_weight);
MAKE_SMOBFUNS(cl_strength);
MAKE_SMOBFUNS(cl_expression);
MAKE_SMOBFUNS(cl_equation);
MAKE_SMOBFUNS(cl_inequality);
MAKE_SMOBFUNS(cl_solver);

static SCM scm_cls_weak;
static SCM scm_cls_medium;
static SCM scm_cls_strong;
static SCM scm_cls_required;

void
init_cassowary_scm()
{
  REGISTER_SMOBFUNS(cl_variable);
  REGISTER_SMOBFUNS(cl_weight);
  REGISTER_SMOBFUNS(cl_strength);
  REGISTER_SMOBFUNS(cl_expression);
  REGISTER_SMOBFUNS(cl_equation);
  REGISTER_SMOBFUNS(cl_inequality);
  REGISTER_SMOBFUNS(cl_solver);

  SCM_DEFER_INTS;
  scm_permanent_object(
    scm_cls_weak = scm_sysintern("cls-weak",ScmMakeClStrength(&clsWeak()))
    );
  scm_permanent_object(
    scm_cls_medium = scm_sysintern("cls-medium",ScmMakeClStrength(&clsMedium()))
    );
  scm_permanent_object(
    scm_cls_strong = scm_sysintern("cls-strong",ScmMakeClStrength(&clsStrong()))
    );
  scm_permanent_object(
    scm_cls_required = scm_sysintern("cls-required",ScmMakeClStrength(&clsRequired()))
    );
  SCM_ALLOW_INTS;
#ifndef SCM_MAGIC_SNARFER
#include "cassowary_scm.x"
#endif
}

} // extern "C"



/* Local Variables: */
/* tab-width: 8 */
/* c-basic-offset: 2 */
/* End: */
