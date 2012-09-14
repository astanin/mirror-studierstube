/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The header file for the Video class.
*
* @author Denis Kalkofen
*
* $Id: Video.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/video/Video.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/VideoUser.h>
#include <stb/kernel/Profiler.h>
#include <stb/base/OS.h>
#include <openvideo/Manager.h>
#include <stb/components/viewer/SoVideoBackground.h>

// this assumes that OpenVideo was compiled
// with the videosink option enabled...
//
#define ENABLE_VIDEOSINK


#include <openvideo/nodes/VideoSinkSubscriber.h>
#include <openvideo/nodes/VideoSink.h>
#include <openvideo/Manager.h>
#include <openvideo/nodes/GL_TEXTURE_2D_Sink.h>


#include <assert.h>
#ifndef __APPLE__
#include <GL/gl.h>			
#include <GL/glu.h>	
#else
#include <OpenGL/gl.h>			
#include <OpenGL/glu.h>	
#endif

CREATE_COMPONENT_FUNC(Video)


BEGIN_NAMESPACE_STB

Video* Video::instance=NULL;
// we have to do this in a separate class because both
// openvideo::Node and stb::Component defined 'bool init()'
// and the compiler will complain when deriving from both...
//
class Stb4VideoSinkSubscriber : public openvideo::VideoSinkSubscriber
{
public:
    stb::string *sinkName;

    Stb4VideoSinkSubscriber(Video* videoComp, stb::string *givenSinkName) : video(videoComp)
	{
        sinkName=givenSinkName;
		firstFrame = true;
	}

	void update(openvideo::State* curState)
	{
		bufferSynchronizer.assign(curState->getCurrentBuffer());

		if(firstFrame)
		{
			if(openvideo::Buffer* buffer = bufferSynchronizer.getLocked())
			{
				video->setVideoFormat(*buffer, sinkName);
				buffer->unlock();
				firstFrame = false;
			}
		}
		else
		{
			if(openvideo::Buffer* buffer = bufferSynchronizer.getLocked())
			{
				video->setNewVideoFrame(*buffer, sinkName);
				buffer->unlock();
			}
		}
	}


	openvideo::Buffer* getCurrentFrameLocked()  {  return bufferSynchronizer.getLocked();  }

	bool gotFirstFrame() const  {  return !firstFrame;  }

	void initPixelFormats()
	{
		pixelFormats.push_back(openvideo::FORMAT_R8G8B8);
		pixelFormats.push_back(openvideo::FORMAT_B8G8R8);
		pixelFormats.push_back(openvideo::FORMAT_R8G8B8X8);
		pixelFormats.push_back(openvideo::FORMAT_B8G8R8X8);
		pixelFormats.push_back(openvideo::FORMAT_L8);
		pixelFormats.push_back(openvideo::FORMAT_R5G6B5);
	}

private:
	Video* video;
	bool firstFrame;
	openvideo::BufferSynchronizer bufferSynchronizer;
};



Video::Video()
{
   configFile="";
   isGLContext=false;
   runSingleThreaded = false;
   ovInitialized = false;
   forceUpdate= false;
   paused=false;

   //videoSinkSubscriber = NULL;
   video_format = NULL;

   ovManager=openvideo::Manager::getInstance(); 

}

Video::~Video()
{
   //nil
}

/// Called before the application is destructed.
bool 
Video::init()
{
    stb::logPrintD("Stb:: init Video\n");
    if(isInit)
        return true;
    isInit=true;
	Video::instance=this;
    retrieveParameter();

    SoVideoBackground::initClass();

	// register so we get the kes_beforeRender() event
	//
	stb::Kernel::getInstance()->registerForKernelEvents(this);

	if(runSingleThreaded)
    {
		if (!initOpenVideo())
        {
            stb::logPrintE("Couldn't start Openvideo, Aborting.\n");
            stb::Kernel::getInstance()->stop();
        }
    }
	else
		start();


    return isInit;
}

void 
Video::setParameter(stb::string key, std::string value)
{
    if(key=="configFile")
    {
        configFile = value;        
    }
    else if((key.find("sinkName",0)!=string::npos)|(key.find("ovSinkName",0)!=string::npos))
    {// Deprecating the usage of ovSinkName in favor of sinkName. Mendez 20070614
        //ovSinkName = value;
        sinkNames.push_back(new stb::string(value));
    }
    else if(key=="single-threaded")
    {
        if(value=="true" || value=="TRUE")
            runSingleThreaded = true;
    }
    else if(key=="force-ot-traversal")
    {
        if(value=="true" || value=="TRUE")
            forceUpdate= true;
        else if (value=="false" || value=="FALSE")
            forceUpdate= false;
    }
}

/// Called before the application is destructed.
void 
Video::shutDown()
{

}


bool
Video::initOpenVideo()
{
    // Load the configuration file of Openvideo
	if(!ovManager->parseConfiguration(stb::Kernel::getInstance()->getConfig(configFile).c_str()))
		return false;

    // Initialize the traversal of openvideo's graph
	ovManager->initTraversal();

    // Check if we have at least one sink name
    if (sinkNames.size()<0)
    {
        stb::logPrintE("No sinkName provided\n");
        return false;
    }

	// Register all opevideo sinks. Note: All have to be valid names, otherwise, abort.
	// We assume they are all of VideoSink type
	Stb4VideoSinkSubscriber *tmpSubscriber;
    
    for (unsigned int i=0;i<sinkNames.size();i++)
    {
        tmpSubscriber = new Stb4VideoSinkSubscriber(this, sinkNames[i]);
	    if(openvideo::VideoSink* sink = reinterpret_cast<openvideo::VideoSink*>(ovManager->getNode(sinkNames[i]->c_str())))
        {
		    sink->subscribe(tmpSubscriber);
            videoSinkSubscribers.push_back(tmpSubscriber);
        }
	    else
        {
            stb::logPrintE("sinkNames '%s' does not exist\n",sinkNames[i]->c_str());
		    return false;
        }
    }

	ovInitialized = true;
	return true;
}


void
Video::run()
{
    if(!initOpenVideo())
    {
        stb::logPrintE("Couldn't start Openvideo, Aborting.\n");
        stb::Kernel::getInstance()->stop();
    }

    ovManager->run();
}


void
Video::vp_registerVideoUser(VideoUser* videouser)
{
	switch(videouser->vu_getUpdateMode())
	{
	case VideoUser::UPDATE_IMMEDIATELY:
		for(VideoUserVector::iterator it=videoUsersImmediate.begin(); it!=videoUsersImmediate.end(); it++)
			if(*it == videouser)
				return;
		videoUsersImmediate.push_back(videouser);
		break;


	case VideoUser::UPDATE_BEFORE_RENDER:
		for(VideoUserVector::iterator it=videoUsersBeforeRender.begin(); it!=videoUsersBeforeRender.end(); it++)
			if(*it == videouser)
				return;
		videoUsersBeforeRender.push_back(videouser);
		break;
	}


    // Commented out, we let setVideoFormat to do this for us. Mendez 20070614
	//if(video_format)
	//	videouser->vu_init(*video_format);
}


void
Video::vp_unregisterVideoUser(VideoUser* videouser)
{
	for(VideoUserVector::iterator it=videoUsersImmediate.begin(); it!=videoUsersImmediate.end(); it++)
		if(*it == videouser)
		{
			videoUsersImmediate.erase(it);
			return;
		}

	for(VideoUserVector::iterator it=videoUsersBeforeRender.begin(); it!=videoUsersBeforeRender.end(); it++)
		if(*it == videouser)
		{
			videoUsersBeforeRender.erase(it);
			return;
		}
}


void
Video::setVideoFormat(const openvideo::Buffer& format, stb::string *givenSinkName)
{
	video_format = new openvideo::Buffer;
	*video_format = format;

	// tell all already registered video users about this format
	//
	for(size_t i=0; i<videoUsersImmediate.size(); i++)
		videoUsersImmediate[i]->vu_init(*video_format, givenSinkName);

	for(size_t i=0; i<videoUsersBeforeRender.size(); i++)
		videoUsersBeforeRender[i]->vu_init(*video_format, givenSinkName);
}


void
Video::kes_beforeRender()
{
	if(!ovInitialized)
		return;

	if(runSingleThreaded)
	{
		STB_PROFILER_AUTOMEASURE(OPEN_VIDEO);
		assert(ovManager);
		ovManager->updateSingleThreaded();
	}

    for (unsigned int i=0;i<videoSinkSubscribers.size();i++)
    {
	    if(!videoSinkSubscribers[i] || !videoSinkSubscribers[i]->gotFirstFrame())
		    continue;

	    if(openvideo::Buffer* buffer = videoSinkSubscribers[i]->getCurrentFrameLocked())
	    {
		    notifyVideoUsers(videoUsersBeforeRender, *buffer, sinkNames[i]);
		    buffer->unlock();
	    }
    }
}


void
Video::setNewVideoFrame(const openvideo::Buffer& frame, stb::string *givenSinkName)
{
	notifyVideoUsers(videoUsersImmediate, frame, givenSinkName);
}


void
Video::notifyVideoUsers(VideoUserVector& videoUsers, const openvideo::Buffer& frame, stb::string *givenSinkName)
{
    if (paused) return;
	for(size_t i=0; i<videoUsers.size(); i++)
		videoUsers[i]->vu_update(frame, givenSinkName, forceUpdate);
}


const openvideo::Buffer*
Video::getCurrentFrameLocked()
{
	return videoSinkSubscribers[0]->getCurrentFrameLocked();
}



END_NAMESPACE_STB
//========================================================================
// End of Video.cxx
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
