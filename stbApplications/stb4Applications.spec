Summary:	Studierstube 4 Applications
Name:		stb4Applications
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
Requires:	studierstube Coin libcal3d12
BuildRequires:	ICGBuilder studierstube-devel Coin-devel libcal3d-devel

%define _prefix %{prefix}

%description
This package contains various APPLICATIONS built upon the STUDIERSTUBE
augmented reality (AR) framework version 4.

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup

%build
scons PREFIX=%{_prefix} LIBDIR=%{_libdir}

%install
scons --cache-disable DESTDIR=$RPM_BUILD_ROOT PREFIX=%{_prefix} LIBDIR=%{_libdir} install
rm -rf $RPM_BUILD_ROOT/%{_prefix}/include/*
rm -rf $RPM_BUILD_ROOT/%{_libdir}/pkgconfig/*
mkdir -p $RPM_BUILD_ROOT/%{_docdir}/%{name}

# camera calibration data should better be installed with opentracker, but this
# needs to be discussed with all developers, so put them here for the moment:
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/stb
cp -a cameraCal $RPM_BUILD_ROOT/%{_datadir}/stb

%clean
scons -c
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/*.so*
%{_datadir}/stb
