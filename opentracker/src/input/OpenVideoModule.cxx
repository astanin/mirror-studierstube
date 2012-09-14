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
 * ======================================================================= */
/** source file for ARToolKit interface module.
 *
 * @author Thomas Pintaric, Gerhard Reitmayr
 *
 * $Id: OpenVideoModule.cxx 900 2006-01-19 16:47:43Z spiczak $
 * @file                                                                   */
/* ======================================================================= */


// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/OpenVideoModule.h>


#ifdef USE_OPENVIDEO


#include <cassert>
#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>


#define ENABLE_VIDEOSINK

#include <openvideo/Manager.h>
#include <openvideo/nodes/VideoSink.h>
#include <openvideo/nodes/VideoSinkSubscriber.h>


namespace ot {

	OT_MODULE_REGISTER_FUNC(OpenVideoModule){
		OpenVideoModule * ovModule = new OpenVideoModule(context);
		context->addModule( "OpenVideoConfig", *ovModule );
	}

    class OpenTrackerVideoSinkSubscriber : public openvideo::VideoSinkSubscriber
    {
        public:
	    OpenTrackerVideoSinkSubscriber(Context* ctx) : context(ctx)
	    {
            firstFrame=true;
        }

	    void update(openvideo::State* curState)
	    {
            bufferSynchronizer.assign(curState->getCurrentBuffer());

            if(firstFrame)
            {
                if(openvideo::Buffer* buffer = bufferSynchronizer.getLocked())
                {
                    assert(context);
                    context->newVideoFrame(buffer->getPixels(), buffer->getWidth(), buffer->getHeight(), static_cast<ot::PIXEL_FORMAT>(buffer->getFormat()));
                    buffer->unlock();
                    firstFrame = false;
                }
            }
            else
            {
                if(openvideo::Buffer* buffer = bufferSynchronizer.getLocked())
                {
                    assert(context);
                    context->newVideoFrame(buffer->getPixels(), buffer->getWidth(), buffer->getHeight(), static_cast<ot::PIXEL_FORMAT>(buffer->getFormat()));
                    buffer->unlock();
                }
            }
        }
        
        
    	void initPixelFormats()
	    {
            pixelFormats.push_back(openvideo::FORMAT_R8G8B8);
            pixelFormats.push_back(openvideo::FORMAT_B8G8R8);
            pixelFormats.push_back(openvideo::FORMAT_R8G8B8X8);
            pixelFormats.push_back(openvideo::FORMAT_B8G8R8X8);
            pixelFormats.push_back(openvideo::FORMAT_L8);
	    }

        private:
	    Context* context;
        bool firstFrame;
        openvideo::BufferSynchronizer bufferSynchronizer;
    };


    // constructor

    OpenVideoModule::OpenVideoModule(Context* ctx) : ThreadModule()
    {
    	context = ctx;
	    ovManager = NULL;
	    videoSinkSubscriber = NULL;
    }

    // destructor clears any nodes
    OpenVideoModule::~OpenVideoModule()
    {
    }

    // opens the openvideo library and video system
    void OpenVideoModule::start()
    {
	    ovManager=openvideo::Manager::getInstance();
    	if(!ovManager->parseConfiguration(configFile.c_str()))
            return;

	    ovManager->initTraversal();

	    videoSinkSubscriber = new OpenTrackerVideoSinkSubscriber(context);
	    if(openvideo::VideoSink* sink = reinterpret_cast<openvideo::VideoSink*>(ovManager->getNode(ovSinkName.c_str())))
            sink->subscribe(videoSinkSubscriber);

        ThreadModule::start();
    }

    // closes the openvideo library

    void OpenVideoModule::close()
    {
        ovManager->stop();
    }

    // initializes the OpenVideo module
    void OpenVideoModule::init(StringTable& attributes, ConfigNode * localTree)
    {
        ThreadModule::init( attributes, localTree );
    	configFile = attributes.get("ov-config");
    	ovSinkName = attributes.get("ov-sink");
    }

    // the work method for the module thread
    void OpenVideoModule::run()
    {
    	ovManager->run();
    }


}  // namespace ot


#endif  // USE_OPENVIDEO

/* 
 * ------------------------------------------------------------
 *   End of OpenVideoModule.cxx
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
