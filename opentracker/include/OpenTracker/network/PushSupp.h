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
/** header file for PushSupp Node.
  *
  * @author Joseph Newman
  *
  * $Id: CORBASink.h 760 2004-11-14 18:20:34Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section eventchannel PushSupp
 * The PushSupp node is a CORBA object that exposes a setEvent method.
 * It has the following attributes :
 * @li @c name the name to bind to in the CORBA NamingService
 * @li @c station the station number to record
 *
 * An example element looks like this :
 * @verbatim
<PushSupp name="Ubitrack.Context/EventChannel/">
@endverbatim
 */

#ifndef _PUSHSUPP_H
#define _PUSHSUPP_H

#include <OpenTracker/OpenTracker.h>

#ifndef SWIG
#include <OpenTracker/tool/OT_ACE_Log.h>
#include <OpenTracker/skeletons/OT_CORBA.hh>
#endif

#include <OpenTracker/network/CORBAUtils.h>

#ifndef SWIG
#include <COS/CosEventComm.hh>
#include <COS/CosEventChannelAdmin.hh>
#endif

/**
 * This class implements a simple EventGenerator that passes any incoming events
 * on to the associated CORBA EventChannel object.
 * @author Joseph Newman
 * @ingroup network
 */


namespace ot {

class CORBAModule;

class Supplier_i : virtual public POA_CosEventComm::PushSupplier {
public:
  Supplier_i () {};
  void disconnect_push_supplier () { };
};

class OPENTRACKER_API PushSupp : public Node
{
// Members
public:
  /// Event channel object associated with this node.
  CosEventChannelAdmin::EventChannel_var channel;
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin;
  CosEventChannelAdmin::ProxyPushConsumer_var proxy_consumer;
  CosEventComm::PushSupplier_var sptr;
  Supplier_i* supplier;
  int cycle;
    

// Methods
protected:
    /** constructor method,sets commend member
     * @param corba_sink_ the corba sink object to call setEvent method on
     * @param frequency_ the frequency at which setEvent should be called */
    PushSupp( CosEventChannelAdmin::EventChannel_var channel_) :
        Node(), 
        channel( channel_ ),
        cycle ( 0 )
    {
      supplier = new Supplier_i();
      try {
	supplier_admin = CORBAUtils::getSupplierAdmin(channel);
      } catch (CORBA::INV_OBJREF) {
	LOG_ACE_ERROR("Object reference of event channel is internally malformed. Exiting....\n");
	exit(-1);
      }	
      proxy_consumer = CORBAUtils::getProxyPushConsumer(supplier_admin);
      CosEventComm::PushSupplier_var sptr = supplier->_this();
      CORBAUtils::connectPushSupplier(proxy_consumer, sptr);
    }
    virtual ~PushSupp() {
      CORBAUtils::disconnectPushSupplier(proxy_consumer);
      delete supplier;
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
      //OT_CORBA::Event* corba_event = new OT_CORBA::Event;
      //CORBAUtils::convertToCORBAEvent(event, *corba_event);
      OT_CORBA::Event corba_event = event.getCORBAEvent();
      try {
        CORBA::Any any;
	any <<= corba_event;
	proxy_consumer->push(any);
      }
      catch (CORBA::COMM_FAILURE) {
	//	logPrintE("Caught CORBA::COMM_FAILURE\n");
      }
      catch (CORBA::TRANSIENT) {
	//	logPrintE("Caught CORBA::TRANSIENT\n");
      }
      updateObservers( event );
    }
    
    friend class CORBAModule;
};

}

#endif
