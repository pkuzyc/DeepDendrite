#
#	Neuron 7.7 and InterViews 17 spec file
#
# Organization: have the installed files in
# /usr/local/nrn
# /usr/local/iv
# and then provide symbolic links to the executables from
# /usr/local/bin
# links to the library files are performed at the moment the
# the neuron package is requested to be installed by having what
# would normally be in PercentBuild in PercentInstall
# 
# note that the version numbers (e.g. 5.3 for nrn and 15 for iv)
# are excluded from the directory names
# because rpm prohibits having multiple versions
# of the same package.  It would be possible for a user to
# change their $PATH so that executables are searched for first
# within their directory hierarchy - so that they can install different
# versions of neuron in their home dir if they desire.

Summary: Neuron for empirically-based simulations of neurons and networks of neurons.

# for redhat 8 this is a hack until buildroot is understood
%define __check_files %{nil}
BuildRoot: /tmp/foo

%define rel 
%define version 7.7
%define pkgname nrn
%define prefix /usr
# This is a hack until I can figure out how to better handle replacing
# autoconf macros... (gotta love autoconf...)
%define __aclocal   aclocal || aclocal -I ./macros
%define configure_args   '--prefix' '/raid/zyc/code_for_dhs_paper/install' '--without-iv' '--with-paranrn' '--with-nrnpython=/usr/bin/python3'
# so that it goes into /usr/src/redhat/RPMS/i686/ instead of i386
%define _target_cpu x86_64

Name: %{pkgname}
Version: %{version}
Release: %{rel}

License: GPL
Group: Sciences/Other
Source: ftp://ftp.neuron.yale.edu/neuron/unix/nrn-7.7.tar.gz
URL: http://neuron.yale.edu/neuron/install/installunix.html
Distribution: Linux Neuron
Vendor: Yale University
Packager: Michael Hines <Michael.Hines@Yale.edu>

%description
A flexible and powerful simulator of neurons and networks
NEURON is a simulation environment for developing and exercising models of neurons and networks of neurons. It is particularly well-suited to problems where cable properties of cells play an important role, possibly including extracellular potential close to the membrane), and where cell membrane properties are complex, involving many ion-specific channels, ion accumulation, and second messengers. It evolved from a long collaboration between Michael Hines and John W. Moore at the Department of Neurobiology, Duke University. Their express goal was to create a tool designed specifically for solving the equations that describe nerve cells.

NEURON has benefited from judicious revision and selective enhancement, guided and assisted by the growing number of neuroscientists who have incorporated empirically-based modeling into their research strategies. Their feedback and collaboration have led to many improvements.

%prep
# the below may be replaced by a percentsetup?
# get rid of any possible remenant of previous attempts to build including symbolic
# links in /usr/local/bin and /usr/local/nrn and /usr/local/iv directories
(
cd /usr/local/bin
rm -r -f memacs neurondemo nrnivmodl modlunit nrnpyenv.sh
rm -r -f nrngui nrniv 
rm -r -f iclass idraw idemo
cd ..
)

# this is where system-wide applications are typically added
# NRNDEST=/usr/local

# see organization note above

%build

%install

#
# nrn bin symbolic links
#
ln -s /usr/local/nrn/%{_target_cpu}/bin/memacs /usr/local/bin/memacs
ln -s /usr/local/nrn/%{_target_cpu}/bin/neurondemo /usr/local/bin/neurondemo
ln -s /usr/local/nrn/%{_target_cpu}/bin/modlunit /usr/local/bin/modlunit
ln -s /usr/local/nrn/%{_target_cpu}/bin/nrnivmodl /usr/local/bin/nrnivmodl
ln -s /usr/local/nrn/%{_target_cpu}/bin/nrngui /usr/local/bin/nrngui
ln -s /usr/local/nrn/%{_target_cpu}/bin/nrniv /usr/local/bin/nrniv
ln -s /usr/local/nrn/%{_target_cpu}/bin/nrnpyenv.sh /usr/local/bin/nrnpyenv.sh
#
# iv bin symbolic links
#
ln -s /usr/local/iv/%{_target_cpu}/bin/iclass /usr/local/bin/iclass
ln -s /usr/local/iv/%{_target_cpu}/bin/idemo /usr/local/bin/idemo
ln -s /usr/local/iv/%{_target_cpu}/bin/idraw /usr/local/bin/idraw
#
#
%files
#
# The files listed here are preserved after rpm installation
# all other files are deleted.  Recall that the listing of
# a directory includes all files from that directory.
#
# nrn bin symbolic links
#
/usr/local/bin/memacs
/usr/local/bin/neurondemo
/usr/local/bin/modlunit
/usr/local/bin/nrnivmodl
/usr/local/bin/nrngui
/usr/local/bin/nrniv
/usr/local/bin/nrnpyenv.sh
#
# nrn files
#
/usr/local/nrn
#
# iv bin symbolic links
#
/usr/local/bin/iclass
/usr/local/bin/idemo
/usr/local/bin/idraw
#
# iv files
#
/usr/local/iv
#
