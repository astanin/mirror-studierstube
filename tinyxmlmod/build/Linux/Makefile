BASEDIR = ../..
SRCDIR = $(BASEDIR)/src
LIBDIR = $(BASEDIR)/lib

LFLAGS = -shared
CXXFLAGS = -O3 -DTINYXML_STATIC -I$(SRCDIR)/TinyXML
LIBS = -lm

OBJECTS = \
	$(SRCDIR)/TinyXML/tinystr.o \
	$(SRCDIR)/TinyXML/tinyxml.o \
	$(SRCDIR)/TinyXML/tinyxmlerror.o \
	$(SRCDIR)/TinyXML/tinyxmlparser.o \
	$(SRCDIR)/TinyXPath/action_store.o \
	$(SRCDIR)/TinyXPath/lex_util.o \
	$(SRCDIR)/TinyXPath/node_set.o \
	$(SRCDIR)/TinyXPath/tokenlist.o \
	$(SRCDIR)/TinyXPath/xml_util.o \
	$(SRCDIR)/TinyXPath/xpath_expression.o \
	$(SRCDIR)/TinyXPath/xpath_processor.o \
	$(SRCDIR)/TinyXPath/xpath_stack.o \
	$(SRCDIR)/TinyXPath/xpath_static.o \
	$(SRCDIR)/TinyXPath/xpath_stream.o \
	$(SRCDIR)/TinyXPath/xpath_syntax.o

TARGET = $(LIBDIR)/libTinyXMLMod.so

all: $(OBJECTS)
	$(CXX) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
