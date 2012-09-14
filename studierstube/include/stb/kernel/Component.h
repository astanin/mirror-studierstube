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
/** The header file for the Component class.  
 *  
 * @author Denis Kalkofen  
 *  
 * $Id: Component.h 25 2005-11-28 16:11:59Z denis $  
 * @file                                                                   */  
/* ======================================================================= */  


#ifndef _COMPONENT_H_
#define _COMPONENT_H_


#include <stb/base/OS.h>
#include <stb/kernel/Studierstube.h>
#include <stb/base/macros.h>
#include <stb/base/string.h>


BEGIN_NAMESPACE_STB

class ComponentInfo;
class VideoProvider;
class VideoUser;
class GUIBinder;


/**@ingroup kernel
 *
 * 
 */
class KERNEL_API Component
{
public:
    /**
     *     The destructor.
     */
    virtual ~Component();

    /************************************************************************
     * Called once during the ComponentManager's update function, right before the Component is added to the ComponentManager
     * Override init() to retrieve parameters or initialize the
     * Component itself or other Components through the ComponentManager
     ************************************************************************/
    virtual bool init() = 0;
    
    /************************************************************************
     * Called before the Component is destructed.
     ************************************************************************/
    virtual void shutDown() = 0;
    


    /************************************************************************
     * Set's the Component's info. object.
     ************************************************************************/
    virtual void setComponentInfo(ComponentInfo* info);
    
    /************************************************************************
     * Calls setParameter for all Component specific parameters.
     ************************************************************************/
    virtual void retrieveParameter();

    /************************************************************************
     * Returns the ComponentInfo object.
     ************************************************************************/
    virtual ComponentInfo* getInfo(){return compInfo_;}

    /************************************************************************
     * Returns the Component's typeI, which is set using the xml.elements value.
     * 
     ************************************************************************/
    virtual stb::string getTypeID();


	/// A component returns a non-NULL instance if it is delivering video data
	virtual VideoProvider* getVideoProviderInterface()  {  return NULL;  }

	/// A component returns a non-NULL instance if it requires video data
	virtual VideoUser* getVideoUserInterface()  {  return NULL;  }

	/// A component returns a non-NULL instance if it is a GUI binding component (usually only viewers)
	virtual GUIBinder* getGUIBinderInterface()  {  return NULL;  }


    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    static const char* getBaseTypeID(){return "Component";}

protected:	
    /**
     *     The Constructor	
     */
    Component();
    
    /************************************************************************
     * retrieves the component's parameters from the config. file
     ************************************************************************/
    virtual void setParameter(stb::string key, std::string value);
    
    Kernel		*kernel_;
    ComponentInfo	*compInfo_;
    bool isInit;


private:
    
};// class 


END_NAMESPACE_STB


#endif//_COMPONENT_H_
