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
 * @file   QtMouseEventModuleBase.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c QtMouseEventModuleBase
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventModuleBase.h>
#include <OpenTracker/otqt/OTQtLog.h>
#include <stdexcept>

namespace ot {

//--------------------------------------------------------------------------------
QtMouseEventModuleBase::QtMouseEventModuleBase()
  : Module(), NodeFactory(),
    state_(0),
    xml_attrib_table_(StringTable()),
    asp_sink_(NULL),
    mp_sink_(NULL),
    mb_sink_(NULL),
    mw_sink_(NULL)
{ }

//--------------------------------------------------------------------------------
QtMouseEventModuleBase::~QtMouseEventModuleBase()
{
  all_sinks_.clear();
  if (asp_sink_ != NULL) delete asp_sink_;
  if (mp_sink_ != NULL) delete mp_sink_;
  if (mb_sink_ != NULL) delete mb_sink_;
  if (mw_sink_ != NULL) delete mw_sink_;
}


//--------------------------------------------------------------------------------
void QtMouseEventModuleBase::init(StringTable & attributes, ConfigNode * localTree) {
  OTQT_DEBUG("QtMouseEventModuleBase()::init(): --- START.\n");

  // set state
  state_ |= CONFIGURED;

  // call OT base class
  Module::init(attributes, localTree);

  /////  store XML attributes

  // store entire table
  xml_attrib_table_ = attributes;
}

//--------------------------------------------------------------------------------
void QtMouseEventModuleBase::start()
{
  OTQT_DEBUG("QtMouseEventModuleBase()::start(): *** START with state = %d\n", state_);
  // do nothing if module was not configured
  if (!(state_ & CONFIGURED))
    return;

  // set state
  state_ |= INITIALIZED;

  ///// check module / sink conditions

  bool ok = true;
  if (asp_sink_ == NULL) {
    OTQT_ERROR("Missing <QtAppScreenPosSink /> Node in OT configuration.");
    ok = false;
  }
  if (mp_sink_ == NULL) {
    OTQT_ERROR("Missing <QtMousePositionSink /> Node in OT configuration.");
    ok = false;
  }
  if (mb_sink_ == NULL) {
    OTQT_ERROR("Missing <QtMouseButtonSink /> Node in OT configuration.");
    ok = false;
  }

  if (!ok) {
    state_ |= MEM_STOP;
    state_ &= ~INITIALIZED;
  }
  OTQT_DEBUG("QtMouseEventModuleBase()::start(): *** FINISHED with state = %d\n", state_);
}


//--------------------------------------------------------------------------------
int QtMouseEventModuleBase::stop()
{
  return (state_ & MEM_STOP);
}

//--------------------------------------------------------------------------------
void QtMouseEventModuleBase::close()
{
}

//--------------------------------------------------------------------------------
Node *
QtMouseEventModuleBase::createNode(const std::string& name, const StringTable& attributes)
{
  OTQT_DEBUG("QtMouseEventModuleBase()::createNode(): *** START with name = %s.\n", name.c_str());

  // Motivation for the CONFIGURED stage flag ::
  //
  // MEM and MECM are associated with equally named sinks (ASPD, MPD etc) and thus share
  // the same base class (this one). Both modules are registered in some OT
  // NodeFactory. The OT node creation procedure is designed greedy, meaning: the first
  // NodeFactory which returns non-NULL node object for a given XML element name wins the
  // race.
  //
  // Now - an example: in operation mode (thus the MEM is in charge) a ASPS should be
  // created (this method is called with a "QtAppScreenPosSink" node string).
  //
  // As the OT node creation procedure iterates through the entire list of registered
  // modules it is possible that the MECM is called prior to MEM and creates and returns
  // the ASPS. This is an error, because the MECM is not in charge in operation mode.
  //
  // Solution :: Only modules which are actually in use should be allowed to create
  // nodes. The best indicator for a module being "in use" is, if its init() method have
  // been called by OT (implying an XML entry in the <configuration> section). Thus,
  // modules are in CONFIGURED state iff they successfully passed their init() method.
  if (!(state_ & CONFIGURED))
    return NULL;

  ///// XML attributes for Nodes

  // copy provided attribute table
  StringTable node_xml_attribs(attributes);
  // add all attributes of the module
  KeyIterator it(xml_attrib_table_);
  while (it.hasMoreKeys()) {
    std::string key = it.nextElement();
    std::string value = xml_attrib_table_.get(key);
    node_xml_attribs.put(key, value);
  }

  ///// Match associated sink node names / Create node objects

  QtMouseEventSinkBase * node = NULL;

  if (name.compare("QtAppScreenPosSink") == 0) {
    asp_sink_ = new QtAppScreenPosSink(node_xml_attribs);
    node = asp_sink_;
    OTQT_DEBUG("QtMouseEventModuleBase()::createNode(): Created QtAppScreenPosSink.\n");
  }
  if (name.compare("QtMousePosSink") == 0) {
    mp_sink_ = new QtMousePosSink(node_xml_attribs);
    node = mp_sink_;
    OTQT_DEBUG("QtMouseEventModuleBase()::createNode(): Created QtMousePosSink.\n");
  }
  if (name.compare("QtMouseButtonSink") == 0) {
    mb_sink_ = new QtMouseButtonSink(node_xml_attribs);
    node = mb_sink_;
    OTQT_DEBUG("QtMouseEventModuleBase()::createNode(): Created QtMouseButtonSink.\n");
  }
  if (name.compare("QtMouseWheelSink") == 0) {
    mw_sink_ = new QtMouseWheelSink(node_xml_attribs);
    node = mw_sink_;
    OTQT_DEBUG("QtMouseEventModuleBase()::createNode(): Created QtMouseWheelSink.\n");
  }

  if (node != NULL) {
    // add node to node container
    all_sinks_.push_back(node);
  }

  return node;
}

//--------------------------------------------------------------------------------
void QtMouseEventModuleBase::resetPendingEventBitAllSinks()
{
  SinkContainer::const_iterator iter = all_sinks_.begin();
  for ( ; iter != all_sinks_.end(); iter++) {
    (*iter)->resetPendingEventBit();
  }
}

//--------------------------------------------------------------------------------
void QtMouseEventModuleBase::enableEventConsumeSignalAllSinks(bool enable)
{
  OTQT_DEBUG("QtMouseEventModuleBase::enableEventConsumeSignalAllSinks():"
             "-- START. enable = %i\n", enable);

  // SPECIAL: reset active button and wheel presses (within
  // applications that take its input tracking events from parents
  // nodes ot the OTQt sinks!!) at the time the event consumption is
  // activated.

  if (enable == true) {
      // reset all active button press events
      if (mb_sink_ != NULL) {
          Event event;
          event.setButton(0);
          mb_sink_->forwardEvent(event);
      }
      // reset all active wheel events 
      if (mw_sink_ != NULL) {
          Event event;
          event.setButton(0);
          mw_sink_->forwardEvent(event);
      }
  }
      
  // enable/disable event consume signal within all registered sinks
  SinkContainer::const_iterator iter = all_sinks_.begin();
  for ( ; iter != all_sinks_.end(); iter++) {
      (*iter)->enableState(QtMouseEventSinkBase::EVENT_CONSUME_SIGNAL, enable);
  }
}


//--------------------------------------------------------------------------------
QtAppScreenPosSink &
QtMouseEventModuleBase::getASPS()
{
  if (asp_sink_ == NULL) {
    throw std::runtime_error("QtMouseEventModuleBase::getASPS():"
                             "QtAppScreenPosSink not initialized (NULL).");
  }
  return *asp_sink_;
}

//--------------------------------------------------------------------------------
QtMousePosSink &
QtMouseEventModuleBase::getMPS()
{
  if (mp_sink_ == NULL) {
    throw std::runtime_error("QtMouseEventModuleBase::getASPS():"
                             "QtMousePosSink not initialized (NULL).");
  }
  return *mp_sink_;
}

//--------------------------------------------------------------------------------
QtMouseButtonSink &
QtMouseEventModuleBase::getMBS()
{
  if (mb_sink_ == NULL) {
    throw std::runtime_error("QtMouseEventModuleBase::getASPS():"
                             "QtMouseButtonSink not initialized (NULL).");
  }
  return *mb_sink_;
}

//--------------------------------------------------------------------------------
QtMouseWheelSink &
QtMouseEventModuleBase::getMWS()
{
  if (mw_sink_ == NULL) {
    throw std::runtime_error("QtMouseEventModuleBase::getASPS():"
                             "QtMouseWheelSink not initialized (NULL).");
  }
  return *mw_sink_;

}

} // namespace ot

#endif // USE_OTQT

/*
 * ------------------------------------------------------------
 *   End of QtMouseEventModuleBase.cxx
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
