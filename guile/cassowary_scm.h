// $Id$
// cassowary_scm.h
// By Greg J. Badros -- 18 July 1998
//
// A Guile Scheme wrapper for the Cassowary Constraint Solving Toolkit
//

#ifndef CASSOWARY_SCM_H__
#define CASSOWARY_SCM_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <guile/gh.h>

#include "ClVariable.h"
#include "ClLinearExpression.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"
#include "ClSimplexSolver.h"

#undef EXTERN
#undef EXTERN_SET
#ifdef CASSOWARY_SCM_IMPLEMENTATION
#define EXTERN
#define EXTERN_SET(x,y) x = y
#else
#define EXTERN extern
#define EXTERN_SET(x,y) extern x
#endif

#define MAKE_SMOBFUNS(T) \
static scm_smobfuns T ## _smobfuns = { \
  &mark_ ## T, \
  &free_ ## T, \
  &print_ ## T,  0 }

#define REGISTER_SMOBFUNS(T) scm_tc16_ ## T = scm_newsmob(& T ## _smobfuns)


extern "C" {
void init_cassowary_scm();

//// ClVariable wrapper


//// ClLinearExpression wrapper



//// ClLinearEquation wrapper



//// ClLinearInequality wrapper



//// ClSimplexSolver wrapper


} // extern "C"

#endif // CASSOWARY_SCM_H__
