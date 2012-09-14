#ifndef _OTOBJECT_H_
#define _OTOBJECT_H_

#include <OpenTracker/OpenTracker.h>

namespace ot {
  class CallbackNode; 
}


class OtObject{
 public:
  //  typedef std::vector<OtObject*> ObjTable;
  static OtObject * objTable[10];
  static int objcount;
protected:
  static  bool initialized;
  std::string scriptCallback;

  //  static ObjTable objTable;
public:

  /// opentracker initialization
  static void initialize(const char * config);
  static void runOnce();
  static void terminate();
  /// the callback that will be registered with opentracker
  static void callback( ot::CallbackNode * node,  ot::Event & event, void * data ) ;  
  
  static void sendPositionC(const char * nodename, Point3F & pos);

  /// use this method to register a callback for a node named nodename
  void registerNode(const char * nodename);
  void setScriptCallback(const char * cbname);
  
  virtual void theCallback(ot::CallbackNode * node, ot::Event & event);
  virtual void sendData(const char *);

  //virtual void sendDataStrings(const char *, int argc, char ** argv);
};


#endif //otobject
