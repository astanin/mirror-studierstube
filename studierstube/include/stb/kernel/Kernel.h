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
/** The header file for the Kernel class.
*
* @author Denis Kalkofen
* @author Michael Kalkusch
*
* $Id: Kernel.h 2006-06-22 11:51:59Z kalkusch $
* @file                                                                   */
/* ======================================================================= */

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stb/base/macros.h>
#include <stb/base/string.h>
#include <stb/kernel/Studierstube.h>
#include <stb/kernel/KernelEventSubscriber.h>
#include <stb/kernel/StbLogger.h>

class TiXmlElement;
class SoSensor;

#include <stb/kernel/Scheduler.h>

/**
*
*/

BEGIN_NAMESPACE_STB


class Config;
class Component;
class ComponentManager;
class SceneManager;
class ComponentInfo;
class SoTrakEngineInterface;
class Video;


/**@ingroup kernel
* The kernel implements the main object in the studierstube framework. 
* The kernel is implemented using a singleton pattern and serves the following functionality;
* @li Runs Studierstube's main loop
* @li Manages the scene graph
* @li Manages component/application loading/unloading and updateing 
* 
* To implement this functionality, the kernel uses the Scheduler(mainLoop), the SceneManager(scene graph management) and the ComponentManager (component management).
*/
class KERNEL_API Kernel
{
public:
    /**
    *	Returns the Kernel. it creates a new kernel, if no one exists.
    *   The kernel is implemented using the singleton pattern. Therefore, this static function is the only ways to instantiate the kernel.
    *   
    */
    static Kernel* getInstance();

    /**
    *	The kernel's destructor.
    */
    virtual ~Kernel();

    /**
    * Starts the kernel. 
    * the start function looks like this
    * @li config->readKernelConfig(kernelConfigFile);
    * @li scheduler->init();
    * @li scheduler->mainLoop(); 	
    */
    void start(int argc,char* argv[]);

    /**
    *	Stop the kernel. --> Notice: stopKernel will not delete the Kernel!
    *   FIXME: stop() does nothing currently.
    */
    void stop();

    /*
    * Returns the current config file as a combination of base_dir\config_file
    */
    stb::string getConfig(stb::string config);

    /*
    * Returns the current config file as a combination of base_dir\config_file
    */
    stb::string getConfig() const;
    	
    /*
    * Returns the current kernel xml config file without base_dir.
    */
    stb::string getConfigFile() const;
    	
    /*
    * Sets the current XML kernel config file without  base_dir.
    */
    void setConfigFile( stb::string config );
    	
    /*
    *	Parses the <kernel> </kernel> element in a kernel.xml configuration.                                                                    
    */
    void parseConfiguration(TiXmlElement* root);

    /*
    * Updates the kernel. This function is part of studierstube's main loop.
    */    
    static void update( void * data, SoSensor * sensor);


    /*
    * Returns the kernel's SceneManager object. 
    */  
    stb::SceneManager* getSceneManager();

    /*
    * Returns the kernel's ComponentManager object. 
    */  
    stb::ComponentManager* getComponentManager(){return componentManager;}

    /*
     * Returns the base_dir of the configuration path
     */
    stb::string getBaseDir() const;
    	
    /*
     * Sets the base_dir of the configuration path
     */
    void setBaseDir( stb::string path );



	/// Called by the StudierstubeViewer to notify the kernel that scene graph rendering will start right now
	void preRenderCallback();


	/// Called by the StudierstubeViewer to notify the kernel that scene graph rendering just finished
	void postRenderCallback();


	virtual void registerForKernelEvents(KernelEventSubscriber* instance);


	virtual void unregisterFromKernelEvents(KernelEventSubscriber* instance);


protected:	
    ///////////////////// 
    /**
    *	The kernel's constructor. The constructor also initializes ACE and COIN.
    */
    Kernel();

    /**
    *	THE KERNEL - The kernel is implemented as a singleton and therefore the instance pointer is static and protected.
    */
    static Kernel *instance;

    /*
    *	The kernel's Config object is used to read the kernel's configuration file.
    */
    stb::Config* config;	

    /*
    * The kernel's Scheduler object is used to implement the kernel's main loop.                                                                   
    */
    stb::Scheduler *scheduler;

    /*
    * The kernel's ComponentManager is used to handle component loading and unloading as well as the application updates.
    */
    stb::ComponentManager *componentManager;

    /*
    *  The SceneManager holds studierstube's scene graph.                                                                   
    */
    stb::SceneManager *sceneManager;


    ////////////////////// config file management ////////////////////////////
    /* 
    * kernel's config file name 
    * default = "kernel.xml"
    */
    stb::string kernel_config_file; //

    /* 
    * define here search paths for config files, public available
    * default = "."
    */
    stb::string base_dir; // 


	/// Vector of all instances subscribed for kernel events
	KernelEventSubscriberVector	kernelEventSubscribers;
};// class 

END_NAMESPACE_STB

#endif //_STBKERNEL_H_
