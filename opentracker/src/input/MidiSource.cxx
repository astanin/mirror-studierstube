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
/** source file for MidiSource.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifdef USE_MIDI
#include <OpenTracker/tool/midi.h>
#include <OpenTracker/input/MidiSource.h>


namespace ot{


MidiSource::MidiSource(unsigned int devid):bufferSize(256), bufferCount(1){
  inDev = new MidiIn;
  inDev->setHandler(this);
  inDev->openId(devid);
};

MidiSource::MidiSource(std::string devname):bufferSize(256), bufferCount(1){
  inDev = new MidiIn;
  inDev->setHandler(this);
  inDev->openStr(devname);
};


MidiSource::~MidiSource(){
  //logPrintW("MIDISRC IS DESTROYED\n");
  stopRecording();
  //logPrintW("MIDISRC closing\n");
  close();
  //logPrintW("MIDISRC releasing buffers\n");
   releaseBuffers();
};

void MidiSource::initBuffers(){
  if (buffers.size() != bufferCount){
    for (unsigned long i = 0; i < bufferCount; i++){
      MIDIBUFFER * buf = new MIDIBUFFER;
      midiInitBuffer(bufferSize, buf);
      inDev->addBuffer(buf);
      buffers.push_back(buf);
    }
  }
};

void MidiSource::releaseBuffers(){
  for (std::vector<MIDIBUFFER*>::iterator i = buffers.begin();
       i != buffers.end(); i++){
    MIDIBUFFER * buf = (*i);
    midiInUnPrepareBuffer(buf);
    midiReleaseBuffer(buf);
  }

};


void MidiSource::startRecording(){
  initBuffers();
  inDev->startRecording();
};

void MidiSource::stopRecording(){
  inDev->stopRecording();
};

int MidiSource::isEventGenerator(){
  return 1;
};
void MidiSource::pushEvent(){
	
  if (changed){
    updateObservers(event);
    changed = 0;
  }
};

  void MidiSource::handleShortMsg( unsigned long msg, unsigned long timestamp ){
    unsigned char byte1, byte2, byte3;
    
    MidiMsg::unpackShortMsg(msg, byte1, byte2, byte3);

    MidiMsg mMsg;
    mMsg.timestamp = timestamp;
    mMsg.msg.push_back(byte1);
    mMsg.msg.push_back(byte2);
    mMsg.msg.push_back(byte3);
    event.setAttribute("midiMsg", mMsg);

    changed = 1;
    
  };

  /* do something when an error from short messages*/
void MidiSource::onShortMsgError( unsigned long msg, unsigned long timestamp) {};



/* receive a long message */
  
  void MidiSource::handleLongMsg(  unsigned long msg, unsigned long timestamp) {
    MIDIBUFFER * buf = (MIDIBUFFER *)((LPMIDIHDR)msg)->dwUser;
    unsigned long size = midiBufferGetBytesRecorded(buf);
    unsigned char * ptr  = midiBufferGetPtr (buf);
    if (size ==0){
      midiInUnPrepareBuffer(buf);
      // and quit
      return;
    }

    printf("MIDI GOT LONG MSG with %u bytes\n", size);
    int bytes = 16;
    bool ended = (*(ptr + (buf->hdr.dwBytesRecorded -1)) == 0xF7);
    
    MidiMsg mMsg;
    std::vector<unsigned char> &out = mMsg.msg;
    mMsg.timestamp = timestamp;

    if (ended){
      while (size --){
	out.push_back((*ptr));
	ptr++;
      }
     
      event.setAttribute("midiMsg", mMsg);
      //      event.setAttribute("midiTimestamp", timestamp);
      //     event.setAttribute("midiMsg", out);
      changed = 1;

    } else {
      // there is no message yet because the buffer was full, the end of the message should be 
      // in the next message... the first part should be stored somewhere to make sure the message
      // can be put together again
    }

  // enqueue the buffer once again
  inDev->queueBuffer(buf);

};
/* do something when an error from long messages */
void MidiSource::onLongMsgError( unsigned long msg, unsigned long timestamp) {};
  

void MidiSource::close(){
  inDev->close();
};

}; // namespace ot

#endif //USE_MIDI
