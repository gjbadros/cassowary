// $Id$

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#include "Cassowary.h"
#include "ClSolver.h"
#include "ClConstraint.h"
#include "ClErrors.h"
#include "ClTypedefs.h"


ClSolver &
ClSolver::AddConstraint(ClConstraint *const ) 
    throw(ExCLTooDifficultSpecial,
          ExCLStrictInequalityNotAllowed,
          ExCLReadOnlyNotAllowed,
          ExCLEditMisuse,
          ExCLRequiredFailure,
          ExCLRequiredFailureWithExplanation,
          ExCLInternalError)
{
  return *this;
}


ostream &
PrintTo(ostream &xo, const ClConstraintSet &setCn)
{
  ClConstraintSet::const_iterator it = setCn.begin();
  for (; it != setCn.end(); ++it) {
    const ClConstraint *pcn = *it;
    xo << *pcn << endl;
  }
  return xo;
}  

ostream &
PrintTo(ostream &xo, const list<FDNumber> &listFDN)
{
  list<FDNumber>::const_iterator it = listFDN.begin();
  for (; it != listFDN.end(); ) {
    FDNumber n = *it;
    xo << n;
    ++it;
    if (it != listFDN.end())
      xo << ",";
  }
  return xo;
}  


ostream &operator<<(ostream &xo, const ClConstraintSet &setCn)
{ return PrintTo(xo,setCn); }


ostream &operator<<(ostream &xo, const ClSolver &solver)
{ return solver.PrintOn(xo); }

ostream &operator<<(ostream &xo, const list<FDNumber> &listFDN)
{ return PrintTo(xo,listFDN); }
