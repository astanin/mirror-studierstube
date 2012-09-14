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
/** source file for NetworkSinkModule module.
 *
 * @author Gerhard Reitmayr, Mathis Csisinko
 *
 * $Id: NetworkSinkModule.cxx 2116 2008-02-05 11:29:45Z sareika $
 * @file                                                                    */
/* ======================================================================== */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4244)
#endif
#include <cstdlib>
#include <string>
#include <algorithm>

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/Time_Value.h>
#include <ace/Unbounded_Set.h>
#include <ace/Thread.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>

#include <OpenTracker/network/NetworkSinkModule.h>

#include <iostream>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <OpenTracker/core/Context.h>
#ifndef OT_NO_NETWORK_SUPPORT




namespace ot {

    // function to register the module with the context
    OT_MODULE_REGISTER_FUNC(NetworkSinkModule){
        OT_MODULE_REGISTRATION_DEFAULT(NetworkSinkModule, "NetworkSinkConfig");
    }


    // definitions for the Network Data protocol
    const int positionQuaternion=1;
    const int positionAngles=2;
    const int positionMatrix=3;

    const int magicNum=0xbeef;
    const int revNum=0x0200;

    /** a simple struct to relate the address of a multicast group, a
     * network data buffer and a socket that is used to build the packets send to the
     * group, depending on the network interface to use.
     * All NetworkSink nodes point to one of these.
     */
    struct NetworkSender {
        FlexibleTrackerDataRecord data;
        char *dataBuffer;
        char *nextRecord;

        NetworkSender( const FlexibleTrackerDataRecord & data_ ) :
            data( data_ )
        {
        }
    };

    struct UdpSender: NetworkSender {
        unsigned short port;
        std::string nic;
        ACE_SOCK_Dgram socket;

        UdpSender( const FlexibleTrackerDataRecord & data_, unsigned short port_, const std::string & nic_ ) :
            NetworkSender( data_ ), port( port_ ), nic( nic_ )
        {}
    };

    struct MulticastSender: UdpSender {
        std::string group;
        ACE_INET_Addr address;

        MulticastSender( const FlexibleTrackerDataRecord & data_, const std::string & group_, unsigned short port_, const std::string & nic_ ) :
            UdpSender( data_, port_, nic_ ), group( group_ ), address( port, group.c_str() )
        {}
    };

    struct UnicastSender: UdpSender {
        ACE_Thread_Mutex mutex;
        int stop;
        ACE_Unbounded_Set<ACE_INET_Addr> addresses;

        UnicastSender( const FlexibleTrackerDataRecord & data_, unsigned short port_, const std::string & nic_ ) :
            UdpSender( data_, port_, nic_ ), stop(0)
        {}
    };

    /** simple functor to find the right multicast sender. */
    struct FindMulticastSender {
        std::string group;
        unsigned short port;
        std::string nic;

        FindMulticastSender( const std::string & group_, unsigned short & port_, const std::string & nic_ ) :
            group( group_ ), port( port_), nic( nic_ )
        {}

        bool operator()( const MulticastSender * other )
        {
            return (group.compare( other->group ) == 0
                    && port == other->port
                    && nic.compare( other->nic ) == 0 );
        }
    };

    /** simple functor to find the right unicast sender. */
    struct FindUnicastSender {
        unsigned short port;
        std::string nic;

        FindUnicastSender( unsigned short & port_, const std::string & nic_ ) :
            port( port_), nic( nic_ )
        {}

        bool operator()( const UnicastSender * other )
        {
            return (    port == other->port
                        && nic.compare( other->nic ) == 0 );
        }
    };

    NetworkSinkModule::NetworkSinkModule() : Module(), NodeFactory(), blockMulticast(false)
    {
    }

    // destructor cleans up any allocated memory
    NetworkSinkModule::~NetworkSinkModule()
    {
        for( MulticastSenderVector::iterator mc_it = multicasts.begin() ; mc_it != multicasts.end(); ++mc_it )
        {
            delete (*mc_it);
        }
        for( UnicastSenderVector::iterator uc_it = unicasts.begin() ; uc_it != unicasts.end(); ++uc_it )
        {
            delete (*uc_it);
        }
    }

    // initializes NetworkSinkModule
    void NetworkSinkModule::init(StringTable& attributes,  ConfigNode * localTree)
    {
        Module::init( attributes, localTree );
        if( attributes.containsKey("name"))
        {
            serverName = attributes.get("name");
        }
        else
        {
            serverName = "OpenTracker";
        }
        blockMulticast = attributes.containsKey("blockMode") && attributes.get("blockMode").compare("multicast") == 0;
    }

    // This method is called to construct a new Node.
    Node * NetworkSinkModule::createNode( const std::string& name,  const StringTable& attributes)
    {
		
        if( name.compare("NetworkSink") == 0 )
        {
			// initialize Network data buffer
            FlexibleTrackerDataRecord data;
            data.headerId = htons(magicNum);
            data.revNum = htons(revNum);
            data.maxStationNum = 0;
            data.numOfStations = 0;

            std::string name = attributes.get("name");
            int number;
            unsigned short port;
            int num = sscanf(attributes.get("number").c_str(), " %i", &number );
            if( num == 0 ){
                logPrintE("ot:Error in converting NetworkSink number !\n");
                return NULL;
            }
            num = sscanf(attributes.get("port").c_str(), " %hu", &port );
            if( num == 0 ){
                logPrintE("ot:Error in converting NetworkSink port number !\n");
                return NULL;
            }

            std::string nic = attributes.get("interface");
            if( attributes.get("mode").compare("multicast") == 0 )
            {
                std::string group = attributes.get("multicast-address");

                MulticastSenderVector::iterator it = std::find_if( multicasts.begin(), multicasts.end(), FindMulticastSender( group, port, nic ));
                MulticastSender * multicastData;
                if( multicasts.end() == it )
                {
                    multicastData = new MulticastSender( data, group, port, nic );
                    multicasts.push_back( multicastData );
                } else
                {
                    multicastData = *it;
                }
                // increase maximum of station numbers to suit the given number
                multicastData->data.maxStationNum = (multicastData->data.maxStationNum < number) ? (number) : (multicastData->data.maxStationNum);

                NetworkSink * sink = new NetworkSink( name, number, multicastData );
                nodes.push_back( sink );
                logPrintS("Built NetworkSink node %s .\n", name.c_str());
                return sink;
            }
            if( attributes.get("mode").compare("unicast") == 0 )
            {
                UnicastSenderVector::iterator it = std::find_if( unicasts.begin(), unicasts.end(), FindUnicastSender( port, nic ));
                UnicastSender * unicastData;
                if( unicasts.end() == it )
                {
                    unicastData = new UnicastSender( data, port, nic );
                    unicasts.push_back( unicastData );
                } else
                {
                    unicastData = *it;
                }
                // increase maximum of station numbers to suit the given number
                unicastData->data.maxStationNum = (unicastData->data.maxStationNum < number)?(number):(unicastData->data.maxStationNum);

                NetworkSink * sink = new NetworkSink( name, number, unicastData );
                nodes.push_back( sink );
                logPrintS("Built NetworkSink node %s .\n", name.c_str());
                return sink;
            }
        }
        return NULL;
    }

    // opens the network socket to use
    void NetworkSinkModule::start()
    {
        // only open a network connection if we actually have something to do
        if( nodes.size() > 0 )
        {
            // sets maxStationNum to network byte order
            for( MulticastSenderVector::iterator mc_it = multicasts.begin() ; mc_it != multicasts.end(); ++mc_it )
            {
                if(! blockMulticast)
                {
                    if( (*mc_it)->socket.open(ACE_Addr::sap_any) == -1 )
                    {
                        ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error opening socket in NetworkSinkModule !\n")));
                        exit(1);
                    }
                    if((*mc_it)->nic.compare("") != 0 )
                    {
                        (*mc_it)->socket.set_nic(ACE_TEXT_CHAR_TO_TCHAR((*mc_it)->nic.c_str()));
                    }
                }
                (*mc_it)->data.maxStationNum = htons((*mc_it)->data.maxStationNum);
            }
            for( UnicastSenderVector::iterator uc_it = unicasts.begin() ; uc_it != unicasts.end(); ++uc_it )
            {
                if( (*uc_it)->socket.open(ACE_INET_Addr((*uc_it)->port), ACE_PROTOCOL_FAMILY_INET,0,0) == -1 )
                {
                    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error opening socket in NetworkSinkModule !\n")));
                    exit(1);
                }
                if((*uc_it)->nic.compare("") != 0 )
                {
                    (*uc_it)->socket.set_nic(ACE_TEXT_CHAR_TO_TCHAR((*uc_it)->nic.c_str()));
                }
                (*uc_it)->data.maxStationNum = htons((*uc_it)->data.maxStationNum);
                ACE_Thread::spawn((ACE_THR_FUNC)NetworkSinkModule::runUnicastTransceiver, *uc_it );
            }
        }
        Module::start();
    }

    // closes the network connection
    void NetworkSinkModule::close()
    {
        for( MulticastSenderVector::iterator mc_it = multicasts.begin() ; mc_it != multicasts.end(); ++mc_it )
        {
            if( ! blockMulticast && (*mc_it)->socket.close() == -1 )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSinkModule !\n")));
            }
        }
        for( UnicastSenderVector::iterator uc_it = unicasts.begin() ; uc_it != unicasts.end(); ++uc_it )
        {
            ACE_Guard<ACE_Thread_Mutex> guard( (*uc_it)->mutex );
            (*uc_it)->stop = 1;
        }
    }

    // checks the NetworkSink nodes and sends any new data to the network
    void NetworkSinkModule::pullEvent()
    {
        if( nodes.size() == 0 )
            return;

        // clear the network buffers
        MulticastSenderVector::iterator mc_it;
        for( mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it )
        {
            (*mc_it)->data.numOfStations = 0;
            (*mc_it)->data.bufferLength = 0;
        }
        UnicastSenderVector::iterator uc_it;
        for( uc_it = unicasts.begin(); uc_it != unicasts.end(); ++uc_it )
        {
            (*uc_it)->data.numOfStations = 0;
            (*uc_it)->data.bufferLength = 0;
        }

        // for each NetworkSink node: copy data to sink's buffer
        unsigned int i;
        char** sinkBuffers = (char**)malloc(nodes.size() * sizeof(char*));
        ACE_INT32* sinkBufferLengths = (ACE_INT32*)malloc(nodes.size() * sizeof(ACE_INT32));

        for (i = 0; i < nodes.size(); i++)
        {
            NetworkSink *sink = nodes[i];
            sinkBufferLengths[i] = 0;

            if( sink->modified == 1 )
            {
                const std::string &eventStr = sink->event.serialize();
                ACE_INT32 si[3];
                char *index;

                // sink buffer length
                sinkBufferLengths[i] = 3 * sizeof(ACE_INT32)
                    + sink->stationName.length() + eventStr.length() + 2*sizeof(char);

                // header
                si[0] = ACE_HTONL((ACE_INT32)sink->stationNumber);
                si[1] = ACE_HTONL(sinkBufferLengths[i]);
                si[2] = ACE_HTONL((ACE_INT32)sink->stationName.length());

                // copy data to this sink's buffer
                sinkBuffers[i] = (char*)malloc(sinkBufferLengths[i]);
                index = sinkBuffers[i];

                memcpy(index, &si, 3 * sizeof(ACE_INT32));
                index += 3 * sizeof(ACE_INT32);
                memcpy(index, sink->stationName.c_str(), sink->stationName.length()+1);
                index += sink->stationName.length()+1;
                memcpy(index, eventStr.c_str(), eventStr.length()+1);
                //std::cerr << "sbl: " << sinkBufferLengths[i] << " - " << (int)((char*)(sinkBuffers[i]) - (char*)(index+eventStr.length()+1)) << std::endl;

                // add to bufferLength and numOfStations
                sink->networkSender->data.bufferLength += sinkBufferLengths[i];
                sink->networkSender->data.numOfStations += 1;
            }
        }

        // allocate memory for each networkSender
        for (mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it)
        {
            (*mc_it)->dataBuffer = (char*)malloc((*mc_it)->data.bufferLength);
            (*mc_it)->nextRecord = (*mc_it)->dataBuffer;
        }
        for (uc_it = unicasts.begin(); uc_it != unicasts.end(); uc_it++)
        {
            (*uc_it)->dataBuffer = (char*)malloc((*uc_it)->data.bufferLength);
            (*uc_it)->nextRecord = (*uc_it)->dataBuffer;
        }

        // add data of each network sink node to according networkSender
        for (i = 0; i < nodes.size(); i++)
        {
            NetworkSink *sink = nodes[i];
            if (sink->modified == 1)
            {
                memcpy(sink->networkSender->nextRecord, sinkBuffers[i], sinkBufferLengths[i]);
                sink->networkSender->nextRecord += sinkBufferLengths[i];
                free(sinkBuffers[i]);
            }
            sink->modified = 0;
        }
        free(sinkBuffers);
        free(sinkBufferLengths);

        // send any non-empty network data buffers
        for( mc_it = multicasts.begin(); mc_it != multicasts.end(); mc_it++ )
        {
            if( (*mc_it)->data.numOfStations > 0 )
            {
                (*mc_it)->data.numOfStations = htons( (*mc_it)->data.numOfStations );
                (*mc_it)->data.bufferLength = ACE_HTONL( (*mc_it)->data.bufferLength );
                short headerLength = sizeof(FlexibleTrackerDataRecord);
                ACE_INT32 bufferLength = ntohl((*mc_it)->data.bufferLength);

                // copy header and data to the same address
                ACE_INT32 sendBufferSize = (ACE_INT32)headerLength + bufferLength;
                char *sendBuffer = (char*)malloc(sendBufferSize);
                memcpy(sendBuffer, &(*mc_it)->data, headerLength);
                memcpy(sendBuffer + headerLength, (*mc_it)->dataBuffer, bufferLength);

                // send without blocking to avoid stalls in the mainloop, packet is thrown away !
                if (! blockMulticast && (*mc_it)->socket.send(sendBuffer, sendBufferSize, (*mc_it)->address, 0, &ACE_Time_Value::zero) < 0)
                {
                    logPrintE("NetworkSinkModule : Error sending Multicast packet for %s:%hu\n", (*mc_it)->address.get_host_name(), (*mc_it)->address.get_port_number());
                }
                free(sendBuffer);
            }
            free((*mc_it)->dataBuffer);
        }
        for( uc_it = unicasts.begin(); uc_it != unicasts.end(); uc_it++ )
        {
            if( (*uc_it)->data.numOfStations > 0 )
            {
                ACE_Guard<ACE_Thread_Mutex> guard( (*uc_it)->mutex );
                (*uc_it)->data.numOfStations = htons( (*uc_it)->data.numOfStations );
                (*uc_it)->data.bufferLength = ACE_HTONL( (*uc_it)->data.bufferLength );
                short headerLength = sizeof(FlexibleTrackerDataRecord);
                ACE_INT32 bufferLength = ntohl((*uc_it)->data.bufferLength);

                // copy header and data to the same address
                int xheadlen=0;
                ACE_INT32 sendBufferSize;
                if(((ACE_INT32)headerLength+bufferLength)>=MAXMSGLEN)
                {
                    sendBufferSize = (ACE_INT32)headerLength + bufferLength + sizeof(ACE_INT32);
                    xheadlen=sizeof(ACE_INT32);
                }
                else
                    sendBufferSize = (ACE_INT32)headerLength + bufferLength;
                char *sendBuffer = (char*)malloc(sendBufferSize);
                // adding length of the whole packet as first value
                if(xheadlen>0)
                    memcpy(sendBuffer, &sendBufferSize, sizeof(ACE_INT32));

                memcpy(sendBuffer + xheadlen, &(*uc_it)->data, headerLength);
                memcpy(sendBuffer + xheadlen + headerLength, (*uc_it)->dataBuffer, bufferLength);

                // send without blocking to avoid stalls in the mainloop, packet is thrown away !
                for( ACE_Unbounded_Set_Iterator<ACE_INET_Addr> it = (*uc_it)->addresses.begin() ; ! it.done(); it.advance() )
                {

                    //logPrintI("sending unicast packet\n");
                    int sentBytes=0;
                    char *tmpBuffer=sendBuffer;
                    int sendBytes;

                    while(sentBytes<sendBufferSize)
                    {
                        sendBytes=MAXMSGLEN;
                        if((sendBufferSize-sentBytes)<MAXMSGLEN)
                            sendBytes=sendBufferSize-sentBytes;
                        int rv=(*uc_it)->socket.send(tmpBuffer, sendBytes, *it, 0, &ACE_Time_Value::zero );
                        if(rv<0)
                        {
                            logPrintE("NetworkSinkModule : Error sending Unicast packet for %s:%hu\n", (*it).get_host_name(), (*it).get_port_number());
                        }
                        else
                        {
                            sentBytes+=rv;
                            tmpBuffer+=rv;
                        }
                    }
                }
                free(sendBuffer);
            }
            free((*uc_it)->dataBuffer);
        }
    }

    // Converts num floats to network byte order

    void NetworkSinkModule::convertFloatsHToNl(std::vector<float>& floats, float* result, int num)
    {
        int i;
        union
        {
            float f;
            long int l;
        } convert;

        for (i=0; i<num; i++)
        {
            convert.f = floats[i];
            convert.l = htonl(convert.l);    // Convert host to network byte order
            result[i] = convert.f;
        }
    }

    void NetworkSinkModule::runUnicastTransceiver( void * data )
    {
        UnicastSender * uc = (UnicastSender *) data;
        ACE_INET_Addr remoteAddr;
        ACE_Time_Value timeOut( 1, 0 );
        int retval;
        char command;
        while(1)
        {
            do
            {
                if((retval = uc->socket.recv( &command, sizeof( command ), remoteAddr, 0,
                                              &timeOut )) == -1 )
                {
                    if ((errno == ECONNRESET) && (! uc->addresses.find(remoteAddr))){
                        static int counter = 0;
                        command = 'L';
                        ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d Connection with %s was reset %d times, forcing leave command!\n"), errno, remoteAddr.get_host_addr(),counter));
                        retval = 1;
                        counter++;

                    }else if( errno != ETIME && errno != 0 )
                    {


                        ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d receiving data in NetworkSinkModule !\n"), errno));
                        exit( -1 );
                    }
                }
            } while( retval < 0 && uc->stop == 0);
            if( uc->stop != 0 )
                break;
            {
                ACE_Guard<ACE_Thread_Mutex> guard( uc->mutex );
                const char poll = 'P';
                const char leave = 'L';
                switch (command)
                {
                    case poll:
                        if( uc->addresses.find( remoteAddr ) )
                        {
                            uc->addresses.insert( remoteAddr );
                        }
                        else
                        {
                            //logPrintI("Client was already here!\n");
                            //exit(1);
                        }
                        break;
                    case leave:
                        if( !uc->addresses.find( remoteAddr ) )
                            uc->addresses.remove( remoteAddr );
                }
            }
        }
        if( uc->socket.close() == -1)
        {
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSinkModule !\n")));
        }
        ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping thread\n")));
    }

} // namespace ot



#else
#pragma message(">>> OT_NO_NETWORK_SUPPORT")
#endif // OT_NO_NETWORK_SUPPORT

/*
 * ------------------------------------------------------------
 *   End of NetworkSinkModule.cxx
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
