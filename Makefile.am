## $Id$
## Process this file with automake to create Makefile.in

SUBDIRS = \
 c++ \
 guile \
 docs \
 java

bin_SCRIPTS = cassowary-config

EXTRA_DIST = ANNOUNCE AUTHORS COPYING IMPORTANT INSTALL LICENSE NEWS README THANKS \
		cassowary.spec cassowary-nofd.spec

distdir_nofd = cassowary-nofd-@VERSION@

distdir-nofd:	distdir
	mv cassowary-@VERSION@ $(distdir_nofd)

dist-nofd:	distdir-nofd
	-chmod -R a+r $(distdir_nofd)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir_nofd).tar.gz $(distdir_nofd)
	-rm -rf $(distdir_nofd)
