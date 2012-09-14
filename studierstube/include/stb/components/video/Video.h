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
* $Id: Video.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  


#ifndef _VIDEO_H_
#define _VIDEO_H_


#include <stb/base/macros.h>
#include <stb/base/OS.h>
#include <stb/base/datatypes.h>
#include <stb/kernel/ComponentThread.h>
#include <stb/kernel/VideoProvider.h>
#include <stb/kernel/KernelEventSubscriber.h>


#ifdef STB_IS_WINDOWS
#  include <windows.h>
#elif defined(STB_IS_LINUX)
#ifndef __APPLE__
#  include <GL/glx.h>
#endif
#endif


#if defined(WIN32) || defined(_WIN32_WCE)
#  pragma warning(disable:4251)
#  pragma warning(disable:4290)
#  ifdef VIDEO_EXPORTS
#    define VIDEO_API __declspec(dllexport)
#  else
#    define VIDEO_API __declspec(dllimport)
#  endif
#else
#  define VIDEO_API
#endif

namespace openvideo{
    class Manager;
//    class GL_TEXTURE_2D_Sink;
	class State;
	class Buffer;
}

BEGIN_NAMESPACE_STB


class  Stb4VideoSinkSubscriber;

typedef std::vector<stb::string *> StringVector;
typedef std::vector<Stb4VideoSinkSubscriber *> Stb4VideoSinkSubscriberVector;

/**
*	
*/
class Video : public stb::ComponentThread, public stb::VideoProvider, public stb::KernelEventSubscriber
{
friend class Stb4VideoSinkSubscriber;
public:
    /**
    *     The Constructor	
    */
    Video();
    /**
    *     The destructor.
    */
    virtual ~Video();

	/// Called before the application is destructed.
	virtual bool init();
	/// Called before the application is destructed.
	virtual void shutDown();
    //
    virtual void setParameter(stb::string key, std::string value);

	// Declare that this component is a video provider
	virtual VideoProvider* getVideoProviderInterface()  {  return this;  }


	/// Implement stb::VideoProvider interface
	virtual void vp_registerVideoUser(VideoUser* videouser);
	virtual void vp_unregisterVideoUser(VideoUser* videouser);


	/// Implement stb::KernelEventSubscriber interface
	virtual void kes_beforeRender();


	/// Returns a locked frame with the latest update from OpenVideo
	/**
	 *  Note: It is crucial that the caller of this method calls
	 *        unlock() when it no longer uses this Buffer. Otherwise
	 *        a leak happens!
	 */
	virtual const openvideo::Buffer* getCurrentFrameLocked();


    /// The following four functions enable video freezing
    void pause() { paused=true; };
    void resume() { paused=false; };
    void togglePause() { paused=!paused; };
    bool isPaused() { return paused; }
	static Video* instance;
protected:
	void setVideoFormat(const openvideo::Buffer& format, stb::string *givenSinkName);
	void setNewVideoFrame(const openvideo::Buffer& format, stb::string *givenSinkName);
	void notifyVideoUsers(VideoUserVector& videoUsers, const openvideo::Buffer& frame, stb::string *givenSinkName);

	bool initOpenVideo();

    stb::string configFile;
    StringVector sinkNames;

    openvideo::Manager* ovManager;

    bool isGLContext;

	bool runSingleThreaded;
	bool ovInitialized;
    bool forceUpdate;

    virtual void run();

	
private:
    bool paused;

    Stb4VideoSinkSubscriberVector videoSinkSubscribers;
    //Stb4VideoSinkSubscriber *videoSinkSubscriber;

	VideoUserVector	videoUsersImmediate, videoUsersBeforeRender;

	openvideo::Buffer* video_format;
};


END_NAMESPACE_STB


#endif//_VIDEO_H_
//========================================================================
// End of Video.h 
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
