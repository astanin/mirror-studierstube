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
/** header file for PhantomMiddlewareSource base class for 
  *                 PhantomMiddleware Nodes.
  *
  * @author Joseph Newman
  *
  * $Id: PhantomMiddlewareSource.h 760 2004-11-14 18:20:34Z jfn $
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

#ifndef _PHANTOMMIDDLEWARESOURCE_H
#define _PHANTOMMIDDLEWARESOURCE_H

#ifndef SWIG
#include <cmath>
#endif

#include <OpenTracker/OpenTracker.h>
#ifndef SWIG
#include <OpenTracker/tool/OT_ACE_Log.h>
#include <ace/Thread_Mutex.h>
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

class OPENTRACKER_API PhantomMiddlewareSource : public Node
{
// Members
public:
  /// Phantom Node (sink) object associated with this node.
  /// the original event
  Event event;

  // Methods
protected:
    /** constructor method,sets commend member
     * @param multicast_group_ the multicast group on which the node should
     * broadcast events */
  PhantomMiddlewareSource( const char* multicast_group ) :
    Node()
      {
	mu = new ACE_Thread_Mutex("phantommiddlewaresource");
      }      
    bool modified;
    virtual ~PhantomMiddlewareSource() {
      delete mu;
      logPrintI("PhantomMiddlewareSource destructor\n");
    }

private:
    ACE_Thread_Mutex* mu;
    
 public:
    void lock() { mu->acquire(); };
    void unlock() { mu->release(); };
    bool isModified() {return modified;};

    void push()
    {
      lock();
      if (modified) {
	updateObservers( event );
	modified = false;
      } 
      unlock();
    }
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
      return 1;
    }
    
    friend class PhantomMiddlewareModule;
    friend class PhantomListener;
};



}

#endif
