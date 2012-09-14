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
/** header file for MidiSource.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/tool/midi.h>

namespace ot {

  class MidiIn;


  class MidiSource: public Node, public MidiInHandler{
  protected:
    MidiIn * inDev;
    std::vector<MIDIBUFFER*> buffers;

    Event event;
    int changed;

    unsigned long bufferSize;
    unsigned long bufferCount;

    MidiSource(unsigned int devid);
    MidiSource(std::string devname);

    void setBufferSize(unsigned long size);
    void setBufferCount(unsigned long count);


    void initBuffers();
    void releaseBuffers();

    void startRecording();

    void stopRecording();


    void close();

  public:
    virtual ~MidiSource();
    virtual int isEventGenerator();
    void pushEvent();

  virtual void handleShortMsg( unsigned long msg, unsigned long timestamp ) ;
  /* do something when an error from short messages*/
  virtual void onShortMsgError( unsigned long msg, unsigned long timestamp) ;
  /* receive a long message */
  virtual void handleLongMsg(  unsigned long msg, unsigned long timestamp) ;
  /* do something when an error from long messages */
  virtual void onLongMsgError( unsigned long msg, unsigned long timestamp) ;
  
    // friends with the factory
    friend class MidiModule;
  };


}; // namespace ot
