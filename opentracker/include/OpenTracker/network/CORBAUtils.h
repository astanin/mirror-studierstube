/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** Header file for CORBAUtils class, a collection of common utility functions.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: CORBAUtils.h 1151 2006-06-13 12:31:43Z  $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _CORBAUTILS_H
#define _CORBAUTILS_H

#include <OpenTracker/dllinclude.h>

#ifndef SWIG
#include <OpenTracker/skeletons/OT_CORBA.hh>
#endif
#include <OpenTracker/core/Event.h>
#ifdef USE_OMNIEVENTS
#ifndef SWIG
#include <COS/CosEventComm.hh>
#include <COS/CosEventChannelAdmin.hh>
#include <OpenTracker/skeletons/OT_EventChannel.hh>
#endif
#endif //ENABLE_OMNIEVENTS

/**
 * This class implements some corba-related functions
 * @author Joseph Newman
 * @ingroup network
 */

namespace ot {

    class OPENTRACKER_API CORBAUtils
    {

    public:
#ifdef USE_OMNIEVENTS
        static void connectPushConsumer(const CosEventChannelAdmin::ProxyPushSupplier_var& proxy_supplier, const CosEventComm::PushConsumer_var& consumer_ref);

        //static void connectPushConsumer(CosEventChannelAdmin::ProxyPushSupplier_var proxy_supplier, OT_EventChannel::PushConsNode_var consumer_ref);
        
        static void disconnectPushConsumer(const CosEventChannelAdmin::ProxyPushSupplier_var& proxy_supplier);

        static CosEventChannelAdmin::ConsumerAdmin_var getConsumerAdmin(const CosEventChannelAdmin::EventChannel_var& channel);

        static CosEventChannelAdmin::ProxyPushSupplier_var getProxyPushSupplier(const CosEventChannelAdmin::ConsumerAdmin_var& consumer_admin);

        static CosEventChannelAdmin::ProxyPushConsumer_var getProxyPushConsumer(const CosEventChannelAdmin::SupplierAdmin_var& supplier_admin);

        static CosEventChannelAdmin::SupplierAdmin_var getSupplierAdmin(const CosEventChannelAdmin::EventChannel_var& channel);

        static void connectPushSupplier(const CosEventChannelAdmin::ProxyPushConsumer_var& proxy_consumer, const CosEventComm::PushSupplier_var& sptr);

        static void disconnectPushSupplier(const CosEventChannelAdmin::ProxyPushConsumer_var& proxy_consumer);
#endif //USE_OMNIEVENTS
        
        //        static void convertToCORBAEvent(Event& ot_event, OT_CORBA::Event& corba_event);

        //        static void convertFromCORBAEvent(Event& ot_event, const OT_CORBA::Event& corba_event);
        
        static CORBA::Object_ptr getObjectReference(const CORBA::ORB_var& orb, const CosNaming::NamingContextExt::StringName_var& string_name);
        
        static void bindObjectReferenceToName(const CORBA::ORB_var& orb, const CORBA::Object_var& obj, const CosNaming::NamingContextExt::StringName_var& string_name);
        
        static CosNaming::NamingContextExt_var getRootContext(const CORBA::ORB_var& orb);
        
        static CosNaming::NamingContext_var getContextFromName(const CosNaming::NamingContext_var& root_context, const CosNaming::Name_var& name);

        static CosNaming::NamingContext_var getContext(const CosNaming::NamingContext_var& root_context, CosNaming::Name_var& name);

        static PortableServer::ObjectId_var getObjectId(const CORBA::ORB_var& orb, const CosNaming::NamingContextExt::StringName_var& string_name);

        static void generateUniqueId(CORBA::Octet* k);
        static std::string generateUniqueId();
    };
            
} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of OSUtils.h
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
