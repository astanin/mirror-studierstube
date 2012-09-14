#include <OpenTracker/OpenTracker.h>

using namespace ot;



class _Ubj{
protected:
  StringTable attr;
 public:
  virtual ~UbiObj(){}=0;
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


class _UbiAppDefault{
 public:
  virtual ~UbiApp(){};
  int onAddObject(const char * id, Ubj *){};
  int onRemoveObject(const char * id) {};
  int onIsAlive() {return 1;};
};



class _UbiTrackClient: public UbiTrackClient{
public:

  typedef std::map<std::string, Ubj *> UbjDictionary;
protected:
  UbjDictionary remotes;
  UbjDictionary local_objs;
  UbiTrackClientAppManager * handler;
  static _UbiAppDefault defaultHandler;
public:
  _UbiTrackClient();
  virtual ~UbiTrackClient(){};

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


_UbiTrackClient:: _UbiTrackClient(){

};

_UbiTrackClient::~_UbiTrackClient(){

};

void _UbiTrackClient::addObject(const char * id, const char * queryid , Ubj * obj){
  handler->onAddObj(id, queryid, obj);
};


void _UbiTrackClient::removeObject(unsigned int reserverd1, unsigned int reserved2 ,StringTable * attributes){
  
};
 
bool _UbiTrackClient::isAlive(){
  return handler->isAlive();
};

void _UbiTrackClient::registerObj(const char * appname, const char * type, const char * name){
  /// needs to call UTS addObj
};

void _UbiTrackClient::deregAll(){
  /// needs to call UTS remObj for all local objects
};

void _UbiTrackClient::resetHandler() {
  handler = &defaultHandler;
};

void _UbiTrackClient::setHandler( UbiApp * application ) {
  if (application == NULL)
    resetHandler;
  else
    handler = application;
};

UbiTrackClient * makeUbiTrackClient(){
  static _UbiTrackClient * singleton = 0;
  
  if (singleton == 0)
    singleton = new _UbiTrackClient();
  
  return singleton;
  
};

Ubj * makeUbj(){
  return new _Ubj;
}

