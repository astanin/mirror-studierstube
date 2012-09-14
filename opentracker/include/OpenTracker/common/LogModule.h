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
/** header file for LogModule.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section logmodule LogModule
 * The LogModule controls the output of log information that OpenTracker creates.
 * Possible targets are stderr and a given file. Logging information can be of
 * different verbosity levels including timestamp and host information and can 
 * be of different severity levels to only output important messages. 
 * The configuration element is called @c LogConfig and has the following attributes :
 * @li @c active (on|off) on controls if any log information should be output
 * @li @c stderr (on|off) on controls if logs should be send to stderr
 * @li @c file   (on|off) off controls if logs should be send to a file - does not work on WinCE
 * @li @c filename the name of the log file 
 * @li @c verbose (full|lite|std) std verbosity levels output information with timestamps and host info. See 
 *     ACE documentation for details.
 * @li @c level (debug|info|error) info severity levels, map somehow to the ACE levels
 * 
 * An example configuration element looks like this :
 * @verbatim
 <LogConfig active="on" stderr="on" file="on" filename="log.txt" level="info" verbose="std"/>@endverbatim
*/

#ifndef _LOGMODULE_H
#define _LOGMODULE_H

#include "../OpenTracker.h"

namespace ot {

  /** A simple module that modifies the ACE_LOG framework to provide some different logging options. 
   * @author Gerhard Reitmayr
   * @ingroup common
   */
  class OPENTRACKER_API LogModule : public Module
    {
    public:
      LogModule(void);

      void init( StringTable & attributes,  ConfigNode * localTree);
    };

} // namespace

#endif // _LOGMODULE_H
