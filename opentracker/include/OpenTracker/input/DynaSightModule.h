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
/** header file for DynaSightModule module. Version 1.02
 *
 * @author Alexander Schaelss
 *
 * $Id: DynaSightModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                    */
/* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section dynasightmodule DynaSightModule
 *
 * The DynaSightModule listens for data from the Origin Instruments DynaSight 
 * Sensor sent via serial port to the host and pushes the event updates into the 
 * tracker tree via @ref DynaSightSource nodes.
 *
 * The DynaSight input module was tested with only one target.
 *
 * It's configuration element has the following attributes:
 * @li @c device the serial port the tracker is connected to. 
 * @li @c lookat defines a point in the local tracker coordinate system (x y z) used
 *        for the calculation of the orientation data. if the option is not specified,
 *        the returned ortientation is always towards the z-axis.
 *
 * An example configuration element looks like this:
 * @verbatim
 <DynaSightConfig device="com1" lookat="0.0 0.8 -1.2"/>
 @endverbatim
*/

#ifndef _DYNASIGHTMODULE_H
#define _DYNASIGHTMODULE_H

#include "../OpenTracker.h"
#include "DynaSightSource.h"
#include "../misc/serialcomm.h"
#include <vector>


#ifndef OT_NO_DYNASIGHT_SUPPORT


namespace ot {

  /// maximum number of targets that can be processed
  const int DYNASIGHT_MAX_TARGETS = 8;
  /// maximum length of conversion packet buffer
  const int DYNASIGHT_PACKET_MAX_LENGTH = 8;
  /// maximum size of serial i/o buffer
  const int DYNASIGHT_COMM_BUFFER_SIZE = 4096;
  /// constant for conversion of target values to meter
  const float SCALE_TO_METER = 20000.0;


  /// status values for the tracker data
  enum TargetStatus { SEARCH=0, COAST=1, CAUTION=2, TRACK=3 };

    class DynaSightSource;


  typedef std::vector<DynaSightSource *> TargetVector;


  /**
   * The module and factory to drive the reception of DynaSight event updates.
   * It builds DynaSightSource nodes that insert data from the DynaSight into
   * the tracker tree. It uses the DynaSight Protocol and runs in a thread.
   * 
   * @author Alexander Schaelss
   * @ingroup input
   */
  class OPENTRACKER_API DynaSightModule : public ThreadModule, public NodeFactory
    {
      // members
    protected:
      /// vector containing all targets, which have an associated sourceNode
      TargetVector targets;

      /// flag to stop the thread
      bool stop;
    
      /// is TRUE if the serial port was opened
      bool serialportIsOpen;

      /// is TRUE if we have to calculate the orientation
      bool hasLookAt;

      /// x, y, z coordinate of point to look at [meter]
      float lookAtVector[3];

      /// port structure for the serial port data
      SerialPort port;

      // methods
    protected:
      /// this method is executed in its own thread and reads data from the DynaSight.
      virtual void run();

    public:    
      /** basic constructor */
      DynaSightModule();

      /** destructor */
      virtual ~DynaSightModule();

      /**
       * initializes the DynaSightModule module. 
       * @param attributes refenrence to StringTable containing attribute values
       * @param localTree pointer to root of configuration nodes tree
       */
      virtual void init(StringTable& attributes, ConfigNode * localTree);

      /** This method is called to construct a new Node. It compares
       * name to the DynaSightSource element name, and if it matches
       * creates a new DynaSightSource node.
       * @param name reference to string containing element name
       * @param attributes reference to StringTable containing attribute values
       * @return pointer to new Node or NULL. The new Node must be
       *         allocated with new!
       */
      virtual Node * createNode( const std::string& name, const StringTable& attributes);  
     
      /**
       * starts the processing thread. It is called after initialisation is done.
       */
      virtual void start();

      /**
       * closes the module and closes any communication and stops thread.
       */
      virtual void close();
    
      /**
       * pushes event information into the tree. It checks whether there is new
       * data form the DynaSightSource node, copies it into the nodes and calls
       * push on them.    
       */
      virtual void pushEvent();             
    };
  OT_MODULE(DynaSightModule);

} // namespace ot


#endif // OT_NO_DYNASIGHT_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of DynaSightModule.h
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
