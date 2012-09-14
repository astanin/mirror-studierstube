//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _SIMBASE_H_
#include "console/simBase.h"
#endif
#include "math/mPoint.h"

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallforwardModule.h>


#include "otObject.h"
#include "game/item.h"

class Puck: public OtObject{
public:
  virtual void theCallback(ot::CallbackNode * node, ot::Event & event);
};

#ifdef OTCORBA

#  define otConfigurator ot::Configurator::instance()
#  define otContext      ot::Configurator::instance() ->getContext()
#  define otCallforwardModule dynamic_cast<ot::CallforwardModule*> ( ( otContext ).getModule("CallforwardConfig") )
#  define otCallbackModule dynamic_cast<ot::CallbackModule*> ( ( otContext ).getModule("CallbackConfig") )

#else

#  define otConfigurator ot::Configurator::instance()
#  define otContext      ot::Configurator::instance() ->getContext()
#  define otCallforwardModule dynamic_cast<ot::CallforwardModule*> (otContext->getModule("CallforwardConfig") )
#  define otCallbackModule dynamic_cast<ot::CallbackModule*> (otContext->getModule("CallbackConfig") )

#endif

//IMPLEMENT_CONOBJECT(OtObject);
//flag if initialized
bool OtObject::initialized=false;

// list of objects that will receive data.
// the initial idea was to use an std::vector to hold the OtObject pointers, 
// however there seems to be some kind of conflict between std::vector and Torque
// for this reason only an array is used
OtObject * OtObject::objTable[10];
int OtObject::objcount = 0;



// this method initializes opentracker, it receives a configuration file
// from which the initial configuration will be loaded. If the file was changed
// opentracker will reinitialize itself to reflect the changes in the dataflow
// graph.
void OtObject::initialize(const char * config){

  if (!initialized){
    for (int i = 0; i < 10; i++)
      objTable[i]=0;
    // important parts of the system
    // get a context, the default modules and factories are
    // added allready ( because of the parameter 1 )
    // this call creates the context only, so that the configurator does not start 
    // the configuration thread with an empty context.
#ifdef OTCORBA
    ot::Context & ct = otContext;
#else
    ot::Context * ct = otContext;
#endif
    printf( "Context established.\n");
    otCallbackModule;
    otCallforwardModule;



#  ifdef OTCORBA
    ct .parseConfiguration(std::string(config));
#  else
    ct ->parseConfiguration(std::string(config));
#  endif //OTCORBA
    otConfigurator ->runConfigurationThread( config );


	printf( "Parsing complete.\nStarting UbiTrackClient !\n");


    //run is done by the otInterface method, by calling context.loopOnce()
    //    context.run();
	initialized=true;

     Puck * objs = new Puck();
     objs->registerNode("Puck");
     objs->setScriptCallback("puckCallback");
     if (OtObject::objcount < 10){
       OtObject::objTable[OtObject::objcount] = objs;
       OtObject::objcount++;
     }

  }
};

void OtObject::terminate(){
    for (int i = 0; i < 10; i++)
      if (objTable[i]!=0)
	delete objTable[i];

};

void OtObject::runOnce( void )
{

 
#ifdef OTCORBA
  otContext.loopOnce();
#else
  otContext->loopOnce();
#endif

}


// this is the actual callback registered with all the CallbackNode(s) from opentracker,
// it expects to receive an opaque (void*) OtObject in the data. The function will call the
// Object's virtual theCallback function to do the work.
// static
void OtObject::callback(ot::CallbackNode * node,  ot::Event & event, void * data ){
  OtObject * self = reinterpret_cast<OtObject*> (data);
  self->theCallback(node, event);

};

// register the object with an opentracker CallbackNode nodename,
// if the node does not actually exist, the registered information will be stored
// and used to register upon creation of the node.
void OtObject::registerNode(const char * nodename){
  otCallbackModule ->setCallback(nodename, callback, (void *)this);	
  
}

// save the name of a script function to be called by the callback method.
// The script is expected to handle position messages, and be able to receive
// Poin3F as position.
void OtObject::setScriptCallback(const char * cbname){
  scriptCallback = cbname;
};

// the default callback for objects. Any subclases have to declare this method to change
// the default handling of incomming events. The default will obtain the position from an 
// ot::Event, convert it to Point3F and then run a function defined in the script.
// ****
// **** At the moment the method to execute the script hangs the torque engine.
// ****
// ****
void OtObject::theCallback(ot::CallbackNode * node, ot::Event & event){    
  // call some script defined function with whatever we got from here
  std::vector<float> & pos = event.getPosition();
  Point3F tpos;
  tpos.x = pos[0];
  tpos.y = pos[1];
  tpos.z = pos[2];
  printf("got position %f %f %f %s\n", (pos[0]), (pos[1]), (pos[2]), scriptCallback.c_str());
  //  Con::executef(2, scriptCallback.c_str(), tpos);
}

// static method to send data through a CallforwardNode (at the moment it is possible to send position 
// only.
void OtObject::sendPositionC(const char * nodename, Point3F & pos){
   ot::Event event;
   event.timeStamp();
   event.getConfidence() = 1.0;
   event.getPosition()[0] = pos.x;
   event.getPosition()[1] = pos.y;
   event.getPosition()[2] = pos.z;

 // event.setAttribute(std::string("vector<float>"), stddad::string("position"), std::string(argv[3]));
   event.setConfidence(1.0f);

  // printf("calling module %s\n", nodename);
   otCallforwardModule->callForward(nodename, event);
   
};

// send data through a CallforwardName named nodename. This is an instance
// method, it should be reimplemented by all subclasses to customize
// their event.
void OtObject::sendData(const char * nodename){
  ot::Event event;
  event.timeStamp();
  event.setAttribute("obj", std::string(nodename));
  event.setAttribute("val", std::string(" not implemented, sending default message"));
  otCallforwardModule ->callForward(nodename, event);

};


// An object (the puck) overloads the theCallback method to appropriately handle ot::Event(s) 
// sent to it.
void Puck::theCallback(ot::CallbackNode * node, ot::Event & event){
  std::vector<float> & pos = event.getPosition();
  Point3F tpos;
  MatrixF mat;
  tpos.x = pos[0];
  tpos.y = pos[1];
  tpos.z = pos[2];

  mat.setPosition(tpos);
  
  Item *pItem = dynamic_cast<Item*>(Sim::findObject(Con::getIntVariable("$actPongBall")));
  if(pItem)
    {
      //      printf("calling the puck callback\n");
      pItem->setTransform(mat);

    }
};


// Script callable function to initialize opentracker
ConsoleFunction(OpenTrackerInit, void, 2, 2, "OpenTrackerInit( configurationFile )"){
  OtObject::initialize(argv[1]);
};

// Script callable function to run the OpenTracker loop once. This has to be scheduled 
// somehow to be run in order to get events and send events
ConsoleFunction(OpenTrackerRunOnce, void, 1, 1, "OpenTrackerRunOnce(  )"){
  OtObject::runOnce();
};

// Script callable function to terminate OpenTracker, it handles all clean up related to 
// opentracker
ConsoleFunction(OpenTrackerTerminate, void, 1, 1, "OpenTrackerTerminate(  )"){
  OtObject::terminate();
};


// Script callable function to register a callback with the system. It will create an OtObject to handle the
// callback and store it in an array. the callback is a function
ConsoleFunction( OpenTrackerRegisterCallback, void, 3, 3, "OpenTrackeRegisterCallback( nodename, callbackfcn )"){
  OtObject * objs = new OtObject();
  objs->registerNode(argv[1]);
  objs->setScriptCallback(argv[2]);
  if (OtObject::objcount < 10){
    OtObject::objTable[OtObject::objcount] = objs;
    OtObject::objcount++;
  }

  //  otCallbackModule ->setCallback(argv[1], fcnCallback, (void *)NULL);	
  
}

ConsoleFunction( OpenTrackerSendPosition, void, 3, 0, ("String NodeName, Point3F position "))

{
  ot::Event event;
	
  //Point3F pos;
	float pos[3];
  dSscanf(argv[2], "%g %g %g",
	  &pos[0],
	  &pos[1],
	  &pos[2]);
  

  event.timeStamp();
  event.getPosition()[0] = pos[0];
  event.getPosition()[1] = pos[1];
  event.getPosition()[2] = pos[2];
  
 // event.setAttribute(std::string("vector<float>"), std::string("position"), std::string(argv[3]));
  //  printf("Argv is %s\n", argv[1]);
  otCallforwardModule ->callForward(argv[1], event);
}

ConsoleFunction(OpenTrackerSetPuckPosition, void, 1, 1, "OpenTrackerSetPuckPosition(  )"){
  printf("calling set puck position\n");
}
