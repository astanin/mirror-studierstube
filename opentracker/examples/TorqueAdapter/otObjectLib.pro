TEMPLATE = lib

CONFIG += debug


HEADERS = \
    otObject.h

SOURCES  = \
    otObject.cc 

mac:INCLUDEPATH += /opt/local/include $(ACEROOT) "/Users/bornik/Documents/software/Torque Game Engine 1.5 SDK/Torque SDK/engine"
QMAKE_LIBS += -lopentracker
mac:QMAKE_LIBDIR += /opt/local/lib /sw/lib

mac:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused
