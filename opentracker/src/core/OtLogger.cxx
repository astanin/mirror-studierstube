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

#include <cassert>
#include <cstdarg>
#include <OpenTracker/core/OtLogger.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#ifdef WIN32
#include <windows.h>
#endif

namespace ot {

    /*
     * These are extra definitions mainly for convenience, so users dont have to call
     * an instance of the StbLogger singleton
     */
    void logPrint(const char *message, ...)
    {
        char tmpString[1024]; 
        va_list marker;

        va_start(marker, message);
        vsprintf(tmpString, message, marker);

        ot::OtLogger::getInstance()->printMessage(tmpString);
    }

    void logPrintD(const char *debugMessage, ...)
    {
        char tmpString[1024]; 
        va_list marker;

        va_start(marker, debugMessage);
        vsprintf(tmpString, debugMessage, marker);

        ot::OtLogger::getInstance()->printDebug(tmpString);
    }

    void logPrintS(const char *setupMessage, ...)
    {
        char tmpString[1024]; 
        va_list marker;

        va_start(marker, setupMessage);
        vsprintf(tmpString, setupMessage, marker);

        ot::OtLogger::getInstance()->printSetup(tmpString);
    }

    void logPrintI(const char *infoMessage, ...)
    {
        char tmpString[1024]; 
        va_list marker;

        va_start(marker, infoMessage);
        vsprintf(tmpString, infoMessage, marker);

        ot::OtLogger::getInstance()->printInfo(tmpString);
    }

    void logPrintW(const char *warningMessage, ...)
    {
        char tmpString[1024]; 
        va_list marker;

        va_start(marker, warningMessage);
        vsprintf(tmpString, warningMessage, marker);

        ot::OtLogger::getInstance()->printWarning(tmpString);
    }

    void logPrintE(const char *errorMessage, ...)
    {
        char tmpString[1024]; 
        va_list marker;

        va_start(marker, errorMessage);
        vsprintf(tmpString, errorMessage, marker);

        ot::OtLogger::getInstance()->printErrorAndContinue(tmpString);
    }

    void logPrintEAbort(const char *errorMessage, ...)
    {
        char tmpString[1024]; 
        va_list marker;

        va_start(marker, errorMessage);
        vsprintf(tmpString, errorMessage, marker);

        ot::OtLogger::getInstance()->printErrorAndAbort(tmpString);
    }

    /*
     * These are the actual class methods implementations of StbLogger
     */

    OtLogger*OtLogger::instance=NULL;

    OtLogger::OtLogger()
    {
        logMode = MODE_CONSOLE;
        logFileOwner = true;
        logFileName = new char[11];
        strcpy(logFileName,"ot_log.txt");        
    }

    OtLogger::~OtLogger()
    {
        if (logFileName && logFileOwner)
        {
            delete [] logFileName;
            logFileOwner = false;
        }
    }

    OtLogger *OtLogger::getInstance()
    {
        if(instance == NULL)
            instance = new OtLogger();

        return instance;
    }

    void OtLogger::setLogMode(LOG_MODE newMode) {
        logMode = newMode;
    }

    void OtLogger::setLogFileName(char * filename)
    {
        if (logFileName && logFileOwner) 
        {
            delete [] logFileName;
            logFileOwner = false;
        }

        logFileName = filename;
    }

    char * OtLogger::getLogFileName() 
    {
        return logFileName;
    }


    void OtLogger::printMessage(const char *message)
    {
        // We should be using ACE here
        switch (logMode)
        {
            case MODE_OFF:
                break;
            case MODE_FILE:
                writeToFile(message);
                break;
            case MODE_CONSOLE:
                printf("%s",message);
                break;
            default:
                break;
        }
    }

    void OtLogger::printDebug(const char *debugMessage)
    {
        // We should be using ACE here
#ifdef DEBUG
        switch (logMode)
        {
            case MODE_OFF:
                break;
            case MODE_FILE:
                writeToFileEx("OT |DEBUG: %s", debugMessage);
                break;
            case MODE_CONSOLE:
                ACE_DEBUG((LM_ERROR, ACE_TEXT("OT |DEBUG: %s", debugMessage)));
                //printf("OT |DEBUG: %s", debugMessage);
                break;
            default:
                break;
        }
#endif // STB_IS_DEBUG
    }

    void OtLogger::printSetup(const char *setupMessage)
    {
        // We should be using ACE here
        switch (logMode)
        {
            case MODE_OFF:
                break;
            case MODE_FILE:
                writeToFileEx("OT |SETUP: %s",setupMessage);
                break;
            case MODE_CONSOLE:
                printf("OT |SETUP: %s",setupMessage);
                break;
            default: break;
        }
    }

    void OtLogger::printInfo(const char *infoMessage)
    {
        // We should be using ACE here
        switch (logMode)
        {
            case MODE_OFF:
                break;
            case MODE_FILE:
                writeToFileEx("OT |INFO : %s",infoMessage);
                break;
            case MODE_CONSOLE:
                LOG_ACE_INFO("OT |INFO : %s",infoMessage);
                //printf("OT |INFO : %s",infoMessage);
                break;
            default:
                break;
        }
    }

    void OtLogger::printWarning(const char *warningMessage)
    {
        // We should be using ACE here
        switch (logMode)
        {
            case MODE_OFF:
                break;
            case MODE_FILE:
                writeToFileEx("OT |WARN : %s",warningMessage);
                break;
            case MODE_CONSOLE:
#ifdef WIN32
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
                LOG_ACE_ERROR("OT |WARN : ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                LOG_ACE_ERROR("%s",warningMessage);
#else
                /// Color changes for console text are platform dependent
                LOG_ACE_ERROR("OT |WARN : %s",warningMessage);
#endif
                break;
            default:
                break;
        }
    }

    void OtLogger::printErrorAndContinue(const char *errorMessage)
    {
        // We should be using ACE here
        switch (logMode)
        {
            case MODE_OFF:
                break;
            case MODE_FILE:
                writeToFileEx("OT |ERROR: %s",errorMessage);
                break;
            case MODE_CONSOLE:
#ifdef WIN32
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                LOG_ACE_ERROR("OT |ERROR: ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                LOG_ACE_ERROR("%s",errorMessage);
#else
                /// Color changes for console text are platform dependent
                LOG_ACE_ERROR("OT |ERROR: %s",errorMessage);
#endif
                break;
            default:
                break;
        }
    }

    void OtLogger::printErrorAndAbort(const char *errorMessage)
    {
        // We should be using ACE here
        printErrorAndContinue(errorMessage);

        // FIXME: Something more elegant here please. Mendez 20060316
        exit(0);
    }

    void OtLogger::writeToFile(const char * message) 
    {
        FILE* fp = fopen(getLogFileName(), "a");
        if(!fp)
            return;

        fprintf(fp, message);
        //if(message[strlen(message)-1]!='\n')
        //fputc('\n', fp);

        fclose(fp);
    }

    void OtLogger::writeToFileEx(const char* nStr, ...)
    {
        char tmpString[512];
        va_list marker;

        va_start(marker, nStr);
        vsprintf(tmpString, nStr, marker);

        writeToFile(tmpString);
    }

}

/* 
 * ------------------------------------------------------------
 *   End of OtLogger.cxx
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

