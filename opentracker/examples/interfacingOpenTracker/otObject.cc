
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallforwardModule.h>


#include "otObject.h"



#  define otConfigurator ot::Configurator::instance()
#  define otContext      ot::Configurator::instance() ->getContext()
#  define otCallforwardModule dynamic_cast<ot::CallforwardModule*> (otContext->getModule("CallforwardConfig") )
#  define otCallbackModule dynamic_cast<ot::CallbackModule*> (otContext->getModule("CallbackConfig") )



//IMPLEMENT_CONOBJECT(OtObject);
//flag if initialized
bool OtObject::initialized=false;




// this method initializes opentracker, it receives a configuration file
// from which the initial configuration will be loaded. If the file was changed
// opentracker will reinitialize itself to reflect the changes in the dataflow
// graph.
void OtObject::initialize(const char * config){

  if (!initialized){
    // important parts of the system
    // get a context, the default modules and factories are
    // added allready ( because of the parameter 1 )
    // this call creates the context only, so that the configurator does not start 
    // the configuration thread with an empty context.
    ot::Context * ct = otContext;

    printf( "Context established.\n");
    otCallbackModule;
    otCallforwardModule;



    ct ->parseConfiguration(std::string(config));

    otConfigurator ->runConfigurationThread( config );


    printf( "Parsing complete.\nStarting UbiTrackClient !\n");


    //run is done by the otInterface method, by calling context.loopOnce()
    //    context.run();
	initialized=true;

     Puck * objs = new Puck();
     objs->registerNode("Puck");


  }
};

void OtObject::terminate(){

};

void OtObject::runOnce( void )
{

  otContext->loopOnce();

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




// Example
class Puck: public OtObject{
public:
  virtual void theCallback(ot::CallbackNode * node, ot::Event & event);
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

