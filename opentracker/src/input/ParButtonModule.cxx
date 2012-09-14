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
/** source file for ParButton module.
 *
 * @author Gerhard Reitmayr 
 *
 *****************************************************************
 *
 * ALF 9.Dec.2002:
 * some of the newer mainboards seem to lack pull-up resistors
 * which results in showing always a value of "0xFF" as data
 *
 * I corrected this by using external pull-ups of about 2kOhm
 * from the input pins to pin 14 (output Autofeed), which gets
 * initialized as HIGH. (This is a hack. Better would be using 
 * +5V from e.g. the PS/2 connector, but this means another 
 * connector, which is too much trouble)
 *
 * Try this hack, if you've got the same problems.
 *
 *****************************************************************
 *
 * $Id: ParButtonModule.cxx 2103 2007-12-19 16:39:50Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */



// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/ParButtonSource.h>
#include <OpenTracker/input/ParButtonModule.h>
#include <OpenTracker/tool/OT_ACE_Log.h>


// enable this define, if you want to use an alternative implementation of
// the parallel port access. You will also have to make sure to include the
// right libraries etc.
//#define _DLPORTIO

#include <ace/OS.h>

#ifdef WIN32
#include <windows.h>
#endif



#include <iostream>

#ifdef WIN32
#ifndef _DLPORTIO
#include <OpenTracker/misc/portio.h>
#else
#include "Dlportio.h"
#endif
#else
#ifndef __APPLE__
#include <sys/ioctl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#ifdef _SGI_SOURCE
#include <sys/plp.h> 
#else
#include <linux/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>
#endif
#endif
#endif


#ifndef OT_NO_PARBUTTON_SUPPORT

#ifndef __APPLE__

namespace ot {

	OT_MODULE_REGISTER_FUNC(ParButtonModule){
		OT_MODULE_REGISTRATION_DEFAULT(ParButtonModule ,"ParButtonConfig" );
	}


#ifdef WIN32
#ifndef _DLPORTIO
    static BOOL bPrivException = FALSE;

    LONG WINAPI HandlerExceptionFilter ( EXCEPTION_POINTERS *pExPtrs )
    {

        if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_PRIV_INSTRUCTION)
        {
            pExPtrs->ContextRecord->Eip ++; // Skip the OUT or IN instruction that caused the exception
            bPrivException = TRUE;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        else
            return EXCEPTION_CONTINUE_SEARCH;
    }
#endif
#endif


    ParButtonModule::ParButtonModule() : 
            ThreadModule(), 
            NodeFactory(),
            stop(false)
    {
    }

    ParButtonModule::~ParButtonModule()
    {
        std::map<std::string, std::pair<Node *, unsigned short> >::iterator it;

        for(  it = nodes.begin(); it != nodes.end(); it++ )
        {
#ifndef WIN32
#ifdef _SGI_SOURCE
            ::close(dynamic_cast<ParButtonSource*>((*it).second.first)->handle);
#else  // LINUX
            ioctl(dynamic_cast<ParButtonSource*>((*it).second.first)->handle, 
                  PPRELEASE);
            ::close(dynamic_cast<ParButtonSource*>((*it).second.first)->handle);
#endif
#endif
        }
        nodes.clear();
    }  

    // This method is called to construct a new Node

    Node * ParButtonModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("ParButtonSource") == 0)
        {
            std::string dev = attributes.get("dev");
            if( nodes.find( dev ) !=  nodes.end() )
            {
                logPrintW("ParButtonSource on port %s already defined!\n", dev.c_str() );
                return NULL;
            }

#ifdef WIN32
            UINT addr;
            if( sscanf( dev.c_str(), " %i", &addr ) != 1 )
            {
                logPrintW("ParButtonModule not an address %s\n", dev.c_str());
                return NULL;
            }
            // setting parallel port for input

#ifndef _DLPORTIO
 
            HANDLE hUserPort;

            hUserPort = CreateFileA("\\\\.\\UserPort", GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            CloseHandle(hUserPort); // Activate the driver
            Sleep(100); // We must make a process switch

            SetUnhandledExceptionFilter(HandlerExceptionFilter);
			
            bPrivException = FALSE;
            inportb(addr);  // Try to access the given port address

            if (bPrivException)
            {
                {
                    printf("	Privileged instruction exception has occured!\r\n\r\n"\
                           "To use this program under Windows NT or Windows 2000\r\n"\
                           "you need to install the driver 'UserPort.SYS' and grant\r\n"\
                           "access to the ports used by this program.");
                    exit(-1);
                }
            }
		
		
		
		
		
            outportb(addr, 0x00 );
            outportb(addr+2, 0x20); // set to byte output mode,
            // all control lines to HIGH
#else
            DlPortWritePortUchar(addr, 0x00);
            DlPortWritePortUchar(addr+2, 0x20); 
            // nothing to be done here
#endif

            ParButtonSource * source = new ParButtonSource( addr );

#else
#ifdef _SGI_SOURCE       
            int handle = open( dev.c_str(), O_RDWR | O_NDELAY  );
            if( handle < 0 )
            {
                logPrintE("ParButtonModule Error opening parallel port %s\n", dev.c_str());
                return NULL;
            }
            if(ioctl(handle, PLPIOMODE, PLP_BI) < 0) {
                ::close(handle);
                logPrintE("ParButtonModule Error setting centronics mode on %s\n", dev.c_str());
                return NULL;
            }
            if(ioctl(handle, PLPIOCREAD, 1) < 0) 
            {
                ::close(handle);
                logPrintE("ParButtonModule Error setting bidirectional mode on %s\n", dev.c_str());
                return NULL;
            }
            if(ioctl(handle, PLPIOCRTO, 1) < 0) 
            {
                ::close(handle);
                logPrintE("ParButtonModule Error timeout on %s\n", dev.c_str());
                return NULL;
            }
            ParButtonSource * source = new ParButtonSource((unsigned int) handle );
#else  // Linux
            int handle = open( dev.c_str(), O_RDWR | O_NDELAY );
            if( handle < 0 )
            {
                logPrintE("ParButtonModule Error opening parallel port %s\n", dev.c_str());
                return NULL;
            }
	
            if(ioctl(handle, PPCLAIM) < 0) 
            {
                logPrintE("ParButtonModule Error claiming port %s\n" , dev.c_str() );
                ::close(handle);
                return NULL;
            }

            unsigned char datadir=0x01;
  
            if (ioctl(handle, PPDATADIR, &datadir) < 0)
            {
                logPrintE("ParButtonModule Error setting datadir %s\n", dev.c_str());
                ::close(handle);
                return NULL;
            }
  
            ParButtonSource * source = new ParButtonSource((unsigned int) handle );
#endif
#endif
            nodes[dev].first = source;
            nodes[dev].second = 0x0000;
            logPrintI("Built ParButtonSource on %s\n", dev.c_str());
            return source;
        }
        return NULL;
    }

    void ParButtonModule::run()
    {
        std::map<std::string, std::pair<Node *, unsigned short> >::iterator it;

        while (1)
        {
            lockLoop();
            if( stop)
            {
                unlockLoop();
                break;
            }
            unlockLoop();  

            // read from parallel ports
            for (it = nodes.begin(); it != nodes.end(); it++)
            {
                unsigned short data;
#ifdef WIN32
                ParButtonSource * src = dynamic_cast<ParButtonSource*>
                    (it->second.first);
                
#ifndef _DLPORTIO
                data = (~inportb(src->handle )) & 0xff;
#else
                // just check for two buttons if using hardware hack
                //data = (~DlPortReadPortUchar(source->handle )) & 0x03;
                data = (~DlPortReadPortUchar(src->handle )) & 0xFF;
                
#endif //_DLPORTIO
                
#if 0
                printf("%d%d%d%d %d%d%d%d\n",
                       (data>>7)&1,
                       (data>>6)&1,
                       (data>>5)&1,
                       (data>>4)&1,
                       (data>>3)&1,
                       (data>>2)&1,
                       (data>>1)&1,
                       (data>>0)&1
                       );
#endif //0

                if( data != it->second.second )
                {
                    src->lock();
                    src->event.getButton() = data;
                    src->event.timeStamp();
                    it->second.second = data;
                    src->changed = 1;
                    src->unlock();                    
                }
#else //WIN32

#ifdef _SGI_SOURCE
                ParButtonSource * src = dynamic_cast<ParButtonSource*>
                    (it->second.first);
                if( read( src->handle, &data, 1 ) == 1 )
                {
                    event.getButton() = data;
                    event.timeStamp();
                    updateObservers( source->event );
                }
#else  // LINUX
                int cstatus;
                ParButtonSource * src = dynamic_cast<ParButtonSource*>
                    (it->second.first);
                cstatus = ioctl(src->handle, PPRDATA, &data);	
	
                if( (unsigned int)(~data) != it->second.second )
                {
                    src->lock();
                    src->event.getButton() = 0x00ff&(~data);
                    src->event.timeStamp();
                    it->second.second = data;
                    src->changed = 1;
                    src->unlock();                    

                }
#endif //_SGI_SOURCE
            
#endif //WIN32
            
            } // for loop
        
            // sleep for 5 milliseconds
            ACE_OS::sleep(ACE_Time_Value(0,5000));
        } // while loop
    }
    
    void ParButtonModule::start()
    {
	if( isInitialized() && !nodes.empty())
	    ThreadModule::start();
    }

    // closes the devices    
    
    void ParButtonModule::close()
    {
        lockLoop();
        stop = 1;
        unlockLoop();
    }

    // pushes events into the tracker tree

    void ParButtonModule::pushEvent()
    {
        // nothing to do
    }

} // namespace ot

#endif

#endif // OT_NO_PARBUTTON_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of ParButtonModule.cxx
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
