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
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The source file for the StbLogger class
*
* @author Erick Mendez
*
* @file                                                                   */
/* ======================================================================= */

#include <stb/base/fixWinCEIssues.h>
#include <stb/kernel/StbLogger.h>
#include <stb/base/OS.h>
#include <stdarg.h>

#ifdef WIN32
#include <windows.h>
#endif

BEGIN_NAMESPACE_STB

/*
 * These are extra definitions mainly for convenience, so users dont have to call
 * an instance of the StbLogger singleton
 */
void logPrint(stb::string message)
{
	StbLogger::getInstance()->printMessage(message.c_str());
}

void logPrint(const char *message, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, message);
	vsprintf(tmpString, message, marker);

	StbLogger::getInstance()->printMessage(tmpString);
}

void logPrintD(stb::string debugMessage)
{
	StbLogger::getInstance()->printDebug(debugMessage.c_str());
}

void logPrintD(const char *debugMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, debugMessage);
	vsprintf(tmpString, debugMessage, marker);

	StbLogger::getInstance()->printDebug(tmpString);
}

void logPrintS(stb::string setupMessage)
{
	StbLogger::getInstance()->printSetup(setupMessage.c_str());
}

void logPrintS(const char *setupMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, setupMessage);
	vsprintf(tmpString, setupMessage, marker);

	StbLogger::getInstance()->printSetup(tmpString);
}

void logPrintI(stb::string infoMessage)
{
	StbLogger::getInstance()->printInfo(infoMessage.c_str());
}

void logPrintI(const char *infoMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, infoMessage);
	vsprintf(tmpString, infoMessage, marker);

	StbLogger::getInstance()->printInfo(tmpString);
}

void logPrintW(stb::string warningMessage)
{
	StbLogger::getInstance()->printWarning(warningMessage.c_str());
}

void logPrintW(const char *warningMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, warningMessage);
	vsprintf(tmpString, warningMessage, marker);

	StbLogger::getInstance()->printWarning(tmpString);
}

void logPrintE(stb::string errorMessage)
{
	StbLogger::getInstance()->printErrorAndContinue(errorMessage.c_str());
}

void logPrintE(const char *errorMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, errorMessage);
	vsprintf(tmpString, errorMessage, marker);

	StbLogger::getInstance()->printErrorAndContinue(tmpString);
}

void logPrintEAbort(stb::string errorMessage)
{
	StbLogger::getInstance()->printErrorAndAbort(errorMessage.c_str());
}

void logPrintEAbort(const char *errorMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, errorMessage);
	vsprintf(tmpString, errorMessage, marker);

	StbLogger::getInstance()->printErrorAndAbort(tmpString);
}

/*
 * These are the actual class methods implementations of StbLogger
 */

StbLogger*	StbLogger::instance=NULL;

StbLogger::StbLogger()
{
	logMode = MODE_CONSOLE;
	logFileName = "stb_log.txt";
}

StbLogger::~StbLogger()
{
}

StbLogger *StbLogger::getInstance()
{
	if(instance == NULL)
		instance = new StbLogger();

	return instance;
}

void StbLogger::setLogMode(LOG_MODE newMode) {
	logMode = newMode;
}

void StbLogger::setLogFileName(char * filename)
{
	logFileName = filename;
}

char * StbLogger::getLogFileName() 
{
	return logFileName;
}


void StbLogger::printMessage(const char *message)
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

void StbLogger::printDebug(const char *debugMessage)
{
	// We should be using ACE here
#ifdef STB_IS_DEBUG
	switch (logMode)
	{
		case MODE_OFF:
			break;
		case MODE_FILE:
            writeToFileEx("STB|DEBUG: %s", debugMessage);
			break;
		case MODE_CONSOLE:
			printf("STB|DEBUG: %s", debugMessage);
			break;
		default:
			break;
	}
#endif // STB_IS_DEBUG
}

void StbLogger::printSetup(const char *setupMessage)
{
	// We should be using ACE here
		switch (logMode)
	{
		case MODE_OFF:
			break;
		case MODE_FILE:
			writeToFileEx("STB|SETUP: %s",setupMessage);
			break;
		case MODE_CONSOLE:
            printf("STB|SETUP: %s",setupMessage);
			break;
		default: break;
	}
}

void StbLogger::printInfo(const char *infoMessage)
{
	// We should be using ACE here
	switch (logMode)
	{
		case MODE_OFF:
			break;
		case MODE_FILE:
			writeToFileEx("STB|INFO : %s",infoMessage);
			break;
		case MODE_CONSOLE:
			printf("STB|INFO : %s",infoMessage);
			break;
		default:
			break;
	}
}

void StbLogger::printWarning(const char *warningMessage)
{
	// We should be using ACE here
	switch (logMode)
	{
		case MODE_OFF:
			break;
		case MODE_FILE:
			writeToFileEx("STB|WARN : %s",warningMessage);
			break;
		case MODE_CONSOLE:
#ifdef WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
            printf("STB|WARN : ");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("%s",warningMessage);
#else
            /// Color changes for console text are platform dependent
            printf("STB|WARN : %s",warningMessage);
#endif
			break;
		default:
			break;
	}
}

void StbLogger::printErrorAndContinue(const char *errorMessage)
{
	// We should be using ACE here
	switch (logMode)
	{
		case MODE_OFF:
			break;
		case MODE_FILE:
			writeToFileEx("STB|ERROR: %s",errorMessage);
			break;
		case MODE_CONSOLE:
#ifdef WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
            printf("STB|ERROR: ");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("%s",errorMessage);
#else
            /// Color changes for console text are platform dependent
            printf("STB|ERROR: %s",errorMessage);
#endif
			break;
		default:
			break;
	}
}

void StbLogger::printErrorAndAbort(const char *errorMessage)
{
	// We should be using ACE here
	printErrorAndContinue(errorMessage);

	// FIXME: Something more elegant here please. Mendez 20060316
	exit(0);
}

void StbLogger::writeToFile(const char * message) 
{
	FILE* fp = fopen(getLogFileName(), "a");
	if(!fp)
		return;

	fprintf(fp, message);
	//if(message[strlen(message)-1]!='\n')
	//	fputc('\n', fp);

	fclose(fp);
}

void StbLogger::writeToFileEx(const char* nStr, ...)
{
	char tmpString[512];
    va_list marker;

	va_start(marker, nStr);
	vsprintf(tmpString, nStr, marker);

	writeToFile(tmpString);
}

END_NAMESPACE_STB
