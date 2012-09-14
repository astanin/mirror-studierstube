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
/** header file for WiiModule.
 *
 * @authors Michele Fiorentino and Alexander Bornik
 * starting from cWiimote 0.2 by Kevin Forbes 
 *
 * $Id$ WiiModule.h
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section wiiModule WiiModule
 * The WiiModule provides the @ref WiiSource nodes that 
 * generate button press events @ref WiiSink
 * @authors Michele Fiorentino and Alexander Bornik
 * starting from cWiimote 0.2 by Kevin Forbes 
 */

#ifndef _WIIMODULE_H
#define _WIIMODULE_H

// this is a linux implementation !!!

#include "../OpenTracker.h"

#ifdef USE_WII_SUPPORT

#include "../core/StringTable.h"


#include <cstdlib>
#include <cstdio>

#ifndef WIN32 // linux
#ifndef __APPLE__
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <asm/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/input.h>
#endif
#else // win code


#endif //end win code

#include <iostream>
#include <string>

#include<OpenTracker/input/WiiHandler.h>
/**
 * Wiimote is auto-calibrated.Please put it on a flat surface with A button up at the starting.
 * Inital data are stored aund used  
 * An example configuration element looks like this:
 * @verbatim
 <WiiSink 
 Led="1 0 2"	
 Vibro ="1"
 />@endverbatim
 */

namespace ot {

    class OPENTRACKER_API WiiModule : public ThreadModule, public NodeFactory
    {
        // Members
    protected:
        
        NodeVector sources; ///< list of WiiSource nodes in the tree
        NodeVector sinks;   ///< list of WiiSink nodes in the tree
        
        enum IrMode{ NOINFRARED =0 ,INFRARED1,  INFRARED2};
        enum InteractionMode{ LASERPOINTER =0 ,NAVIGATOR,  ROTATOR};

        int irmode;
        int interactionmode;

        bool sendAttribites;
        int stop; ///< flag whether the thread should stop

        // Methods
      
        /**
         * Mainlopp */
        void run();  ///< mainloop

    public:
        WiiHandler* wiimote; ///< as it is implemented now it supports just one device
        /** constructor method. */
        WiiModule();

        /** Destructor method, clears nodes member. */
        virtual ~WiiModule();

        /** This method is called to construct a new Node. It compares
         * name to the TargusSource element name, and if it matches
         * creates a new TargusSource node.
         * @param name reference to string containing element name
         * @attributes reference to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  StringTable& attributes);

        /**
         * closes the X keyboard source */
        virtual void close();

        /**
         * start function
         */
        virtual void start();

        /**
         * pushes events into the tracker tree. Checks all TargusSources and
         * pushes new events, if a TargusSource fires. The events store
         * structure with position and status of the buttons.
         */
        virtual void pushEvent();

        /**
         * initializes the Wii. 
         */
        virtual void init(StringTable& attributes, ConfigNode * localTree);


    };

    OT_MODULE(WiiModule);
} // _WIIMODULE_H

#endif //#ifdef USE_WII_SUPPORT
#endif

/* 
 * ------------------------------------------------------------
 *   End of WiiModule.h
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
