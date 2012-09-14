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

#  include <OpenTracker/input/MidiModule.h>
#  include <OpenTracker/input/MidiSource.h>
#  include <OpenTracker/input/MidiSink.h>
#  include <OpenTracker/tool/midi.h>
#  include <OpenTracker/core/Event.h>
#  include <vector>

#  define MIDISOURCE_ID 0
#  define MIDISINK_ID   1

namespace ot{

  OT_MODULE_REGISTER_FUNC(MidiModule){
    OT_MODULE_REGISTRATION_DEFAULT(MidiModule, "MidiConfig");

    //    Event::registerGenericTypeName((std::vector<unsigned char>*)NULL, "vector<unsigned char>");
    Event::registerGenericTypeName((MidiMsg*)NULL, "MidiMsg");
  }



  void MidiModule::cleanUpSrcs(){
      logPrintE("MIDI MODULE: deleting all sources\n");  
    for (MidiSrcDict::iterator i = srcDict.begin(); i != srcDict.end(); i++){
      MidiSource * src = (*i);
      logPrintE("MIDI MODULE: deleting src %p\n", src);  
      delete src;
    }
  }

  void MidiModule::cleanUpSinks(){
    for (MidiSinkDict::iterator i = sinkDict.begin(); i != sinkDict.end(); i++){
      MidiSink * sink = (*i);
      delete sink;
    }
  }


  MidiModule::MidiModule(){

  };

 MidiModule::~MidiModule(){
    cleanUpSinks();
    cleanUpSrcs();
    logPrintE("MIDI MODULE: deleting finished cleaning up\n");  
  };
    
  void MidiModule::init(StringTable & attributes, ConfigNode * localTree){

  };
  
  void MidiModule::close(){
    
  };
  
  void MidiModule::start(){
    for (MidiSrcDict::iterator i= srcDict.begin(); i != srcDict.end(); i++){
      (*i)->startRecording();
    }
    
  };
  
  void MidiModule::addNode(const Node *node, unsigned long data){
    Node * tmp = const_cast<Node *> (node);
    if (data == MIDISOURCE_ID)
        srcDict.push_back( dynamic_cast<MidiSource *>(tmp));
    else 
        sinkDict.push_back(dynamic_cast<MidiSink *>(tmp));
  };
  void MidiModule::removeNode(const Node*){};
  
  // NodeFactory interface
  Node * MidiModule::createNode( const std::string & name, StringTable & attributes){
    Node * result = 0;
	//printf("MIDIMODULE::CREATENODE creating a node %s\n", name.c_str());
    if (name.compare ("MidiSource") == 0){
      std::string device = attributes.get("device");
	//  printf("MIDIMODULE::CREATENODE using device %s\n", device.c_str());
      if (device.compare("")!=0){
	  MidiSource * node = new MidiSource(device);
	  addNode(node, (unsigned long)MIDISOURCE_ID);
	  result = node;
      }
    } else if(name.compare ("MidiSink") == 0){
      std::string device = attributes.get("device");
      if (device.compare("")!=0){
	MidiSink * node = new MidiSink;
	node->openStr(device);
	addNode(node, (unsigned long)MIDISINK_ID);
	result = node;
      }
    }

    return result;
  };
  
  void MidiModule::pushEvent(){
	  for (MidiSrcDict::iterator i = srcDict.begin(); i != srcDict.end(); i++){
		MidiSource * src = (*i);
		src->pushEvent();
	  }
};
}; // namespace ot

#endif //USE_MIDI
/* 
 * ------------------------------------------------------------
 *   End of MidiModule.cxx
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
