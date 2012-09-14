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
/** source file for ButtonFilter Node.
 *
 * @author Flo Ledermann flo@subnet.at
 *
 * $Id: ButtonFilterNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/common/ButtonFilterNode.h>

#include <cstdlib>
#include <iostream>

#include <ace/Log_Msg.h>


#define DEBUG_BUTTONFILTER
#if defined(DEBUG_BUTTONFILTER) && defined(_DEBUG) && !defined(_WIN32_WCE)
#define DEBUG_CODE(a) {a;}
#else
#define DEBUG_CODE(a) {}
#endif

//using namespace std;

// constructor method.


#ifndef OT_NO_BUTTON_SUPPORT


namespace ot {

    ButtonFilterNode::ButtonFilterNode(const char* buttonmaskstr, const char* buttonmapstr, const char * invertstr,
                                       const char * validtransstr , const char * radiobuttonsstr, const char *validTransSetButtonStr )
        : Node()
    {
        int i;
        unsigned char buttonbit = 1;

        // initialize neutral
        buttonmask = 0xff;
        invert = 0;
        for (i=0;i<8;i++){
            buttonmap[i] = i;
        }

        // set buttonmask
        for (i=0; i<8; i++){
            // check end of string
            if (buttonmaskstr[i] == '\0') break;
            if (!(buttonmaskstr[i] == '1')) buttonmask = buttonmask ^ buttonbit;

            buttonbit = buttonbit << 1;
        }

        // set buttonmap
        for (i=0; i<8; i++){
            // check end of string
            if (buttonmapstr[i] == '\0') break;

            if (buttonmapstr[i] >= '0' && buttonmapstr[i] <= '7') buttonmap[i] = buttonmapstr[i]-48;
            else buttonmap[i] = i;
        }

        // set invertmask
        buttonbit = 1;
        for (i=0; i<8; i++){
            // check end of string
            if (invertstr[i] == '\0') break;
            if (invertstr[i] == '1') invert = invert ^ buttonbit;

            buttonbit = buttonbit << 1;
        }

        // set validtrans
        for (i=0; i< 8; i++)
        {
            validtrans[i] = -1;
            validTransSetButton[i] = -1;
            radiobuttons[i] = 0;
        }

        if (validtransstr!= NULL && radiobuttonsstr != NULL)
        {
            if (strlen(validtransstr) == 8 && strlen(radiobuttonsstr) == 8)
            {
                // these could be processed in one loop, but readiblity improves with 2 extra loops
                // set validtrans
                for (i=0; i< 8; i++)
                {
                    if (validtransstr[i] >= '0' && validtransstr[i] <= '7')
                    {
                        int vtId = validtransstr[i] - 48;
                        validtrans[i] = vtId; // set position to given id
                    }

                    if (strlen(validTransSetButtonStr) > (unsigned int)i)
                    {
                        if (validTransSetButtonStr[i] >= '0' && validTransSetButtonStr[i] <= '7')
                        {
                            int vtId = validTransSetButtonStr[i] - 48;
                            validTransSetButton[i] = vtId; // set position to given id
                        } else { validTransSetButton[i] = -1; }

                    }
                }

                // set radiobuttons
                for (i=0; i< 8; i++)
                {
                    if (radiobuttonsstr[i] >= '0' && radiobuttonsstr[i] <= '7')
                    {
                        int radioButtonId = radiobuttonsstr[i] - 48;
                        radiobuttons[i] = radioButtonId; // set position in radio button with id given in string
                    }
                    else
                    {
                        radiobuttons[i] = -1; // set position in radio button with not a radiobutton
                    }

                }
            }
            else
            {
                //cerr << "ButtonFilterNode error. validtrans && radiobuttons must be exact 8 chars long. SETTINGS IGNORED !!" << endl;
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:ButtonFilterNode error. validtrans && radiobuttons must be exact 8 chars long. SETTINGS IGNORED !!\n")));
            }
        }
    }

    int ButtonFilterNode::isEventGenerator()
    {
        return 1;
    }

    // this method is called by the EventGenerator to update it's observers.
#define COUT_BINARY(bina) { for (int bi=0; bi <8; bi++) std::cout << (((bina)>>bi) & 1); }
    void ButtonFilterNode::onEventGenerated( Event& event, Node& generator)
    {
        int i;
        unsigned char buttonout = 0;

        lastEvent = event;
        DEBUG_CODE(std::cout << std::endl;);
        DEBUG_CODE(std::cout << "got button                  :"; COUT_BINARY(lastEvent.getButton()); std::cout << std::endl;);

        unsigned char deleteButtonBitsNotValidTrans = 0x00; // delete no bits
        unsigned char validTransSetButtonMask = 0x00; // change no bits
        // just for debug:
        unsigned char validTransBits = 0x00;
        unsigned char validTransSetButtonBit = 0x00;
        // go through all 8 bits and check if
        for (i=0; i < 8; i++)
        {

            //just for debug:
            if (validTransSetButton[i] > -1) validTransSetButtonBit |= 1<< 1;

            if (validtrans[i] > -1) // validtrans bit is set
            {
                //just for debug:
                validTransBits |= 1 <<i;

                // it is a validtrans bit
                unsigned char bit = (lastEvent.getButton() >> i) &1;

                if (bit) // if vt is set -> set all buttonbits (that belong to the vt) to 0
                {
                    deleteButtonBitsNotValidTrans |= (1<<i); // vt must be set to 0 !!
                    for (int j =0 ; j< 8; j++)
                    {
                        if (radiobuttons[j] == validtrans[i])
                            deleteButtonBitsNotValidTrans |= (1<<j);
                    }
                } // vt is 0 -> vt transmission signal !!, change nothing button will show as 1
                else
                {	 // fix for broken hardware :<
                    // set all bits where the validtrans id and the validTransSetButton mask match
                    // validtrans: 3-------  setbuttononvalidtrans: -33----- would set buttonbits 1-2 if valid trans bit 0 is cleared
                    for (int j =0 ; j< 8; j++)
                    {
                        if (validTransSetButton[j] == validtrans[i])
                            validTransSetButtonMask |= (1<<j);
                    }
                }
            }
        }

        // invert and AND
        // if there are no deleteButtonBitsNotValidTrans (0x00) nothing is changed, all '1' within the deleteButtonBitsNotValidTrans
        // will set the lastEvent.getButton() to zero

        DEBUG_CODE(std::cout << "validTransBits              :"; COUT_BINARY(validTransBits); std::cout << std::endl;);
        DEBUG_CODE(std::cout << "set to 0 buttonbits         :"; COUT_BINARY(deleteButtonBitsNotValidTrans); std::cout << std::endl;);

        deleteButtonBitsNotValidTrans = ~deleteButtonBitsNotValidTrans;
        lastEvent.getButton() &= deleteButtonBitsNotValidTrans;
        DEBUG_CODE(std::cout << "buttonbits after no VT      :"; COUT_BINARY(lastEvent.getButton()); std::cout << std::endl;);

        DEBUG_CODE(std::cout << "validTransSetButtonBit      :"; COUT_BINARY(validTransSetButtonBit); std::cout << std::endl;);

        lastEvent.getButton() |= validTransSetButtonMask;
        DEBUG_CODE(std::cout << "set buttonbits on VT        :"; COUT_BINARY(validTransSetButtonMask); std::cout << std::endl;);
        DEBUG_CODE(std::cout << "buttonbits  after VT        :"; COUT_BINARY(lastEvent.getButton()); std::cout << std::endl;);

        lastEvent.getButton() ^= invert;
        DEBUG_CODE(std::cout << "invert with                 :"; COUT_BINARY(invert); std::cout << std::endl;);
        DEBUG_CODE(std::cout << "buttonbits after invert     :"; COUT_BINARY(lastEvent.getButton()); std::cout << std::endl;);

        lastEvent.getButton() &= buttonmask;
        DEBUG_CODE(std::cout << "buttonmask                  :"; COUT_BINARY(buttonmask); std::cout << std::endl;);
        DEBUG_CODE(std::cout << "buttonbits after buttonmask :"; COUT_BINARY(lastEvent.getButton()); std::cout << std::endl;);


        for (i=0; i<8; i++){
            // simple stuff
            // get current bit (0 / 1) and move it to new position
            buttonout |= ((lastEvent.getButton()>>i) & 1) << buttonmap[i];

            // complicated stuff:
            // mask current button, shift into bit 0, shift into target position and OR into output
            //buttonout = buttonout | (((lastEvent.getButton() & buttonbit) >> i) << buttonmap[i]);
            // cool ;)
            // buttonbit = buttonbit << 1;
        }

        lastEvent.getButton() = buttonout;

        DEBUG_CODE(std::cout << "buttonbits after buttonmap  :"; COUT_BINARY(lastEvent.getButton()); std::cout << std::endl;);

        updateObservers( lastEvent );
    }

    void ButtonFilterNode::pushEvent()
    {
        // nothing to do
    }

    void ButtonFilterNode::pullEvent()
    {
        // nothing to do
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_BUTTON_SUPPORT")
#endif //OT_NO_BUTTON_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of ButtonFilterNode.cxx
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
