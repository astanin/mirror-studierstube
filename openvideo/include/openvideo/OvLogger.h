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
* PROJECT: OpenVideo  
* ======================================================================== */  
/** The header file for the OvLogger class.  
*  
* @author Erick Mendez  
*  
* @file                                                                   */  
/* ======================================================================= */  


#ifndef _OVLOGGER_H_
#define _OVLOGGER_H_

#include "openVideo.h"

namespace openvideo{


/*
 * Usage:  
 *		- logPrintE("File Not Found");
 *		- logPrintE(LOG_ERROR_FILE_NOT_FOUND);
 *		- logPrintE("File %s Not Found", filename);
 *
*/
void OPENVIDEO_API logPrint(const char *message, ...);
void OPENVIDEO_API logPrintD(const char *debugMessage, ...);
void OPENVIDEO_API logPrintS(const char *setupMessage, ...);
void OPENVIDEO_API logPrintI(const char *infoMessage, ...);
void OPENVIDEO_API logPrintW(const char *warningMessage, ...);
void OPENVIDEO_API logPrintE(const char *errorMessage, ...);
void OPENVIDEO_API logPrintEAbort(const char *errorMessage, ...);

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

class OPENVIDEO_API OvLogger
{
public:

	/// The instance retriever, this is a singleton
	static OvLogger *getInstance();

	/// The destructor
	virtual ~OvLogger();

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
	static OvLogger *instance;

    /// The File Name where to output the log
	char * logFileName;

	/// The constructor
	OvLogger();
};

} //namespace openvideo

#endif //_OVLOGGER_H_
