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
/** header file for CORBASink Node.
  *
  * @author Joseph Newman
  *
  * $Id: CORBASink.h 760 2004-11-14 18:20:34Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filesink CORBASink 
 * The CORBASink node is a CORBA object that exposes a setEvent method.
 * It has the following attributes :
 * @li @c name the name to bind to in the CORBA NamingService
 * @li @c station the station number to record
 *
 * An example element looks like this :
 * @verbatim
<CORBASink name="CORBANodes.Context/Sink001.Object/">
@endverbatim
 */

#ifndef _CORBASINK_H
#define _CORBASINK_H

#include <OpenTracker/OpenTracker.h>
#ifndef SWIG
#include <OpenTracker/tool/OT_ACE_Log.h>
#include <OpenTracker/skeletons/OT_CORBA.hh>
#endif
#include <OpenTracker/network/CORBAUtils.h>
#include <OpenTracker/core/Context.h>

#ifdef USE_LIVE
//#include <OpenTracker/core/Module.h>
//#include <OpenTracker/network/CORBAModule.h>
#ifndef SWIG
#include <OpenTracker/skeletons/OTGraph.hh>
#include <ace/Thread_Mutex.h>
#endif
#endif

#ifndef SWIG
#include <ace/Guard_T.h>
#endif
/**
 * This class implements a simple EventGenerator that passes any incoming events
 * on to the associated CORBA object.
 * @author Joseph Newman
 * @ingroup core
 */


namespace ot {

  class CORBAModule;

class OPENTRACKER_API CORBASink : public Node
{
// Members
public:
  /// CORBA Node (sink) object associated with this node.
  //OT_CORBA::OTEntity_var corba_sink;
  /// frequency of updates
    int frequency;
    int cycle;
private:
    ACE_Thread_Mutex * refmutex;
    OT_CORBA::OTEntity_ptr entity;

// Methods
protected:
    CORBASink() :
      frequency( 1 ),
      cycle( 0 ) 
	{
	  type = "CORBASink";
	  entity = OT_CORBA::OTEntity::_nil();
	  refmutex = new ACE_Thread_Mutex; 
	};


 CORBASink( int frequency_ ) :
      Node(), 
        frequency( frequency_ ),
        cycle ( 0 )
	  {
	    refmutex = new ACE_Thread_Mutex;	    
	    type = "CORBASink";
	    entity = OT_CORBA::OTEntity::_nil();
	  }
      
    /** constructor method,sets commend member
     * @param corba_sink_ the corba sink object to call setEvent method on
     * @param frequency_ the frequency at which setEvent should be called */
      CORBASink(OT_CORBA::OTEntity_ptr entity_, int frequency_);
 public:
      virtual ~CORBASink() {
	// CORBASink destructor
	
	std::cout << "CORBASink destructor" << std::endl;
	//need to make sure that all node activities have ended before deleting!
	delete refmutex;
      }

    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
      return 1;
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
      //logPrintI("CORBASink::onEventGenerated %d %d\n", cycle, frequency);
      //ACE_Guard<ACE_Thread_Mutex> mutexlock(*mutex);
	if ((cycle++ % frequency) == 0) {
	  OT_CORBA::Event corba_event = event.getCORBAEvent();
	  //CORBAUtils::convertToCORBAEvent(event, corba_event);
	  try {
	    ACE_Guard<ACE_Thread_Mutex> mutexlock(*refmutex);
	    if (!CORBA::is_nil(entity)) {
	      entity->setEvent(corba_event);
	    }
	  }
	  catch (CORBA::COMM_FAILURE) {
	    std::cerr << "Caught CORBA::COMM_FAILURE" << std::endl;
	  }
	  catch (CORBA::TRANSIENT) {
	    std::cerr << "Caught CORBA::TRANSIENT" << std::endl;
	  }
	  updateObservers( event );
	}
    }
    
#ifdef USE_LIVE    
    virtual char* get_attribute(const char* _key);
    virtual void set_attribute(const char* _key, const char* _value);

    virtual OT_CORBA::OTEntity_ptr getEntity() {
      ACE_Guard<ACE_Thread_Mutex> mutexlock(*refmutex);
      return OT_CORBA::OTEntity::_duplicate(entity);
    }

    virtual void setEntity(OT_CORBA::OTEntity_ptr entity_);
#endif    // USE_LIVE
    friend class CORBAModule;
};

}

#endif
