/* acconfig.h --- documentation for local preprocessor symbols 
                  defined by configure.
   By Greg J. Badros -- 16-March-1999
   */

/* Package and version macros defined by automake */
#undef PACKAGE 
#undef VERSION 

/* Check solver data structures' integrity at run time.
   Performance penalty, but greater peace of mind */
#undef CL_SOLVER_CHECK_INTEGRITY

/* Do not permit use of deprecated functions */
#undef CL_NO_DEPRECATED

/* Output verbose trace messages to STDERR while executing */
#undef CL_TRACE

/* Use GNU STL library's hash_map, hash_set for the data structures
   instead of the standard sorted dictionary implementations */
#undef CL_USE_HASH_MAP_AND_SET

/* The divisor for the hash function -- this machine's word size
   may be a reasonable value, or just use 1 */
#undef CL_PTR_HASH_DIVISOR

/* Do not use any input/output functionality (for embedded systems) */
#undef CL_NO_IO

/* Output some solver statistics to STDERR while executing */
#undef CL_SOLVER_STATS

/* Define this if you want to build the finite domain solver */
#undef CL_BUILD_FD_SOLVER

/* Define this if you have the GTL, graph template library, installed
   you need that for the FD solver */
#undef CL_HAVE_GTL

/* Does cc/gcc take -fpermissive? */
#undef NO_CC_PERMISSIVE

/* Define this if your libguile has scm_make_smob_type_mfpe,
   the new-style SMOB interface (>= guile-1.3.2 */
#undef HAVE_SCM_MAKE_SMOB_TYPE_MFPE
