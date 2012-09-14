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
/** header file for MulticastInputModule module.
 *
 * @author Christopher Schmidt
 *
 * $Id: MulticastInputModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section MulticastInputModule MulticastInputModule
 * The MulticastInputModule is a device driver module for the optical tracking
 * system by ART Data. It uses @ref artdatatrackersource nodes to input data into
 * the tracking tree. It does something useful as soon as at least one @ref artdatatrackersource node is created.
 * It is configured using the configuration element 'ARTDataTrackerConfig'. This
 * element has the following attributes :
 * @li @c maxbodies  the maximal number of different objects tracked
 * @li @c port the port number to listen on, the ART tracker should be configured to send to this number
 * 
 * An example configuration element looks like this :
 * @verbatim
 <ARTDataTrackerConfig maxbodies="10" port="12346"/>@endverbatim
 *
 * Some hints to configuring the ART Tracker host to work with this driver :
 * @li When you calibrate the rigid body, you need to choose the "due to
 *     body" setting.
 * @li Go to the general settings and make sure they are set as displayed in this picture :
 *     @image html artsettings.jpg
 */

#ifndef _MulticastInputModule_H
#define _MulticastInputModule_H

#include "../dllinclude.h"
#include "../OpenTracker.h"

#include <vector>

#include "ARTDataTrackerSource.h"

class ACE_SOCK_Dgram;


#ifndef OT_NO_NETWORK_SUPPORT


namespace ot {

  typedef std::vector<Node::Ptr> NodeVector;


  /**
   * The module and factory to drive the ARTDataTrackersource nodes. It constructs
   * ARTDataTrackerSource nodes via the NodeFactory interface and pushes events into
   * the tracker tree according to the nodes configuration.
   * @author Christopher Schmidt
   * @ingroup input
   */
  class OPENTRACKER_API MulticastInputModule : public ThreadModule, public NodeFactory
    {
      // Members
	
    protected:
    
      /// list of ARTDataTrackerSource nodes in the tree
      NodeVector sources;
      /// port number
      int port;
      /// stop flag 
      int stop;
      /// pointer to socket class
      ACE_SOCK_Dgram * socket;
	
    private:

      // Methods
	
    protected:
      /**
       * Mainloop */
      void run();
	
    public:

      typedef struct StationRecord	// Structure for the Bodies
      {
	u_short valid;	// Flag is true if body is tracked 
	u_short id;		// Body ID taken from the Datagramm
	float position[3];			// Array for the position 	
	float orientation[3];		// Array for the Quaternion (calculated by MulticastInputModule) pushed in the Nodetree
	u_short buttons;	// Flag is true if body is tracked 
	float confidence;			// 
      } StationRecord;
	
      int		version;
      int		maxBodyNumber;
      int		receiveBufferSize;
      char	*receiveBuffer;

      /** constructor method. */
      MulticastInputModule();
	
      /** Destructor method, clears nodes member. */
      virtual ~MulticastInputModule();
    
      /** This method is called to construct a new Node. It compares
       * name to the ARTDataTrackerSource element name, and if it matches
       * creates a new ARTDataTrackerSource node.
       * @param name reference to string containing element name
       * @attributes refenrence to StringMap containing attribute values
       * @return pointer to new Node or NULL. The new Node must be
       *         allocated with new ! */
      virtual Node * createNode( const std::string& name, const StringTable& attributes);
	
	
      /**
       * This method is called after initialisation is finished and before the
       * main loop is started.*/
      virtual void start();
	
      /**
       * Close */
      virtual void close();
	
      /**
       * pushes events into the tracker tree. Checks all source nodes for
       * new events and pushes them into the tracker tree.
       */
      virtual void pushEvent();
	
      /**
       * initializes the MulticastInputModule. 
       * @param attributes StringMap of elements attribute values. Should be
       *        possibly , but is not for convenience.
       * @param localTree pointer to root of configuration nodes tree
       */
      virtual void init(StringTable& attributes, ConfigNode * localTree);
	
      void parseString(std::string input, StationRecord *recordTemp) ;

    };

  OT_MODULE(MulticastInputModule);

}  // namespace ot


#endif // OT_NO_NETWORK_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of MulticastInputModule.h
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
