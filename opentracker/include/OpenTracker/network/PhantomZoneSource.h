#include <OpenTracker/network/PhantomMiddlewareSource.h>

#ifndef _PHANTOMZONESOURCE_H
#define _PHANTOMZONESOURCE_H

namespace ot {

 class OPENTRACKER_API PhantomZoneSource : public PhantomMiddlewareSource
{
// Members
public:

  // Methods
protected:
    /** constructor method,sets commend member
     * @param multicast_group_ the multicast group on which the node should
     * broadcast events */
 PhantomZoneSource( const char* multicast_group, short _enter_id, short _exit_id, int _zid, int _oid) :
  PhantomMiddlewareSource(multicast_group), 
    enter_id( _enter_id ), 
    exit_id(  _exit_id ),
    zid( _zid ),
    oid( _oid ) { ; }

 PhantomZoneSource( int _zid, int _oid ) :
  PhantomMiddlewareSource("225.0.2.2"), 
    enter_id( 7 ), 
    exit_id( 8 ),
    zid( _zid ),
    oid( _oid ) { ; }

 PhantomZoneSource( const char* multicast_group, int _zid, int _oid ) :
  PhantomMiddlewareSource(multicast_group), 
    enter_id( 7 ), 
    exit_id( 8 ),
    zid( _zid ),
    oid( _oid ) { ; }

  virtual ~PhantomZoneSource() {
      logPrintI("PhantomZoneSource destructor\n");
    }
private:
    short enter_id, exit_id;
    int zid, oid;
    
 public:
    short getEnterId() {
      return enter_id;
    };
    short getExitId() {
      return exit_id;
    };

    int getZoneId() {
      return zid;
    }

    int getObjectId() {
      return oid;
    }

    void setEvent(short eid, double t1, double t2, int _oid, const char* spacename, const char* type) {
      lock();
      if (_oid == getObjectId())
	{
	  if (eid == getEnterId()) {
	    event.setAttribute("type", std::string("ZONE_ENTER"));
	  } else if (eid == getExitId()) {
	    event.setAttribute("type", std::string("ZONE_EXIT"));
	  } else {
	    unlock();
	    return; // not a compatible event id
	  }
	  event.setAttribute("zid", zid);
	  event.setAttribute("oid", oid);
	  event.setAttribute("spacename", std::string(spacename));
	  event.setAttribute("spacetype", std::string(type));
	  event.time = (double) t1 + ((double) t2)/1000000.0;
	  modified=true;
	}
	unlock();
    };


    friend class PhantomZoneListener;
    friend class PhantomMiddlewareModule;
};
  
  typedef std::multimap<int, PhantomZoneSource*> ZidSourceMultiMap;
  typedef std::pair<int, PhantomZoneSource*> ZidSourcePair;
  typedef std::map<std::string, PhantomZoneListener*> GroupZoneListenerMap;
  typedef ZidSourceMultiMap::iterator ZidSourceMultiMapIterator;
  typedef std::map<std::string, ZidSourceMultiMap*>  GroupZidMapping;
  typedef std::pair<std::string, ZidSourceMultiMap*> GroupZidMappingPair; 

}

#endif // _PHANTOMZONESOURCE_H
