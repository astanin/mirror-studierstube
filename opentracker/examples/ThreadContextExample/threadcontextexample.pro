TEMPLATE = app

CONFIG += debug

FORMS = tctxmainwindow.ui

HEADERS = \
    ponghelp.h \
    tctxmainwindow.h \
    pongview.h \
    pongscene.h \
    poseable.h \
    axes.h \
    goal.h \
    checkerfloor.h \
    arena.h \
    dilatedrect.h \
    playingfield.h \
    paddle.h \
    puck.h

SOURCES  = \
    main.cpp \
    tctxmainwindow.cpp \
    pongview.cpp \
    pongscene.cpp \
    poseable.cpp \
    axes.cpp \
    goal.cpp \
    checkerfloor.cpp \
    arena.cpp \
    dilatedrect.cpp \
    paddle.cpp \
    playingfield.cpp \
    puck.cpp 

INCLUDEPATH += $(OTROOT)/include
mac:INCLUDEPATH += /opt/local/include $(ACEROOT)
QMAKE_LIBS += -L$(OTROOT)/lib -lopentracker
mac:QMAKE_LIBDIR += /opt/local/lib /sw/lib

mac:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused
