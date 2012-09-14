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
/** header file for ICubeXSource.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/tool/midi.h>
#include <OpenTracker/tool/SyncQueue.h>
#include <vector>
#include <map>
#include <string>

namespace ot{


  class ixMidiSocket;
  class MidiMsg;
  class ICubeXSensor;
  class ICubeXSource: public Node{
  public:
    typedef std::vector<ICubeXSensor *> SensorArray;
  protected:
    SensorArray sensors;
    std::string config;
    unsigned char confignr;  
    char name[8];
    unsigned char version[5];
    unsigned char id;
    unsigned char mode;
    unsigned char running_status;
    unsigned char midi_thru;

    unsigned char default_config[9];


    unsigned char nextControllerNumber;

    ixMidiSocket * socket;
    bool changed;
    Event event;


    friend class ICubeXModule;
    ICubeXSource();
    void configure(StringTable & config);

    void start();
    void stop();

  public:
    static const unsigned int numberOfPorts;
    typedef SyncQueue<MidiMsg *> MsgQueue;
    MsgQueue mQueue;

    virtual ~ICubeXSource();
    virtual int isEventGenerator();
    void pushEvent();

    void addSensor(ICubeXSensor * );
	ICubeXSensor * getSensor( int index );
    

    void asHostMode(unsigned char idd);
    bool isHostMode();
    bool isStandAloneMode();
    unsigned char getId();
    unsigned char getConfigNr();
    unsigned char getRunningStatus();
    unsigned char getMidiThru();
    float getFirmwareVersion();
    float getHardwareVersion();
    unsigned int getSerial();
    void setVersion( unsigned char * ptr ); 
    void setSensorConfig(int index, unsigned char * ptr);
    unsigned char * getSensorConfig(int index);
    void setName(unsigned char * ptr);
    void clear(MidiMsg & mMsg) ;
    void makeHeader(MidiMsg & mMsg);
    void makeFooter(MidiMsg & mMsg);
    void makeCmdMsg(MidiMsg & mMsg, unsigned char cmd);
    void makeOneByteMsg(MidiMsg & mMsg, unsigned char cmd, unsigned char data1);

    void makeTwoByteMsg(MidiMsg & mMsg, unsigned char cmd, unsigned char data1, unsigned char data2);



  };
  


} // namespace ot
