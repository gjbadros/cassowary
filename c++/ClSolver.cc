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

ostream &operator<<(ostream &xo, const ClConstraintSet &setCn)
{ return PrintTo(xo,setCn); }
