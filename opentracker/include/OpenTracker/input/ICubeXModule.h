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
/** header file for ICubeXModule.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <vector>
#include <map>
#include <string>

namespace ot{

  class ICubeXSource;
  class ICubeXSink;
  class ICubeXSensor;
  class ixMidiSocket;
  class ixPreset;

  class ICubeXModule : public Module, public NodeFactory{
  public:
    typedef std::vector<ICubeXSource *> ICubeXSourceDict;
    typedef std::vector<ICubeXSink *> ICubeXSinkDict;
    typedef std::vector<ICubeXSensor *> ICubeXSensorDict;
    typedef std::map<std::string, ixMidiSocket * > SocketDict;
    typedef std::map<std::string, ixPreset *> PresetDict;
  protected:
    ICubeXSourceDict srcs;
    ICubeXSinkDict   sinks;
    ICubeXSensorDict sensors;
    SocketDict sockets;
    PresetDict presets;
    

    void cleanUpSrcs();
    void cleanUpSinks();
    void cleanUpSensors();
    void cleanUpSockets();
    void loadPresetFile(std::string);
  public:
    ICubeXModule();
    virtual ~ICubeXModule();
    
    virtual void init(StringTable & attributes, ConfigNode * localTree);

    virtual void close();

    virtual void start();

    
    virtual void addNode(const Node *);
    virtual void removeNode(const Node*);

    // NodeFactory interface
    virtual Node * createNode( const std::string & name, StringTable & attributes);
    virtual void pushEvent();
  };

  OT_MODULE(ICubeXModule);

}; // namespace ot
