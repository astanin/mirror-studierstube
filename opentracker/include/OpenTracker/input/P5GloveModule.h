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
/** header file for P5Glove module.
 *
 * @author Hannes Kaufmann, Istvan Barakonyi
 *
 * $Id: P5GloveModule.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section p5glovemodule P5GloveModule
 * The P5GloveModule provides and drives @ref p5glovesource nodes. It implements
 * a driver for the P5 Glove by Essential Reality http://www.essentialreality.com/. 
 * To use this driver you will have to download the SDK from the above website and
 * compile OpenTracker with the appropriate define as described in @ref config. The
 * implementation was only tested under Windows because there was no Linux driver 
 * available at the time of development. 
 *
 * The module uses the configuration element 'P5GloveConfig'. This
 * element has the following attributes :
 * @li @c P5Id @c 0 the glove id passed to the driver
 * 
 * An example configuration element looks like this :
 * @verbatim
 <P5GloveConfig P5Id="0"/>@endverbatim
 *
 *
 */

#ifndef _P5GLOVEMODULE_H
#define _P5GLOVEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_P5GLOVE

#include "P5dll.h"

/**
 * The module and factory to drive the P5GloveSource nodes. It constructs
 * P5GloveSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Hannes Kaufmann, Istvan Barakonyi
 * @ingroup input
 */

namespace ot {

    class OPENTRACKER_API P5GloveModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// list of P5GloveSource nodes in the tree
        NodeVector nodes;

        // Methods
    public:
        CP5DLL *P5device;
        int P5Id;

        /// Treshold in degrees for button activation. 
        /// If the index finger is bent more than BEND_TRESHOLD degrees then the button is activated.
#define BEND_THRESHOLD 50

#define P5MOTION_XYZFILTERSIZE	5
#define P5MOTION_YPRFILTERSIZE	18

#define P5MOTION_YPRABSOLUTE	0
#define P5MOTION_YPRRELATIVE	1

#define	P5MOTION_NORMALAXIS		1
#define	P5MOTION_INVERTAXIS		-1

        int nXPos, nYPos, nZPos;
        float fXMickey, fYMickey, fZMickey;
        float fAbsYawPos, fAbsPitchPos, fAbsRollPos;
        float fRelYawPos, fRelPitchPos, fRelRollPos;
        float fFilterX, fFilterY, fFilterZ;

        /** constructor method. */
        P5GloveModule();

        /** Destructor method, clears nodes member. */
        virtual ~P5GloveModule();
	/**
         * initializes the P5Glove module
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes,  ConfigNode * localTree);
        /** This method is called to construct a new Node. It compares
         * name to the P5GloveSource element name, and if it matches
         * creates a new P5GloveSource node.
         * @param name reference to string containing element name
         * @attributes reference to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  StringTable& attributes);
	/**
         * closes P5Glove */
        virtual void close();
        /**
         * starts P5Glove
         */
        virtual void start();
        /**
         * pushes events into the tracker tree. Checks all P5GloveSources and
         * pushes new events, if a P5GloveSource fires. The events store
	 * structure with position and status of the buttons.
         */
        virtual void pushEvent();

        void P5Motion_SetClipRegion(int xstart, int xend, int ystart, int yend, int zstart, int zend);
        void P5Motion_InvertMouse (int xaxis, int yaxis, int zaxis);

        void P5Motion_Process();
        void P5Motion_FilterXYZ();
        void P5Motion_FilterYPR();

    };
	OT_MODULE(P5GloveModule);

} // namespace ot

#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of P5GloveModule.h
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
