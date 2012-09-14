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
/** source file for TCPModule Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: TCPModule.cxx 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4244)
#pragma warning(disable:4786)
#endif
#include <cstdlib>
#include <string>

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Time_Value.h>
#include <OpenTracker/network/TCPModule.h>

#include <iostream>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>


#define SERVER_DATA_PRECISION 100
#define SERVER_DATA_SHIFT     1000


#ifndef OT_NO_NETWORK_SUPPORT


namespace ot {

	OT_MODULE_REGISTER_FUNC(TCPModule){
		OT_MODULE_REGISTRATION_DEFAULT(TCPModule , "TCPConfig" );
	}


    // destuctor, clears up any sinks

    TCPModule::~TCPModule(){
        for(NodeVector::iterator it = sinks.begin(); it != sinks.end(); it++ ){
            delete (*it);
        }
        sinks.clear();
    }

    // creates new nodes for the parser

    Node * TCPModule::createNode( const std::string& name,  const StringTable& attributes){
        if( name.compare("TCPSink") == 0 ){
            int station, posFlag, rotFlag, buttonFlag,timeFlag;
            int num = sscanf( attributes.get("station").c_str()," %i", 
                              &station );
            if( num != 1 ){
                logPrintE("TCPModule : not a number in TCPSink station %s\n", attributes.get("station").c_str());
                return NULL;
            }
            if( attributes.get("position").compare("on") == 0 )
                posFlag = 1;
            else
                posFlag = 0;
            if( attributes.get("orientation").compare("on") == 0 )
                rotFlag = 1;
            else
                rotFlag = 0;
            if( attributes.get("button").compare("on") == 0 )
                buttonFlag = 1;
            else
                buttonFlag = 0;
            if( attributes.get("time").compare("on") == 0 )
                timeFlag = 1;
            else
                timeFlag = 0;
            TCPSink * sink = new TCPSink( station, posFlag, rotFlag, buttonFlag, timeFlag );
            sinks.push_back( sink );
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:TCPSink for station %d created.\n"), station));
            return sink;
        }
        return NULL;
    }

    // checks the stations for changes and transmits changes to clients

    void TCPModule::pullEvent(){
        char buffer[8*sizeof(int)+2*sizeof(short unsigned)+sizeof(double)];
        short unsigned protocol;
        int index;

        if( isInitialized() == 1 && connections.size() != 0 && sinks.size() != 0 ){
            for( NodeVector::iterator it = sinks.begin(); it != sinks.end(); it ++ ){
                TCPSink * sink = (TCPSink*) ((Node*) *it);
                Event & event = sink->event;
                if( sink->changed == 1 ){
                    protocol = sink->posFlag | (sink->orientFlag << 1) | (sink->buttonFlag << 2) |
                        (sink->timeFlag << 3);
                    index = 0;
                    // write converted data into buffer
                    *(int*)(&buffer[index]) = htonl(sink->station);
                    index += sizeof(int);
                    *(short unsigned *)(&buffer[index]) = htons(protocol);
                    index += sizeof(short unsigned);
                    if( sink->posFlag == 1 ){
                        *(int*)(&buffer[index]) = htonl(*(int*)(&event.getPosition()[0]));             
                        index += sizeof(int);
                        *(int*)(&buffer[index]) = htonl(*(int*)(&event.getPosition()[1])); 
                        index += sizeof(int);
                        *(int*)(&buffer[index]) = htonl(*(int*)(&event.getPosition()[2]));
                        index += sizeof(int);
                    }
                    if( sink->orientFlag == 1 ){
                        *(int*)(&buffer[index]) = htonl(*(int*)(&event.getOrientation()[0]));
                        index += sizeof(int);
                        *(int*)(&buffer[index]) = htonl(*(int*)(&event.getOrientation()[1]));
                        index += sizeof(int);
                        *(int*)(&buffer[index]) = htonl(*(int*)(&event.getOrientation()[2]));
                        index += sizeof(int);
                        *(int*)(&buffer[index]) = htonl(*(int*)(&event.getOrientation()[3]));
                        index += sizeof(int);
                    }
                    if( sink->buttonFlag == 1 ){
                        *(short int*)(&buffer[index]) = htons(event.getButton());
                        index += sizeof(short int);
                    }
                    if( sink->timeFlag == 1 )
                    {
                        *(int*)(&buffer[index]) = htonl((*(long*)(&event.time))&&0xffffffff);
                    
                        // FIXXXME : What are the following lines good for?
                        // Since they do not make any sense to me and the cause a warning I commented them out
                        // *(int*)(&buffer[index]) = htonl((*(long*)(&state.time))>>32);
                        // index += sizeof(int);
		     
                        // FIXXXME: Once again - I don't see why this code is needed
                        // at least on a 32 bit machine it adds only a value of 0 to the buffer
                        // strange ...
                        //index += sizeof(int);
                        //*(int*)(&buffer[index]) = htonl((*(long*)(&event.time))>>32);
                        //index += sizeof(int);
                    }
                    // send to all connections
                    lockLoop();
                    for( std::vector<ACE_SOCK_Stream *>::iterator conit = connections.begin(); conit != connections.end(); conit ++ ){
                        // FIXME: send could be blocking on slow connections !
                        if( (*conit)->send_n( buffer, index, 0 ) != index ){
                            (*conit)->close();
                            delete (*conit);
                            connections.erase( conit-- );
                            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:TCPModule : closed connection\n")));
                        }
                    }
                    unlockLoop();
                    sink->changed = 0;
                }
            }
        }
    }

    // tells the acceptor thread to stop

    void TCPModule::close(){
        if( isInitialized() == 1 ){
            running = 0;
        }
    }

    // sets the port the module is listening to and initializes module

    void TCPModule::init(StringTable& attributes,  ConfigNode * localTree){
        if(attributes.get("port").compare("") != 0 ){
            int num = sscanf(  attributes.get("port").c_str(), " %i", &port );
            if( num == 1 ){
                ThreadModule::init( attributes, localTree );
                ACE_DEBUG((LM_INFO, ACE_TEXT("ot:TCPModule listening to port %d\n"), port));
            }
        }
    }
    
    // starts the main loop

    void TCPModule::start(){
        if( isInitialized() == 1 ){
            ThreadModule::start();
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:TCPModule started !\n")));
        }
    }

    // the thread functions opens an acceptor socket and listens to new connections

    void TCPModule::run(){
        ACE_INET_Addr client, server( port );
        ACE_Time_Value timeout(10000); 
        char buffer[100];

        ACE_SOCK_Acceptor acceptor( server );
        running = 1;

        while(running){
            ACE_SOCK_Stream * stream = new ACE_SOCK_Stream();
            if(acceptor.accept(*stream, &client, &timeout) == -1){ 
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:TCPModule : error listening to socket\n")));
                continue; 
            } 
            client.addr_to_string( ACE_TEXT_CHAR_TO_TCHAR(buffer), 100 );
            logPrintE("TCPModule : new connection from %s\n", buffer);
            lockLoop();
            connections.push_back( stream );
            unlockLoop();
        }
        acceptor.close();
        for( std::vector<ACE_SOCK_Stream *>::iterator conit = connections.begin(); conit != connections.end(); conit ++ ){
            (*conit)->close();
            delete (*conit);
        }
        connections.clear();
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_NETWORK_SUPPORT")
#endif // OT_NO_NETWORK_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of TCPModule.cxx
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
