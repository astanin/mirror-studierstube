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
/** source file for GoGoModule module.
 *
 * @author Markus Sareika
 *
 * $Id: GoGoModule.cxx sareika $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/GoGoSinkSource.h>
#include <OpenTracker/input/GoGoModule.h>

#ifdef USE_GOGO

#include <stdio.h>
#if defined (WIN32) || defined (GCC3)
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include <opentracker\tool\OT_ACE_Log.h>
#include <opentracker\core\MathUtils.h>


namespace ot {

	OT_MODULE_REGISTER_FUNC(GoGoModule){
		OT_MODULE_REGISTRATION_DEFAULT(GoGoModule, "GoGoConfig");
	}
    // Destructor method
    GoGoModule::~GoGoModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.
    Node * GoGoModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("GoGoSinkSource") == 0 )
        {       
            GoGoSinkSource * source = new GoGoSinkSource;
            // add extra attributes
            source->event.addAttribute("float", "scalingFactor", "1");
            source->event.addAttribute("float", "cursorDistance", "1");

            // read default values from ot config 
            if (!attributes.get("translationWeight").empty()) {
                float tw = (float)atof(attributes.get("translationWeight").c_str());
                source->tWeightDefault = tw;
                source->tWeight = tw;
                printf ("default translation weight: %.2f\n", tw);
            }
            if (!attributes.get("rotationWeight").empty()) {
                float rw = (float)atof(attributes.get("rotationWeight").c_str());
                source->rWeightDefault = rw;
                source->rWeight = rw;
                printf ("default rotation weight: %.2f\n", rw);
            }
            source->event.setConfidence( 1.0f );
            nodes.push_back( source );
            logPrintS("Built GoGoSinkSource node\n");
            initialized = 1;
            return source;
        }
        if( name.compare("RelativeInput") == 0 ) 
        {
            // create just a pass-through node
            NodePort *np = new NodePort();
            np->name = name;
            return np;
        }
        if( name.compare("ViewerLocation") == 0 ) 
        {
            // create just a pass-through node
            NodePort *np = new NodePort();
            np->name = name;
            return np;
        }
        if( name.compare("GoGoDeviceKit") == 0 ) 
        {
            // create just a pass-through node
            NodePort *np = new NodePort();
            np->name = name;
            return np;
        }
		if( name.compare("SetAbsoluteLocation") == 0 ) 
		{
			// create just a pass-through node
			NodePort *np = new NodePort();
			np->name = name;
			return np;
		}

        return NULL;
    }

    // opens SpaceMouse library
    void GoGoModule::start()
    {
        if( isInitialized() == 1 && !nodes.empty())
        {
            ThreadModule::start();
        }
    }

    // closes SpaceMouse library
    void GoGoModule::close()
    {
        // stop thread
        lockLoop();
        stop = 1;
        unlockLoop();
    }


    // This is the method executed in its own thread and might not be needed.
    void GoGoModule::run()
    {
        static int init = 0;

        if( init == 0 )
        {
            initialized = 1;
            init = 1;
        }
        while(stop == 0)
        {
            processMessages();
        }
    }


    void GoGoModule::pushEvent()
    {
        GoGoSinkSource *source;

        if( isInitialized() == 1 )
        {   
            for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
            {
                source = (GoGoSinkSource *) ((Node*)*it);

                lockLoop();            
                if (source->changed == 1)
                {
                    source->event.setPosition(source->tmpEventPos);
                    source->event.setOrientation(source->tmpEventOri);
                    source->event.setButton(source->tmpEventBut);
                    source->event.setAttribute<float>("scalingFactor", source->tmpEventScalingFactor);
                    source->event.setAttribute<float>("cursorDistance", source->tmpEventCursorDistance);
					
                    source->changed = 0;
                    unlockLoop();        
                    source->push();
                }
                else
                    unlockLoop();
            }
        }
    }


    void GoGoModule::processMessages()
    {
        if( isInitialized() == 1 )
        {
            GoGoSinkSource *source;
            for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
            {
                source = (GoGoSinkSource *)((Node*)*it);
                if(source->newEvent)
                {
                    source->newEvent=false;
                    source->computeNewLocation();
                    source->changed = 1;
                }
                if(source->newKitEvent)
                {
                    source->newKitEvent=false;
                    source->computeGoGoNodeKitEvent();
                    source->changed = 1;
                }
				if(source->setAbsPos)
				{
					source->setAbsPos = false;
					source->setAbsoluteLocation();
					source->changed = 1;
				}
            }	
        }
        OSUtils::sleep(2);
    }



} // namespace ot

#endif //USE_GOGO

/* 
 * ------------------------------------------------------------
 *   End of GoGoModule.cxx
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
