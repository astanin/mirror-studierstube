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
/** The header file for the Logger .  
*  
* @author Denis Kalkofen  
*  
* $Id: Logger.h 223 2006-11-06 14:18:27Z mendez $  
* @file                                                                   */  
/* ======================================================================= */  

/*
 *
 * THIS CLASS IS DEPRECATED
 *
 */
#ifndef _LOGGER_H_
#define _LOGGER_H_

#if defined(OV_IS_WINXP) || defined(OV_IS_WINCE)
    #include "LoggerPC.h"
    namespace openvideo{
        typedef openvideo::LoggerPC Logger;
    }
#endif

#if defined(OV_IS_LINUX)
    #include "LoggerPC.h"
    namespace openvideo{
        typedef openvideo::LoggerPC Logger;
    }
#endif


#endif//_COMPONENTINFO_H_
