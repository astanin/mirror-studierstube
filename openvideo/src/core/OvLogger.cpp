/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The source file for the OvLogger class.
 *
 * @author Erick Mendez
 * 
 * @file                                                                   
 * ======================================================================= */

#include <openvideo/OvLogger.h>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>

namespace openvideo {

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

    OvLogger::getInstance()->printMessage(tmpString);
  }

  void logPrintD(const char *debugMessage, ...)
  {
    char tmpString[1024]; 
    va_list marker;

    va_start(marker, debugMessage);
    vsprintf(tmpString, debugMessage, marker);

    OvLogger::getInstance()->printDebug(tmpString);
  }

  void logPrintS(const char *setupMessage, ...)
  {
    char tmpString[1024]; 
    va_list marker;

    va_start(marker, setupMessage);
    vsprintf(tmpString, setupMessage, marker);

    OvLogger::getInstance()->printSetup(tmpString);
  }

  void logPrintI(const char *infoMessage, ...)
  {
    char tmpString[1024]; 
    va_list marker;

    va_start(marker, infoMessage);
    vsprintf(tmpString, infoMessage, marker);

    OvLogger::getInstance()->printInfo(tmpString);
  }

  void logPrintW(const char *warningMessage, ...)
  {
    char tmpString[1024]; 
    va_list marker;

    va_start(marker, warningMessage);
    vsprintf(tmpString, warningMessage, marker);

    OvLogger::getInstance()->printWarning(tmpString);
  }

  void logPrintE(const char *errorMessage, ...)
  {
    char tmpString[1024]; 
    va_list marker;

    va_start(marker, errorMessage);
    vsprintf(tmpString, errorMessage, marker);

    OvLogger::getInstance()->printErrorAndContinue(tmpString);
  }

  void logPrintEAbort(const char *errorMessage, ...)
  {
    char tmpString[1024]; 
    va_list marker;

    va_start(marker, errorMessage);
    vsprintf(tmpString, errorMessage, marker);

    OvLogger::getInstance()->printErrorAndAbort(tmpString);
  }

  /*
   * These are the actual class methods implementations of StbLogger
   */

  OvLogger*OvLogger::instance=NULL;

  OvLogger::OvLogger()
  {
    logMode = MODE_CONSOLE;
    logFileName = "ov_log.txt";
  }

  OvLogger::~OvLogger()
  {
  }

  OvLogger *OvLogger::getInstance()
  {
    if(instance == NULL)
      instance = new OvLogger();

    return instance;
  }

  void OvLogger::setLogMode(LOG_MODE newMode) {
    logMode = newMode;
  }

  void OvLogger::setLogFileName(char * filename)
  {
    logFileName = filename;
  }

  char * OvLogger::getLogFileName() 
  {
    return logFileName;
  }


  void OvLogger::printMessage(const char *message)
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

  void OvLogger::printDebug(const char *debugMessage)
  {
    // We should be using ACE here
#ifdef DEBUG
    switch (logMode)
      {
      case MODE_OFF:
	break;
      case MODE_FILE:
	writeToFileEx("OV |DEBUG: %s", debugMessage);
	break;
      case MODE_CONSOLE:
	printf("OV |DEBUG: %s", debugMessage);
	break;
      default:
	break;
      }
#endif // STB_IS_DEBUG
  }

  void OvLogger::printSetup(const char *setupMessage)
  {
    // We should be using ACE here
    switch (logMode)
      {
      case MODE_OFF:
	break;
      case MODE_FILE:
	writeToFileEx("OV |SETUP: %s",setupMessage);
	break;
      case MODE_CONSOLE:
	printf("OV |SETUP: %s",setupMessage);
	break;
      default: break;
      }
  }

  void OvLogger::printInfo(const char *infoMessage)
  {
    // We should be using ACE here
    switch (logMode)
      {
      case MODE_OFF:
	break;
      case MODE_FILE:
	writeToFileEx("OV |INFO : %s",infoMessage);
	break;
      case MODE_CONSOLE:
	printf("OV |INFO : %s",infoMessage);
	break;
      default:
	break;
      }
  }

  void OvLogger::printWarning(const char *warningMessage)
  {
    // We should be using ACE here
    switch (logMode)
      {
      case MODE_OFF:
	break;
      case MODE_FILE:
	writeToFileEx("OV |WARN : %s",warningMessage);
	break;
      case MODE_CONSOLE:
	printf("OV |WARN : %s",warningMessage);
	break;
      default:
	break;
      }
  }

  void OvLogger::printErrorAndContinue(const char *errorMessage)
  {
    // We should be using ACE here
    switch (logMode)
      {
      case MODE_OFF:
	break;
      case MODE_FILE:
	writeToFileEx("OV |ERROR: %s",errorMessage);
	break;
      case MODE_CONSOLE:
	printf("OV |ERROR: %s",errorMessage);
	break;
      default:
	break;
      }
  }

  void OvLogger::printErrorAndAbort(const char *errorMessage)
  {
    // We should be using ACE here
    printErrorAndContinue(errorMessage);

    // FIXME: Something more elegant here please. Mendez 20060316
    exit(0);
  }

  void OvLogger::writeToFile(const char * message) 
  {
    FILE* fp = fopen(getLogFileName(), "a");
    if(!fp)
      return;

    fprintf(fp, message);
    //if(message[strlen(message)-1]!='\n')
    //fputc('\n', fp);

    fclose(fp);
  }

  void OvLogger::writeToFileEx(const char* nStr, ...)
  {
    char tmpString[512];
    va_list marker;

    va_start(marker, nStr);
    vsprintf(tmpString, nStr, marker);

    writeToFile(tmpString);
  }

}
