Summary:	Studierstube 4 Components
Name:		stb4Components
Version:	1.0
Release:	%(date '+%Y%m%d')
License:	GPL
Group:		Development/Libraries
Source:		%{name}-%{version}.tar.bz2
Vendor:		Institute for Computer Graphics and Vision, Graz University of Technology, Austria
URL:		http://www.studierstube.org
Packager:	Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Prefix:		/usr
BuildRoot: 	%{_tmppath}/buildroot-%{name}-%{version}
Requires:	studierstube Coin
BuildRequires:	ICGBuilder studierstube-devel Coin-devel swig

%define _prefix %{prefix}

%description
STUDIERSTUBE COMPONENTS are optional modules which provide miscellaneous
functionality for the STUDIERSTUBE AR framework version 4. Each COMPONENT is
built as dynamic library which can be loaded by the STUDIERSTUBE kernel at
runtime. The COMPONENTS to load are specified in the STUDIERSTUBE kernel
configuration file (XML).     

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup

%build
scons PREFIX=%{_prefix} LIBDIR=%{_libdir}

%install
scons --cache-disable DESTDIR=$RPM_BUILD_ROOT PREFIX=%{_prefix} LIBDIR=%{_libdir} install
mkdir -p $RPM_BUILD_ROOT/%{_docdir}/%{name}
#cp -a doc/doc/html $RPM_BUILD_ROOT/%{_docdir}/%{name}

%clean
scons -c
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/*.so*


%package devel
Summary:	Studierstube Components header files
Group:		Development/Libraries
Requires:	%{name} = %{version} ICGBuilder studierstube-devel Coin-devel

%description devel
This package contains header files and include files that are needed for development using Studierstube Components.

%files devel
%defattr(-,root,root)
%{_prefix}/include/*
%{_libdir}/pkgconfig/*
