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
/** source file for UltraTrakModule module.
 *
 * @author Rainer Splechtna
 *
 * $Id: UltraTrakModule.cxx 2120 2008-02-07 15:19:59Z bornik $
 * @file                                                                    */
/* ======================================================================== */

// a trick to avoid warnings when ace includes the STL headers

#ifdef WIN32
#pragma warning(disable:4244)
#pragma warning(disable:4786)
#endif
#include <cstdlib>
#include <string>

#include <ace/INET_Addr.h>
#include <ace/Time_Value.h>

#include <OpenTracker/input/UltraTrakModule.h>

#include <iostream>

#include <ace/Log_Msg.h>


#ifndef OT_NO_ULTRATRACK_SUPPORT


namespace ot {

	
	OT_MODULE_REGISTER_FUNC(UltraTrakModule){
		OT_MODULE_REGISTRATION_DEFAULT(UltraTrakModule , "UltraTrakConfig" );
	}

    // constructor initializing the thread manager
    UltraTrakModule::UltraTrakModule() : ThreadModule(), NodeFactory(), stop(0)
    {

    }

    // destructor cleans up any allocated memory
    UltraTrakModule::~UltraTrakModule()
    {
        stations.clear();
    }

    // Converts num floats from Network byte order.
    void UltraTrakModule::convertFloatsNToHl(float* floats, float* result, int num)
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
            convert.l = ntohl(convert.l);  // Convert Network to host byte order
            result[i] = convert.f;
        }
    }

    // reads from the Ultratrak and parses Ultratrak packages
    void UltraTrakModule::run()
    {
        ACE_DEBUG((LM_INFO, ACE_TEXT("ot:starting ultratrak module thread\n")));

        ACE_INET_Addr local((u_short)port);
        ACE_INET_Addr remoteAddr;
        ACE_SOCK_Dgram socket = ACE_SOCK_Dgram(local);
        ACE_Time_Value timeOut( 1, 0 );
        int retval;

        while(1)
        {
            do
            {
                if((retval = socket.recv( &buffer, sizeof( buffer ), remoteAddr, 0,
                                          &timeOut )) == -1 )
                {
                    if( errno != ETIME && errno != 0 )
                    {
                        ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d receiving data in UltraTrakModule !\n"), errno));
                        exit( -1 );
                    }
                }
            } while( retval < 0 && stop == 0);
            if( stop != 0 )
                break;

            int frameNum,frameSize,headerSize,recordSize;
            sscanf(buffer.header,"%*s %d %*s %d %*s %d %*s %d\n",
                   &frameNum,&frameSize,&headerSize,&recordSize);
            int receivedStations = (frameSize-headerSize)/recordSize;

            for (int i=0; i<receivedStations; i++)
            {
                //compute station number
                int stationNumber=(buffer.stationData[i].boardNumber-'0')*2 +
                    buffer.stationData[i].stationNumber-'1';

                if( stationNumber >= 0 && stationNumber <= ultraTrakMaxUnits)
                {
                    // get source with same stationNumber
                    StationVector::iterator station;
                    for( station = stations.begin();
                         station != stations.end(); station++ )
                    {
                        if( (*station)->number == stationNumber )
                            break;
                    }

                    // if source with same stationNumber exists
                    if( station != stations.end())
                    {
                        // critical section start
                        (*station)->lock();

                        Event & event = (*station)->event;
                        // convert & store position
                        float eventPosition[3];
                        memcpy(eventPosition,buffer.stationData[i].position,3*sizeof(float));
                        copyA2V(eventPosition, 3, event.getPosition());
                        //convertFloatsNToHl(event.position,event.position,3);
                        correctData(event.getPosition(), positionMapping, invertPosition);
                        // convert & store orientation
                        float eventOrientation[4];
                        memcpy( eventOrientation, buffer.stationData[i].orientation, 4*sizeof(float));
                        copyA2V(eventOrientation, 4, event.getOrientation());
                        //convertFloatsNToHl( event.orientation, event.orientation, 4);
                        // UltraTrak delivers quaternions in w,x,y,z format, but
                        // OpenTracker supposes x,y,z,w. Change it here ???.
                        /*float help = event.getOrientation()[3];
                          for (int j=0; j<3; j++)
                          event.getOrientation()[(j+3)%4] = event.getOrientation()[j];
                          event.getOrientation()[2] = help;
                        */
                        correctData(event.getOrientation(), orientationMapping, invertOrientation);

                        (*station)->modified = 1;
                        event.timeStamp();

                        // end of critical section
                        (*station)->unlock();
                        if (context != NULL && context->doSynchronization())
                        {
                            context->dataSignal();
                            context->consumedWait();
                        }
                    }
                }
            }//for all received stations
        } // forever

        socket.close();
        ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping thread\n")));
    }


    //  constructs a new Node
    Node * UltraTrakModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("UltraTrakSource") == 0 )
        {
            int number;
            int num = sscanf(attributes.get("number").c_str(), " %i", &number );
            if( num == 0 ){
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting UltratrakSource number !\n")));
                return NULL;
            }

            StationVector::iterator it;
            for( it = stations.begin(); it != stations.end(); it++ )
            {

                if( (*it)->number == number )
                {
                    break;
                }
            }
            if( it != stations.end())
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Source with number %d exists allready\n"), number));
                return NULL;
            }

            UltraTrakSource * source = new UltraTrakSource;
            source->number = number;
            stations.push_back( source );
            logPrintI("Built UltratrakSource node.\n");

            return source;
        }
        return NULL;
    }


    // opens the sockets needed for communication and starts the receive thread
    void UltraTrakModule::start()
    {
        if (isInitialized() && !stations.empty())
            ThreadModule::start();
    }

    // closes the module and closes any communication sockets and stops thread
    void UltraTrakModule::close()
    {
        // stop thread
        lockLoop();
        stop = 1;
        unlockLoop();
    }

    // pushes event information into the tree
    void UltraTrakModule::pushEvent()
    {
      
    }

    void UltraTrakSource::pushEvent()
    {
        // critical section start

        lock();

        if(modified == 1 )
        {
            //(*it)->source->event = (*it)->event;
            modified = 0;
            updateObservers( event );        
        }

        unlock();

        // end of critical section
    }

    void UltraTrakSource::pullEvent()
    {
        // nothing to do
    }

    int UltraTrakModule::parseVector(const std::string & line, int * val )
    {
        int help[3];
        int num;
        num = sscanf( line.c_str()," %d %d %d", &help[0], &help[1], &help[2] );
        if( num != 3 )
        {
            return 1;
        }
        for (int i=0; i<3; i++)
            if (help[i] < 0 || help[i] > 2)
                return 1;

        val[0] = help[0];
        val[1] = help[1];
        val[2] = help[2];
        return 0;
    }


    void UltraTrakModule::correctData(std::vector<float> &d, int *mapping, int *inversion)
    {
        float h[3];
        int i;
        for(i=0; i<3; i++) h[i] = d[mapping[i]]*inversion[i];
        for(i=0; i<3; i++) d[i] = h[i];
    }


    void UltraTrakModule::initMappping(int *mapping)
    {
        for (int i=0; i<3; i++)
            mapping[i] = i;
    }

    void UltraTrakModule::initInversion(int *inversion)
    {
        for (int i=0; i<3; i++)
            inversion[i] = 0;
    }

    void UltraTrakModule::calcInversion(int *inversion)
    {
        for (int i=0; i<3; i++)
            inversion[i] = inversion[i] ? -1 : 1;
    }


    void UltraTrakModule::init(StringTable& attributes, ConfigNode * localTree)
    {
        ThreadModule::init( attributes, localTree );

        // Scannig port number from XML-File
        int num = sscanf(attributes.get("port").c_str(), " %i", &port );
        if( num == 0 )
        {
            port = 12345;
        }

        if( parseVector(attributes.get("positionMapping"), positionMapping ) != 0 )
        {
            logPrintE("Error parsing positionMapping !\n");
            initMappping(positionMapping);
        }
        if( parseVector(attributes.get("orientationMapping"), orientationMapping ) != 0 )
        {
            logPrintE("Error parsing orientationMapping !\n");
            initMappping(orientationMapping);
        }
        if( parseVector(attributes.get("invertPosition"), invertPosition ) != 0 )
        {
            logPrintE("Error parsing invertPosition !\n");
            initInversion(invertPosition);
        }
        calcInversion(invertPosition);
        if( parseVector(attributes.get("invertOrientation"), invertOrientation ) != 0 )
        {
            logPrintE("Error parsing invertOrientation!\n");
            initInversion(invertOrientation);
        }
        calcInversion(invertOrientation);
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_ULTRATRACK_SUPPORT")
#endif // OT_NO_ULTRATRACK_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of UltraTrakModule.cxx
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
