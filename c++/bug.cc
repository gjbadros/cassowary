#include <map>

/* compile w/ egcs-1.1b, -pedantic and/or -DCRASH */

#ifndef CRASH
class ClAbstractVariable;
typedef const ClAbstractVariable *PconstClAbstractVariable;
#endif

template <class T>
class ClGenericLinearExpression  {
 public:
  typedef std::map<PconstClAbstractVariable,T> ClVarToCoeffMap;

  T coefficientFor(const ClAbstractVariable &var) const
    { 
    ClVarToCoeffMap::const_iterator it = _terms.find(&var);
    }
};
