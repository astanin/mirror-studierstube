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
/** header file for Ubisense source node.
 *
 * @author Mathis Csisinko
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section ubisensesource UbisenseSource
 * The UbisenseSource node is a simple EventGenerator that inserts events 
 * by making use of the Ubisense API. See the @ref ubisensemodule for
 * information about how to configure the module as a whole. It has the
 * following elements :
 * @li @c object the name of any Ubisense object which should be tracked
 *
 * An example element looks like this :
 * @verbatim
 <UbisenseSource object="Person" />@endverbatim
*/

#ifndef _UBISENSESOURCE_H
#define _UBISENSESOURCE_H

#include <OpenTracker/OpenTracker.h>

#ifdef USE_UBISENSE
#ifdef WIN32

#include <OpenTracker/input/UbisenseModule.h>
#include "UClientAPI/location_client.h"
#include "UClientAPI/data_client.h"
#include "UClientAPI/name_client.h"
#include <iostream>

using namespace UClientAPI;

namespace ot {

    /** A UbisenseSource represents a single Ubisense object that is tracked by the 
     * Ubisense library.
     * @ingroup input
     */
    class OPENTRACKER_API UbisenseSource: public Node
    {
        friend class UbisenseModule;
        friend class UbisenseModule::WrappedDataClient;

        // methods
    protected:
        /** constructor method
         * @param object the Ubisense object to track
         * @param locationClient a Ubisense LocationClient instance
         * @param dataClient a Ubisense DataClient instance
         */
        UbisenseSource(const Object &object,const LocationClient &locationClient,DataClient &dataClient);

        /// destructor method
        virtual ~UbisenseSource();

        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator() { return 1; }

        const Object & getObject() const { return object; }

        bool calcEvent();

        Event & getEvent() { return event; }

        void setButtonEvent(unsigned short,double);

        // members
    private:
		unsigned short button;
		double lastTime,buttonTime;
		Event event;
		const Object object;
		const LocationClient &locationClient;
		DataClient &dataClient;
    };


    /*class MyNameClient : public NameClient 
    { 
    public: MyNameClient() { }
		virtual void on_insert(const Object& object, const String& name) { ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Insert %s name %s\n"),object.to_string().c_str(),name.c_str())); }
        virtual void on_update(const Object& object, const String& name) { ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Update %s name %s\n"),object.to_string().c_str(),name.c_str())); }
        virtual void on_remove(const Object& object, const String& name) { ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Remove %s name %s\n"),object.to_string().c_str(),name.c_str())); }
        virtual void on_commit() { ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Commit\n"))); }
        virtual void on_establish() { ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Establish\n"))); } 
    };*/

} // namespace ot

#endif
#endif  // USE_UBISENSE

#endif

/* 
 * ------------------------------------------------------------
 *   End of UbisenseSource.h
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
