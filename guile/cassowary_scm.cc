// $Id$
// By Greg J. Badros -- 18 July 1998
//
// A Guile Scheme wrapper for the Cassowary Constraint Solving Toolkit
//

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <guile/gh.h>

#include <iostream.h>
#include <assert.h>
#include <strstream.h>
#include "scwm-snarf.h"

#define CASSOWARY_SCM_IMPLEMENTATION
#include "cassowary_scm.h"

extern "C" {

#define NEW(x) ((x *) safemalloc(sizeof(x)))
#define NEWC(c,x) ((x *) safemalloc((c)*sizeof(x)))
#define FREE(x) free(x)

inline SCM SCM_BOOL_FromBool(bool f) { return (f? SCM_BOOL_T: SCM_BOOL_F); }
inline bool FUnsetSCM(SCM scm) { return (scm == SCM_UNDEFINED || scm == SCM_BOOL_F); }

//// ClVariable wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_variable

long SCMTYPEID;

inline bool FIsClVariableSCM(SCM scm) 
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
  delete pclv;
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
  return SCM_BOOL_FromBool(FIsClVariableSCM(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_variable, "make-cl-variable", 0, 2, 0,
           (SCM scmName, SCM scmValue))
#define FUNC_NAME s_make_cl_variable
{
  char *szName = NULL;
  double value = 0;
  int iarg = 1;

  if (!FUnsetSCM(scmName) && !gh_string_p(scmName)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmName);
  } else {
    szName = gh_scm2newstr(scmName,NULL);
  }

  if (!FUnsetSCM(scmValue) && !gh_number_p(scmValue)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmValue);
  } else {
    value = gh_scm2double(scmValue);
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


//// ClLinearExpression wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_expression


long SCMTYPEID;

inline bool FIsClLinearExpressionSCM(SCM scm) 
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
  return SCM_BOOL_FromBool(FIsClLinearExpressionSCM(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_expression, "make-cl-expression", 1, 0, 0,
           (SCM scmClv))
#define FUNC_NAME s_make_cl_expression
{
  int iarg = 1;
  ClVariable *pclv = NULL;

  if (!FUnsetSCM(scmClv) && !FIsClVariableSCM(scmClv)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmClv);
  } else {
    pclv = PclvFromScm(scmClv);
  }

  ClLinearExpression *pexpr = new ClLinearExpression(*pclv);

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pexpr);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME



//// ClLinearEquation wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_equation

long SCMTYPEID;

inline bool FIsClLinearEquationSCM(SCM scm) 
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
  return SCM_BOOL_FromBool(FIsClLinearEquationSCM(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_equation, "make-cl-equation", 1, 0, 0,
           (SCM scmExpr))
#define FUNC_NAME s_make_cl_equation
{
  int iarg = 1;
  ClLinearExpression *pexpr = NULL;

  if (!FUnsetSCM(scmExpr) && !FIsClLinearExpressionSCM(scmExpr)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmExpr);
  } else {
    pexpr = PexprFromScm(scmExpr);
  }

  ClLinearEquation *peq = new ClLinearEquation(*pexpr);

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

inline bool FIsClLinearInequalitySCM(SCM scm) 
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
  ss << "#<cl-inequality" << *pineq << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}

SCWM_PROC (cl_inequality_p, "cl-inequality?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_inequality_p
{
  return SCM_BOOL_FromBool(FIsClLinearInequalitySCM(scm));
}
#undef FUNC_NAME

SCWM_PROC (make_cl_inequality, "make-cl-inequality", 1, 0, 0,
           (SCM scmExpr))
#define FUNC_NAME s_make_cl_inequality
{
  int iarg = 1;

  ClLinearExpression *pexpr = NULL;

  if (!FUnsetSCM(scmExpr) && !FIsClLinearExpressionSCM(scmExpr)) {
    scm_wrong_type_arg(FUNC_NAME, iarg++, scmExpr);
  } else {
    pexpr = PexprFromScm(scmExpr);
  }

  ClLinearInequality *pineq = new ClLinearInequality(*pexpr);

  SCM answer;

  SCM_DEFER_INTS;
  SCM_NEWCELL(answer);
  SCM_SETCAR(answer, (SCM) SCMTYPEID);
  SCM_SETCDR(answer, (SCM) pineq);
  SCM_ALLOW_INTS;

  return answer;
}
#undef FUNC_NAME



//// ClSimplexSolver wrapper
#undef SCMTYPEID
#define SCMTYPEID scm_tc16_cl_solver

long SCMTYPEID;

inline bool FIsClSimplexSolverSCM(SCM scm) 
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
  ss << "#<cl-solver" << *psolver << ">" << ends;
  scm_puts(ss.str(), port);
  return 1;
}

SCWM_PROC (cl_solver_p, "cl-solver?", 1, 0, 0,
           (SCM scm))
#define FUNC_NAME s_cl_solver_p
{
  return SCM_BOOL_FromBool(FIsClSimplexSolverSCM(scm));
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





//// cassowary_scm initialization

MAKE_SMOBFUNS(cl_variable);
MAKE_SMOBFUNS(cl_expression);
MAKE_SMOBFUNS(cl_equation);
MAKE_SMOBFUNS(cl_inequality);
MAKE_SMOBFUNS(cl_solver);

void
init_cassowary_scm()
{
  REGISTER_SMOBFUNS(cl_variable);
  REGISTER_SMOBFUNS(cl_expression);
  REGISTER_SMOBFUNS(cl_equation);
  REGISTER_SMOBFUNS(cl_inequality);
  REGISTER_SMOBFUNS(cl_solver);

#ifndef SCM_MAGIC_SNARFER
#include "cassowary_scm.x"
#endif
}

} // extern "C"



/* Local Variables: */
/* tab-width: 8 */
/* c-basic-offset: 2 */
/* End: */
