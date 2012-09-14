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
/** source file for NetworkSourceModule module.
 *
 * @author Gerhard Reitmayr, Mathis Csisinko
 *
 * $Id: NetworkSourceModule.cxx 2115 2008-02-05 10:09:17Z bornik $
 * @file                                                                    */
/* ======================================================================== */


#include <OpenTracker/OpenTracker.h>

#ifndef OT_NO_NETWORK_SUPPORT

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <cstdlib>
#include <string>
#include <algorithm>

//#include <ace/Thread_Manager.h>
#include <ace/Thread.h>
#include <ace/Synch.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/Signal.h>

#include <OpenTracker/network/NetworkSourceModule.h>

#include <iostream>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <OpenTracker/core/Context.h>
#include <OpenTracker/core/Configurator.h>

namespace ot {

    OT_MODULE_REGISTER_FUNC(NetworkSourceModule){
        OT_MODULE_REGISTRATION_DEFAULT(NetworkSourceModule, "NetworkSourceConfig");
    }

    
    struct Station
    {
        int number;
        Event event;
        int modified;
        NetworkSource * source;

        Station( const int number_, NetworkSource * source_ ) :
            number( number_ ), modified( 0 ), source( source_ )
        {};
    };
    

    typedef std::vector<Station *> StationVector;

    // definitions for the Network Data protocol
    const int positionQuaternion=1;
    const int positionAngles=2;
    const int positionMatrix=3;

    const int magicNum=0xbeef;
    const int revNum=0x0200;

    struct NetworkReceiver
    {
        /// Mutex to synchronize access to Station data
        ACE_Thread_Mutex mutex;
        /// data record for incoming package
        FlexibleTrackerDataRecord *dataRec;
        StationVector sources;
        int stop;
        bool running;
        Context* context;

        NetworkReceiver( ) :
            dataRec(NULL), stop(0), running(false), context(NULL)
        {}
    };

    struct UdpReceiver: NetworkReceiver
    {
        unsigned short port;

        UdpReceiver( unsigned short port_ ) :
            NetworkReceiver(), port( port_ )
        {}
    };

    struct MulticastReceiver: UdpReceiver
    {
        ACE_SOCK_Dgram_Mcast socket;
        std::string group;

        MulticastReceiver( const std::string & group_, unsigned short port_ ) :
            UdpReceiver( port_ ), group( group_ )
        {}
    };

    struct UnicastReceiver: UdpReceiver
    {
        ACE_SOCK_Dgram socket;
        std::string host;
        ACE_INET_Addr address;

        UnicastReceiver( const std::string & host_, unsigned short port_, const ACE_INET_Addr & address_ ) :
            UdpReceiver( port_ ), host( host_ ), address( address_ )
        {}
    };

    /** simple functor to find the right multicast receiver. */
    struct FindMulticastReceiver {
        std::string group;
        unsigned short port;

        FindMulticastReceiver( const std::string & group_, unsigned short & port_ ) :
            group( group_ ), port( port_)
        {}

        bool operator()( const MulticastReceiver * other )
        {
            return (    group.compare( other->group ) == 0
                        && port == other->port );
        }
    };

    /** simple functor to find the right unicast receiver. */
    struct FindUnicastReceiver {
        std::string host;
        unsigned short port;

        FindUnicastReceiver( const std::string & host_, unsigned short & port_ ) :
            host( host_ ), port( port_)
        {}

        bool operator()( const UnicastReceiver * other )
        {
            return (    host.compare( other->host ) == 0
                        && port == other->port );
        }
    };

    // constructor initializing the thread manager
    NetworkSourceModule::NetworkSourceModule() : Module(), NodeFactory()
    {
        //	manager = new ACE_Thread_Manager;
    }

    // destructor cleans up any allocated memory
    NetworkSourceModule::~NetworkSourceModule()
    {
        for( MulticastReceiverVector::iterator mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it )
        {
            for( StationVector::iterator st = (*mc_it)->sources.begin(); st != (*mc_it)->sources.end(); ++st )
            {
                delete (*st);
            }
            (*mc_it)->sources.clear();
            delete (*mc_it);
        }
        for( UnicastReceiverVector::iterator uc_it = unicasts.begin(); uc_it != unicasts.end(); ++uc_it )
        {
            for( StationVector::iterator st = (*uc_it)->sources.begin(); st != (*uc_it)->sources.end(); ++st )
            {
                delete (*st);
            }
            (*uc_it)->sources.clear();
            delete (*uc_it);
        }
    }

    // Converts num floats from network byte order.
    void NetworkSourceModule::convertFloatsNToHl(float* floats, float* result, int num)
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
            convert.l = ntohl(convert.l);  // Convert host to network byte order
            result[i] = convert.f;
        }
    }

    // reads from the network and parses network packages
    void NetworkSourceModule::runMulticastReceiver( void * data )
    {
        MulticastReceiver * rec = (MulticastReceiver *) data;
        ACE_INET_Addr remoteAddr;
        ACE_Time_Value timeOut( 1, 0 );
        int bytesRead;
        iovec io_vec;

        while(1)
        {
            do
            {
                bytesRead = rec->socket.recv(&io_vec, remoteAddr, 0, &timeOut);
                rec->dataRec = (FlexibleTrackerDataRecord*)io_vec.iov_base;

                if (errno == EINTR || errno == EAGAIN)
                {
		    continue;
                    //ACE_Sig_Action asa;
                    //ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d receiving data in NetworkSourceModule: Signal Flags: %d!\n"), errno, asa.flags()));
                    //asa.flags(asa.flags()|SA_RESTART);
                }
		
                else if (bytesRead == -1 && errno != ETIME && errno != 0 )
                {
                    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d receiving data in NetworkSourceModule!\n"), errno));
                    exit( -1 );
                }
            } while( bytesRead < 0 && rec->stop == 0);
            if( rec->stop != 0 )
                break;

            //logPrintI("(mc) procrec\n");
            processRecord( rec );

	    if (rec->context->doSynchronization())
	    {
	        //logPrintI("before locks (mc)...\n");
                rec->context->dataSignal();
                rec->context->consumedWait();	
	        //logPrintI("after locks (mc)\n");
	    }
        }
        if( rec->socket.close() == -1)
        {
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSourceModule !\n")));
        }

        rec->running = false;

        ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping thread\n")));
    }

    // reads from and writes to the network and parses network packages
    void NetworkSourceModule::runUnicastTransceiver( void * data )
    {
        UnicastReceiver * rec = (UnicastReceiver *) data;
        ACE_INET_Addr remoteAddr;
        ACE_Time_Value timeOut( 1, 0 );
        int bytesRead;
        iovec io_vec;
        const char poll = 'P';
        const char leave = 'L';

        while(1)
        {
            do
            {
                bytesRead = rec->socket.recv(&io_vec, remoteAddr, 0, &timeOut);
                if(bytesRead<MAXMSGLEN)
                    rec->dataRec = (FlexibleTrackerDataRecord*)io_vec.iov_base;
                else
                {
                    if(bytesRead>0)
                    {
                        ACE_INT32 *tmp = (ACE_INT32*)io_vec.iov_base;
                        ACE_INT32 bytes2read=*tmp-sizeof(ACE_INT32);
                        bytesRead-=sizeof(ACE_INT32);

                        rec->dataRec = (FlexibleTrackerDataRecord*)malloc(bytes2read);
                        memcpy(rec->dataRec,(char*)io_vec.iov_base+sizeof(ACE_INT32),bytesRead);


                        free(io_vec.iov_base);

                        int br;
                        while(bytesRead<bytes2read)
                        {
                            br = rec->socket.recv(&io_vec, remoteAddr, 0, &timeOut);
                            if(br>0)
                            {
                                memcpy((char*)rec->dataRec+bytesRead,io_vec.iov_base,br);
                                bytesRead+=br;
                            }
                        }
                    }
                }
                if(bytesRead == -1 && errno == ETIME)
                {
                    //logPrintI("nothing received -> reinitializing ...\n");
                    rec->socket.send( &poll, sizeof(poll), rec->address,0, &ACE_Time_Value::zero );
                }
            } while( bytesRead < 0 && rec->stop == 0);
            if( rec->stop != 0 )
                break;

            //logPrintI("(uc) procrec\n");
            processRecord( rec );

	    if (rec->context->doSynchronization())
	    {
	        //logPrintI("before locks (uc)...\n");
                rec->context->dataSignal();
                rec->context->consumedWait();
                //logPrintI("after locks (uc)");
	    }
        }
        rec->socket.send( &leave, sizeof(leave), rec->address,0, &ACE_Time_Value::zero );
        if( rec->socket.close() == -1)
        {
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSourceModule !\n")));
        }

        rec->running = false;

        ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping thread\n")));
    }

    // processes the received data buffer
    bool NetworkSourceModule::processRecord( NetworkReceiver * rec )
    {
        FlexibleTrackerDataRecord & buffer = *rec->dataRec;

        if( ((unsigned short) ntohs(buffer.headerId) != magicNum)||
            ((unsigned short) ntohs(buffer.revNum) != revNum))
        {
            delete [] rec->dataRec;
            return false;
        }

        short maxStationNum = ntohs(buffer.maxStationNum);
        short numOfStations = ntohs(buffer.numOfStations);
        char *stationData = (char*)(&buffer) + sizeof(FlexibleTrackerDataRecord);

        ACE_INT32 si[3];
        for( int cnt = 0; cnt < numOfStations; cnt ++ )
        {
            memcpy(si, stationData, 3*sizeof(ACE_INT32));
            ACE_INT32 stationNumber = ACE_NTOHL(si[0]);
            ACE_INT32 stationBufferLength = ACE_NTOHL(si[1]);
            ACE_INT32 stationNameLength = ACE_NTOHL(si[2]);
            ACE_INT32 msgSize = stationBufferLength - ((3 * sizeof(ACE_INT32)) + stationNameLength+1);
            if( stationNumber >= 0 && stationNumber <= maxStationNum)
            {
                StationVector::iterator station;
                // find station according to stationNumber
                for( station = rec->sources.begin(); station != rec->sources.end(); ++station )
                {
                    if( (*station)->number == stationNumber )
                        break;
                }
                if( station != rec->sources.end())
                {
                    Event & event = (*station)->event;

                    {
                        ACE_Guard<ACE_Thread_Mutex> guard( rec->mutex );
                        char *eventStr = stationData + (3 * sizeof(ACE_INT32)) + stationNameLength+1;
#ifndef WIN32
                        /* this is the right way to do it*/
                        std::stringstream ss;
                        ss.write(eventStr, msgSize);
                        ss >> std::noskipws >> event;
#else
                        /* VS8 has buggy allocator for stringstream that leaks memory. This is a workaround, but will most likely fail
                           for binary event attributes */
                        std::ostringstream os(std::ostringstream::out|std::ostringstream::binary);

                        os.write(eventStr, msgSize);

                        std::istringstream is(os.str(), std::istringstream::in|std::istringstream::binary);
                        is >> std::noskipws >> event;
#endif
                        (*station)->modified = 1;
                    }
                }
            }
            // goto next station
            stationData += stationBufferLength;
        }
        delete [] rec->dataRec;
        return true;
    }

    //  constructs a new Node
    Node * NetworkSourceModule::createNode( const std::string& name,  const StringTable& attributes)
    {
        if( name.compare("NetworkSource") == 0 )
        {
            int number;
            unsigned short port;
            int num = sscanf(attributes.get("number").c_str(), " %i", &number );
            if( num == 0 ){
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting NetworkSource number !\n")));
                return NULL;
            }
            num = sscanf(attributes.get("port").c_str(), " %hu", &port );
            if( num == 0 ){
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting NetworkSource port number !\n")));
                return NULL;
            }
            if( attributes.get("mode").compare("multicast") == 0 )
            {
                std::string group = attributes.get("multicast-address");
                NetworkSource * source = new NetworkSource;
                MulticastReceiverVector::iterator it = std::find_if( multicasts.begin(), multicasts.end(), FindMulticastReceiver( group, port ));
                MulticastReceiver * receiver;
                if( multicasts.end() == it )
                {
                    receiver = new MulticastReceiver( group, port );
                    source->setMulticastReceiver(receiver);
                    multicasts.push_back( receiver );
                    Station * newstat = new Station( number, source );
                    source->setStation(newstat);
                    receiver->sources.push_back(newstat );
                } else
                {
                    receiver = *it;
                    StationVector::iterator sit;
                    for(sit = receiver->sources.begin(); sit != receiver->sources.end();
                        ++sit )
                    {
                        if((*sit)->number == number )
                            break;
                    }
                    if( sit != receiver->sources.end())
                    {
                        logPrintE("There is already a node for station %d in group %s:%hu !\n", number, group.c_str(), port);
                        delete source;
                        return NULL;
                    }
                    source->setMulticastReceiver(receiver);
                    Station * newstat = new Station( number, source );
                    source->setStation(newstat);
                    receiver->sources.push_back(newstat);
                }
                logPrintI("Built NetworkSource node.\n");
                return source;
            }
            if( attributes.get("mode").compare("unicast") == 0 )
            {
                std::string address = attributes.get("address");
                NetworkSource * source = new NetworkSource;
                UnicastReceiverVector::iterator it = std::find_if( unicasts.begin(), unicasts.end(), FindUnicastReceiver( address, port ));
                UnicastReceiver * receiver;
                if( unicasts.end() == it )
                {
                    receiver = new UnicastReceiver( address, port, ACE_INET_Addr( port, address.c_str() ) );
                    unicasts.push_back( receiver );
                    source->setUnicastReceiver(receiver);
                    Station * newstat = new Station( number, source );
                    source->setStation(newstat);
                    receiver->sources.push_back(newstat);
                } else
                {
                    receiver = *it;
                    StationVector::iterator sit;
                    for(sit = receiver->sources.begin(); sit != receiver->sources.end();
                        ++sit )
                    {
                        if((*sit)->number == number )
                            break;
                    }
                    if( sit != receiver->sources.end())
                    {
                        logPrintE("There is already a node for station %d in %s:%hu !\n", number, address.c_str(), port);
                        delete source;
                        return NULL;
                    }
                    source->setUnicastReceiver(receiver);
                    Station * newstat = new Station( number, source );
                    source->setStation(newstat);
                    receiver->sources.push_back( newstat);
                }
                logPrintI("Built NetworkSource node.\n");
                return source;
            }
        }
        return NULL;
    }

    // opens the sockets needed for communication and starts the receive thread
    void NetworkSourceModule::start()
    {
        // start multicast receivers
        for( MulticastReceiverVector::iterator mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it )
        {
            if (!(*mc_it)->running)
            {
                (*mc_it)->context = context;
                (*mc_it)->socket.join( ACE_INET_Addr((*mc_it)->port, (*mc_it)->group.c_str()));
                ACE_Thread::spawn((ACE_THR_FUNC)NetworkSourceModule::runMulticastReceiver, *mc_it );
                (*mc_it)->running = true;
            }
        }

        // start unicast receivers
        for( UnicastReceiverVector::iterator uc_it = unicasts.begin(); uc_it != unicasts.end(); ++uc_it )
        {
            if (!(*uc_it)->running)
            {
                (*uc_it)->context = context;
                if( (*uc_it)->socket.open(ACE_Addr::sap_any) == -1 )
                {
                    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error opening socket in NetworkSourceModule !\n")));
                    exit(1);
                }
                ACE_Thread::spawn((ACE_THR_FUNC)NetworkSourceModule::runUnicastTransceiver, *uc_it );
                (*uc_it)->running = true;
            }
        }
    }

    // closes the module and closes any communication sockets and stops thread
    void NetworkSourceModule::close()
    {
        for( MulticastReceiverVector::iterator mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it )
        {
            ACE_Guard<ACE_Thread_Mutex> guard( (*mc_it)->mutex );
            (*mc_it)->stop = 1;
        }
        for( UnicastReceiverVector::iterator uc_it = unicasts.begin(); uc_it != unicasts.end(); ++uc_it )
        {
            ACE_Guard<ACE_Thread_Mutex> guard( (*uc_it)->mutex );
            (*uc_it)->stop = 1;
        }
    }

    // pushes event information into the tree
    void NetworkSourceModule::pushEvent()
    {
        // is done per node
    }

    void NetworkSource::setMulticastReceiver(MulticastReceiver * recv)
    {
        multicastreceiver = recv;
    }

    void NetworkSource::setUnicastReceiver(UnicastReceiver * recv)
    {
        unicastreceiver = recv;
    }

    void NetworkSource::setStation(Station * xstation)
    {
        station = xstation;
    }

    void NetworkSource::pushEvent()
    {
        bool updateObservers = false;
        
        if ( multicastreceiver )
        {
            // critical section
            {
                ACE_Guard<ACE_Thread_Mutex> guard( multicastreceiver->mutex );
                if(station->modified == 1 )
                {
                    event = station->event;
                    station
->modified = 0;
                    updateObservers = true;
                }
            }
            if (updateObservers)
                this->updateObservers(event );
        }
        else
        {
            bool updateObservers = false;
            // critical section
            {
                ACE_Guard<ACE_Thread_Mutex> guard( unicastreceiver->mutex );
                if(station->modified == 1 )
                {
                    event = station->event;
                    station->modified = 0;
                    updateObservers = true;
                }
            }
            if (updateObservers)
                this->updateObservers(event );
        }

    }

    void NetworkSource::pullEvent()
    {
        // nothing to do
    }
} // namespace ot


#else
#pragma message(">>> OT_NO_NETWORK_SUPPORT")
#endif // OT_NO_NETWORK_SUPPORT

/*
 * ------------------------------------------------------------
 *   End of NetworkSourceModule.cxx
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
