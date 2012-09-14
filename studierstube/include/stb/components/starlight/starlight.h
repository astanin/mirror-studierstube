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
/** The header file for the Starlight class.  
*  
* @author Denis Kalkofen  
*  
* $Id: Starlight.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  


#ifndef _STARLIGHT_H_
#define _STARLIGHT_H_


#if defined(WIN32) || defined(_WIN32_WCE)
#  pragma warning(disable:4251)
#  pragma warning(disable:4290)
#  ifdef STARLIGHT_EXPORTS
#    define STARLIGHT_API __declspec(dllexport)
#  else
#    define STARLIGHT_API __declspec(dllimport)
#  endif
#else
#  define STARLIGHT_API
#endif



#include <stb/base/macros.h>
#include <stb/kernel/Component.h>


class SoClassLoader;
class SoConditionalTrigger;

class SoContext;
class SoContextMultiSwitch;
class SoContextReport;
class SoContextSeparator;
class SoContextSwitch;
class SoEaseIn;
class SoEngineWrapper;
class SoFactorRotation;
class SoFileSubgraph;
class SoFrameBuffer;
class SoKeyToggle;
class SoMultiSwitch;
class SoMultMatrix;
class SoNodeContext;
class SoNodeContextReport;
class SoNodeToName;
class SoRoute;
class SoSFFieldContainer;
class SoStencilBuffer;
class SoStringCalculator;
class SoStringMap;
class SoUse;
class SoVariable;

BEGIN_NAMESPACE_STB

/**
*	
*/
class Starlight : public stb::Component
{
public:
    /**
    *     The Constructor	
    */
    Starlight();
    /**
    *     The destructor.
    */
    virtual ~Starlight();

    /// Called before the application is destructed.
    virtual bool init();

    virtual void shutDown();

    virtual SoClassLoader* createSoClassLoader();
    virtual SoConditionalTrigger*    createSoConditionalTrigger();
    virtual SoContext*   createSoContext();
    virtual SoContextMultiSwitch*    createSoContextMultiSwitch();
    virtual SoContextReport*    createSoContextReport();
    virtual SoContextSeparator*    createSoContextSeparator();
    virtual SoContextSwitch*    createSoContextSwitch();
    virtual SoEaseIn*    createSoEaseIn();
    virtual SoEngineWrapper*    createSoEngineWrapper();
    virtual SoFactorRotation*    createSoFactorRotation();
    virtual SoFileSubgraph*    createSoFileSubgraph();
    virtual SoFrameBuffer*    createSoFrameBuffer();
    virtual SoKeyToggle*    createSoKeyToggle();
    virtual SoMultiSwitch*   createSoMultiSwitch();
    virtual SoMultMatrix*    createSoMultMatrix();
    virtual SoNodeContext*    createSoNodeContext();
    virtual SoNodeContextReport*    createSoNodeContextReport();
    virtual SoNodeToName*    createSoNodeToName();
    virtual SoRoute*    createSoRoute();
    virtual SoSFFieldContainer*    createSoSFFieldContainer();
    virtual SoStencilBuffer*    createSoStencilBuffer();
    virtual SoStringCalculator*    createSoStringCalculator();
    virtual SoStringMap*    createSoStringMap();
    virtual SoUse*    createSoUse();
    virtual SoVariable*    createSoVariable();

protected:

private:

};// class 


END_NAMESPACE_STB

#endif//_STARLIGHT_H_
