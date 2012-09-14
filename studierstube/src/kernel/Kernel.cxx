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
/** The cxx file for the StbKernel class.
 *
 * @author Denis Kalkofen
 * @author Michael Kalkusch
 * @author Erick Mendez
 *
 * $Id: Kernel.cxx 2006-06-22 11:51:59Z kalkusch $
 * @file                                                                   */
/* ======================================================================= */

#include <stb/base/fixWinCEIssues.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/Studierstube.h>
#include <stb/base/string.h>
#include <stdarg.h>
#include <stdio.h>
#ifndef USE_TINYXMLMOD
#include <tinyxml/tinyxml.h>
#else
#include <TinyXML/tinyxml.h>
#endif
#include <stb/base/fixWinXPIssues.h>
#include <ace/ACE.h>
#include <ace/OS.h>

#include <Inventor/SoDB.h>
#include <Inventor/sensors/SoSensor.h>

#include <stb/kernel/Config.h>
#include <stb/kernel/Scheduler.h>
#include <stb/kernel/SceneManager.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/SoStbScene.h>
#include <stb/components/event/SoOpenTrackerSource.h>

#include <fstream>
#include <sstream>
#include <iostream>

//
BEGIN_NAMESPACE_STB

Kernel*	Kernel::instance=NULL;


Kernel::Kernel()
{
    ACE::init();
    SoDB::init();
    //additional coin nodes
    SoStbScene::initClass();
    //
    scheduler= new stb::Scheduler();
    sceneManager= new stb::SceneManager();
    componentManager= new stb::ComponentManager();
    config=new stb::Config();
    //
    kernel_config_file = "kernel.xml"; //default kernel config
}


Kernel::~Kernel()
{
    ACE::fini();
    delete config;
    delete scheduler;
    delete sceneManager;
    delete componentManager;
}

//static
Kernel*
Kernel::getInstance()
{
    if(instance == NULL)
        instance = new Kernel();

    return instance;
}

stb::string
Kernel::getConfig(stb::string config)
{	  
	  setConfigFile( config );
	  return getConfig();
}

stb::string
Kernel::getConfig() const
{
	 ///FIXME: correct parsing of config string!
	  
    using namespace std;
    ostringstream tmp;
    tmp << base_dir << "/" << kernel_config_file;
    return tmp.str();
}

//
void
Kernel::start(int argc, char* argv[])
{
#ifdef STB_IS_WINCE
//	StbLogger::getInstance()->setLogMode(MODE_FILE);
	StbLogger::getInstance()->setLogFileName("Program Files/stb4/stb_log.txt");
#endif

    logPrint("**********************\n");
    logPrint(STUDIERSTUBE_VERSION_STRING);logPrint("\n");
    logPrint("(C) ");logPrint(STUDIERSTUBE_YEAR_STRING);
    logPrint(" Graz University of Technology\n");
    logPrint("**********************\n\n");

    // extract application base directory name as search path
    // only useful for linux
    //
    ///FIXME: correct parsing of config string!
    if (argc > 1)
        base_dir = stb::string(argv[1]);
    else
        base_dir = ".";

    logPrintI("Using configuration base directory name %s\n", base_dir.c_str());
    ////////////////////////////////////////////////////////
    stb::string kernelConfigFile=getConfig(kernel_config_file);

    // read the kernel's configuration file
    config->readKernelConfig(kernelConfigFile);

	if(GUIBinder* guiBinder = componentManager->getGUIBinder())
		scheduler->run(guiBinder);
	else
		logPrintE("No GUI-binding component loaded, cannot continue...\n\n");
}

//
void
Kernel::stop()
{
    componentManager->shutDown();

    // FIXME: give some threads time to close down. Mendez. 20061114
    int time=1000;
    stb::logPrintI("Waiting %d milliseconds for components to shutdown\n",time);
    ACE_Time_Value timeVal(0, (int)(1000*time) );
    ACE_OS::sleep( timeVal );

    if(GUIBinder* guiBinder = componentManager->getGUIBinder())
        scheduler->stop(guiBinder);
    stb::logPrintI("Closing Studierstube\n");
    exit(0);
}

////////////////////////////////////////////////////////////////

void
Kernel::parseConfiguration(TiXmlElement* element)
{
    ////////////<logging mode="xxxY filename="xxx"/> /////////
    TiXmlAttribute* attribute = element->FirstAttribute();
    while(attribute) //////////// kernel's parameter
    {
        ///////////////// logMode /////////////////
        scheduler->readConfiguration(attribute);
	    /////////////////// ------- /////////////////
	    ////else if(!stricmp(attribute->Name( 00000000000000000000000000000000000000000000000000000000000000000000000),"----"))
	    ////{
	    ////}
        attribute = attribute->Next();
    }
}


void
Kernel::update( void * /*data*/, SoSensor * /*sensor*/)
{
    instance->componentManager->update();
    instance->sceneManager->update();
}


stb::SceneManager*
Kernel::getSceneManager()
{
    return sceneManager;
}


stb::string
Kernel::getBaseDir() const
{
    return base_dir;
}

void 
Kernel::setBaseDir( stb::string path ) 
{
	base_dir = path;
}

stb::string
Kernel::getConfigFile() const
{
    return kernel_config_file;
}

void
Kernel::setConfigFile( stb::string config )
{
    kernel_config_file = config;
}

void
Kernel::preRenderCallback()
{
	//printf("pre-render\n");

	// notify all registered KernelEventSubscribers that rendering is gonna happen now
	//
	for(KernelEventSubscriberVector::iterator it=kernelEventSubscribers.begin(); it!=kernelEventSubscribers.end(); it++)
		(*it)->kes_beforeRender();

	// make sure we get the latest opentracker data into Coin before rendering...
	//
	if(SoNode* trackerSource = sceneManager->getTrackerSource())
    {
        if (reinterpret_cast<SoOpenTrackerSource*>(trackerSource)->processing.getValue()==SoOpenTrackerSource::POLL)
        {
    		reinterpret_cast<SoOpenTrackerSource*>(trackerSource)->runTracker();
        }
    }
}


void
Kernel::postRenderCallback()
{
	//printf("post-render\n");

	// notify all registered KernelEventSubscribers that rendering just finished
	//
	for(KernelEventSubscriberVector::iterator it=kernelEventSubscribers.begin(); it!=kernelEventSubscribers.end(); it++)
		(*it)->kes_afterRender();
}


void
Kernel::registerForKernelEvents(KernelEventSubscriber* instance)
{
	if(!instance)
		return;

	for(KernelEventSubscriberVector::iterator it=kernelEventSubscribers.begin(); it!=kernelEventSubscribers.end(); it++)
		if(*it == instance)
			return;

	kernelEventSubscribers.push_back(instance);
}


void
Kernel::unregisterFromKernelEvents(KernelEventSubscriber* instance)
{
	for(KernelEventSubscriberVector::iterator it=kernelEventSubscribers.begin(); it!=kernelEventSubscribers.end(); it++)
		if(*it == instance)
		{
			kernelEventSubscribers.erase(it);
			return;
		}
}



END_NAMESPACE_STB

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
