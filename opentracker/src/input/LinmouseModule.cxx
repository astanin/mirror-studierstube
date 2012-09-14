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
/** source file for LinmouseModule.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this is a linux implementation
#ifndef WIN32
#ifndef __APPLE__
#include <OpenTracker/input/LinmouseModule.h>

#include <OpenTracker/input/LinmouseSource.h>

//#ifdef USE_LINMOUSEMODULE

#include <cstdio>
#include <iostream>
#include <cassert>

#include <ace/Log_Msg.h>

#define OT_NAMESPACE_BEGIN namespace ot {
#define OT_NAMESPACE_END }

OT_NAMESPACE_BEGIN

	OT_MODULE_REGISTER_FUNC(LinmouseModule){
			OT_MODULE_REGISTRATION_DEFAULT(LinmouseModule, "LinmouseConfig");
	}

// -------------------------------------------------------------------------------------------------------
// constructor initializing the thread manager
LinmouseModule::LinmouseModule() : ThreadModule(), NodeFactory(), fd(-1), stop(0)
{
	
}

// ------
// Destructor method
LinmouseModule::~LinmouseModule() {
    sources.clear();
}

// This method is called to construct a new Node.
Node * LinmouseModule::createNode( const std::string& name, const StringTable& attributes) {
   
    if( name.compare("LinmouseSource") == 0 ) {       
        stop = 0;

        NodeVector::iterator it;
        if ( sources.size() > 0) {
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Can only have one LinmouseSource!\n")));
            return NULL;
        }
	 
        for( it = sources.begin(); it != sources.end(); it++ ){
            LinmouseSource * source = (LinmouseSource*)((Node*)*it);
            if( source->devname.compare(attributes.get("dev")) == 0 )
	    {
                break;
	    }
        }
        if( it != sources.end())
        {
	    std::string dn(attributes.get("dev"));
	    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Source accessing %s exists already\n"), dn.c_str()));
	    return NULL;
        }
      
        LinmouseSource * source = new LinmouseSource(attributes.get("dev"));

        source->event.getButton() = 0;
        source->event.getConfidence() = 1.0f;
        sources.push_back( source );

        logPrintI("Build LinmouseSource node\n");

        return source;
    }
    return NULL;
}

// opens file desctipter
void LinmouseModule::start() 
{
    //ACE_DEBUG((LM_INFO, ACE_TEXT("LinmouseModule::start() \n")));
    if( isInitialized() && !sources.empty())
        ThreadModule::start();    
}

// closes CyberMouse library
void LinmouseModule::close() {
    //ACE_DEBUG((LM_INFO, ACE_TEXT("LinmouseModule::close() \n")));
    lockLoop();
    stop = 1;
    unlockLoop();
}
void LinmouseModule::run()
{
    //ACE_DEBUG((LM_INFO, ACE_TEXT("LinmouseModule::run() \n")));
    size_t read_bytes;
    struct input_event ev;
    short buttons = 0;
    ot::MathUtils::Quaternion orientation;

    char name[256]= "Unknown";
   
    if ((fd = open(dynamic_cast<LinmouseSource*>((Node *) sources.at(0))->devname.c_str(), O_RDONLY)) < 0) {
        perror("evdev open");
        exit(1);
    }

    if(ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
        perror("evdev ioctl");
    }

    //ACE_DEBUG((LM_INFO, ACE_TEXT("The device on %s says it's name is %s\n"), fdfilename.c_str(), name));

    bool wheelflag = false;

    while(1) {
        lockLoop();
        if (stop == true) 
        {
            unlockLoop();
            break;
        }
        else
        {
            unlockLoop();
        }

        if (wheelflag) {  // the wheel does not reset the button -> insert artificial button release event
            usleep(10000);
            buttons &= 0xffcf;
            wheelflag = false;
        }
        else { // "normal" case -> read from devict
            read_bytes = read(fd, &ev, sizeof(struct input_event));
	 
            if (read_bytes < (int) sizeof(struct input_event)) {
                perror("evtest: short read");
                exit (1);
            }
     
            /*printf("Event: time %ld.%06ld, type %d, code %d, value %d\n",
              ev.time.tv_sec, ev.time.tv_usec, ev.type,
              ev.code, ev.value);*/


            switch (ev.type) {
                case EV_KEY :
                    switch (ev.code) {
                        case BTN_LEFT:
                        case BTN_SIDE:
                            if (ev.value) buttons |= 0x0001; else buttons &= ~0x0001;
                            break;
                        case BTN_RIGHT:
                        case BTN_EXTRA:
                            if (ev.value) buttons |= 0x0002; else buttons &= ~0x0002;
                            break;
                        case BTN_MIDDLE:
                            if (ev.value) buttons |= 0x0004; else buttons &= ~0x0004;
                            break;	 
                        case BTN_FORWARD:
                            if (ev.value) buttons |= 0x0008; else buttons &= ~0x0008;
                            break;
                            /*
                              case BTN_EXTRA:
                              if (ev.value) buttons |= 0x0010; else buttons &= ~0x0010;
                              break;	 
                              case BTN_SIDE:
                              if (ev.value) buttons |= 0x0020; else buttons &= ~0x0020;
                              break;	
                            */
                        case BTN_BACK:
                            if (ev.value) buttons |= 0x0040; else buttons &= ~0x0040;
                            break;
                        case BTN_TASK:
                            if (ev.value) buttons |= 0x0080; else buttons &= ~0x0080;
                            break;
                            // not more than eight buttons allowed in studierstube
                            /*
                              case 0x118:
                              if (ev.value) buttons |= 0x0100; else buttons &= ~0x0100;
                              break;
                              case 0x119:
                              if (ev.value) buttons |= 0x0200; else buttons &= ~0x0200;
                              break;
                              case 0x11a:
                              if (ev.value) buttons |= 0x0400; else buttons &= ~0x0400;
                              break;
                              case 0x11b:
                              if (ev.value) buttons |= 0x0800; else buttons &= ~0x0800;
                              break;
                            */
                        default:
                            break;
                    }
                    break;
                case EV_REL: // relative movement
                    ot::MathUtils::Quaternion relquat;
                    ot::MathUtils::Quaternion startquat;
                    startquat[0] = orientation[0];
                    startquat[1] = orientation[1];
                    startquat[2] = orientation[2];
                    startquat[3] = orientation[3];
                    double axisa[4]; axisa[0] = axisa[1] = axisa[2] = 0;
                    axisa[3] = 1;
	 
                    if (ev.code == REL_X) {
                        axisa[0] = ev.value/100.0;
                    }
                    else if (ev.code == REL_Y) {
                        axisa[1] = ev.value/100.0;
                    }
                    else if (ev.code == REL_Z) { 
                        axisa[2] = ev.value/100.0;
                    }
                    else if (ev.code == REL_WHEEL) {
                        if (ev.value == -1) buttons |= 0x0010; 
                        else if (ev.value == 1) buttons |= 0x0020; 
                        //ACE_DEBUG((LM_INFO, ACE_TEXT("Wheel %d - %d"), ev.value, buttons));
                        wheelflag = true;
                    }
	 
                    ot::MathUtils::axisAngleToQuaternion(axisa, relquat);
                    ot::MathUtils::normalizeQuaternion(relquat);
                    ot::MathUtils::multiplyQuaternion(startquat, relquat, orientation);
                    ot::MathUtils::normalizeQuaternion(orientation);
	    
                    break;
                default:
                    continue;
                    break;
            }
        }

        NodeVector::iterator it;
      
      
        for( it = sources.begin(); it != sources.end(); it++) {
	 
            LinmouseSource * source = (LinmouseSource*)((Node*) *it);

            source->lock();

            // buttons
            source->event.getButton() = buttons;
	 
            // orientation
            source->event.getOrientation()[0] = orientation[0];
            source->event.getOrientation()[1] = orientation[1];
            source->event.getOrientation()[2] = orientation[2];
            source->event.getOrientation()[3] = orientation[3];
	 
            source->event.timeStamp();
            source->event.getConfidence() = 1.0;
            source->changed = 1;

            source->unlock();

            if (context != NULL)
	    { 
                if (context->doSynchronization())
                {
                    context->dataBroadcast();
                    context->consumedWait();
                }
	    } 
        }
    }
}

// pushes events into the tracker tree.
void LinmouseModule::pushEvent() 
{
    /// nothing to do
}

// initializes the ARTDataTracker module

void LinmouseModule::init(StringTable& attributes, ConfigNode * localTree)
{
    using namespace std;
    ACE_DEBUG((LM_INFO, ACE_TEXT("LinmouseModule::init() \n")));

    ThreadModule::init( attributes, localTree );      
   
    fd = 0;		// just to make the compiler happy
}

OT_NAMESPACE_END

//#endif

// WIN32
#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of LinMouseModule.cxx
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
