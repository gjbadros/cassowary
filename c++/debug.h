#include <map.h>
#include <set.h>

class ClVariable;
class ClLinearExpression;

ostream &operator<<(ostream &xo, const set<ClVariable> & varset);

ostream &operator<<(ostream &xo, const map<ClVariable, set<ClVariable> > & varmap);

ostream &operator<<(ostream &xo, const map<ClVariable, ClLinearExpression > & rows);


class Tracer {
 public:
  Tracer(const char *const sz) : sz_(sz) { cerr << "* " << sz; }
  ~Tracer() { cerr << "x " << sz_ << " exited." << endl; }
 private:
  const char *const sz_;
};
