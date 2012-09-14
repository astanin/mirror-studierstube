## Makefile.am -- Process this file with automake to produce Makefile.in

#        CORBAModule.cxx \
#        CORBAModule.h \
#        CORBASink.h \
#        CORBASource.h \
#        CORBAUtils.cxx \
#        CORBAUtils.h 

include $(top_srcdir)/src/Makefile.common

otnetworkincdir = $(otincdir)/network
otnetworkinc_HEADERS = \
        CORBAModule.h \
        CORBASink.h \
        CORBASource.h \
        CORBAUtils.h \
        DwarfModule.h \
        DwarfSink.h \
        DwarfSource.h \
        Network.h \
        NetworkSink.h \
        NetworkSinkModule.h \
        NetworkSource.h \
        NetworkSourceModule.h \
        TCPModule.h \
        TCPSink.h \
        VRPNModule.h \
        VRPNSink.h \
        VRPNSource.h

noinst_LTLIBRARIES = libnetwork.la
libnetwork_la_SOURCES = \
        DwarfModule.cxx \
        DwarfModule.h \
        DwarfSink.h \
        DwarfSource.h \
        Network.h \
        NetworkSink.h \
        NetworkSinkModule.cxx \
        NetworkSinkModule.h \
        NetworkSource.h \
        NetworkSourceModule.cxx \
        NetworkSourceModule.h \
        TCPModule.cxx \
        TCPModule.h \
        TCPSink.h \
        VRPNModule.cxx \
        VRPNModule.h \
        VRPNSink.cxx \
        VRPNSink.h \
        VRPNSource.cxx \
        VRPNSource.h
