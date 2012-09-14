#ifndef _OTOBJECT_H_
#define _OTOBJECT_H_

#include <OpenTracker/OpenTracker.h>

namespace ot {
  class CallbackNode; 
}


class OtObject{
 public:
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
  


  /// use this method to register a callback for a node named nodename
  /// whenever an event is received in a CallbackNode named "nodename",
  /// opentracker will run theCallback for the registered object.
  void registerNode(const char * nodename);  
  /// reimplement this in subclasses
  virtual void theCallback(ot::CallbackNode * node, ot::Event & event);
  /// reimplement this in subclasses that wish to send events into the opentracker graph
  virtual void sendData(const char *);


};


#endif //otobject
