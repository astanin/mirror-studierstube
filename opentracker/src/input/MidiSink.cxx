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
/** source file for MidiModule.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifdef USE_MIDI

#  include <OpenTracker/input/MidiSink.h>
namespace ot{

MidiSink::MidiSink():MidiOut(){};
int MidiSink::isEventGenerator(){
  return 1;
};

MidiSink::~MidiSink(){};

void MidiSink::onEventGenerated(Event & event, Node & generator){
  // search the event for an attribute of type MidiMsg
  int size = event.getSize();
  for (int i = 0; i< size; i++){
    std::string name = event.getAttributeName(i);
    try {
      MidiMsg & theMsg = event.getAttribute((MidiMsg*)NULL, name);
      // check for sysEx message      
      if (theMsg.isSysEx()){
	sendSysExMessage(&theMsg.msg[0], (unsigned long)theMsg.msg.size());
      } else {
	// its short, whooo 
	sendShortMessage(theMsg.packMsg());
      }
    }catch(std::exception & e){
      // catch all invalid_argument exceptions
    }
  }
  //in the end, resend the message
  updateObservers(event);
};


} //namespace ot

#endif //USE_MIDI
