#include "otcb_obj.h"

void otCallbackObj::registerCallback(OTCallbackFunction * function, void * obj){
  if (objname.compare("") != 0){
	otCallbackModule ->setCallback(objname.c_str(), function, obj);	
	registered = true;
  }
};

otCallbackObj::otCallbackObj():objname(""){
  
  _register();
};

otCallbackObj::~otCallbackObj(){
  deregister();
};

void otCallbackObj::callback( ot::CallbackNode * node,  ot::Event & event, void * data ){
	std::vector<float> & pos = event.getPosition();
	printf("position is x: %f y: %f z: %f\n", pos[0], pos[1], pos[2]);
};

void otCallbackObj::deregister(){
  if (registered)
	otCallbackModule ->setCallback(objname.c_str(), NULL, NULL);	
};

void otCallbackObj::_register(){
  registerCallback(&otCallbackObj::callback, (void*)this);	
};

void otCallbackObj::setName(const char * name){
  objname = name;
};
void otCallbackObj::sendData(){
  ot::Event event;
  event.timeStamp();
  event.setAttribute("obj", objname);
  event.setAttribute("val", std::string(" not implemented, sending default message"));
  otCallforwardModule ->callForward(objname, event);
};









