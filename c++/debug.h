#ifndef CASSOWARY_DEBUG_H_
#define CASSOWARY_DEBUG_H_

#include <map.h>
#include <set.h>
#include <vector.h>

class ClAbstractVariable;
class ClLinearExpression;

ostream &operator<<(ostream &xo, const set<const ClAbstractVariable *> & varset);

ostream &operator<<(ostream &xo, const map<const ClAbstractVariable *, set<const ClAbstractVariable *> > & varmap);

ostream &operator<<(ostream &xo, const map<const ClAbstractVariable *, ClLinearExpression * > & rows);

ostream &operator<<(ostream &xo, const vector<const ClAbstractVariable *> &varlist);

class Tracer {
 public:
  Tracer(const char *const sz) : sz_(sz) { cerr << "* " << sz; }
  ~Tracer() { cerr << "x " << sz_ << " exited." << endl; }
 private:
  const char *const sz_;
};

#endif
