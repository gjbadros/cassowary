## $Id$
## Process this file with automake to create Makefile.in

EXTRACT_DOCS = perl ../../../scwm/utilities/dev/scwmdoc -PCL


EXTRA_DIST=cassowary_scm-procedures.txt cassowary_scm-variables.txt cassowary_scm.sgml

DISTCLEANFILES = cassowary_scm-*.txt cassowary_scm.sgml

cassowary_scm.sgml cassowary_scm-procedures.txt cassowary_scm-variables.txt: ../cassowary_scm.cc
	$(EXTRACT_DOCS) -o cassowary_scm.sgml -O cassowary_scm-variables.txt $^ > cassowary_scm-procedures.txt
