class istream;
class ClConstraint;
class ClVariable;

// Attempts to read a constraint of input stream in
// Returns constraint (freshly allocated, client responsibility to deallocate)
// if succesful. Otherwise, returns 0.
ClConstraint * parseConstraint(istream & in, ClVariable * aVars);
