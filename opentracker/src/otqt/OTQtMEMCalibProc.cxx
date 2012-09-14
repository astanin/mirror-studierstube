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
 * @file   OTQtMEMCalibProc.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c OTQtMEMCalibProc
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/OTQtMEMCalibProc.h>
#include <OpenTracker/otqt/OTQtMath.h>
#include <OpenTracker/otqt/OTQtLog.h>
#include <stdexcept>
#include <cstdio>
#include <stdexcept>
#include <cmath>
#ifdef USE_QT4
#include <Qt/qdatetime.h>
#else
#include <qdatetime.h>
#endif

namespace ot {

//--------------------------------------------------------------------------------
OTQtMEMCalibProc::OTQtMEMCalibProc(std::string input_cfg_filename, std::string output_cfg_filename )
  : OTQt(),
    input_cfg_filename_(input_cfg_filename),
    output_cfg_filename_(output_cfg_filename),
    editor_(OTQtConfigFileEditor(input_cfg_filename))
{
}

//--------------------------------------------------------------------------------
void OTQtMEMCalibProc::exec() {

  QtAppScreen::CalibInputData calib_input_data;

  printf("\n");
  printf("Welcome to the OTQt Calibration Tool!\n");
  printf("Author: Christian Pirchheim (chrispi@sbox.tugraz.at)\n");
  printf("\n");
  printf("-- Input Data:\n");
  printf("OT configuration input file: %s\n", input_cfg_filename_.c_str());
  printf("OT configuration output file: %s\n",
         (output_cfg_filename_.empty()) ? "STDOUT" : output_cfg_filename_.c_str());
  printf("OT configuration DTD file: opentracker.dtd\n");
  printf("\n");
  printf("-- Usage:\n");
  printf("Quit anytime by pressing \"ctrl-c\"\n");
  printf("\n");
  waitForEnterKey();
  printf("\n");

  ///// open + test OT configuration file

  printf("** Checking OT input configuration file syntax\n");
  editor_.init();
  if (!isInputConfigFileValid()) {
    throw std::invalid_argument("OT Config file is invalid.");
  }
  printf(".. OK\n");

  printf("** Starting OpenTracker\n");
  ///// Initialize + Start OpenTracker
  init(input_cfg_filename_);
  printf(".. OK\n");

#define NEWMAT_EXCEPTION_TEST   0
#if defined(WIN32) && NEWMAT_EXCEPTION_TEST
  RowVector v(4); RowVector w(3);
  OTQtMath::crossProductR3(v,w);
#endif

  ///// Track ASP
  printf("** Tracking ASP (Qt Application Screen Position)\n");
  Event app_screen;
  int retries=0;
  while (!trackASPos(app_screen)) {
	if(retries>5) throw std::invalid_argument("Exeeded number of retries.");
    printf("Could not track Application Screen Position.\nMove target into tracking range.");
	printf("\n");
	waitForEnterKey();
	retries++;
  }
  printf("Tracked at: %f %f %f\n",
         app_screen.getPosition()[0], app_screen.getPosition()[1], app_screen.getPosition()[2]);
  printf(".. OK\n");
  calib_input_data.as_cs_root = app_screen;

  ///// Track MPD
  printf("** Tracking MPD (Mouse Position Device)\n");
  Event mouse_pos_obj;
  retries=0;
  while (!trackMPD(mouse_pos_obj)) {
	if(retries>5) throw std::invalid_argument("Exeeded number of retries.");
    printf("Could not track MPD (Mouse Position Device).\nMove target into tracking range.");
	printf("\n");
	waitForEnterKey();
	retries++;
  }
  printf("Tracked at: %f %f %f\n",
         mouse_pos_obj.getPosition()[0], mouse_pos_obj.getPosition()[1], mouse_pos_obj.getPosition()[2]);
  printf(".. OK\n");

  ///// Track MBD
  printf("** Tracking MBD (Mouse Button Device) Button One (aka \"Left Mouse Button\")\n");
  printf("(TASK): Press and release MBD Button One once ..\n");
  Event mbo_button_one;
  if (!trackMBDButtonOne(mbo_button_one)) {
    throw std::runtime_error("Could not track MBD (Mouse Button Device) Button One.");
  }
  printf(".. OK\n");

  printf("\n\n");

  // track SC top/left
  trackASCornerMain("top/left", calib_input_data.top_left);

  // track SC top/right
  trackASCornerMain("top/right", calib_input_data.top_right);

  // track SC top/right
  trackASCornerMain("bottom/right", calib_input_data.bottom_right);

  // track SC top/right
  trackASCornerMain("bottom/left", calib_input_data.bottom_left);

  ///// convert calibration input data

  // calib input -> calib output
  QtAppScreen::CalibOutputData calib_output_data;
  QtAppScreen::convert(calib_input_data, calib_output_data);

  // calib output -> string table

  StringTable attribs;
  QtAppScreen::convert(calib_output_data, attribs);

  ///// edit OT configuration file

  OTQtConfigFileEditor::LineList ins_lines;

  // kill lines including
  //
  // <QtMouseEventCalibConfig
  // [..]
  // />

  int kill_line_no_start = editor_.firstLineNoOfExpr("<QtMouseEventCalibConfig");
  int kill_line_no_end = editor_.firstLineNoOfExpr("/>", kill_line_no_start);
  for (int count = kill_line_no_start; count <= kill_line_no_end; count++) {
    if (!editor_.killLine(kill_line_no_start))
      throw logic_error("Killing XML element 'QtMouseEventCalibConfig' failed.");
  }

  // insert the following lines
  //
  // <QtMouseEventConfig>
  //   [ QtMouseEventCalibConfig attributes ]
  // </QtMouseEventConfig>
  int ins_mem_line_no = kill_line_no_start;
  int ins_as_line_no = -1;
  StringTable mecm_attribs = getMECM().getXMLAttribTable();
  int mecm_attrib_count = mecm_attribs.size();
  if (mecm_attrib_count != 0) {
    ins_lines.push_back("<QtMouseEventConfig");
    KeyIterator it_mecm(mecm_attribs);
    std::string spaces("  ");
    while (it_mecm.hasMoreKeys()) {
      std::string key = it_mecm.nextElement();
      std::string value = mecm_attribs.get(key);
      std::string attribute = spaces + key + "=\"" + value + "\"";
      ins_lines.push_back(attribute);
    }
    ins_lines.push_back(">");
    ins_as_line_no = ins_mem_line_no + 1 + mecm_attrib_count + 1;
  } else {
    ins_lines.push_back("<QtMouseEventConfig>");
    ins_as_line_no = ins_mem_line_no + 1;
  }
  ins_lines.push_back("</QtMouseEventConfig>");
  int const indent = 4;
  if (!editor_.insertLines(ins_mem_line_no, ins_lines, indent))
    throw logic_error("Inserting XML element 'QtMouseEventConfig' failed.");
  ins_lines.clear();

  // insert
  //
  // <QtAppScreen
  //   [ attributes ]
  // />
  ins_lines.push_back("<QtAppScreen");
  ins_lines.push_back("/>");
  if (!editor_.insertLines(ins_as_line_no, ins_lines, indent + 2))
    throw logic_error("Inserting XML element 'QtAppScreen' failed.");
  ins_lines.clear();
  // insert attributes
  KeyIterator it(attribs);
  OTQT_DEBUG("--- CalibOutputData ---\n");
  while (it.hasMoreKeys()) {
    std::string key = it.nextElement();
    std::string value = attribs.get(key);
    std::string attrib = key + "=\"" + value + "\"";
    OTQT_DEBUG("attrib = %s\n", attrib.c_str());
    ins_lines.push_back(attrib);
  }
  if (!editor_.insertLines(++ins_as_line_no, ins_lines, indent + 4))
    throw logic_error("Inserting XML attributes of element 'QtAppScreen' failed.");
  ins_lines.clear();


  ///// print final OT configuration file

  printf("\n");
  printf("OTQt calibration sucessful!\n");
  printf("OT configuraton file is written NOW.\n");
  printf("\n");
  waitForEnterKey();

  // write config file to stdout / to file
  if (!output_cfg_filename_.empty()) {
    editor_.writeToFile(output_cfg_filename_);
  } else {
    editor_.writeToStdOut();
  }
}

//--------------------------------------------------------------------------------
bool
OTQtMEMCalibProc::isInputConfigFileValid() const {
  bool ret = true;
  ret &= editor_.containsExpr("<QtMouseEventCalibConfig");
  ret &= editor_.containsExpr("<QtAppScreenPosSink");
  ret &= editor_.containsExpr("<QtMousePosSink");
  ret &= editor_.containsExpr("<QtMouseButtonSink");
  return ret;
}

//--------------------------------------------------------------------------------
bool
OTQtMEMCalibProc::trackMPD(Event & event) {
  QTime const MAX_TIME = QTime::currentTime().addMSecs(DEV_TRACKING_MAX_TIMEOUT_MSEC);
  while (QTime::currentTime() <= MAX_TIME) {
//    getMECM().resetPendingEventBitAllSinks();
    driveOT();
    if (getMECM().getMPS().isEventPending()) {
      event = getMECM().getMPS().getCurrentEvent();
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------
bool
OTQtMEMCalibProc::trackASPos(Event & event) {
  QTime const MAX_TIME = QTime::currentTime().addMSecs(DEV_TRACKING_MAX_TIMEOUT_MSEC);
  while (QTime::currentTime() <= MAX_TIME) {
//    getMECM().resetPendingEventBitAllSinks();
    driveOT();
    if (getMECM().getASPS().isEventPending()) {
      event = getMECM().getASPS().getCurrentEvent();
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------
bool
OTQtMEMCalibProc::trackMBDButtonOne(Event & event) {
  unsigned char test_mask = 0x00;
  while (true) {
//    getMECM().resetPendingEventBitAllSinks();
    driveOT();
    bool is_event_pending = getMECM().getMBS().isEventPending();
    OTQT_DEBUG("OTQtMEMCalibProc::trackMBDButtonOne(): +++ MBS.isEventPending() = %d\n", is_event_pending);
    if (is_event_pending) {
      if (getMECM().getMBS().buttonPressed(QtMouseButtonSink::LEFT_BUTTON)) {
        printf("MBD Button One pressed.\n");
        test_mask |= 0xF0;
      } else if (getMECM().getMBS().buttonReleased(QtMouseButtonSink::LEFT_BUTTON)) {
        printf("MBD Button One released.\n");
        test_mask |= 0x0F;
      }
      if (test_mask == 0xFF) {
        event = getMECM().getMBS().getCurrentEvent();
        return true;
      }
    } // if
    if (OTQT_DEBUG_ON) is_event_pending = getMECM().getMBS().isEventPending();
    OTQT_DEBUG("OTQtMEMCalibProc::trackMBDButtonOne(): --- MBS.isEventPending() = %d\n", is_event_pending);
  } // while
  return false;
}

//--------------------------------------------------------------------------------
void OTQtMEMCalibProc::trackASCornerMain(char const * desc, QtAppScreen::ASCorner & as_corner) {
  printf("** Tracking Screen Corner: %s\n", desc);
  printf("(TASK): Guide MPD to %s screen corner and keep MBD Button One pressed when ready ..\n", desc);
  if (!trackASCorner(as_corner)) {
    std::string err_msg = "Failed tracking screen corner ";
    err_msg += std::string(desc) + std::string(".");
    throw std::runtime_error(err_msg.c_str());
  }
  printf("Tracked at: %f %f %f (ASP: %f %f %f)\n",
         as_corner.corner.getPosition()[0], as_corner.corner.getPosition()[1], as_corner.corner.getPosition()[2],
         as_corner.local_cs_root.getPosition()[0], as_corner.local_cs_root.getPosition()[1], as_corner.local_cs_root.getPosition()[2]);
  printf(".. OK\n");
  printf("\n");
  if (getMECM().getMBS().buttonOn(QtMouseButtonSink::LEFT_BUTTON)) {
    printf("Release MBD Button One to continue ..");
    fflush(stdout);
    while (!getMECM().getMBS().buttonReleased(QtMouseButtonSink::LEFT_BUTTON)) {
//      getMECM().resetPendingEventBitAllSinks();
      driveOT();
    }
    printf("\n\n");
  }
}

//--------------------------------------------------------------------------------
bool OTQtMEMCalibProc::trackASCorner(QtAppScreen::ASCorner & as_corner) {

  OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): *** START.\n");

  // init timer
  QTime timer;
  // number of cycles while timer is running
  unsigned int cycle_counter = 0;
  // corner tracking trigger flag
  bool is_button_one_pressed = false;

  // one meter in tracking scale
  float tracking_one_meter;
  getMECM().getXMLAttribTable().get("TrackingSystemScaleOneMeter", &tracking_one_meter);
  // converted to 3mm radius
  float THRESH_SPHERE_RADIUS = tracking_one_meter * 0.03;

  while (true) {

#define SINGLE_CORNER_TEST    0
#define CONVERT_DATA_TEST     0
#if SINGLE_CORNER_TEST
    sleep(1);
#endif
//    getMECM().resetPendingEventBitAllSinks();
    driveOT();
    OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): +++ CYCLE STARTED.\n");

    ///// check MBD button one

    if (getMECM().getMBS().isEventPending()) {
      OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): MBS is pending\n");
      // start timer / set corner position
      if (getMECM().getMBS().buttonPressed(QtMouseButtonSink::LEFT_BUTTON)) {
        printf("Button One pressed: tracked corner position, started timer.\n");
        // start timer
        timer.start();
        cycle_counter = 1;
        // store corner data
        as_corner.local_cs_root = getMECM().getASPS().getCurrentEvent();
        as_corner.corner = getMECM().getMPS().getCurrentEvent();
        is_button_one_pressed = true;
        continue;
      }
      // stop timer / release corner
      if (getMECM().getMBS().buttonReleased(QtMouseButtonSink::LEFT_BUTTON)) {
        printf("Button One released: stopped timer.\n");
        is_button_one_pressed = false;
        // reset timer
        timer = QTime();
        cycle_counter = 0;
        continue;
      }
    }

#if CONVERT_DATA_TEST
#else
    // return if trigger not enabled
    if (!is_button_one_pressed) {
      continue;
    }
#endif

    // do not test distance by default
    bool do_distance_check = false;

    ///// check MPD

    // NOTE: as_corner = [ prev_local_cs_root, prev_corner ]

    Event curr_corner;
    if (getMECM().getMPS().isEventPending()) {
      OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): MPS is pending\n");
      curr_corner = getMECM().getMPS().getCurrentEvent();
      do_distance_check = true;
    } else {
      curr_corner = as_corner.corner;
    }
    OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): curr_corner = %f %f %f\n",
               curr_corner.getPosition()[0], curr_corner.getPosition()[1], curr_corner.getPosition()[2]);

    ///// check ASPD

    // convert previous corner so that it is compareable with current corner
    Event prev_cmpable_curr_corner;
    if (getMECM().getASPS().isEventPending()) {
      OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): ASP is pending\n");
      Event curr_local_cs_root = getMECM().getASPS().getCurrentEvent();
      OTQtMath::transformVectorFromCSToCS(as_corner.local_cs_root, curr_local_cs_root,
                                          as_corner.corner, prev_cmpable_curr_corner);
      do_distance_check = true;
    } else {
      prev_cmpable_curr_corner = curr_corner;
    }
    OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): prev_cmpable_curr_corner = %f %f %f\n",
               prev_cmpable_curr_corner.getPosition()[0], prev_cmpable_curr_corner.getPosition()[1], prev_cmpable_curr_corner.getPosition()[2]);

    // check if current event is within threshold sphere around previous event
    if (do_distance_check) {
      // compare distance between current and previous corner with threshold radius
      float const curr_prev_distance = OTQtMath::distance(prev_cmpable_curr_corner.getPosition(),
                                                          curr_corner.getPosition());
      OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): distance check, curr_prev_distance = %f\n", curr_prev_distance);
      if (curr_prev_distance >= THRESH_SPHERE_RADIUS) {
        OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): curr outside of THRESH_SPHERE_RADIUS\n");
        // reset timer
        printf("Corner position out of bounds: tracked new corner position, restarted timer.\n");
        timer.restart();
        cycle_counter = 1;
      }
      // set corner data
      as_corner.local_cs_root = getMECM().getASPS().getCurrentEvent();
      as_corner.corner = getMECM().getMPS().getCurrentEvent();
    }

    ///// check termination OK criteria

#if CONVERT_DATA_TEST
    return true;
#else
    if (timer.elapsed() >= CORNER_ACCEPTANCE_TIMEOUT_MSEC) {
      OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): *** FINISHED with ret = true.\n");

      // TODO: compute mean sum of corner position

      return true;
    }
#endif
  } // while END

  OTQT_DEBUG("OTQtMEMCalibProc::trackASCorner(): *** FINISHED with ret = false.\n");
  return false;
}

//--------------------------------------------------------------------------------
void OTQtMEMCalibProc::waitForEnterKey() const {
  char ch = '\0';
  printf("Press ENTER to continue ..");
  fflush(stdout);
  while (!(ch == '\n' || ch == '\r'))
    ch = (char)getchar();
  return;
}

} // namespace ot

#endif // USE_OTQT


/*
 * ------------------------------------------------------------
 *   End of OTQtMEMCalibProc.cxx
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
