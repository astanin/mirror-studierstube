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
/** Include file to make using DLL simpler.
 * The following ifdef block is the standard way of creating macros which
 * make exporting from a DLL simpler. All files within this DLL are
 * compiled with the OPENTRACKER_EXPORTS symbol defined on the command
 * line. this symbol should not be defined on any project that uses this
 * DLL. This way any other project whose source files include this file see
 * OPENTRACKER_API functions as being imported from a DLL, wheras this DLL sees
 * symbols defined with this macro as being exported.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: dllinclude.h 1977 2007-08-27 11:48:30Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _OPENTRACKER_H_INCLUDED
#define _OPENTRACKER_H_INCLUDED

#if defined(WIN32) || defined(_WIN32_WCE)
#pragma warning(disable:4786)
#pragma warning(disable:4251)
#pragma warning(disable:4996)
#pragma warning(disable:4290)
#pragma warning(disable:4244)
//#pragma warning(disable:4275)
//#pragma warning(disable:4305)
#ifdef PRESENCCIA_BUILD
#define OPENTRACKER_API
#else
#ifdef OPENTRACKER_STATIC
#define OPENTRACKER_API
#define OPENTRACKER_EXTERN extern
#else
#ifdef OPENTRACKER_EXPORTS
#define OPENTRACKER_API __declspec(dllexport)
#define OPENTRACKER_EXTERN
#else
#define OPENTRACKER_API __declspec(dllimport)
#define OPENTRACKER_EXTERN extern
#endif
#endif
#endif //PRESENCCIA_BUILD

# ifndef USING_SCONS
#   include "../../config_win.h"
# else // USING_SCONS
#   define ARTOOLKIT_UNFLIP_V
# endif // USING_SCONS
#else

#if HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef WIN32
#ifndef OT_NO_SPEECH_SUPPORT
#define OT_NO_SPEECH_SUPPORT
#endif
#endif

#define OPENTRACKER_API

#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of dllinclude.h
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
