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
/** header file for the Ot Logger
 *
 * @author Erick Mendez
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */


#ifndef _OTLOGGER_H_
#define _OTLOGGER_H_

#ifndef SWIG
#include <cassert>
#endif

#include "../OpenTracker.h"

namespace ot {

    /*
     * Usage:  
     *        - logPrintE("File Not Found");
     *        - logPrintE(LOG_ERROR_FILE_NOT_FOUND);
     *        - logPrintE("File %s Not Found", filename);
     *
     */
    void OPENTRACKER_API logPrint(const char *message, ...);
    void OPENTRACKER_API logPrintD(const char *debugMessage, ...);
    void OPENTRACKER_API logPrintS(const char *setupMessage, ...);
    void OPENTRACKER_API logPrintI(const char *infoMessage, ...);
    void OPENTRACKER_API logPrintW(const char *warningMessage, ...);
    void OPENTRACKER_API logPrintE(const char *errorMessage, ...);
    void OPENTRACKER_API logPrintEAbort(const char *errorMessage, ...);
 
    /*
     * Enumerations
     */
    enum LOG_MODE
        {
            MODE_OFF = 0,
            MODE_FILE = 1,
            MODE_CONSOLE = 2,
            MODE_CALLBACK = 3
        };
 
    class OPENTRACKER_API OtLogger
    {
    public:
         
        /// The instance retriever, this is a singleton
        static OtLogger *getInstance();
             
        /// The destructor
        virtual ~OtLogger();
                 
        virtual void setLogMode(LOG_MODE newMode);
                     
        virtual void printMessage(const char *message);
        virtual void printDebug(const char *debugMessage);
        virtual void printSetup(const char *setupMessage);
        virtual void printInfo(const char *infoMessage);
        virtual void printWarning(const char *warningMessage);
        virtual void printErrorAndContinue(const char *errorMessage);
        virtual void printErrorAndAbort(const char *errorMessage);
                                                 
        virtual void writeToFile(const char * message);
        virtual void writeToFileEx(const char * format, ...);
                                                         
        virtual char * getLogFileName();
        virtual void setLogFileName(char * filename);
                                                                 
    protected:
        LOG_MODE logMode;
                                                                         
                                                                         
        /// The instance
        static OtLogger *instance;
                                                                             
        char * logFileName;
        bool logFileOwner;
                                                                                 
        /// The constructor
        OtLogger();
    };
 
} // namespace ot {

#endif //_OTLOGGER_H_

/* 
 * ------------------------------------------------------------
 *   End of OtLogger.h
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
