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
/** source file for SpaceDeviceModule module.
  *
  * @author Michael Woegerbauer
  *
  * $Id: SpaceDeviceModule.cxx 1271 2006-07-18 19:15:28Z sareika $
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <ace/OS.h>

#include <OpenTracker/input/SpaceDeviceSource.h>
#include <OpenTracker/input/SpaceDeviceModule.h>

#ifdef USE_SPACEDEVICE

#include <cstdio>
#if defined (WIN32) || defined (GCC3)
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include <Windows.h>
#include <cmath>

/* SpaceWare specific includes and library */
#include <spwmacro.h>  /* Common macros used by SpaceWare functions. */
#include <si.h>        /* Required for any SpaceWare support within an app.*/
#include <siapp.h>     /* Required for siapp.lib symbols */

#pragma comment(lib,"siapp")


#include <opentracker\tool\OT_ACE_Log.h>
#include <opentracker\core\MathUtils.h>

namespace ot {

	
	OT_MODULE_REGISTER_FUNC(SpaceDeviceModule){
		OT_MODULE_REGISTRATION_DEFAULT(SpaceDeviceModule, "SpaceDeviceConfig");
	}
	HWND		SpaceDeviceModule::hWndSpaceDevice = NULL;
	SiHdl		devHdl;		/* Handle to Spaceball Device */
	SiOpenData	oData;		/* OS Independent data to open ball  */ 


	LRESULT FAR PASCAL WndSpaceMouseProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};

	// Destructor method
	SpaceDeviceModule::~SpaceDeviceModule()
	{
		nodes.clear();
	}

	// This method is called to construct a new Node.
	Node * SpaceDeviceModule::createNode( const std::string& name, const StringTable& attributes)
	{
		if( name.compare("SpaceDeviceSource") == 0 )
		{       
			SpaceDeviceSource * source = new SpaceDeviceSource;
			source->event.setConfidence( 1.0f );
			nodes.push_back( source );
			logPrintS("Built SpaceDeviceSource node\n");
			initialized = 1;
			return source;
		}
		return NULL;
	}

	// opens SpaceMouse library
	void SpaceDeviceModule::start()
	{
		if( isInitialized() == 1 && !nodes.empty())
		{
			ThreadModule::start();
		}
	}

	// closes SpaceMouse library
	void SpaceDeviceModule::close()
	{
		// stop thread
		lockLoop();
		stop = 1;
		unlockLoop();

		if( (isInitialized() == 1) && (devHdl != NULL)) 
		{
			/* should maybe be called even if devHdl == NULL */
			SiTerminate();  /* called to shut down the SpaceWare input library */
		}
	}


	// This is the method executed in its own thread. It polls the spacedevices.
	void SpaceDeviceModule::run()
	{
		WNDCLASS  wndclass ;
		static int init = 0;

		if( init == 0 )
		{
			/*init the SpaceWare input library */
			if (SiInitialize() == SPW_DLL_LOAD_ERROR)
			{
				logPrintE("SpaceDeviceModule Error Loading SIAPPDLL.DLL.\n");
				exit(1);
			}

			// Fill in window class structure with parameters that describe the message-only window.
			wndclass.style =         0;
			wndclass.lpfnWndProc =   WndSpaceMouseProc ;
			wndclass.cbClsExtra =    0;
			wndclass.cbWndExtra =    0; 
			wndclass.hInstance =     NULL;
			wndclass.hIcon =         NULL;
			wndclass.hCursor =       NULL;
			wndclass.hbrBackground = 0;
			wndclass.lpszMenuName =  NULL ;
			wndclass.lpszClassName = "dummyClassSpaceDeviceModule";
	 
			/* Register display window class */
			if ( RegisterClass(&wndclass) == 0) 
			{
				logPrintE("SpaceDeviceModule Could not register windowclass.\n");
				exit(1);
			}

			hWndSpaceDevice = CreateWindow(
				"dummyClassSpaceDeviceModule",
				NULL,
				NULL,
				0,
				0,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				NULL,
				NULL,
				NULL,
				NULL
			);
			
			if (hWndSpaceDevice == NULL)
			{
				logPrintE("SpaceDeviceModule Could not create message-only window.\n");
				exit(1);
			}

			SiOpenWinInit (&oData, hWndSpaceDevice);  /* init Win. platform specific data  */
			SiSetUiMode(devHdl, SI_UI_ALL_CONTROLS); /* Config SoftButton Win Display */

			/* open data, which will check for device type and return the device handle
			   to be used by this function */ 
			if ( (devHdl = SiOpen ("ot_spacedevice", SI_ANY_DEVICE, SI_NO_MASK,  
							 SI_EVENT, &oData)) == NULL)	
			{
				// could not open spacemouse device handle
				SiTerminate();  /* called to shut down the SpaceWare input library */
				logPrintE("Could not fetch device handle for SpaceMouse.\n");
				initialized = 0; 
			}

			initialized = 1;
			init = 1;
		}
		while(stop == 0)
		{
			processMessages();
		}
	}


	void SpaceDeviceModule::pushEvent()
	{
		SpaceDeviceSource *source;

		if( isInitialized() == 1 )
		{   
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				source = (SpaceDeviceSource *) ((Node*)*it);

				lockLoop();            
				if (source->changed == 1)
				{
					source->event = source->tmpEvent;

					source->changed = 0;
					unlockLoop();        
					source->push();
				}
				else
					unlockLoop();
			}
		}
	}


	// pushes events into the tracker tree.
	void SpaceDeviceModule::processMessages()
	{
		MSG            msg;      /* incoming message to be evaluated */
		BOOL           handled;  /* is message handled yet */ 
		SiSpwEvent     SpaceEvent;    /* SpaceWare SpaceEvent */ 
		SiGetEventData EData;    /* SpaceWare Event Data */

		int				num;
		unsigned short	newBut;

		float			newPosX=0;
		float			newPosY=0;
		float			newPosZ=0;
		float			newRotX=0;
		float			newRotY=0;
		float			newRotZ=0;
		
		if( isInitialized() == 1 )
		{
			/* as long as there are messages waiting */
			WaitMessage();
			while ( PeekMessage( &msg, hWndSpaceDevice, 0, 0, PM_REMOVE ) ) 
			{
				//ACE_LOG_INFO("successful!\n");
				handled = SPW_FALSE;

				/* init Window platform specific data for a call to SiGetEvent */
				SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);
	      
				/* check whether msg was a Spaceball event and process it */
				if (SiGetEvent (devHdl, 0, &EData, &SpaceEvent) == SI_IS_EVENT)
				{
					if (SpaceEvent.type == SI_MOTION_EVENT)
					{
						float q[4];
						/* process Spaceball motion event */     
						newPosX = (SpaceEvent.u.spwData.mData[SI_TX] / 330.0f);
						newPosY = (SpaceEvent.u.spwData.mData[SI_TY] / 330.0f);
						newPosZ = (-SpaceEvent.u.spwData.mData[SI_TZ] / 330.0f);

						newRotX = ((SpaceEvent.u.spwData.mData[SI_RX] / 330.0f)*SPW_PI/2.0f);
						newRotY = ((SpaceEvent.u.spwData.mData[SI_RY] / 330.0f)*SPW_PI/2.0f);
						newRotZ = ((SpaceEvent.u.spwData.mData[SI_RZ] / 330.0f)*SPW_PI/2.0f);

						for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
						{
							lockLoop();
    						SpaceDeviceSource * source = (SpaceDeviceSource *)((Node*)*it);

							source->tmpEvent.getPosition()[0] = newPosX;
							source->tmpEvent.getPosition()[1] = newPosY;
							source->tmpEvent.getPosition()[2] = newPosZ;
							MathUtils::eulerToQuaternion (newRotX, newRotY, newRotZ, q);
							source->tmpEvent.setOrientation(q);

							/*source->tmpEvent.getOrientation()[0];
							source->tmpEvent.getOrientation()[1];
							source->tmpEvent.getOrientation()[2];
							source->tmpEvent.getOrientation()[3];*/

							source->changed = 1;
							unlockLoop();
						}
					}
					if (SpaceEvent.type == SI_ZERO_EVENT)
					{
						// process SpaceDevice zero event 
						// Zero Event does nothing
					}
				
					if (SpaceEvent.type == SI_BUTTON_EVENT)
					{
						if ((num = SiButtonPressed (&SpaceEvent)) != SI_NO_BUTTON)	
						{
							/* process SpaceDevice button event */
							newBut = (unsigned short)pow(2.f, num - 1.f);
							if(newBut>265) 
							{
								newBut =0;
								continue;
							}
							for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
							{
								lockLoop();
								SpaceDeviceSource * source = (SpaceDeviceSource *)((Node*)*it);
								source->tmpEvent.getButton() |= newBut;
								source->changed = 1;
								unlockLoop();
							}
						}
						if ((num = SiButtonReleased (&SpaceEvent)) != SI_NO_BUTTON)	
						{
							/* process SpaceDevice button release event */
							newBut = (unsigned short)pow(2.f, num - 1.f);
							for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
							{
								lockLoop();
								SpaceDeviceSource * source = (SpaceDeviceSource *)((Node*)*it);
								source->tmpEvent.getButton() ^= newBut;
								source->changed = 1;
								unlockLoop();
							}
						}
					}
			        
					handled = SPW_TRUE;              /* spacedevice event handled */                 
					if (context != NULL)
					  {
					    if (context->doSynchronization())
					    {
					      context->dataSignal();
					      context->consumedBroadcast();
					    }
					  }

				}

				/* not a Spacedevice event, let windows handle it */
				if (handled == SPW_FALSE)
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}
			ACE_OS::sleep(ACE_Time_Value(0, 500));
		}
	}



} // namespace ot

#endif
