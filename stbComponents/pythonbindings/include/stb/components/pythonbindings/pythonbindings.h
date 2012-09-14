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
/** The header file for the Pythonbindings class.  
*  
* @author Alexander Bornik
*  
* $Id$  
* @file                                                                   */  
/* ======================================================================= */  


#ifndef _PYTHONBINDINGS_H_
#define _PYTHONBINDINGS_H_


#if defined(WIN32) || defined(_WIN32_WCE)
#  pragma warning(disable:4251)
#  pragma warning(disable:4290)
#  ifdef PYTHONBINDINGS_EXPORTS
#    define PYTHONBINDINGS_API __declspec(dllexport)
#  else
#    define PYTHONBINDINGS_API __declspec(dllimport)
#  endif
#else
#  define PYTHONBINDINGS_API
#endif



#include <stb/base/macros.h>
#include <stb/kernel/Component.h>

class SoPyScript;

BEGIN_NAMESPACE_STB

/**
*	
*/
class Pythonbindings : public stb::Component
{
public:
    /**
    *     The Constructor	
    */
    Pythonbindings();
    /**
    *     The destructor.
    */
    virtual ~Pythonbindings();

    /// Called before the component is destructed.
    virtual bool init();

    virtual void shutDown();

	virtual void setParameter(stb::string key, std::string value);

    virtual SoPyScript* createSoPyScript();

protected:
	stb::string configFile; 

private:

};// class 


END_NAMESPACE_STB

#endif//_PYTHONBINDINGS_H_
