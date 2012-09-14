/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institut for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** header file for PhantomMiddlewareSink Node.
  *
  * @author Joseph Newman
  *
  * $Id: PhantomMiddlewareSink.h 760 2004-11-14 18:20:34Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filesink PhantomMiddlewareSink 
 * The PhantomMiddlewareSink node is an object that 
 * takes an OpenTracker event.
 * It has the following attributes :
 * @li @c group the group to send multicast Phantom events
 * @li @c pid the unique phantom id of the object
 * @li @c frequency the frequency at which OT events 
 * are converted to Phantom events
 *
 * An example element looks like this :
 * @verbatim
<PhantomMiddlewareSink group="225.0.0.12" pid="147" frequency="1">
@endverbatim
 */

#ifndef _PHANTOMMIDDLEWARESINK_H
#define _PHANTOMMIDDLEWARESINK_H

#ifndef SWIG
#include <cmath>
#endif

#include <OpenTracker/OpenTracker.h>
#ifndef SWIG
#include <OpenTracker/tool/OT_ACE_Log.h>
#include <phantom/MulticastSocket.hh>
#include <phantom/PhantomMessageV1.hh>
#endif

/**
 * This class implements a simple EventGenerator that passes any incoming events
 * on to the associated CORBA object.
 * @author Joseph Newman
 * @ingroup core
 */


namespace ot {

class PhantomMiddlewareModule;

class OPENTRACKER_API PhantomMiddlewareSink : public Node
{
// Members
public:
  /// Phantom Node (sink) object associated with this node.
  //OT_CORBA::Node_var corba_sink;
  /// frequency of updates
    int frequency;
    int cycle;
    int pid;
    unsigned char seq;
    short eid;
    std::string src;
    Phantom::Utils::MulticastSocketSender* mss;
    

// Methods
protected:
    /** constructor method,sets commend member
     * @param multicast_group_ the multicast group on which the node should
     * broadcast events
     * @param frequency_ the frequency at which setEvent should be called */
 PhantomMiddlewareSink( const char* multicast_group, int frequency_, int pid_, short eid_, const std::string& src_) :
      Node(), 
      frequency( frequency_ ),
      pid( pid_ ), 
      cycle ( 0 ),
      seq ( 0 ),
      eid ( eid_ )
      {
	mss = new Phantom::Utils::MulticastSocketSender(multicast_group);
	src = src_;
      }
    
      PhantomMiddlewareSink( const char* multicast_group, int frequency_, int pid_, short eid_):
      Node(), 
      frequency( frequency_ ),
      pid( pid_ ), 
      cycle ( 0 ),
      seq ( 0 ),
      eid ( eid_ )
	{
	mss = new Phantom::Utils::MulticastSocketSender(multicast_group);
	src = "";	  
	}

    virtual ~PhantomMiddlewareSink() {
      delete mss;
      logPrintE("PhantomMiddlewareSink destructor\n");
    }

public:
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
      return 1;
      //return 0;
    }

    int getEventId() {
      return eid;
    }
    
    /**
     * this method notifies the object that a new event was generated.
     * It writes the event value out to the file and passes it
     * on to its parent.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( Event& event, Node& generator)
    {
      cycle++;
      if ((cycle % frequency) == 0) {
	seq++;
	try {
	  Phantom::PhantomMessageV1 pm(eid, seq);
	  std::vector<float> position = event.getPosition();
	  std::vector<float> orientation = event.getOrientation();
	  float theta = 2.0f *atan2(orientation[1], orientation[3]);
	  float alpha = MathUtils::Pi - theta;
	  pm << pid;
	  pm << (float) position[0];
	  pm << (float) -position[2];
	  pm << (float) position[1];
	  pm << (float) (alpha / MathUtils::GradToRad);
	  //	  if (eid == 24) {
	  //	    pm << src;
	  //	  }
	  (*mss) << pm;
	}
	catch (...) {}
	event.setAttribute("pid", pid);
	updateObservers( event );
      }
    }
 
 friend class PhantomMiddlewareModule;
};

}

#endif
