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
/** The header file for the ComponentManager class.  
 *  
 * @author Denis Kalkofen  
 *  
 * $Id: ComponentManager.h 25 2005-11-28 16:11:59Z denis $  
 * @file                                                                   */  
/* ======================================================================= */  


#ifndef _COMPONENTMANAGER_H_
#define _COMPONENTMANAGER_H_

#include <stb/base/macros.h>
#include <stb/kernel/Studierstube.h>

#include <vector>

BEGIN_NAMESPACE_STB


class ComponentRetriever;
class ComponentInfo;
class Component;
class Application;
class GUIBinder;


/**@ingroup kernel
 *
 * 
 */
class KERNEL_API ComponentManager
{
public:
    /**
     *     The Constructor	
     */
    ComponentManager();

    /**
     *     The destructor.
     */
    ~ComponentManager();

    /************************************************************************
     * Add a Component using it's ComponentInfo description object
     ************************************************************************/
    void addComponent(ComponentInfo* compInfo);

    /************************************************************************
     * Loads a specific Component, which is defined by it's name.
     * if the Component is already loaded the function does only return a pointer a pointer to it.
     ************************************************************************/
    Component* load(std::string compName);

    /************************************************************************
    * Updates the ComponentManager during the kernel's update traversal, 
    * which will in turn update all currently the loaded applications. 
    ************************************************************************/
    void update(); 

    /************************************************************************
    * Shuts down all the components and applications
    ************************************************************************/
    void shutDown(); 


	/// Returns a GUI binding component - if one has been loaded to far
	GUIBinder* getGUIBinder()  {  return guiBinder;  }

protected:
    /************************************************************************
     * List of all loaded Component's and ComopnentThread's - Notice: The Applications are stored in 'appList'
     ************************************************************************/
    std::vector<stb::Component*> compList;

    /************************************************************************
     * List of all Loaded Applications
     ************************************************************************/
    std::vector<stb::Application*> appList;
    
    /************************************************************************
     * List of all ComponentInfos which are not loaded yet
     ************************************************************************/
    std::vector<stb::ComponentInfo*> demandList;
    
    /************************************************************************
     * This is a temporary list of Component's. A Component will be initialized only during the ComponentManagers
     * update (which is trigger using either coin's idle or timer sensor). If Component is created before or after the update it will be stored in the initList until the next update call.
     ************************************************************************/
    std::vector<stb::Component*> initList;
    
    /************************************************************************
     * The ComponentRetriever is used to create Component from a ComponentInfo.
     ************************************************************************/
    ComponentRetriever *compRetriever;

	/// One component loaded at startup has to implement the GUI binder interface
	GUIBinder *guiBinder;

    /************************************************************************
     * Initializes a Component and stores it in either appList or compList.
     ************************************************************************/
    bool initComponent(Component *comp);

    /************************************************************************
     * Checks if a Component is already in either the compList or the appList
    ************************************************************************/
    stb::Component* isLoaded(std::string compName);


	void connectComponents(Component* component);

private:
	
};// class 

END_NAMESPACE_STB



#endif//_COMPONENTMANAGER_H_
