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
<CORBASource name="Ubitrack.Context/Fred.Source" frequency="10" />@endverbatim
 */

#ifndef _CORBASOURCE_H
#define _CORBASOURCE_H

#include "../dllinclude.h"

#include <OpenTracker/core/Node.h>
#ifndef SWIG
#include <OpenTracker/skeletons/OT_CORBA.hh>
#include <ace/Thread_Mutex.h>
#include <stdio.h>
#include <iostream>
#endif
/**
 * This class implements a CORBA source that has its Event set by remote
 * invocation and updates any EventObservers.
 * @ingroup network
 * @author Joseph Newman
 */

namespace ot {

    class OPENTRACKER_API CORBASource : public Node
    {
        // Members
    public:

        // Methods
    protected:
        /** simple constructor, sets members to initial values
         */
        // CORBASource( ) : Node(), modified(false)
        CORBASource( ) : Node()
        {
            newevent=false;
            context = NULL;
            mu = new ACE_Thread_Mutex("corbasource");
            type = "CORBASource";
        }

    public:            
        virtual ~CORBASource( ) {
            //delete mu;
        }

        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        int isEventGenerator()
        {
            return 1;
        }

        void setEvent(const OT_CORBA::Event& new_event);
        void setContext(Context *ctx) { context = ctx; };
        void pushEvent( );

        friend class CORBAModule;
    
    private:
        ACE_Thread_Mutex* mu;
        Context *context;
        //    bool modified;
        Event event;
        bool newevent;
        // public:
        //void _lock() { mu->acquire(); };
        //void _unlock() { mu->release(); };
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of CORBASource.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
