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
/** The header file for the LoggerPC class.  
*  
* @author Denis Kalkofen  
*  
* $Id: LoggerPC.h 223 2006-11-06 14:18:27Z mendez $  
* @file                                                                   */  
/* ======================================================================= */  

/*
 *
 * THIS CLASS IS DEPRECATED
 *
 */
#ifndef _LOGGERPC_H_
#define _LOGGERPC_H_
#include <openvideo/LoggerBase.h>

namespace openvideo{

/**@ingroup core
*	PC version of the Logger.
*/
class LoggerPC : public LoggerBase
{
public:

    /**
    *     The Constructor	
    */
    LoggerPC();

    /**
    *     The destructor.
    */
    virtual ~LoggerPC();

    /**
    * 
    */
    virtual void log(const char* message);

protected:	

private:
	
};// class 
}//namespace
#endif//_LOGGERPC_H_
//========================================================================
// End of LoggerPC.h 
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
