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
 /**
  *
  * @author Markus Sareika
  * 
  * $Id: VirtualKeyModule.cxx
  * @file                                                                   */
 /* ======================================================================= */


#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/VirtualKeySource.h>
#include <OpenTracker/common/VirtualKeyModule.h>


#ifdef USE_VIRTUALKEYSOURCE


#include <opentracker\tool\OT_ACE_Log.h>

namespace ot {
	OT_MODULE_REGISTER_FUNC(VirtualKeyModule){
		OT_MODULE_REGISTRATION_DEFAULT(VirtualKeyModule, "VirtualKeyConfig");
	}

	VirtualKeyModule::VirtualKeyModule() : ThreadModule(), NodeFactory(),stop(0)
	{
	
	};

	// Destructor method
	VirtualKeyModule::~VirtualKeyModule()
	{
		nodes.clear();
	}

	// This method is called to construct a new Node.
	Node * VirtualKeyModule::createNode( const std::string& name, const StringTable& attributes)
	{
		if( name.compare("VirtualKeySource") == 0 )
		{       
			VirtualKeySource * source = new VirtualKeySource(this);
			if ( !attributes.get("virtualKeyCode").empty() ) 
				source->virtualKeyCode = (int)atof(attributes.get("virtualKeyCode").c_str());
			source->event.setConfidence( 1.0f );
			nodes.push_back( source );
			logPrintS("Built VirtualKeySource node\n");
			initialized = 1;
			return source;
		}
		// no node configured 
		return NULL;
	}

	// starts the ptu thread
	void VirtualKeyModule::start()
	{
		if( isInitialized() == 1 && !nodes.empty())
		{
			ThreadModule::start();
		}
	}

	// stops the thread
	void VirtualKeyModule::close()
	{
		// stop thread
		logPrintD("ARTDataTrackerModule::close() ... \n");
		lockLoop();
		stop = 1;
		unlockLoop();
        logPrintD("ARTDataTrackerModule::close() done. \n");
	}


	void VirtualKeyModule::run()
	{
		static int init = 0;

		if( init == 0 )
		{
			initialized = 1;
			init = 1;
		}
		while(1)
		{
			lockLoop();
            if( stop)
            {
                unlockLoop();
                break;
            }
           
			processLoop();
		}
		lockLoop();
		stop = 0;
		unlockLoop();
	}


	void VirtualKeyModule::pushEvent()
	{
		VirtualKeySource *source;

		if( isInitialized() == 1 )
		{   
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				source = (VirtualKeySource *) ((Node*)*it);     
				if( source->process )
				{
					source->push();
					source->process = false;
				}
			}
		}
	}

	void VirtualKeyModule::processLoop()
	{
		bool havekey = false;
		if( isInitialized() == 1 )
		{
			VirtualKeySource *source;
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				source = (VirtualKeySource *) ((Node*)*it);
#ifdef WIN32
				source->lock();
				if( GetAsyncKeyState(source->virtualKeyCode) )	// default pgup
				{
					//logPrintD("ButtonDown of: %i\n", source->virtualKeyCode);
					if( source->event.getButton() == 0 )
					{
						source->event.getButton() = 1;
						source->process = true;
						havekey = true;
					}
				}else{
					int buttonState = source->event.getButton();
					//printf("Button %i\n", buttonState);
					if( buttonState != 0 )
					{
						source->event.getButton() = 0;
						source->process = true;
						havekey = true;
					}
				}
				source->unlock();
#endif
			}
		}
		if (havekey && context != NULL)
		{
			if (context->doSynchronization())
			{
				context->dataSignal();
				context->consumedWait();
			}
		}
		// wait 1msec for now - this sets the update intervall to the system
		ACE_OS::sleep( ACE_Time_Value(0, 20000) );
	}
} // namespace ot

#endif //USE_VIRTUALKEYSOURCE
