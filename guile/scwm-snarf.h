/* $id$
 * scwm-snarf.h
 * (C) 1998 Maciej Stachowiak and Greg J. Badros
 */

#ifndef SCWM_SNARF_H__
#define SCWM_SNARF_H__

#include <libguile/snarf.h>

/* Can extract comment post-cpp using, e.g.:
gcc -DHAVE_CONFIG_H -I. -I. -I../include -I/usr/X11R6/include -I/uns/include -E -C -DSCWM_EXTRACT_COMMENTS binding.c
*/

#ifndef SCWM_EXTRACT_COMMENTS
#ifndef SCM_MAGIC_SNARFER
#define SCWM_PROC(fname,primname, req, opt, var, ARGLIST) \
	SCM_PROC(s_ ## fname, primname, req, opt, var, fname); \
SCM fname ARGLIST
#else
#define SCWM_PROC(fname,primname, req, opt, var, ARGLIST) \
	SCM_PROC(s_ ## fname, primname, req, opt, var, fname);
#endif
#endif

#endif
