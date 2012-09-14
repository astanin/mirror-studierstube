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
/** source file for VRPN module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: VRPNModule.cxx 1846 2007-05-23 08:16:47Z jfn $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/network/VRPNModule.h>
#include <OpenTracker/network/VRPNSource.h>
#include <OpenTracker/network/VRPNSink.h>

#ifdef USE_VRPN

#ifdef WIN32
#pragma comment(lib,"vrpn.lib")
#endif

// #define ACE_NTRACE 0

#include <ace/Log_Msg.h>

#include <vrpn_Connection.h>

using namespace std;

namespace ot {


	OT_MODULE_REGISTER_FUNC(VRPNModule){
		OT_MODULE_REGISTRATION_DEFAULT(VRPNModule , "VRPNConfig" );
	}

VRPNModule::VRPNModule() : 
    Module(), 
    NodeFactory()
{
}

VRPNModule::~VRPNModule()
{
    VRPNSourceVector::iterator it;
    for( it = sources.begin(); it != sources.end(); it++ )
    {
        delete (*it);
    }
    sources.clear();
    VRPNSinkVector::iterator it2;
    for( it2 = sinks.begin(); it2 != sinks.end(); it2++ )
    {
        delete (*it2);
    }
    sinks.clear();
}

void VRPNModule::init(StringTable & attributes,  ConfigNode * localTree)
{
    Module::init( attributes, localTree );
    if( attributes.get("port", &port ) != 1 )
        port = vrpn_DEFAULT_LISTEN_PORT_NO;
    ip = attributes.get("interface");
}

// called to construct a new Node.

ot::Node * VRPNModule::createNode( const std::string& name, StringTable& attributes)
{
    ACE_TRACE(ACE_TEXT("VRPNModule::createNode"));
    if( name.compare("VRPNSource") == 0 )
    {
        const string & nameVal = attributes.get("name");
        VRPNSource::Type typeVal = VRPNSource::TRACKER;;
        if( attributes.get("type").compare("button") == 0 )
            typeVal = VRPNSource::BUTTON;
        int stationVal = 0;
        attributes.get("station", &stationVal);
        VRPNSource * source = new VRPNSource;
        source->name = nameVal;
        source->type = typeVal;
        source->station = stationVal;
        sources.push_back( source );
        return source;
    } 
    else if( name.compare("VRPNSink") == 0 )  
    {
        const string & nameVal = attributes.get("name");
        VRPNSink::Type typeVal = VRPNSink::TRACKER;
        if( attributes.get("type").compare("button") == 0 )
            typeVal = VRPNSink::BUTTON;
        int stationVal = 0;
        attributes.get("station", &stationVal);
        VRPNSink * sink = new VRPNSink;
        sink->name = nameVal;
        sink->type = typeVal;
        sink->station = stationVal;
        sinks.push_back( sink );
        return sink;
    }
    return NULL;
}

void VRPNModule::start()
{
    ACE_TRACE(ACE_TEXT("VRPNModule::start"));
    VRPNSourceVector::iterator it;
    for( it = sources.begin(); it != sources.end(); it++ )
    {
        (*it)->start();
    }

    if( ip.compare("") == 0 )
        connection = new vrpn_Connection( port );
    else
        connection = new vrpn_Connection( port, NULL, NULL, ip.c_str() );

    VRPNSinkVector::iterator it2;
    for( it2 = sinks.begin(); it2 != sinks.end(); it2++ )
    {
        (*it2)->start(connection);
    }
}

void VRPNModule::pushEvent()
{
    VRPNSourceVector::iterator it;
    for( it = sources.begin(); it != sources.end(); it++ )
    {
        (*it)->mainloop();
    }
}

void VRPNModule::pullEvent()
{
    ACE_TRACE(ACE_TEXT("VRPNModule::pullEvent"));
    VRPNSinkVector::iterator it;
    for( it = sinks.begin(); it != sinks.end(); it++ )
    {
        (*it)->mainloop();
    }
    connection->mainloop();
}


} //namespace ot
#else
#ifdef WIN32
#pragma message(">>> no VRPN support")
#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of VRPNModule.cxx
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
