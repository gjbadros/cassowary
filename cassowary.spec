Name: cassowary
Summary: A Linear Arithmetic Constraint Solving Library.
Version: 0.43
Release: 1
Source: http://www.cs.washington.edu/research/constraints/cassowary/cassowary-0.42.tar.gz
Group: Development/Libraries
BuildRoot: /tmp/cassowary-build
Copyright: Copyright (C) 1998,1999 Greg J. Badros
Packager: Greg J. Badros <gjb@cs.washington.edu>
URL: http://vicarious-existence.mit.edu/scwm/

%description
Cassowary is an incremental constraint solving toolkit that efficiently
solves systems of linear equalities and inequalities.  Constraints may
be either requirements or preferences.  Client code specifies the
constraints to be maintained, and the solver updates the constrained
variables to have values that satisfy the constraints.


%changelog
* Wed Apr 14 1999 Greg J. Badros <gjb@cs.washington.edu>

- Initial release of this package.

%prep

%setup

%build
./configure --prefix=/usr/share/contrib --exec-prefix=/usr/contrib --with-guile-prefix=/usr/contrib
make 

%install
make prefix=$RPM_BUILD_ROOT/usr/share/contrib exec_prefix=$RPM_BUILD_ROOT/usr/contrib install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%doc ANNOUNCE AUTHORS COPYING IMPORTANT INSTALL LICENSE NEWS README THANKS
%doc c++/ChangeLog docs/cassowary-tr.pdf docs/cassowary-tr.ps.gz
%doc guile/cassowary_scm-procedures.txt guile/cassowary_scm-variables.txt
%doc guile/cassowary_scm.sgml
