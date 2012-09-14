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
/** source file for midi helper functions.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/OtLogger.h>
#ifdef USE_MIDI


//#  ifdef WIN32

#  include <OpenTracker/tool/midi.h>
#  include <OpenTracker/tool/midiCodes.h>
#  include <string>

#  pragma message("MidiModule: linking against : Winmm.lib")
#  pragma comment( lib, "Winmm.lib" )

namespace ot {
const unsigned char SHORTMSG_MASK = 15;
const unsigned char SHORTMSG_DATA1 = 8;
const unsigned char SHORTMSG_DATA2 = 16;




/* helper class to handle messages */
void MidiMsg::unpackShortMsg(unsigned long msg, unsigned char & byte1, unsigned char & byte2, unsigned char & byte3){
 byte1  = static_cast <unsigned char>(msg);
 byte2  = static_cast <unsigned char>((msg) >> SHORTMSG_DATA1) ;
 byte3  = static_cast <unsigned char>((msg) >> SHORTMSG_DATA2) ;
};

unsigned long MidiMsg::packMsg(){
  unsigned long result;
  
  result = (((unsigned long)msg[2])<< SHORTMSG_DATA2)|(((unsigned long)msg[1])<< SHORTMSG_DATA1)|(unsigned long)msg[0];
  return result;
};

unsigned char MidiMsg::getCommand(){
  unsigned char result = msg[0];
  result = result & ~((unsigned char) CHANNEL_MASK);
  return result;
};

unsigned char MidiMsg::getChannel(){
  unsigned char result = msg[0];
  result = result & ((unsigned char) CHANNEL_MASK);
  return result;
};

unsigned char MidiMsg::getStatus(){
  return msg[0];
};


  bool MidiMsg::isSysEx(){

    return (msg[0] == 0xF0);
  };

std::ostream& operator<<(std::ostream& os, MidiMsg& object)
{
  // write the midi timestamp
  os << object.timestamp << ":[" << object.msg.size() <<":";
  std::vector<unsigned char>::iterator it;
  for (it = object.msg.begin(); it != object.msg.end(); it++)
    {
      unsigned char uByte = (*it);
      if (it != object.msg.begin())
	os << ";";

      // write the values as hex (does not seem to work)
      // os << std::ios::hex << (unsigned short)uByte;
      // write the values as dec
      os <<  (unsigned short)uByte;
    }
  os << "]";
  return os;
};


std::istream& operator>>(std::istream& is, MidiMsg& object)
{
  std::vector<unsigned char>::size_type n;
  std::vector<unsigned char>::size_type i;
  char c;

  object.timestamp = 0;
  object.msg.clear();
  
  // read the timestamp
  if (!(is >> object.timestamp)){
      is.setstate(std::ios_base::failbit);
      return is;
  }
  // read "["
  if (!(is >> c))
    return is;
  if (c != '[')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
  
  // read size
  if (!(is >> n))
    return is;
  object.msg.reserve(n);
  
  // read ":"
  if (!(is >> c))
    return is;
  if (c != ':')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
  
  // read data
  for (i = 0; i < n; i++)
    {
      unsigned short t;
      is >> t;
      object.msg.push_back(static_cast<unsigned char>(t));
      // read ";" or "]"
      if (!(is >> c))
	return is;
      if (c != ';' && c != ']')
	{
	  is.setstate(std::ios_base::failbit);
	  return is;
	}
    }
  
        return is;
};

unsigned long midiCloseIn(MIDIINHANDLE hnd){
  return midiInClose(hnd);
}

unsigned long midiStartIn(MIDIINHANDLE hdl){
  return midiInStart(hdl);
};

unsigned long midiStopIn(MIDIINHANDLE hdl){
  return midiInReset(hdl);
}

void midiFormatErrorString(unsigned long err, char * buf, unsigned long size){
  midiInGetErrorText(err, &buf[0], size);
}
std::string midiErrorString(unsigned long err){
#define BUFFERSIZE 200
  char buffer [BUFFERSIZE];
  midiFormatErrorString(err, &buffer[0], BUFFERSIZE);
  return std::string(buffer);
}



unsigned long midiInitBuffer(unsigned long size, MIDIBUFFER * buf){
  unsigned char * buffer = new unsigned char [size];
  for (unsigned long i = 0; i < size; i++)
    buffer[i] = 0;
  buf->hdr.lpData = (LPSTR) buffer;
  buf->hdr.dwBufferLength = (DWORD)size;
  buf->hdr.dwFlags = 0;
  buf->hdr.dwUser = (DWORD)buf;
  return MIDINOERROR;
};

unsigned long midiReleaseBuffer(MIDIBUFFER * buf){
  unsigned char * buffer = (unsigned char *)buf->hdr.lpData;
  delete [] buffer;
  buf->hdr.lpData = 0;
  buf->hdr.dwBufferLength = 0;
  return MIDINOERROR;
};

unsigned long midiInPrepareBuffer(MIDIINHANDLE inHdl, MIDIBUFFER * buf){
  buf->Hdl = (unsigned long) inHdl;
  return midiInPrepareHeader(inHdl, &(buf->hdr), sizeof((buf->hdr)));
};

unsigned long midiInUnPrepareBuffer( MIDIBUFFER * buf){
  MIDIINHANDLE in = (MIDIINHANDLE)(buf->Hdl);
  buf->Hdl = 0;
  return midiInUnprepareHeader(in, &(buf->hdr), sizeof((buf->hdr)));
};

unsigned long midiInQueueBuffer(MIDIBUFFER *buf){
  return midiInAddBuffer((MIDIINHANDLE)(buf->Hdl), &(buf->hdr), sizeof((buf->hdr)));
};

unsigned long midiBufferGetSize(MIDIBUFFER * buf){
  return (unsigned long) buf->hdr.dwBufferLength;
};

unsigned long midiBufferGetBytesRecorded(MIDIBUFFER * buf){
  return (unsigned long) buf->hdr.dwBytesRecorded;
};

unsigned char * midiBufferGetPtr(MIDIBUFFER * buf){
  return (unsigned char *) buf->hdr.lpData;
};


/********************************************************************************
 *
 *                      MidiIn implementation
 *
 *********************************************************************************/
 MidiIn::~MidiIn(){};
  MidiIn::MidiIn(): hdl(0), recording(false),_open(false), inHandler(0){
   
  };

  MIDIINHANDLE MidiIn::getHandle(){ return hdl;};
  
  void CALLBACK MidiIn::handleFunc(MIDIINHANDLE hdl, unsigned long msg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 ){
    MidiIn * self = (MidiIn *)(dwInstance);

    switch(msg){
      /* Receive a short message */
    case MIM_DATA: {
      self->inHandler->handleShortMsg((unsigned long)dwParam1, (unsigned long)dwParam2);
      break;
    }
      /* ERROR */
    case MIM_ERROR: {
      self->inHandler->onShortMsgError((unsigned long)dwParam1, (unsigned long)dwParam2);
      break;
    }
      /* Receive all or part of some System Exclusive message */
    case MIM_LONGDATA: {
      // handle the message ourselves
      self->inHandler->handleLongMsg((unsigned long)dwParam1, (unsigned long)dwParam2);    

      break;
    }
      /*  */
    case MIM_LONGERROR: {
      /*      MIDIBUFFER * lMsgbuf = (MIDIBUFFER *)((LPMIDIHDR)dwParam1)->dwUser;
      // get the size of the msg
      lMsgbuf->timestamp = (unsigned long) dwParam2;
      */
      // handle the message ourselves
      self->inHandler->onLongMsgError((unsigned long)dwParam1, (unsigned long)dwParam2);
    
      break;
    }

      /* A device is now open */
    case MIM_OPEN: {
      //      printf("CALLBACK: received open");
      break;
    }
      
      /* A device is now close */
    case MIM_CLOSE: {
      logPrintI("MIDICALLBACK: received close");
      break;
    }
      /*  */
    case MIM_MOREDATA: {
      //      printf("CALLBACK: received MOREDATA");
      break;
    }

    }
  }

  void MidiIn::openId(unsigned long id ){
    unsigned long err;
    if((err= midiInOpen(&hdl, (UINT)id, (DWORD)handleFunc, (DWORD)this, CALLBACK_FUNCTION))){

      throw MidiEx(midiErrorString(err));
    }
    _open = true;
  };



  void MidiIn::openStr(std::string devname){
    MIDIINCAPS inCaps;
    unsigned long devCount = midiInGetNumDevs();
    unsigned long devid = 0;
    
    //    printf("MIDI openning %s service found %u midi devices\n", devname.c_str(), devCount);

    for (unsigned long i = 0; i < devCount ; i++){

      unsigned long result =midiInGetDevCaps(i, &inCaps, sizeof(inCaps));
      if (result == MIDINOERROR){

	if (devname.compare(inCaps.szPname) == 0){
	  logPrintI("MIDI openning InputDevice\n\tMid=\t%d\n\tPid=\t%d\n\tDriverVersion=\t%u\n\tPname=\t%s\n", inCaps.wMid, inCaps.wPid, inCaps.vDriverVersion, inCaps.szPname);
	  devid = i;
	  break;
	}
      } else{
	MidiEx(midiErrorString(result));
      }
    }
    openId(devid);

  }

  bool MidiIn::isOpen(){
    return _open;
  };

  void MidiIn::close(){
    unsigned long err;
    while ((err = midiInClose(hdl)) == MIDISTILLPLAYING){
      stopRecording();
      Sleep(0);
    }

    if (err){
      throw MidiEx(midiErrorString(err));
    }
  };

  bool MidiIn::isRecording(){ return recording;};
  
  void MidiIn::startRecording(){
    unsigned long err = midiStartIn(hdl);
    if (err != MIDINOERROR){
      throw MidiEx(midiErrorString(err));
    }
    recording = true;
  };

  void MidiIn::stopRecording(){ 
    midiStopIn(hdl);
  };

  void MidiIn::setHandler(MidiInHandler * inHdl){
    inHandler = inHdl;
  }

  void MidiIn::addBuffer(MIDIBUFFER * buf){
    midiInPrepareBuffer(hdl, buf);
    midiInQueueBuffer(buf);

  }

void MidiIn::queueBuffer(MIDIBUFFER * buf){
  midiInQueueBuffer(buf);
}


/********************************************************************************
 *
 *                      MidiOut implementation
 *
 *********************************************************************************/


  MidiOut::MidiOut():_opened(false){};
  MidiOut::~MidiOut(){
    if (_opened){
      close();
    }
  };
  
  void MidiOut::close (){
    //logPrintE("Closing midi output\n");
    unsigned long err = midiOutClose(out);
    if (err != MIDINOERROR){
      logPrintE("Error closing out dev %s\n", (midiErrorString(err).c_str()));
    }
  };
  bool MidiOut::isOpen(){return _opened;};
  void MidiOut::openId(unsigned long devid){
    unsigned long result = midiOutOpen(&out, devid, 0, 0, CALLBACK_NULL);
    if (result == MIDINOERROR)
      _opened = true;
    else {
      throw MidiEx(midiErrorString(result));
    }
  };
  void MidiOut::openStr(std::string devName){
    MIDIOUTCAPS  outDevCaps ;
    unsigned long devCount = midiOutGetNumDevs();
    unsigned long devid = 0;

    for (unsigned long i = 0; i < devCount ; i++){

      unsigned long result = midiOutGetDevCaps(i, &outDevCaps, sizeof(outDevCaps));
      if (result == MIDINOERROR){
	if (devName.compare(outDevCaps.szPname) == 0){
	  logPrintI("MIDI openning OutDevice\n\tMid=\t%d\n\tPid=\t%d\n\tDriverVersion=\t%u\n\tPname=\t%s\n", outDevCaps.wMid, outDevCaps.wPid, outDevCaps.vDriverVersion, outDevCaps.szPname);
	  devid = i;
	  break;
	}
      }
    }
    openId(devid);
  }

  void MidiOut::sendShortMessage( unsigned long msg){
    unsigned long err = (midiOutShortMsg(out, msg));
    if (err != MIDINOERROR){
      throw MidiEx(midiErrorString(err));
    }
  };

  void MidiOut::sendSysExMessage( unsigned char * buf, unsigned long size){
    MIDIHDR midiHdr;
    midiHdr.lpData = (LPSTR)&buf[0];
    unsigned long err;
    // Store its size in the MIDIHDR 
    midiHdr.dwBufferLength = (DWORD)size;
    
    // Flags must be set to 0 
    midiHdr.dwFlags = 0;
    
    // Prepare the buffer and MIDIHDR 
    err = midiOutPrepareHeader(out, &midiHdr, sizeof(MIDIHDR));
    if (!err){
      err =  midiOutLongMsg(out, &midiHdr, sizeof(MIDIHDR));
      if (err){
	throw MidiEx(midiErrorString(err));
      }
    } else {
      throw MidiEx(midiErrorString(err)); 
    }

    while (MIDIERR_STILLPLAYING == midiOutUnprepareHeader(out, &midiHdr, sizeof(MIDIHDR)))
      {
	/* Delay to give it time to finish */
	Sleep(1000);
      }

  };



}; // namespace ot
//#endif //WIN32

#endif // USE_MIDI
