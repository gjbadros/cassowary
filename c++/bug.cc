#include <map>

/* compile w/ egcs-1.1b and -DCRASH */

#ifndef CRASH
class ClAbstractVariable;
typedef const ClAbstractVariable *PconstClAbstractVariable;
#endif

template <class T>
class ClGenericLinearExpression  {
 public:
  typedef std::map<PconstClAbstractVariable,T> ClVarToCoeffMap;

  T CoefficientFor(const ClAbstractVariable &var) const
    { 
    typename ClVarToCoeffMap::const_iterator it = _terms.find(&var);
    }
};
