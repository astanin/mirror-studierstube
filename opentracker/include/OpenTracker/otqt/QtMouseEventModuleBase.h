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
/**
 * @file   QtMouseEventModuleBase.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c QtMouseEventModuleBase
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#ifndef OTQT_QTMOUSEEVENTMODULEBASE_H
#define OTQT_QTMOUSEEVENTMODULEBASE_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtAppScreenPosSink.h>
#include <OpenTracker/otqt/QtMousePosSink.h>
#include <OpenTracker/otqt/QtMouseButtonSink.h>
#include <OpenTracker/otqt/QtMouseWheelSink.h>
#include <OpenTracker/core/Module.h>
#include <OpenTracker/core/NodeFactory.h>
#include <OpenTracker/core/MathUtils.h>
#include <list>

namespace ot {

//--------------------------------------------------------------------------------
/**
 * @class QtMouseEventModuleBase
 * @ingroup group_otqt_classes
 * @brief Mouse Event Modules (MEMs) base class
 *
 * Implements an OpenTracker module. The initialization workflow of an OpenTracker module
 * looks the following: The OTQt::init() method creates a module class object and
 * registers the module at an OpenTracker context factory with under a unique XML element
 * name.
 *
 * The XML element can be put into the @c \<configuration\> section of an OpenTracker
 * configuration file. Parsing the XML DOM tree, detecting the module XML element,
 * OpenTracker calls the init() method of the corresponding module.
 *
 * During the parsing procedure of the XML node elements (forming the data flow graph) the
 * createNode() method of each module is called by OpenTracker. Once a module feels
 * responsible and returns a node object, OpenTracker continues with the next XML node
 * element. This procedure is applied until the entire XML config file is parsed and the
 * data flow graph has beeen created. The completition of the data flow graph is reported
 * to the module by calling start().
 *
 * Afterwards, the OpenTracker data flow must be driven. OTQt calls one such execution of
 * the data flow graph a \c cycle. The procedure of one cylce is wrapped by the
 * OTQt::driveOT() method. Within one cycle, each module is asked if it demands
 * OpenTracker to close the data flow cyle (see stop()). If some module wants OpenTracker
 * to shoot down, it does give each module the possibility to perform some shutdown steps
 * in close(). Otherwise the actual cycle is performed by first calling the pushState()
 * method, followed by calling the pullState() method.  With the first call, the module
 * requests associated source nodes to push pending tracking events to their
 * parents. Within the latter method events received by associated sink nodes can be
 * processed.
 */
class OPENTRACKER_API QtMouseEventModuleBase : public Module, public NodeFactory {

protected:
  /// object state flag collection: each flags is either enabled (equals true) or disabled
  enum {
    /// enabled if module was configured (see init() method)
    CONFIGURED = 0x01,
    /// enabled if module was initialized (see start() method)
    INITIALIZED = 0x02,
    /// enabled if module requests OpenTracker main loop to stop
    MEM_STOP = 0x04,
    /// mouse event dispatch to the target Qt application in operation mode either enabled
    /// or disabled
    DISPATCH_QT_EVENTS = 0x08
  } StateFlag;
  /// type of the registered sink list
  typedef std::list<QtMouseEventSinkBase *> SinkContainer;
  /// type of the state bit string
  typedef unsigned int state_t;

public:
  /**
   * Initializes the object as OpenTracker Module and NodeFactory, all sink pointers to
   * NULL, and all state flags to disabled (false).
   */
  QtMouseEventModuleBase();
  /**
   * Clears the list of sinks. Deletes all sink objects and .
   */
  virtual ~QtMouseEventModuleBase();

private:
  QtMouseEventModuleBase(QtMouseEventModuleBase const & ref);
  QtMouseEventModuleBase  operator=(QtMouseEventModuleBase const & ref);

public:

  ///// class NodeFactory interface

  /**
   * Returns NULL if the @c CONFIGURED state is not enabled. If the given XML element name
   * matches one of the dedicated sink names, creates a corresponding sink object.
   * Extends the given attribute key/value map with all module attributes, passing the
   * resulting attribute map to the sink constructor. Stores the new sink object in the
   * registered sink list and returns the sink object.
   *
   * @param name XML element name
   * @param attributes XML attribute key/value map
   *
   * @return NULL if given name does not match any dedicated sink name, otherwise the
   * created sink object
   */
  virtual Node * createNode(const std::string& name, const StringTable& attributes);


  ///// class Module interface

  /**
   * Stores the given attribute table of the XML module configuration element. Enables the
   * @c CONFIGURED state.
   *
   * @param attributes XML attribute key/value map
   * @param localTree current XML element and its nested elements
   */
  virtual void init(StringTable& attributes,  ConfigNode * localTree);
  /**
   * Returns immediately if @c CONFIGURED state is disabled. Otherwise, verifies whether
   * all mandatory sinks (ASPD, MPD) have been initialized correctly during the XML
   * configuration file parsing procedure. If the verfication result yields valid, enables
   * the @c INITIALIZED state flag. Otherwise enables the @c STOP state flag. Prints error
   * messages to stderr stream for each non-successful check.
   */
  virtual void start();
  /**
   * Return true if the @c STOP state flag is enabled, thus the module requests the
   * OpenTracker loop to stop.
   *
   * @return true if @c STOP state flag is enabled
   */
  virtual int stop();
  /**
   * Does nothing.
   */
  virtual void close();
  /**
   * Does nothing.
   */
  virtual void pullEvent() { };
  /**
   * Does nothing.
   */
  virtual void pushEvent() { };
  /**
   * Enables or disables the dispatch of mouse events (see state flag @c
   * DISPATCH_QT_EVENTS) to the Qt target application depending on given boolean
   * value. Returns the current state after the update.
   *
   * @param enable true if enable, false if disable is requested
   * @return current mouse event dispatch state (true equals enabled)
   */
  bool enableQtEventDispatch(bool enable) {
    if (enable) state_ |= DISPATCH_QT_EVENTS;
    else state_ &= ~DISPATCH_QT_EVENTS;
    return (state_ & DISPATCH_QT_EVENTS);
  }

  ///// Sink Getters

  /**
   * Throws an exception if ASPS object was not created previously. Otherwise returns the
   * ASPS object.
   * @return ASPS object
   */
  QtAppScreenPosSink & getASPS();
  /**
   * Throws an exception if MPS object was not created previously. Otherwise returns the
   * MPS object.
   * @return MPS object
   */
  QtMousePosSink & getMPS();
  /**
   * Throws an exception if MBS object was not created previously. Otherwise returns the
   * MBS object.
   * @return MBS object
   */
  QtMouseButtonSink & getMBS();
  /**
   * Throws an exception if MWS object was not created previously. Otherwise returns the
   * MWS object.
   * @return MWS object
   */
  QtMouseWheelSink & getMWS();

  /**
   * Resets the @e pending @e event flag bits of all registered sinks to false.
   */
  void resetPendingEventBitAllSinks();
  /**
   * Enables/Disables the @e event @e consume @e signal of all registered sinks depending
   * of the given boolean value. The signal typically stays enabled while the MPD resides
   * inside the screen cuboid, thus the MEM consumes the tracking events by generating Qt
   * mouse events.
   *
   * @param enable true to enable, false to disable the signal
   */
  void enableEventConsumeSignalAllSinks(bool enable);
  /**
   * Returns the XML attribute key/value map.
   * @return XML attribute key/value map
   */
  inline StringTable & getXMLAttribTable() { return xml_attrib_table_; };

protected:
  /// state bit string
  state_t state_;
  /// XML attribute key/value map
  StringTable xml_attrib_table_;

  /// ASPS object
  QtAppScreenPosSink * asp_sink_;
  /// MPS object
  QtMousePosSink * mp_sink_;
  /// MBS object
  QtMouseButtonSink * mb_sink_;
  /// MWS object
  QtMouseWheelSink * mw_sink_;

  /// list of all sinks nodes created in response to corresponding XML configuration
  /// elements (registered sink list)
  SinkContainer all_sinks_;

};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_QTMOUSEEVENTMODULEBASE_H


/*
 * ------------------------------------------------------------
 *   End of QtMouseEventModuleBase.h
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
