#include <OpenTracker/OpenTracker.h>

using namespace ot;

class otUbiTrackException: public OtException{
public:

  otUbiTrackException(std::string e)throw(): OtException(e){};
};

class otUbj{
protected:
  StringTable attr;
 public:
  virtual ~otUbj(){}=0;
  bool hasKey(const char * key) {
    return attr.hasKey(key);
  };
  const char * getValue(const char * key){
    return attr.get(key).c_str();
  };
  void setValue(const char* key, const char * value){
    attr.put(key, value);
  };
};


 class otUbiAppDefault{
 public:
  virtual ~otUbiAppDefault(){};
  int onAddObject(const char * id, Ubj *){};
  int onRemoveObject(const char * id) {};
  int onIsAlive() {return 1;};
};



class otUbiTrackClient: public UbiTrackClient{
public:

  typedef std::map<std::string, otUbj *> UbjDictionary;
protected:
  UbjDictionary remotes;
  UbjDictionary local_objs;
  UbiTrackClientAppManager * handler;
  static otUbiAppDefault defaultHandler;
  CORBA::Object_var uts;
public:
  otUbiTrackClient();
  virtual ~otUbiTrackClient();

  /// Server -> Client interface
  virtual void UTSaddObject(const char * id, const char *queryid , Ubj *) = 0;
  virtual void UTSremoveObject(const char * id) = 0;
  virtual bool isAlive()=0;  

  /// Application management interface
  virtual const char * appRegisterObj(const char * appname, const char * type, Ubj *)=0;
  virtual void appDeregAll()=0;
  virtual void setHandler( UbiApp * application ) = 0;
  virtual void resetHandler() = 0;  

};


otUbiTrackClient:: otUbiTrackClient(){

};

otUbiTrackClient::~otUbiTrackClient(){
  deregAll();
};

/// this has to be called after initializing opentracker, probably a good idea is to do it whithin
/// an otInit method.
void otUbiTrackClient::initialize(){
      CORBA::ORB_var orb = CORBAModule::getORB();
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) "UbiTrackServer");
      
      try {
	obj = CORBAUtils::getObjectReference(orb, string_name);
      }catch(...){
	logPrintE("CAUGHT AN EXCEPTION while trying to reach UbiTrackServer\n");
      }

      const char* ior= orb->object_to_string(obj);
      logPrintW("%s\n",ior);

      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to object supposedly bound to %s.\nExiting....\n", (const char*) string_name);

	throw otUbiTrackException("Could not obtain a reference to UbiTrackServer");
      }

      /* Narrow to a var of type ????
      OT_CORBA::Node_var sink_ref = OT_CORBA::Node::_narrow(obj);
      
      CORBASink * sink = new CORBASink( sink_ref , frequency );
      sinks.push_back( sink );
      logPrintI("Build CORBASink node\n");
      */

};

void otUbiTrackClient::addObject(const char * id, const char * queryid , Ubj * obj){
  handler->onAddObj(id, queryid, obj);
};


void otUbiTrackClient::removeObject(const char * id){
  handler->onRemoveObject(id);
};
 
bool otUbiTrackClient::isAlive(){
  return handler->isAlive();
};



void otUbiTrackClient::registerObj(const char * appname, const char * type, const char * name){
  /// needs to call UTS addObj
};

void otUbiTrackClient::deregAll(){
  /// needs to call UTS remObj for all local objects
};

void otUbiTrackClient::resetHandler() {
  handler = &defaultHandler;
};

void otUbiTrackClient::setHandler( UbiApp * application ) {
  if (application == NULL)
    resetHandler;
  else
    handler = application;
};



UbiTrackClient * makeUbiTrackClient(){
  static otUbiTrackClient * singleton = 0;
  
  if (singleton == 0)
    singleton = new otUbiTrackClient();
  
  return singleton;
  
};

Ubj * makeUbj(){
  return new _Ubj;
}

