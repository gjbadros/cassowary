#ifndef CASSOWARY_DEBUG_H_
#define CASSOWARY_DEBUG_H_

#include <map.h>
#include <set.h>
#include <vector.h>

class ClVariable;
class ClLinearExpression;

ostream &operator<<(ostream &xo, const set<ClVariable> & varset);

ostream &operator<<(ostream &xo, const map<ClVariable, set<ClVariable> > & varmap);

ostream &operator<<(ostream &xo, const map<ClVariable, ClLinearExpression > & rows);

ostream &operator<<(ostream &xo, const vector<ClVariable> &varlist);


class Tracer {
 public:
  Tracer(const char *const sz) : sz_(sz) { cerr << "* " << sz; }
  ~Tracer() { cerr << "x " << sz_ << " exited." << endl; }
 private:
  const char *const sz_;
};


// Don't define this if exceptions work!
#define EXCEPTION_ABORT abort()

#endif
