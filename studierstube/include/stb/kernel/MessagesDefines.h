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
/** Message Definitions
*
* @author Erick Mendez
*
* @file                                                                   */
/* ======================================================================= */

#ifndef _MESSAGESDEFINES_H_
#define _MESSAGESDEFINES_H_


/*
 Setup Messages 
	- start with the prefix "LOG_SETUP_"

 Info Messages 
	- start with the prefix "LOG_INFO_"

 Warnings 
	- start with the prefix "LOG_WARNING_"

 Error Messages 
	- start with the prefix "LOG_ERROR_"

*/

/*
 * Setup Messages
 */
#define LOG_SETUP_START_KERNEL					"Kernel has started\n"
#define LOG_SETUP_START_EVENT_SYSTEM			"Event System has started\n"
#define LOG_SETUP_START_STARLIGHT				"Starlight has started\n"
#define LOG_SETUP_START_VIDEO					"Video has started\n"
#define LOG_SETUP_START_VIEWER					"Viewer has started\n"
#define LOG_SETUP_START_STUDIERSTUBE			"Studierstube has started\n"
#define LOG_SETUP_START_OPENTRACKER				"Opentracker has started\n"
#define LOG_SUCCESSFUL							"Succesful\n"
#define LOG_SUCCESSFULLY_STARTED				"Succesfuly Started\n"

/*
 * Error Messages
 */
#define LOG_ERROR_FILE_NOT_FOUND				"File Not Found\n"
#define LOG_ERROR_FILE_CORRUPTED				"File Corrupted\n"
#define LOG_ERROR_FAILED						"Failed \n"





#endif //_MESSAGESDEFINES_H_
