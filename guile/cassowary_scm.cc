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

inline bool FIsClVariableScm(SCM scm) 
{ return SCM_NIMP(scm) && SCM_CAR(scm) == (SCM) SCMTYPEID; }

inline ClVariable *PclvFromScm(SCM scm)
{ return (ClVariable *)(SCM_CDR(scm)); }

SCM mark_cl_variable(SCM scm)
{
  SCM_SETGC8MARK(scm);
  return SCM_BOOL_F;
}

size_t free_cl_variable(SCM scm)
{
  ClVariable *pclv = PclvFromScm(scm);
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

SCWM_PROC (cl_variable_p, "cl-variable?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_variable_p
{
  return SCM_BOOL_FromF(FIsClVariableScm(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_variable, "make-cl-variable", 0, 2, 0,
           (SCM scmName, SCM scmValue))
#define FUNC_NAME s_make_cl_variable
{
  char *szName = NULL;
  double value = 0;
  int iarg = 1;

  if (!FUnsetSCM(scmName)) {
    if (!gh_string_p(scmName)) {
      scm_wrong_type_arg(FUNC_NAME, iarg++, scmName);
    } else {
      szName = gh_scm2newstr(scmName,NULL);
    }
  }

  if (!FUnsetSCM(scmValue)) {
    if (!gh_number_p(scmValue)) {
      scm_wrong_type_arg(FUNC_NAME, iarg++, scmValue);
    } else {
      value = gh_scm2double(scmValue);
    }
  }

  ClVariable *pclv = NULL;

  if (szName) {
    pclv = new ClVariable(szName, value);
    FREE(szName);
  } else {
    pclv = new ClVariable(value);
  }

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pclv);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME



SCWM_PROC (cl_value, "cl-value", 1, 0, 0,
           (SCM scmVar))
#define FUNC_NAME s_cl_value
{
  int iarg = 1;
  if (!FIsClVariableScm(scmVar)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmVar);
  }

  ClVariable *pclv = PclvFromScm(scmVar);
  return gh_double2scm(pclv->value());
}
#undef FUNC_NAME

SCWM_PROC (cl_int_value, "cl-int-value", 1, 0, 0,
           (SCM scmVar))
#define FUNC_NAME s_cl_int_value
{
  int iarg = 1;
  if (!FIsClVariableScm(scmVar)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmVar);
  }

  ClVariable *pclv = PclvFromScm(scmVar);
  return gh_double2scm(pclv->intValue());
}
#undef FUNC_NAME



//// ClSymbolicWeight wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_weight

long SCMTYPEID;

inline bool FIsClSymbolicWeightScm(SCM scm) 
{ return SCM_NIMP(scm) && SCM_CAR(scm) == (SCM) SCMTYPEID; }

inline ClSymbolicWeight *PclswFromScm(SCM scm)
{ return (ClSymbolicWeight *)(SCM_CDR(scm)); }

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

SCWM_PROC (cl_weight_p, "cl-weight?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_weight_p
{
  return SCM_BOOL_FromF(FIsClSymbolicWeightScm(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_weight, "make-cl-weight", 3, 0, 0,
           (SCM w1, SCM w2, SCM w3))
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

inline bool FIsClStrengthScm(SCM scm) 
{ return SCM_NIMP(scm) && SCM_CAR(scm) == (SCM) SCMTYPEID; }

inline ClStrength *PclsFromScm(SCM scm)
{ return (ClStrength *)(SCM_CDR(scm)); }

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

SCWM_PROC (cl_strength_p, "cl-strength?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_strength_p
{
  return SCM_BOOL_FromF(FIsClStrengthScm(scm));
}
#undef FUNC_NAME

static SCM
ScmMakeClStrength(ClStrength *pcls)
{
  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pcls);
  SCM_ALLOW_INTS;

  return answer;
}

SCWM_PROC (make_cl_strength, "make-cl-strength", 2, 0, 0,
           (SCM name, SCM weight))
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


SCWM_PROC (make_cl_strength_3, "make-cl-strength-3", 4, 0, 0,
           (SCM name, SCM w1, SCM w2, SCM w3))
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

inline bool FIsClLinearExpressionScm(SCM scm) 
{ return SCM_NIMP(scm) && SCM_CAR(scm) == (SCM) SCMTYPEID; }

inline ClLinearExpression *PexprFromScm(SCM scm)
{ return (ClLinearExpression *)(SCM_CDR(scm)); }

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

SCWM_PROC (cl_expression_p, "cl-expression?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_expression_p
{
  return SCM_BOOL_FromF(FIsClLinearExpressionScm(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_expression, "make-cl-expression", 1, 0, 0,
           (SCM scmClv))
#define FUNC_NAME s_make_cl_expression
{
  int iarg = 1;
  ClLinearExpression *pexpr = NULL;

  if (!FIsClVariableScm(scmClv) && !gh_number_p(scmClv)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmClv);
  } 
  if (FIsClVariableScm(scmClv)) {
    pexpr = new ClLinearExpression(*PclvFromScm(scmClv));
  } else {
    pexpr = new ClLinearExpression(gh_scm2double(scmClv));
  }

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pexpr);
  SCM_ALLOW_INTS;

  return answer;
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


SCWM_PROC (cl_plus, "cl-plus", 2, 0, 0,
           (SCM scmA, SCM scmB))
#define FUNC_NAME s_cl_plus
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;
  
  if (NULL == (pexprA = PexprNewConvertSCM(scmA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(scmB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmB);
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


SCWM_PROC (cl_minus, "cl-minus", 2, 0, 0,
           (SCM scmA, SCM scmB))
#define FUNC_NAME s_cl_minus
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(scmA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(scmB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmB);
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


SCWM_PROC (cl_times, "cl-times", 2, 0, 0,
           (SCM scmA, SCM scmB))
#define FUNC_NAME s_cl_times
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(scmA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(scmB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmB);
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


SCWM_PROC (cl_divide, "cl-divide", 2, 0, 0,
           (SCM scmA, SCM scmB))
#define FUNC_NAME s_cl_divide
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(scmA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(scmB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmB);
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
    scm_misc_error(FUNC_NAME, "NonlinearExpression exception", SCM_EOL);
  }
}
#undef FUNC_NAME



//// ClLinearEquation wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_equation

long SCMTYPEID;

inline bool FIsClLinearEquationScm(SCM scm) 
{ return SCM_NIMP(scm) && SCM_CAR(scm) == (SCM) SCMTYPEID; }

inline ClLinearEquation *PeqFromScm(SCM scm)
{ return (ClLinearEquation *)(SCM_CDR(scm)); }

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

SCWM_PROC (cl_equation_p, "cl-equation?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_equation_p
{
  return SCM_BOOL_FromF(FIsClLinearEquationScm(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_equation, "make-cl-equation", 1, 2, 0,
           (SCM expression, SCM strength, SCM weight))
#define FUNC_NAME s_make_cl_equation
{
  int iarg = 1;
  ClLinearExpression *pexpr = NULL;

  if (!FIsClLinearExpressionScm(expression) && !FIsClVariableScm(expression)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, expression);
  }
  if (FIsClLinearExpressionScm(expression)) {
    pexpr = PexprFromScm(expression);
  } else if (FIsClVariableScm(expression)) {
    pexpr = new ClLinearExpression(*PclvFromScm(expression));
  }

  ClStrength *pcls = &clsRequired();
  if (FIsClStrengthScm(strength)) {
    pcls = PclsFromScm(strength);
  } else if (!FUnsetSCM(strength)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,strength);
  }

  double nWeight = 1.0;
  if (gh_number_p(weight)) {
    nWeight = gh_scm2double(weight);
  } else if (!FUnsetSCM(weight)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,weight);
  }

  ClLinearEquation *peq = new ClLinearEquation(*pexpr,*pcls,nWeight);

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) peq);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME

SCWM_PROC (make_cl_equality, "make-cl-equality", 2, 2, 0,
           (SCM expressionA, SCM expressionB, SCM strength, SCM weight))
#define FUNC_NAME s_make_cl_equality
{
  int iarg = 1;
  ClLinearExpression *pexprA = NULL;
  ClLinearExpression *pexprB = NULL;

  if (NULL == (pexprA = PexprNewConvertSCM(expressionA))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, expressionA);
  }
  if (NULL == (pexprB = PexprNewConvertSCM(expressionB))) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, expressionB);
  }
  if (!(FIsClLinearExpressionScm(expressionA) || FIsClVariableScm(expressionA) ||
        FIsClLinearExpressionScm(expressionB) || FIsClVariableScm(expressionB))) {
    scm_misc_error(FUNC_NAME,"One of arguments must contain a variable",SCM_EOL);
  }
  ClStrength *pcls = &clsRequired();
  if (FIsClStrengthScm(strength)) {
    pcls = PclsFromScm(strength);
  } else if (!FUnsetSCM(strength)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,strength);
  }

  double nWeight = 1.0;
  if (gh_number_p(weight)) {
    nWeight = gh_scm2double(weight);
  } else if (!FUnsetSCM(weight)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,weight);
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

inline bool FIsClLinearInequalityScm(SCM scm) 
{ return SCM_NIMP(scm) && SCM_CAR(scm) == (SCM) SCMTYPEID; }

inline ClLinearInequality *PineqFromScm(SCM scm)
{ return (ClLinearInequality *)(SCM_CDR(scm)); }

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

SCWM_PROC (cl_inequality_p, "cl-inequality?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_inequality_p
{
  return SCM_BOOL_FromF(FIsClLinearInequalityScm(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_inequality, "make-cl-inequality", 3, 2, 0,
           (SCM exprA, SCM op, SCM exprB, SCM strength, SCM weight))
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
  if (gh_number_p(weight)) {
    nWeight = gh_scm2double(weight);
  } else if (!FUnsetSCM(weight)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,weight);
  }

  ClLinearInequality *pineq;
  if (op == gh_lookup("<=")) {
    pineq = new ClLinearInequality(*pexprA,cnLEQ,*pexprB,*pcls,nWeight);
  } else if (op == gh_lookup(">=")) {
    pineq = new ClLinearInequality(*pexprA,cnGEQ,*pexprB,*pcls,nWeight);
  }

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pineq);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME


//// cl-constraint -- a wrapper for cl-equation and cl-inequality
/// NOT a new SMOB type, just for convenience

inline bool FIsClConstraintScm(SCM scm) {
  if (!SCM_NIMP(scm)) return false; 
  SCM car = SCM_CAR(scm);

  if (car == (SCM) scm_tc16_cl_equation) return true;
  if (car == (SCM) scm_tc16_cl_inequality) return true;

  return false;
}

inline ClConstraint *PcnFromScm(SCM scm)
{ return (ClConstraint *)(SCM_CDR(scm)); }

SCWM_PROC (cl_constraint_p, "cl-constraint?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_constraint_p
{
  return SCM_BOOL_FromF(FIsClConstraintScm(scm));
}
#undef FUNC_NAME


SCWM_PROC (make_cl_constraint, "make-cl-constraint", 3, 2, 0,
           (SCM exprA, SCM op, SCM exprB, SCM strength, SCM weight))
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
  if (gh_number_p(weight)) {
    nWeight = gh_scm2double(weight);
  } else if (!FUnsetSCM(weight)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,weight);
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

inline bool FIsClSimplexSolverScm(SCM scm) 
{ return SCM_NIMP(scm) && SCM_CAR(scm) == (SCM) SCMTYPEID; }

inline ClSimplexSolver *PsolverFromScm(SCM scm)
{ return (ClSimplexSolver *)(SCM_CDR(scm)); }

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


SCWM_PROC (cl_solver_debug_print, "cl-solver-debug-print", 1, 1, 0,
           (SCM solver, SCM port))
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


SCWM_PROC (cl_solver_p, "cl-solver?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_solver_p
{
  return SCM_BOOL_FromF(FIsClSimplexSolverScm(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_solver, "make-cl-solver", 0, 0, 0,
           ())
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

SCWM_PROC (cl_add_constraint, "cl-add-constraint", 1, 0, 1,
           (SCM solver, SCM args))
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
    }
  } catch (const ExCLRequiredFailure &e) {
    return SCM_BOOL_F;
  }

  return SCM_BOOL_T;
}
#undef FUNC_NAME

// FIXGJB: add strength argument
SCWM_PROC (cl_remove_constraint, "cl-remove-constraint", 1, 0, 1,
           (SCM solver, SCM args))
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
    }
  } catch (const ExCLConstraintNotFound &e) {
    return SCM_BOOL_F;
  }

  return SCM_BOOL_T;
}
#undef FUNC_NAME


// FIXGJB: add strength argument
SCWM_PROC (cl_add_editvar, "cl-add-editvar", 1, 0, 1,
           (SCM solver, SCM args))
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
  } catch (const ExCLError &e) {
    scm_misc_error(FUNC_NAME, "Solver error", SCM_EOL);
  }

  return SCM_UNDEFINED;
}
#undef FUNC_NAME


SCWM_PROC (cl_add_stay, "cl-add-stay", 1, 0, 1,
           (SCM solver, SCM args))
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
    scm_misc_error(FUNC_NAME, "Solver error", SCM_EOL);
  }

  return SCM_UNDEFINED;
}
#undef FUNC_NAME



SCWM_PROC (cl_begin_edit, "cl-begin-edit", 1, 0, 0,
           (SCM solver))
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

SCWM_PROC (cl_end_edit, "cl-end-edit", 1, 0, 0,
           (SCM solver))
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


SCWM_PROC (cl_suggest_value, "cl-suggest-value", 3, 0, 0,
           (SCM solver, SCM variable, SCM value))
     /**
      */
#define FUNC_NAME s_cl_suggest_value
{
  int iarg = 1;
  if (!FIsClSimplexSolverScm(solver)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,solver);
  }
  if (!FIsClVariableScm(variable)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,variable);
  }
  if (!gh_number_p(value)) {
    scm_wrong_type_arg(FUNC_NAME,iarg++,value);
  }
  ClSimplexSolver *psolver = PsolverFromScm(solver);
  ClVariable *pclv = PclvFromScm(variable);
  double n = gh_scm2double(value);

  psolver->suggestValue(*pclv,n);
  return SCM_UNDEFINED;
}
#undef FUNC_NAME


SCWM_PROC (cl_resolve, "cl-resolve", 1, 0, 1,
           (SCM solver, SCM args))
     /**
      */
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
    scm_misc_error(FUNC_NAME, "Resolve protocol misused", SCM_EOL);
  }
  
  return SCM_UNDEFINED;
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

SCM scm_cls_weak;
SCM scm_cls_strong;
SCM scm_cls_required;

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
  scm_cls_weak = scm_sysintern("cls-weak",ScmMakeClStrength(&clsWeak()));
  scm_protect_object(scm_cls_weak);
  scm_cls_strong = scm_sysintern("cls-strong",ScmMakeClStrength(&clsStrong()));
  scm_protect_object(scm_cls_strong);
  scm_cls_required = scm_sysintern("cls-required",ScmMakeClStrength(&clsRequired()));
  scm_protect_object(scm_cls_required);
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
