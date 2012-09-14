Summary:	Library for handling XML
Name:		tinyxmlmod
Version:	1.0
Release:	1
License:	GPL
Group:		Development/Libraries
Source:		%{name}-%{version}.tar.bz2
Vendor:		Institute for Computer Graphics and Vision, Graz University of Technology, Austria
URL:		http://www.studierstube.org
Packager:	Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Prefix:		/usr
BuildRoot: 	%{_tmppath}/buildroot-%{name}-%{version}
#Requires:	...
BuildRequires:	scons ICGBuilder

%define _prefix %{prefix}

%description
TinyXMLMod is a modified small library for handling XML.

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup

%build
scons PREFIX=%{_prefix} LIBDIR=%{_libdir}

%install
scons --cache-disable DESTDIR=$RPM_BUILD_ROOT PREFIX=%{_prefix} LIBDIR=%{_libdir} install

%clean
scons -c
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/*.so*


%package devel
Summary:	TinyXMLMod header files
Group:		Development/Libraries
Requires:	%{name} = %{version}

%description devel
This package contains header files and include files that are needed for development using tinyxmlmod.

%files devel
%defattr(-,root,root)
%{_libdir}/*.a
%{_libdir}/pkgconfig/*
%{_prefix}/include/*
