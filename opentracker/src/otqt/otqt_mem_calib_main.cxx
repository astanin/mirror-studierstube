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
 * @file   otqt_mem_calib_main.cxx
 * @ingroup group_otqt_classes
 * @author Christian Pirchheim
 *
 * @brief  OTQt MEM calibration tool main method
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/OTQtMEMCalibProc.h>
#include <OpenTracker/otqt/OTQtLog.h>
#include <tclap/CmdLine.h>
#include <newmat10/include.h> // activates the RBD_COMMON namespace in myexcept.h
#include <newmat10/myexcept.h>
#include <stdexcept>

//--------------------------------------------------------------------------------
/**
 * Calibration command line tool main method.
 *
 * Parses command line arguments. Starts calibration procedure. Catches any exceptions and
 * reports error messages to console.
 *
 * The command line interface looks the following:
 *
@verbatim
USAGE:

   ./otqt-mem-calib  [-o <outputfile>] [--] [-v] [-h] <inputfile>


Where:

   -o <outputfile>,  --output <outputfile>
     (value required)  output filename

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   -v,  --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <inputfile>
     (required)  (value required)  input filename@endverbatim
 *
 * @return 0 if successful, -1 otherwise
 */
int main(int argc, char *argv[]) {

  // default return value (success)
  int ret = 0;

  try {

    // create command line
    TCLAP::CmdLine cmd("author: Christian Pirchheim, chrispi@sbox.tugraz.at", ' ', "0.1");
    // optional for both modes: print (default/final) configuration to given file
    TCLAP::ValueArg<std::string> output_arg("o", "output", "output filename",
                                            false, "", "outputfile");
    cmd.add(output_arg);
    TCLAP::UnlabeledValueArg<std::string> input_arg("input", "input filename", "", "inputfile");
    cmd.add(input_arg);
    // parse the commandline args
    cmd.parse(argc, argv);

    ///// DO CALIBRATION

    ot::OTQtMEMCalibProc proc(input_arg.getValue(), (output_arg.isSet()) ? output_arg.getValue() : "");
    proc.exec();

  }
  // exception from TCLAP lib (commandline parser)
  catch(TCLAP::ArgException & ex_tclap) {
    OTQT_ERROR("%s %s\n", ex_tclap.error().c_str(), ex_tclap.argId().c_str());
    ret = -1;
  }
#ifndef WIN32
  // exception from newmat lib
  catch (RBD_COMMON::Exception & ex_nummat) {
    OTQT_ERROR("%s\n", ex_nummat.what());
    ret = -1;
  }
 
#endif
  // all other exceptions
  catch (std::exception & ex) {
    OTQT_ERROR("%s\n", ex.what());
    ret = -1;
  }

#ifdef WIN32
   catch(...)
  {
	OTQT_ERROR("probably some tinyxml issue occured\n");
	ret = -1;
  }
#endif

  OTQT_INFO("Regular Exit!\n");
  return ret;
}

#else // USE_OTQT

#include <stdio.h>
//--------------------------------------------------------------------------------
/**
 * Fallback method in case the OTQt module was disabled. Does nothing.
 * @return Always -1.
 */
int main() {
  fprintf(stderr, "OTQt was disabled. Exiting..\n");
  return -1;
}

#endif // USE_OTQT

/* 
 * ------------------------------------------------------------
 *   End of otqt_mem_calib_main.cxx
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
