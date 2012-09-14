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
/** header file for ButtonFilter Node.
 *
 * @author Flo Ledermann flo@subnet.at
 *
 * $Id: ButtonFilterNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section buttonfilternode ButtonFilter
 * The Button Filter allows the user to filter and re-map the individual buttons of an event.
 *
 * WATCH OUT !! Button bits are leastsignificant first invert (10000000) with buttonin(00000000) will result in (00000001) !!
 * sequence :
 * validtrans, radiobuttons, invertstr, buttonmask, buttonmap
 *
 * The element has the following attributes :
 *
 * @li @c validtrans (8bit) specify valid transmission bit for radio pen     (1_2_____) means radio pen 1 has VT on bit 0, radio pen 2 has VT on bit 2
 * @li @c radiobuttons (8bit) specify bits for radio pen (ref to validtrans) (_1_22___) means bit 1 is for radio-pen 1, bit 3&4  is for radio-pen 2 (has two buttons)
 * @li @c buttonmask binary mask to enable the individual buttons, e.g. "11110000" enables buttons 0-3
 * @li @c buttonmap 8-digit string, assigning an output number to every individual button input, e.g.
 * "01324567" swaps buttons 2 and 3. Multiple assignments ar ORed together, e.g "00001111" means button
 * 0=true if one (or more) of buttons 0-3 is pressed, button 1=true if one of buttons 4-7 is pressed.
 * @li @c invert binary mask for inverting individual buttons. 0 stands for leave alone, 1 for invert.
 *        this is applied before the button mapping !
 *
 * An example element looks like this :
 * @verbatim
 <ButtonFilter buttonmask="11110000" buttonmap="32107777" invert="00100000">
 <ParButtonSource dev="0x378" DEF="buttons"/>
 </ButtonFilter>@endverbatim
 * outputs buttons 0-3 in reverse order, ignoring the others. (note that button 7 is never TRUE because buttons
 * 4-7 are masked out.
 */

#ifndef _BUTTONFILTERNODE_H
#define _BUTTONFILTERNODE_H

#include <OpenTracker/OpenTracker.h>

/**
 * The Button Filter allows the user to filter and re-map the individual buttons of an event.
 * @author Flo Ledermann flo@subnet.at
 * @author Thomas Psik tomp@ims.tuwien.ac.at
 *
 *
 * ButtonFilter description
 * WATCH OUT !! Button bits are leastsignificant first invert (10000000) with buttonin(00000000) will result in (00000001) !!
 * sequence :
 * validtrans, radiobuttons, invertstr, buttonmask, buttonmap
 *
 * validtrans (8bit) specify valid transmission bit for radio pen     (-----2-1) means radio pen 1 has VT on bit 0, radio pen 2 has VT on bit 2
 * radiobuttons (8bit) specify bits for radio pen (ref to validtrans) (---22-1-) means bit 1 is for radiopen 1, bit 3&4  is for radiopen 2 (has two buttons)
 * buttonmask (8bit) set bit to 0 if you want to disable the button (00000010) will enable only button 1
 * buttonmap  (8bit) rearrange buttons (01234567) is identity mapping (02134567) will change order of button 2 and 1
 * invertstr  (8bit) invert single button (00000010) button 1 is pressed if value reads 0
 * @ingroup common
 */


#ifndef OT_NO_BUTTON_SUPPORT


namespace ot {

    class OPENTRACKER_API ButtonFilterNode
        : public Node
    {
        // Members
    protected:
        /// last saved event variable
        Event lastEvent;

        // supply 8 buttons
        signed char validtrans[8];
        signed char radiobuttons[8];
        signed char validTransSetButton[8];
        unsigned char buttonmap[8];
        unsigned char buttonmask;
        unsigned char invert;

        // Methods
    protected:

        /** constructor method
         */
        ButtonFilterNode( const char* buttonmask, const char* buttonmap, const char * invertstr, const char * validtrans , const char * radiobuttons, const char *setValidTrans);

    public:

        /** tests for EventGenerator interface being present. Returns the
         * result of the childs implementation of this method.
         * @return 1 if child implements EventGenerator, 0 otherwise */
        virtual int isEventGenerator() ;

        /**
         * this method is called by the EventGenerator to update it's observers.
         */
        virtual void onEventGenerated( Event& event, Node& generator);

        void pushEvent();
        void pullEvent();

        friend class CommonNodeFactory;
    };

} // namespace ot


#endif //OT_NO_BUTTON_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of ButtonFilterNode.h
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
