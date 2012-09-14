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
 * @file   OTQtLog.cxx
 * @author Christian Pirchheim
 *
 * @brief  Logging functions (Win32) source file
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#ifdef WIN32

#include <OpenTracker/otqt/OTQtLog.h>
#include <ace/Log_Msg.h>
#include <string>

/// destination log file name
extern OPENTRACKER_API char * const OTQT_LOGFILE_NAME = "C:/Windows/Temp/OTQt.log";
extern OPENTRACKER_API FILE * otqt_logfile = NULL;

//--------------------------------------------------------------------------------
void OPENTRACKER_API log_to_file(char const * log_msg)
{
	if (!OTQT_LOG_ON || !OTQT_LOG_TO_FILE_ON) return;
	if (otqt_logfile == NULL) {
    otqt_logfile = fopen(OTQT_LOGFILE_NAME, "w");
  }
  fprintf (otqt_logfile, log_msg);
	fflush(otqt_logfile);
}

//--------------------------------------------------------------------------------
void OPENTRACKER_API OTQT_DEBUG(const char* nString, ...)
{
	if (!OTQT_LOG_ON || !OTQT_LOG_TO_CONSOLE_ON) return;
	if (!OTQT_DEBUG_ON) return;

	char* str = new char[8192];

	va_list marker;
	va_start(marker, nString);
	vsprintf(str, nString, marker);

	ACE_ERROR((LM_ERROR, str ));
	log_to_file(str);
	delete str;
}

//--------------------------------------------------------------------------------
void OPENTRACKER_API OTQT_INFO(const char* nString, ...)
{
	if (!OTQT_LOG_ON || !OTQT_LOG_TO_CONSOLE_ON) return;
	if (!OTQT_INFO_ON) return;

	char* str = new char[8192];

	va_list marker;
	va_start(marker, nString);
	vsprintf(str, nString, marker);

  std::string msg("INFO: ");
  msg += str;
	ACE_DEBUG((LM_INFO, msg.c_str()));
	log_to_file(str);
	delete str;
}

//--------------------------------------------------------------------------------
void OPENTRACKER_API OTQT_WARNING(const char* nString, ...)
{
	if (!OTQT_LOG_ON || !OTQT_LOG_TO_CONSOLE_ON) return;
	if (!OTQT_WARNING_ON) return;

	char* str = new char[8192];

	va_list marker;
	va_start(marker, nString);
	vsprintf(str, nString, marker);

  std::string msg("WARNING: ");
  msg += str;
	ACE_DEBUG((LM_INFO, msg.c_str()));
	log_to_file(str);
	delete str;
}

//--------------------------------------------------------------------------------
void OPENTRACKER_API OTQT_ERROR(const char* nString, ...)
{
	if (!OTQT_LOG_ON || !OTQT_LOG_TO_CONSOLE_ON) return;
	if (!OTQT_ERROR_ON) return;

	char* str = new char[8192];

	va_list marker;
	va_start(marker, nString);
	vsprintf(str, nString, marker);

  std::string msg("ERROR: ");
  msg += str;
	ACE_DEBUG((LM_INFO, msg.c_str()));
	log_to_file(str);
	delete str;
}

#endif // WIN32

#endif // USE_OTQT


/* 
 * ------------------------------------------------------------
 *   End of OTQtLog.cxx
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
