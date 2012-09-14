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
/** The header file for the LoggerBase class.  
*  
* @author Denis Kalkofen  
*  
* $Id: LoggerBase.h 223 2006-11-06 14:18:27Z mendez $  
* @file                                                                   */  
/* ======================================================================= */  

/*
 *
 * THIS CLASS IS DEPRECATED
 *
 */
#ifndef _LOGGERBASE_H_
#define _LOGGERBASE_H_

namespace openvideo{

/**@ingroup core
*	The base class for the logger implementations
*/
class LoggerBase
{
public:
    /**
    *     The destructor.
    */
    virtual ~LoggerBase(){};

    /*
    *  
    */
    virtual void log(const char* nStr) = 0;

    /*
    *  
    */
    virtual void logEx(const char* nStr, ...);

protected:	
    /**
    *     The Constructor	
    */
    LoggerBase(){};

	
};// class 
}//namespace
#endif//_LOGGERBASE_H_
//========================================================================
// End of LoggerBase.h 
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
