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
/** The header file for the ComponentInfo class.  
 *  
 * @author Denis Kalkofen  
 *  
 * $Id: ComponentInfo.h 25 2005-11-28 16:11:59Z denis $  
 * @file                                                                   */  
/* ======================================================================= */  

#ifndef _COMPONENTINFO_H_
#define _COMPONENTINFO_H_

#include <stb/base/OS.h>
#include <stb/base/string.h>
#include <stb/base/macros.h>
#include <stb/kernel/Component.h>
#include <map>


class TiXmlElement;


BEGIN_NAMESPACE_STB


/**@ingroup kernel
 * A ComponentInfo object is used to store all relevant information about a component.
 * These are in particular:
 * @li: stb::string typeID ["Component"|"Application"|"ComponentThread"]
 * @li: stb::string name (component's name)
 * @li stb::string libName; (library name)
 * @li hModule libHandle; (used when component is loaded)
 * @ AVAILABILITY availability [ON_LOAD|ON_DEMAND]
 * @li std::map<stb::string,stb::string> parameter; (a map of component specific parameters)
 */
class KERNEL_API ComponentInfo
{
public:
    friend class Component;
    /**
     *     The Constructor	
     */
    ComponentInfo();
   
    /**
     *     The destructor.
     */
    ~ComponentInfo();

    /************************************************************************
     * Supported availabilities of a component
     ************************************************************************/
    enum AVAILABILITY{
        ON_LOAD=0,
        ON_DEMAND=1
    };

    /**
     * Reads an xml element to configure this ComponentInfo
     * @li the elements value is <Component></Component>, <Application></Application>, or <ApplicationThread> </ApplicationThread>
     * and it defines the stb::string typeID which is later used to identify the current component's base type.
     * @li parameter lib  - defines the libraries name
     * @li parameter name - defines the componets name
     * @li parameter availability [ondemand|onload]
     * @li Foreeach component specific parameter
     *     key="aKey" value="aValue"
     */
    void readConfiguration(TiXmlElement* element);
 
    /**
     * Returns the components name
     */
    stb::string getName(){return name;}
   
    /**
     * Return the components library name.
     */
    stb::string getLibName(){return libName;}
   
    /************************************************************************
     * Returns the cCmponent's library handle
     ************************************************************************/
    hModule getLibHandle(){return libHandle;}

    /************************************************************************
     * Sets the Component's library handle
     ************************************************************************/
    void setHModule(hModule aLibHandle);
   
    /************************************************************************
     * Returns the Component's availability
     ************************************************************************/
    ComponentInfo::AVAILABILITY getAvailability();

    /************************************************************************
     * Sets the Component's availability
     ************************************************************************/
    void setAvailability(AVAILABILITY avl);

protected:
    /************************************************************************
     * Component's typeID
     ************************************************************************/
    stb::string typeID;

    /************************************************************************
     * Component's name
     ************************************************************************/
    stb::string name;

    /************************************************************************
     * Component's library name
     ************************************************************************/
    stb::string libName;

    /************************************************************************
     * Component's library handle
     ************************************************************************/
    hModule libHandle;

    /************************************************************************
     * Component's availability 
     ************************************************************************/
    AVAILABILITY availability;

    /************************************************************************
     * Component's additional parameter
     ************************************************************************/
    std::map<stb::string,stb::string> parameter;
private:
	
};// class 

END_NAMESPACE_STB



#endif//_COMPONENTINFO_H_

//========================================================================
// End of file
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
