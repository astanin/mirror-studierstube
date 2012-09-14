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
 * @file   OTQtMEMCalibProc.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c OTQtMEMCalibProc
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#ifndef OTQT_OTQTMEMCALIBPROC_H
#define OTQT_OTQTMEMCALIBPROC_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/OTQt.h>
#include <OpenTracker/otqt/OTQtConfigFileEditor.h>
#include <OpenTracker/otqt/QtAppScreen.h>
#include <string>

namespace ot {

//--------------------------------------------------------------------------------
/**
 * @class OTQtMEMCalibProc
 * @ingroup group_otqt_classes
 * @brief Calibration procedure implementation
 *
 * A class object is created specifying input and output (XML) configuration file
 * names. Provides the exec() method implementing the calibration procedure.
 *
 * The calibration mode is applied during the calibration routine. The purpose of the
 * calibration routine is to calculate and store the location and extent of the target
 * application screen relative to the position and orientation of the tracked 6-DOF ASPD.
 *
 * The class is initialized with an intermediate OpenTracker/OTQt XML configuration file.
 * The given configuration file is edited, thus the results of the calibration are written
 * to the file resulting in a final, applicable version. The class demands the given XML
 * file to content a valid data flow graph and specifications of the external sources
 * (tracked input devices) assigned to OTQt sink nodes.  Another mandatory condition is
 * the existance of an @c QtMouseEventCalibConfig element nested in the XML @c
 * \<configuration\> section.
 */
class OPENTRACKER_API OTQtMEMCalibProc : public OTQt {
protected:
  /// maximal time to wait for signal in input device (ASPD, MPD) tracking test
  static int const DEV_TRACKING_MAX_TIMEOUT_MSEC = 5000;
  /// timeout between button press and final acceptance of a marked screen corner position
  static int const CORNER_ACCEPTANCE_TIMEOUT_MSEC = 2000;

public:
  /**
   * Stores given input and output XML configuration file names. Input filename argument
   * is mandatory, output filename remains optional. In case the output filename string is
   * empty (by default) the resulting XML file of the calibration process will be printed
   * to console standard output.
   *
   * @param input_cfg_filename input XML configuration file name
   * @param output_cfg_filename output XML configuration file name
   *
   */
  OTQtMEMCalibProc(std::string input_cfg_filename, std::string output_cfg_filename = "");

  /**
   * Does nothing.
   */
  ~OTQtMEMCalibProc() { };

private:
  OTQtMEMCalibProc();
  OTQtMEMCalibProc(OTQtMEMCalibProc const & ref);
  OTQtMEMCalibProc operator = (OTQtMEMCalibProc const & ref);

public:

  /**
   * Implements the OTQt calibration procedure.
   *
   * Opens the input XML configuration file with an editor. Performs some basic syntax
   * checks on it, throwing exceptions if some criteria is not fulfilled. Initializes and
   * starts the OpenTracker context, parsing the XML file, creating the node graph. Errors
   * occuring during the parsing of the file are reported to console output by
   * OpenTracker. Performs some tracking tests on the mandatory input devices such as
   * ASPD, MPD, MBD.  If all of these devices are perceived correctly the actual
   * calibration routine is entered, otherwise throws an exception and quits.
   *
   * The calibration routine asks the user to mark the four corners of the target
   * application desktop screen in the following sequence: top left, top right, bottom
   * right, bottom left.
   *
   * Each screen corner is detected over the position of the MPD. The user has to direct
   * the MPD to the desired position and press the "left" button of the MBD.  The corner
   * is recognized by the tool after the MPD position remained stable for more than two
   * seconds and the "left" button remained pressed throughout that time. If the MPD
   * position leaves a certain threshold region during the measurement time, the timer is
   * restarted. Releasing the ``left'' MPD button also resets the timer.
   *
   * Once all four corner points of the desktop screen have been marked, the tool computes
   * the calibration data, edits the input XML file and prints the resulting output XML
   * file either to console or to the prior specified output destination file.
   *
   */
  void exec();

private:
  /**
   * Performs a basic syntax check. Returns true if all mandatory XML elements are
   * contained in the input XML file.
   * @return true if all mandatory XML elements are contained in imported file
   */
  bool isInputConfigFileValid() const;
  /**
   * Listens for tracking events originating from the ASPD. Returns false if no such event
   * is received after a timeout of five seconds. Otherwise the received event is passed
   * to the given Event argument and true is returned.
   *
   * @param event placeholder for first incoming ASPD tracking event
   *
   * @return true if ASPD is perceived within some time offset, else false
   */
  bool trackASPos(Event & event);
  /**
   * Listens for tracking events originating from the MPD. Returns false if no such event
   * is received after a timeout of five seconds. Otherwise the received event is passed
   * to the given Event argument and true is returned.
   *
   * @param event placeholder for first incoming MPD tracking event
   *
   * @return true if MPD is perceived within some time offset, else false
   */
  bool trackMPD(Event & event);

  /**
   * Listens for tracking events from the MPD. Does not return until both events, one @e
   * pressed, and one @e released event from the MPD button one (aka the left mouse
   * button) were received. If both events have been received the last tracking event is
   * passed to the given argument and true is returned.
   *
   * @param event placeholder for the "convincing" MBD tracking event
   *
   * @return true if MPD button no one was perceived with exactly one @e pressed and one
   * @e released event
   */
  bool trackMBDButtonOne(Event & event);

  /**
   * Tracks one AS corner. Starts timer as soon as MBD button one was pressed. If MPD
   * position remains stable within some threshold for more than two seconds, the MPD
   * position and corresponding ASPD pos/orient is passed to the given structure and it
   * returns true. Otherwise, if the corner tracking is interrupted by releasing the MBD
   * button one or the MPD position leaves the threshold region, the timer is reseted.
   *
   * @param corner placeholder for corner MPD position and corresponding ASPD
   * pos/orient
   * @return true if corner successfully tracked
   */
  bool trackASCorner(QtAppScreen::ASCorner & corner);

  /**
   * Main method for measuring one AS corner position. Throws an exception if tracking the
   * corner failed. Passes the MPD position of the corner and the corresponding ASPD
   * tracking state to the ASCorner struct argument. Takes a string description of the
   * current corner to be tracked.
   *
   * @param desc description of the current corner to be measured.
   * @param as_corner placeholder for corner MPD position and corresponding ASPD
   * pos/orient
   */
  void trackASCornerMain(char const * desc, QtAppScreen::ASCorner & as_corner);

  /**
   * Prompts for the press of the \<ENTER\> key.
   */
  void waitForEnterKey() const;

private:
  /// input XML configuration file name
  std::string input_cfg_filename_;
  /// output XML configuration file name
  std::string output_cfg_filename_;
  /// simple configuration file editor object
  OTQtConfigFileEditor editor_;
};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_OTQTMEMCALIBPROC_H

/*
 * ------------------------------------------------------------
 *   End of OTQtMEMCalibProc.h
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
