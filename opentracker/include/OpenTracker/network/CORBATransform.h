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
/** header file for CORBATransform Node.
  *
  * @author Joseph Newman
  *
  * $Id: CORBATransform.h 760 2004-11-14 18:20:34Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filesink CORBATransform 
 * The CORBATransform node is a CORBA object that exposes a setEvent method.
 * It has the following attributes :
 * @li @c name the name to bind to in the CORBA NamingService
 * @li @c station the station number to record
 *
 * An example element looks like this :
 * @verbatim
<CORBATransform name="CORBANodes.Context/Sink001.Object/">
@endverbatim
 */

#ifndef _CORBATRANSFORM_H
#define _CORBATRANSFORM_H

#include <OpenTracker/OpenTracker.h>
#ifndef SWIG
#include <OpenTracker/tool/OT_ACE_Log.h>
#include <OpenTracker/skeletons/OT_CORBA.hh>
#endif
#include <OpenTracker/network/CORBAUtils.h>
/**
 * This class implements a simple EventGenerator that passes any incoming events
 * on to the associated CORBA object and gets an Event in return, which it passes
 * on to its observers.
 * @author Joseph Newman
 * @ingroup core
 */


namespace ot {

class CORBAModule;

class OPENTRACKER_API CORBATransform : public CORBASink
{
// Members
public:
  /// CORBA Node (sink) object associated with this node.
  OT_CORBA::TransformNode_var corba_transform;
  /// frequency of updates
    //    int frequency;
    //int cycle;
    

// Methods
protected:
    /** constructor method,sets commend member
     * @param corba_transform_ the corba sink object to call setEvent method on
     * @param frequency_ the frequency at which setEvent should be called */
    CORBATransform( OT_CORBA::TransformNode_var corba_transform_) :
      CORBASink(), 
      corba_transform( corba_transform_ )
	{
	  // empty constructor
	}
    virtual ~CORBATransform() {
      LOG_ACE_ERROR("CORBATransform destructor");
    }

public:
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
      logPrintI("CORBATransform::onEventGenerated\n");
      cycle++;
      //if ((cycle % frequency) == 0) {
      //Event new_event;
      //CORBAUtils::convertToCORBAEvent(event, corba_event);
      OT_CORBA::Event corba_event = event.getCORBAEvent();
      try {
	OT_CORBA::Event* new_corba_event = corba_transform->transformEvent(corba_event);
	//CORBAUtils::convertFromCORBAEvent(new_event, new_corba_event);
	Event new_event(*new_corba_event);
	updateObservers( new_event );
      }
      catch (CORBA::COMM_FAILURE) {
	std::cerr << "Caught CORBA::COMM_FAILURE" << std::endl;
      }
      catch (CORBA::TRANSIENT) {
	std::cerr << "Caught CORBA::TRANSIENT" << std::endl;
      }
      //      }
    }
    
    friend class CORBAModule;
};

}

#endif
