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
 * @file   OTQtLog.h
 * @ingroup group_otqt_classes
 * @author Christian Pirchheim
 *
 * @brief  Logging macros (Unix) and functions (Win32)
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#ifndef OTQT_OTQTLOG_H
#define OTQT_OTQTLOG_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <cstdio>
#include <iostream>

/// global logging enable/disable switch
#define OTQT_LOG_ON           1

/// enable/disable logging to console
#define OTQT_LOG_TO_CONSOLE_ON 1
/// enable/disable logging to file
#define OTQT_LOG_TO_FILE_ON   0

#define OTQT_DEBUG_ON         0
#define OTQT_INFO_ON          1
#define OTQT_WARNING_ON       1
#define OTQT_ERROR_ON         1

#ifdef WIN32

void OPENTRACKER_API OTQT_DEBUG(const char* nString, ...);
void OPENTRACKER_API OTQT_INFO(const char* nString, ...);
void OPENTRACKER_API OTQT_WARNING(const char* nString, ...);
void OPENTRACKER_API OTQT_ERROR(const char* nString, ...);

#else

/// destination log file name
static char * const OTQT_LOGFILE_NAME = "/tmp/otqt.log";
static FILE * otqt_logfile = NULL;

/// log to specified stream and/or log destination file
#define OTQT_LOG(stdstream, format, ...) \
  if (OTQT_LOG_ON && OTQT_LOG_TO_CONSOLE_ON) { fprintf (stdstream, format, ## __VA_ARGS__); fflush(stdstream); } \
  if (OTQT_LOG_ON && OTQT_LOG_TO_FILE_ON) { \
    if (otqt_logfile == NULL) { \
      otqt_logfile = fopen(OTQT_LOGFILE_NAME, "w"); \
    } \
    fprintf (otqt_logfile, format, ## __VA_ARGS__); fflush(otqt_logfile); \
  }

#define OTQT_DEBUG(format, ...) \
  if (OTQT_DEBUG_ON) { OTQT_LOG(stderr, format, ## __VA_ARGS__); }

#define OTQT_INFO(format, ...) \
  if (OTQT_INFO_ON) { \
    OTQT_LOG(stdout, "INFO: "); \
    OTQT_LOG(stdout, format, ## __VA_ARGS__); \
  }

#define OTQT_WARNING(format, ...) \
  if (OTQT_WARNING_ON) { \
    OTQT_LOG(stdout, "WARNING: "); \
    OTQT_LOG(stdout, format, ## __VA_ARGS__); \
  }

#define OTQT_ERROR(format, ...) \
  if (OTQT_ERROR_ON) { \
    OTQT_LOG(stdout, "ERROR: "); \
    OTQT_LOG(stdout, format, ## __VA_ARGS__); \
  }

#endif // WIN32

#endif // USE_OTQT

#endif


/*
 * ------------------------------------------------------------
 *   End of OTQtLog.h
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
