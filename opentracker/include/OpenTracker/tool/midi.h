#ifndef TOOL_MIDI_H
#define TOOL_MIDI_H

#ifdef USE_MIDI

#include <string>

#ifdef __WIN32__
# include <OpenTracker/tool/midiw32.h>
#else

#endif // WIN32

#include <iostream>
#include <sstream>
#include <vector>
#include <OpenTracker/core/OtException.h>
namespace ot{

class MidiEx: public OtException{
 public:
  MidiEx(std::string e) throw():OtException(e)
    { };

};

class MidiInHandler {
public:
  virtual ~MidiInHandler(){};
  /* receive a short message */
  virtual void handleShortMsg( unsigned long msg, unsigned long timestamp ) {};
  /* do something when an error from short messages*/
  virtual void onShortMsgError( unsigned long msg, unsigned long timestamp) {};
  /* receive a long message */
  virtual void handleLongMsg(  unsigned long msg, unsigned long timestamp) {};
  /* do something when an error from long messages */
  virtual void onLongMsgError( unsigned long msg, unsigned long timestamp) {};
  
};


void midiFormatErrorString(unsigned long err, char * buf, unsigned long size);

std::string midiErrorString(unsigned long err);

unsigned long midiCloseIn(MIDIINHANDLE hnd);

unsigned long midiStartIn(MIDIINHANDLE hdl);

unsigned long midiStopIn(MIDIINHANDLE hdl);

typedef struct MIDIBUFFER{
  MIDIHDR hdr;
  unsigned long timestamp;
  unsigned long Hdl;
}MIDIBUFFER;

unsigned long midiInitBuffer(unsigned long size, MIDIBUFFER * buf);
unsigned long midiReleaseBuffer(MIDIBUFFER * buf);

unsigned long midiInPrepareBuffer(MIDIINHANDLE inHdl, MIDIBUFFER * buf);
unsigned long midiInUnPrepareBuffer(MIDIBUFFER * buf);
unsigned long midiInQueueBuffer(MIDIBUFFER *buf);
unsigned long midiBufferGetSize(MIDIBUFFER * buf);
unsigned long midiBufferGetBytesRecorded(MIDIBUFFER * buf);
unsigned char * midiBufferGetPtr(MIDIBUFFER * buf);




class MidiIn{
protected:
  MIDIINHANDLE hdl;
  MidiInHandler * inHandler;
  bool recording;
  bool _open;
public:
  virtual ~MidiIn();
  MidiIn();

  MIDIINHANDLE getHandle();
  
  static void CALLBACK handleFunc(MIDIINHANDLE hdl, unsigned long msg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 );

  void openId(unsigned long id );
  void openStr(std::string devname);
  bool isOpen();
  void close();

  bool isRecording();
  void startRecording();
  void stopRecording();
  void setHandler(MidiInHandler * inHdl);
  void addBuffer(MIDIBUFFER * buf);
  void queueBuffer(MIDIBUFFER * buf);
};


class MidiOut{
protected:
  MIDIOUTHANDLE out;
  bool _opened;

public:
  MidiOut();
  virtual ~MidiOut();
  
  void close ();
  bool isOpen();
  void openId(unsigned long devid);
  void openStr(std::string devName);

  void sendShortMessage( unsigned long msg);
  void sendSysExMessage( unsigned char * buf, unsigned long size);
};


/* helper class to handle messages */
class MidiMsg{
 public:
  std::vector<unsigned char> msg;
  unsigned long timestamp;
 public:
  unsigned long packMsg();
  bool isSysEx();
  unsigned char getCommand();
  unsigned char getChannel();
  unsigned char getStatus();
  static void unpackShortMsg(unsigned long msg, unsigned char & byte1, unsigned char & byte2, unsigned char & byte3);
};

std::ostream& operator<<(std::ostream& os, MidiMsg& object);
std::istream& operator>>(std::istream& is, MidiMsg& object);

}; // namespace ot
#endif // USE_MIDI

#endif // TOOL_MIDI_H
