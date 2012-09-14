#ifndef OSGPONG_UTCLIENT_HH_INCLUDED
#define OSGPONG_UTCLIENT_HH_INCLUDED

/*
 *  Represents an object, a unit of exchange between ubitrack server and client.
 *  Also used as exchange between ubitrack client and application. 
 *  The Ubitrack Object contains only key-value pairs.
 *
 */
class Ubj{
 public:
  virtual ~UbiObj(){}=0;
  bool hasKey(const char *)=0;
  const char * getValue(const char * )=0;
  void setValue(const char*, const char *)=0;
};

/*
 *  Represents an application that uses the ubitrack client to receive information
 *  about objects of interest. An application that wishes to use UbiTrack must provide
 *  a handler for 3 events as described by the UbiApp interface.
 *
 */

class UbiApp{
 public:
  virtual ~UbiApp(){}=0;
  int onAddObject(const char * id, Ubj *) =0;
  int onRemoveObject(const char * id) = 0;
  int onIsAlive() = 0;
};


/*
 *  Abstract class representing the AppManagement aspects of a UbiTrack client. 
 *  Any UbiTrackClient is required to implement application management.
 *  An application can register and deregister objects with the UbiTrack system, and 
 *  also provide an event handler for UniTrack events as described by the UbiApp interface
 */
class UbiTrackClientAppManager{
 public:
  virtual  UbiTrackClientAppManager(){}=0;
  /// Application management interface
  virtual const char * appRegisterObj(const char * appname, const char * type, Ubj *)=0;
  virtual void appDeregAll()=0;
  virtual void setHandler( UbiApp * application ) = 0;
  virtual void resetHandler() = 0;
};

/*
 *  Abstract class representing the UTS communication aspects of a UbiTrack client. 
 *  This interface is used by the UbiTrackServer to talk to the client, providing
 *  information about new objects, and old objects, and checking whether clients
 *  are still listening.
 */
class UbiTrackClientUTSSlave{
 public:
  virtual ~UbiTrackClientUTSSlave(){}=0;
  /// Server -> Client interface
  virtual void UTSaddObject(const char * id, const char *queryid , Ubj *) = 0;
  virtual void UTSremoveObject(const char * id) = 0;
  virtual void UTSeditObject(const char * id, Ubj *)=0;
  virtual bool isAlive()=0;  
};

/*
 * The UbiTrackClient is the main communication means between the application and 
 * the UbiTrack system. It allows an application to initialize the local ubitrack, register
 * its handlers and also to receive information about the objects of interest.
 * 
 */

class UbiTrackClient: public UbiTrackClientAppManager, public UbiTrackClientUTSSlave{
 public:
  virtual ~UbiTrackClient(){}=0;
  virtual void initialize()=0;
  virtual void finalize()=0;
};



#endif // OSGPONG_UTCLIENT_HH_INCLUDED
