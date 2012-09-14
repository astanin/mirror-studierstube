#include <OpenTracker/network/PhantomListener.h>

#ifndef _PHANTOMLOCATIONLISTENER_H
#define _PHANTOMLOCATIONLISTENER_H

namespace ot {

 class OPENTRACKER_API PhantomLocationListener : public PhantomListener
    {
  public:
      PhantomLocationListener (const std::string& group) : PhantomListener(group) { 
      pid_node_mapping = new PidSourceMultiMap;
    };

    virtual ~PhantomLocationListener () {
      delete pid_node_mapping;
    };

    
    void addNode(PhantomMiddlewareSource* node) {
      PhantomLocationSource* location_source = dynamic_cast<PhantomLocationSource*>(node);
      lock();
      pid_node_mapping->insert(PidSourcePair(location_source->getPid(), location_source));
      unlock();
    };

    void removeNode(PhantomMiddlewareSource* node) {
      int pid;
      sscanf(node->get("pid").c_str(), " %i", &pid );
      lock();
      // Get the range of iterators which have the key "pid"
      std::pair<PidSourceMultiMapIterator, PidSourceMultiMapIterator> range_it = pid_node_mapping->equal_range(pid);
      PidSourceMultiMapIterator it = range_it.first;
      while (it != range_it.second) {
	PidSourceMultiMapIterator _it = it;
	it++;
	if (_it->second == node) {
	  pid_node_mapping->erase(_it);
	  delete (PhantomLocationSource*) node;
	}
      }
      unlock();
    }

    void pushEvent() {
      lock();
      for (PidSourceMultiMap::const_iterator pidsource_it = (*pid_node_mapping).begin(); pidsource_it != (*pid_node_mapping).end(); ++pidsource_it )
	{
	  pidsource_it->second->push();
	}
      unlock();
    }

    virtual int svc (void)
    {
      while (isStopping() == false) {
	Phantom::Utils::UCharMessageReader ucm;
	(*msr) >> ucm;
	
	unsigned char ver;
	short eid;
	unsigned char seq;
	long t1, t2;
	int discard;
	
	ucm >> ver >> eid >> seq >> t1 >> t2 >> discard;
	int pid;
	float x,y,z, theta;
	char source[32];
	std::string src;
	if (eid != 13 && eid !=14) {
	  try {
	    ucm >> pid >> x >> y >> z;
	    ucm >> theta;
	    if (eid == 24) {
	      ucm >> source;
	    }
	    src = source;
	    lock();
	    std::pair<PidSourceMultiMapIterator, PidSourceMultiMapIterator> range_it = pid_node_mapping->equal_range(pid);
	    for (PidSourceMultiMapIterator it = range_it.first; it != range_it.second; ++it) {
	      if (eid == (it->second)->getEventId()) {
		(it->second)->setEvent(x, y, z, theta, t1, t2, eid, source);
	      }
	    }
	    unlock();
	  } catch (PhantomException) {
	    logPrintE("Caught Phantom Exception: probably attempting to extract from empty buffer\n");
	  }
	}
      }
      return 0;
    };
    
 protected:

 private:
    PidSourceMultiMap* pid_node_mapping;
  };

}

#endif //_PHANTOMLOCATIONLISTENER_H
