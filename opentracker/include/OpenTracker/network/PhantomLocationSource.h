#include <OpenTracker/network/PhantomMiddlewareSource.h>

#ifndef _PHANTOMLOCATIONSOURCE_H
#define _PHANTOMLOCATIONSOURCE_H

namespace ot {

  class OPENTRACKER_API PhantomLocationSource : public PhantomMiddlewareSource
  {
    // Members
  public:
    int pid;
    short eid;
    std::string source;
    
    // Methods
  protected:
    /** constructor method,sets commend member
     * @param multicast_group_ the multicast group on which the node should
     * broadcast events */
  PhantomLocationSource( const char* multicast_group, int pid_, short eid_, const std::string& source_) :
    PhantomMiddlewareSource(multicast_group), 
      pid( pid_ ),
      eid( eid_ ),
      source( source_ ) { 
	logPrintI("PhantomLocationSource constructor\n");
      }
    
  PhantomLocationSource( const char* multicast_group, int pid_, short eid_) :
  PhantomMiddlewareSource(multicast_group), pid( pid_ ), eid( eid_ )
  {
    source = "";
  }
  
  int getEventId() {
    return eid;
  }
  
  int getPid() {
    return pid;
  }
  
  ~PhantomLocationSource() {
    logPrintI("PhantomLocationSource destructor\n");
  }
  private:
  
  public:
      void setEvent(float x, float y, float z, float theta, int t1, int t2, short event_id, char* event_source) {
	setEvent(x, y, z, theta, t1, t2, event_id);
      };
  
      void setEvent(float x, float y, float z, float alpha, int t1, int t2, short event_id) {
	std::vector<float> position(3);
	std::vector<float> orientation(4);
	position[0] = x; position[1] =  z; position[2] = -y;
	float theta    = 180.0f - alpha; 
	float thetaby2 = theta * MathUtils::GradToRad / 2.0;
	orientation[0] = 0.0;
	orientation[1] = sin(thetaby2);
	orientation[2] = 0.0; 
	orientation[3] = cos(thetaby2);
	lock();
	event.setPosition(position);
	event.setOrientation(orientation);
	event.setAttribute("pid", pid);
	event.time = (double) t1 + ((double) t2)/1000000.0;
	modified=true;
	unlock();
      };
      
      friend class PhantomLocationListener;
      friend class PhantomMiddlewareModule;
  };
  
  typedef std::multimap<int, PhantomLocationSource*> PidSourceMultiMap;
  typedef std::pair<int, PhantomLocationSource*> PidSourcePair;
  typedef std::map<std::string, PhantomLocationListener*> GroupLocationListenerMap;
  typedef PidSourceMultiMap::iterator PidSourceMultiMapIterator;
  typedef std::map<std::string, PidSourceMultiMap*> GroupMapping;
  typedef std::pair<std::string, PidSourceMultiMap*> GroupMappingPair; 
}

#endif //_PHANTOMLOCATIONSOURCE_H
