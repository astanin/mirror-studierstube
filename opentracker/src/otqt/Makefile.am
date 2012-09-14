include $(top_srcdir)/src/Makefile.common

## Qt3 - MOC
%.moc.cxx: %.h
	$(QTDIR)/bin/moc -o $@ $<

## Qt3 - UIC
%.h: %.ui
	$(QTDIR)/bin/uic -o $@ $<

%.cxx: %.ui
	$(QTDIR)/bin/uic -o $@ -impl $*.h $<

noinst_LTLIBRARIES = libotqt.la

if WANT_OTQT
libotqt_la_MOC = OTQt.moc.cxx
endif

BUILT_SOURCES = \
	${libotqt_la_MOC}

libotqt_la_SOURCES = \
	${libotqt_la_MOC} \
	OTQt.cxx \
	OTQtMEMCalibProc.cxx \
	OTQtConfigFileEditor.cxx \
	QtMouseEventModuleBase.cxx \
	QtMouseEventCalibModule.cxx \
	QtMouseButtonSink.cxx \
	QtAppScreenPosSink.cxx \
	QtMousePosSink.cxx \
	OTQtMath.cxx \
	QtAppScreen.cxx \
	QtMouseEventModule.cxx \
	QtMouseEventSinkBase.cxx \
	QtMouseWheelSink.cxx

ototqtincdir = $(otincdir)/otqt
ototqtinc_HEADERS = \
	OTQt.h \
	OTQtConfigFileEditor.h \
	OTQtMEMCalibProc.h \
	OTQtLog.h \
	OTQtMath.h \
	QtAppScreen.h \
	QtAppScreenPosSink.h \
	QtMouseButtonSink.h \
	QtMouseEventCalibModule.h \
	QtMouseEventModuleBase.h \
	QtMouseEventModule.h \
	QtMouseEventSinkBase.h \
	QtMousePosSink.h \
	QtMouseWheelSink.h

EXTRA_DIST = \
	OTQtLog.cxx

CLEANFILES = \
	${libotqt_la_MOC}
