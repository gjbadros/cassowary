#include <map.h>
#include <set.h>

class ClVariable;

ostream &operator<<(ostream &xo, const set<ClVariable> & varset)
{
  set<ClVariable>::const_iterator it = varset.begin();
  xo << "{ ";
  if (it != varset.end())
    {
    xo << (*it);
    ++it;
    }
  for (; it != varset.end(); ++it) 
    {
    xo << ", " << (*it);
    }
  xo << " }" << endl;
  return xo;
}  


ostream &operator<<(ostream &xo, const map<ClVariable, set<ClVariable> > & varmap)
{
  map<ClVariable, set<ClVariable> >::const_iterator it = varmap.begin();
  for (; it != varmap.end(); ++it) 
    {
    xo << (*it).first << " -> " << (*it).second << endl;
    }
  return xo;
}

ostream &operator<<(ostream &xo, const map<ClVariable, ClLinearExpression > & rows)
{
  map<ClVariable, ClLinearExpression >::const_iterator it = rows.begin();
  for (; it != rows.end(); ++it) 
    {
    xo << (*it).first << " <-=-> " << (*it).second << endl;
    }
  return xo;
}
