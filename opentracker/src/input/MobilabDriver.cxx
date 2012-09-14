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
/** source file for MobilabDriver
 *
 * @author Alexander Bornik
 * 
 * $Id$
 *
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifndef EXCLUDE_MOBILAB_SUPPORT

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <cstdlib>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/Reactor.h>
#include <iostream>
#include <algorithm>
#include <memory>

#include <OpenTracker/input/MobilabDriver.h>
#include <OpenTracker/input/MobilabSource.h>
#include <OpenTracker/input/Mobilab_Handler.h>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#ifndef WIN32
#include <termios.h>
#endif

namespace ot {

    MobilabDriver::MobilabDriver(ACE_Reactor * reactor_, 
                                 MobilabModule * module_) :
        reactor( reactor_ ),        
        receiver( NULL ),
        module (module_),
        debugOn(true),
	samplecounter(0)

    {
        if (getDebug())
        {
            logPrintI("MobilabDriver::MobilabDriver()\n");
        }

	if( NULL == reactor )
	{   
            logPrintW("MobilabDriver: reactor is null!\n");
            reactor = ACE_Reactor::instance();
	}
    }

    MobilabDriver::~MobilabDriver()
    {
        if (getDebug())
        {
            logPrintI("MobilabDriver::~MobilabDriver()\n");
        }

	//if( receiver != NULL )
        //    close();

	listeners.clear();

    }

    int MobilabDriver::open(std::string device)
    {
        if (getDebug())
        {
            logPrintI("MobilabDriver::open(%s)\n", device.c_str());
        }

	int result=0;
	// open the serial port the the mobilab device

        receiver = new Mobilab_Handler( this );

	Mobilab_Connector mobilabconnect( reactor );
	result = mobilabconnect.connect( receiver, 
                                         ACE_DEV_Addr(ACE_TEXT_CHAR_TO_TCHAR(device.c_str())), 
                                         ACE_Synch_Options::defaults,
                                         (Mobilab_Connector::peer_addr_type&)Mobilab_Connector::ACE_PEER_CONNECTOR_ADDR_ANY,
                                         1 /*reuse*/,
                                         O_RDWR|
#ifndef WIN32
                                         O_NOCTTY|
#endif
                                         O_NONBLOCK);

	if( result == 0)
	{
            // set the appropriate parameters
            ACE_TTY_IO::Serial_Params params;		
            result = receiver->peer().control( ACE_TTY_IO::GETPARAMS, &params);
            /*
            logPrintI(" prev baudrate: %d\n", params.baudrate);
            logPrintI(" prev databits: %d\n", params.databits);
            logPrintI(" prev stopbits: %d\n", params.stopbits);
            logPrintI(" prev parityEN: %d\n", params.parityenb);
            logPrintI(" prev dtrDIS  : %d\n", params.dtrdisable);
            logPrintI(" prev rtsEN   : %d\n", params.rtsenb);
            logPrintI(" prev ctsEN   : %d\n", params.ctsenb);
            logPrintI(" prev dsrEN   : %d\n", params.dsrenb);
            logPrintI(" prev rcvEN   : %d\n", params.rcvenb);
            logPrintI(" prev modem   : %d\n", params.modem);
            logPrintI(" prev parity  : %s\n", params.paritymode);
            logPrintI(" prev rdminch : %d\n", params.readmincharacters);
            logPrintI(" prev rdtout  : %d\n", params.readtimeoutmsec);
            logPrintI(" prev xinEN   : %d\n", params.xinenb);
            logPrintI(" prev xoffLIM : %d\n", params.xofflim);
            logPrintI(" prev xonLIM  : %d\n", params.xonlim);
            logPrintI(" prev xoutEN  : %d\n", params.xoutenb);
            */

            params.baudrate = 57600;
            //params.baudrate = 38400;
            params.databits = 8;
            params.stopbits = 1;
            params.parityenb = false;

            params.dtrdisable = false;
            params.rtsenb = 1;
            params.ctsenb = false;
            params.dsrenb = false;
            params.readtimeoutmsec = 100;

            params.rcvenb = true;

            result = receiver->peer().control(ACE_TTY_IO::SETPARAMS, &params );

            if( result != 0 )
            {
                //receiver->destroy();
                //receiver = NULL;
                logPrintE("MobilabDriver port open failed: %s\n", 
                          device.c_str());
                return -1;
            }
            /*
            result = receiver->peer().control( ACE_TTY_IO::GETPARAMS, &params);
            logPrintI(" current baudrate: %d\n", params.baudrate);
            logPrintI(" current databits: %d\n", params.databits);
            logPrintI(" current stopbits: %d\n", params.stopbits);
            logPrintI(" current parity  : %d\n", params.parityenb);
            logPrintI(" current dtrDIS  : %d\n", params.dtrdisable);
            logPrintI(" current rtsEN   : %d\n", params.rtsenb);
            logPrintI(" current ctsEN   : %d\n", params.ctsenb);
            logPrintI(" current dsrEN   : %d\n", params.dsrenb);
            logPrintI(" current rcvEN   : %d\n", params.rcvenb);
            logPrintI(" current modem   : %d\n", params.modem);
            logPrintI(" current parity  : %s\n", params.paritymode);
            logPrintI(" current rdminch : %d\n", params.readmincharacters);
            logPrintI(" current rdtout  : %d\n", params.readtimeoutmsec);
            logPrintI(" current xinEN   : %d\n", params.xinenb);
            logPrintI(" current xoffLIM : %d\n", params.xofflim);
            logPrintI(" current xonLIM  : %d\n", params.xonlim);
            logPrintI(" current xoutEN  : %d\n", params.xoutenb);
            */

	}
        else
        {
            logPrintE("MobilabDriver connect to port %s failed (%d)!\n", 
                      device.c_str(), result);
            //receiver->destroy();
            return -1;
        }



        //ACE_OVERLAPPED ovl;

        /// initialize channels
        unsigned char send_channel_command[ 3 ];
        send_channel_command[ 0 ] = 'c';
        send_channel_command[ 1 ] = 0xff;
        send_channel_command[ 2 ] = ' ';    
        char send_transfer_command = 'a';
        char send_etransfer_command = 'e';
#ifndef WIN32
        usleep(200000);
#else
		Sleep(20);
#endif

#ifndef WIN32
        /* FIXXXME: On Linux this needs to be done to get the Mobilab device to
           to work properly
        */

        ACE_HANDLE porthandle = receiver->peer().get_handle();
        struct termios devpar;
        memset(&devpar, 0x0, sizeof(devpar));
        
        // read device parameters
        if (!tcgetattr(porthandle, &devpar) == -1)
        {
            logPrintW("Parameter query failed!");
        }

        // mask out some bits ...
        devpar.c_cflag &= 0x0000fffff;

        // write modified parameters
        tcflush(porthandle, TCIFLUSH);
        tcsendbreak(porthandle,0);
        tcsetattr(porthandle, TCSANOW, &devpar);
#endif
          
        result = receiver->peer().send(send_channel_command, 
                                       sizeof(send_channel_command));
                
        if (result != sizeof(send_channel_command))
        {
            logPrintE("MobilabDriver device initialization failed (%d)\n",
                      result);
            //receiver->destroy();
            return -1;
        }
        

        char response = 0x00;
        result = receiver->peer().recv(&response, sizeof(response));
        if (result != sizeof(response))
        {
            logPrintE("MobilabDriver got no response from device (%d)\n", 
                      result);
            //receiver->destroy();
            return -1;
        }

        if (response != 0x63)
        {
            logPrintW("received unexpected response '%x'\n", response);
            receiver->peer().send(&send_etransfer_command, 
                                  sizeof(send_etransfer_command));

            int trials = 0;
            int maxtrials = 10000;

            while (result !=1 || response != 0x63 )
            {            

                result = receiver->peer().send(send_channel_command, 
                                               sizeof(send_channel_command));
                result = receiver->peer().recv(&response, sizeof(response));

                trials++;
                if (trials > maxtrials) break;
            }
            if (response != 0x63)
            {
                logPrintE("received unexpected response '%x' even trying to setup %d times\n", response, trials);
                //receiver->destroy();
                return -1;
            }
            else
            {
                logPrintI(" -> problem seems to be resolved!\n");
            }
        }
        else 
        {            
            //logPrintI("device response '%x'\n", response);
        }
        
        /// initiate data transfer

        //int writeicount = write(porthandle, &send_transfer_command, 1);
        //logPrintI("Write count: %d\n", writeicount);

        
        //int readcount = read(porthandle, &result, 1);
        //logPrintI("Read count: %d\n", readcount);

        result = receiver->peer().send(&send_transfer_command, 
                                       sizeof(send_transfer_command));

        if (result != sizeof(send_transfer_command))
        {
            logPrintE("MobilabDriver device invocation failed (%d)\n",
                      result);
            //receiver->destroy();
            return -1;
        }
        else
        {
            result = 0;
        }

	return result;
    }

    void MobilabDriver::close()
    {
        logPrintI("MobilabDriver::close()\n");

        if (receiver !=NULL)
        {
            sendStopTransferCommand();
        }

        if ( reactor != NULL)
        {       
            reactor->end_reactor_event_loop();
        }

	if( receiver != NULL )
	{		                       
            receiver->destroy();
            //receiver = NULL;
	}

        /*if ( reactor != NULL)
        {       
            reactor->close();
        }
        */

    }

    int MobilabDriver::sendStopTransferCommand() const
    {
        /// stop data transfer
        int result = 0;
        char send_stoptransfer_command = 'b';
        result = receiver->peer().send(&send_stoptransfer_command, 1);
        if (result != 1)
        {
            logPrintW("MobilabDriver device stopping failed(%d)\n",
                      result);
        } 
        else
        {
            logPrintI("MobilabDriver successfully stoppend device!\n");
        }
        return result;
    }

    void MobilabDriver::addListener( MobilabListener * listener, 
                                     void * userData )
    {
        if (getDebug())
        {
            logPrintI(" MobilabDriver::addListener()\n");
        }
	listeners[listener] = userData;
    }

    void MobilabDriver::removeListener( MobilabListener * listener, 
                                        void * userData )
    {
        if (getDebug())
        {
            logPrintI(" MobilabDriver::removeListener()\n");
        }

	std::map<MobilabListener *, void *>::iterator it = 
            listeners.find( listener );

	if( it != listeners.end())
            listeners.erase( listener );
    }


    void MobilabDriver::newSample( const short * sample )
    {  
        //logPrintI("MobilabDriver::newSample\n");
        /*
        if (debugOn)
	{
	    double sampletime = OSUtils::currentTime();
	    if (samplecounter == 0)
	    {
	        lastsampletime = sampletime;
	        samplecounter = 1;
	    }
	    else if (sampletime - lastsampletime > 10000.0)
	    {
	        samplecounter++;
	        logPrintI("Mobilab sample rate: %f Hz\n", samplecounter/10.0);
	        samplecounter = 0;
	  
	    }
	    else
	    {
	        samplecounter++;
	    }
	}
        */

        /// set the same time for all sources
        double tv = OSUtils::currentTime();

        /// traverse Source list and put the new data to each of them

        std::map<MobilabListener *, void *>::iterator it;
        for( it = listeners.begin(); it != listeners.end(); it++ )
        {
            MobilabSource *ms = dynamic_cast<MobilabSource*>(it->first);
            if (ms) // regular source -> only one value
            {                
                ms->newData(sample[ms->channel], tv);
            }
            else // The module needs the whole sample to log the data
            {
                MobilabModule *mm = dynamic_cast<MobilabModule*>(it->first);
                if (mm)
                {
                    //logPrintI("devstr: %s\n", mm->device.c_str());
                    mm->newData(sample, 8);  
                }
            }
        }

        // notify the graph traversal loop, that new data has arrived!

        if (module && module->getContext())
        {            
            if (module->getContext()->doSynchronization())
            {
                //logPrintI("MobilabDriver: have new data -> tell context\n");
                module->getContext()->dataSignal();
                //logPrintI("MobilabDriver: wait for processing ...\n");
                module->getContext()->consumedWait();
                //logPrintI("MobilabDriver: context finished processing\n");
            }
        }
    }

    void MobilabDriver::setDebug( bool debug )
    {
	debugOn = debug;
    }

} // namespace ot

#endif //EXCLUDE_MOBILAB_SUPPORT
/* 
 * ------------------------------------------------------------
 *   End of MobilabDriver.cxx
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
