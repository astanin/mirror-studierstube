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
 * @file   QtMouseEventCalibModule.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c QtMouseEventCalibModule
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */
/**
 * @page module_ref Module Reference
 * @section qtmouseeventcalibmodule QtMouseEventCalibModule
 *
 * Employed in the configuration of @ref qtmouseeventmodule. This module acts as
 * compatibility module during the Qt target application desktop screen calibration (see
 * @ref sec_otqt_configuration of the %OTQt manual). The module does not provide any
 * relevant functionality. Its config file XML element is eventually replaced with a @c
 * QtMouseEventConfig element by the calibration tool. All its XML attributes are passed
 * to the substitute.
 *
 * Has the following XML attributes:
 *
 * @li @c TrackingSystemScaleOneMeter one real-world meter in tracking system scale
 * (optional; default value: "1.0")
 * @li @c ScreenDepthFrontInMeter lenght of depth vector (z-axis) perpendicular
 * on the screen plane, spanning the mouse sensitive volume (screen cuboid) in
 * the front direction of the target desktop screen plane (optional; default
 * value: "0.01" - is 1 cm)
 * @li @c ScreenDepthBackInMeter lenght of depth vector (z-axis) perpendicular
 * on the screen plane, spanning the mouse sensitive volume (screen cuboid) in
 * the back direction of the target desktop screen plane (optional; default
 * value: "0.005" (yes, also a positive value!) - is 5 mm)
 *
 * An example element looks like this:
 *
@verbatim
<QtMouseEventCalibConfig TrackingSystemScaleOneMeter="1.0" />@endverbatim
 */

#ifndef OTQT_QTMOUSEEVENTCALIBMODULE_H
#define OTQT_QTMOUSEEVENTCALIBMODULE_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventModuleBase.h>
#include <string>

namespace ot {

//--------------------------------------------------------------------------------
/**
 * @class QtMouseEventCalibModule
 * @ingroup group_otqt_classes
 * @brief %Module (MECM) employed in calibration mode
 *
 * Dummy module which is employed in the calibration mode.
 *
 * @see calibration procedure impl in class @c OTQtMEMCalibProc
 *
 * @see calibration tool main method impl in otqt_mem_calib_main.cxx
 */
class OPENTRACKER_API QtMouseEventCalibModule : public QtMouseEventModuleBase {

public:
  /**
   * Does nothing.
   */
  QtMouseEventCalibModule() : QtMouseEventModuleBase() { };
  /**
   * Does nothing.
   */
  virtual ~QtMouseEventCalibModule() { };
private:
  QtMouseEventCalibModule(QtMouseEventCalibModule const & ref);
  QtMouseEventCalibModule operator = (QtMouseEventCalibModule const & ref);

public:

  ///// class NodeFactory interface
  // virtual Node * createNode( const std::string& name,  StringTable& attributes);

  ///// class Module interface
  // virtual void init(StringTable& attributes,  ConfigNode * localTree);
  // virtual void start();
  // virtual int stop();
  /**
   * Does nothing.
   */
  virtual void close();
  // virtual void pullState();
  // virtual void pushState();
};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_QTMOUSEEVENTCALIBMODULE_H


/* 
 * ------------------------------------------------------------
 *   End of QtMouseEventCalibModule.h
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
