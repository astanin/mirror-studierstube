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
/** header file for TestSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: TestSource.h 760 2004-11-14 18:20:34Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section testsource TestSource
 * The TestSource node is a simple EventGenerator that fires in fixed intervals
 * standard events. The events can be customized to have other then the default 
 * values. 
 *
 * It also supports simulation of noisy data. The parameter @c noise defines the 
 * size of a uniform distribution used to perturb the given default position and
 * orientation. In addition to that it also defines the probability that
 * the orientation representation is using the negative representation.
 *
 * The node has the following elements :
 * @li @c frequency every freq. cycle it fires
 * @li @c offset starting after offset cycles
 * @li @c position position value of the event to fire as 3 floats
 * @li @c orientation orientation value of the event to fire as 4 floats representing a quaternion
 * @li @c button a 16 bit integer value representing the button states
 * @li @c confidence a float value in [0,1] to represent the confidence value
 * @li @c noise a float value > 0, if present will output noisy data for simulations
 *
 * An example element looks like this :
 * @verbatim
<PushCons name="Ubitrack.Context/Fred.EventChannel"/>@endverbatim
 */

#ifndef _PUSHCONS_H
#define _PUSHCONS_H

#include <OpenTracker/dllinclude.h>
#include <OpenTracker/core/Node.h>
#ifndef SWIG
#include <OpenTracker/skeletons/OT_CORBA.hh>
#include <ace/Thread_Mutex.h>
#include <cstdio>
#include <iostream>
#endif

/**
 * This class implements a PushCons source that has its Event set by a CORBA event channel
 * invocation and updates any EventObservers.
 * @ingroup network
 * @author Joseph Newman
 */

namespace ot {

class OPENTRACKER_API PushCons : public Node
{
// Members
public:
    /// the original event
    //Event event;

// Methods
protected:
    /** simple constructor, sets members to initial values
     */
 PushCons( ) : Node()
    {
      mu = new ACE_Thread_Mutex("pushcons");
      type = "PushCons";
    }

public:            
    virtual ~PushCons( ) {
      delete mu;
    }

    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    int isEventGenerator()
    {
        return 1;
    }

    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator. Note that the 
     * implementation of this method is in the file CORBAModule.cxx !
     */
    //    void push();

    void push(const CORBA::Any& data);
#ifndef SWIG
    void setEvent(const OT_CORBA::Event& new_event);
#endif
    friend class CORBAModule;
    
 private:
    ACE_Thread_Mutex* mu;

 public:
    void _lock() { mu->acquire(); };
    void _unlock() { mu->release(); };
    void disconnect_push_consumer ();
};

} // namespace ot

#endif
