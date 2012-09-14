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
/** The cxx file for the ComponentManager class.
*
* @author Denis Kalkofen
*
* $Id: ComponentManager.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/Component.h>
#include <stb/kernel/ComponentThread.h>
#include <stb/kernel/Application.h>
#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/ComponentRetriever.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/GUIBinder.h>
#include <stb/kernel/VideoProvider.h>
#include <stb/kernel/VideoUser.h>

#include <iostream>

BEGIN_NAMESPACE_STB

ComponentManager::ComponentManager()
{
	guiBinder = NULL;
	compRetriever=new stb::ComponentRetriever();
}

ComponentManager::~ComponentManager()
{
   delete compRetriever;
}

void
ComponentManager::update()
{
    // init all new components
    if(initList.size()>0)
    {
        for(unsigned int i=0;i<initList.size();i++)
        {
            Component* newComp=initList[i];
            if(!initComponent(newComp))
                initList[i]=NULL;
        }
        initList.clear();
    }

    // update application list
    for(unsigned int i=0;i<appList.size();i++)
    {
        appList[i]->update();
    }
}

bool
ComponentManager::initComponent(Component *comp)
{
    char padding[4];
    if(comp->init())
    {
        if (comp->getInfo()->getName().size()<8) strcpy(padding,"\t\t\t");
        else strcpy(padding,"\t\t\t");
        logPrintS("Initializing component %s %s [ OK ]\n", comp->getInfo()->getName().c_str(), padding);

		connectComponents(comp);
        // add to applist or comp.list
        stb::string id=comp->getTypeID();
        if(id==Application::getBaseTypeID())
        {
            appList.push_back((Application*)comp);
        }
        else if(id==Component::getBaseTypeID()
              ||id==ComponentThread::getBaseTypeID())
            compList.push_back(comp);

        return true;
    }

    logPrintE("Couldn't initialize %s\n", comp->getInfo()->getName().c_str());
    return false;
}

void
ComponentManager::addComponent(ComponentInfo* compInfo)
{
    switch(compInfo->getAvailability())
    {
        case ComponentInfo::ON_DEMAND:
            demandList.push_back(compInfo);
            break;

        case ComponentInfo::ON_LOAD:
            if(Component* newComp = compRetriever->getComponent(compInfo))
			{
				if(!guiBinder && newComp->getGUIBinderInterface())
					guiBinder = newComp->getGUIBinderInterface();

				initList.push_back(newComp);
			}
            break;
    }
}

stb::Component*
ComponentManager::isLoaded(std::string compName)
{
    using namespace std;
//     cerr << "Check if component is loaded: " <<  compName << endl;
    for(int i=0;i<(int)compList.size();i++)
    {
// 	cerr << "Check if component is loaded: " << compList[i]->getInfo()->getName()  << endl;
        if(compList[i]->getInfo()->getName()==compName)
            return compList[i];
    }
    for(int i=0;i<(int)appList.size();i++)
    {
        if(appList[i]->getInfo()->getName()==compName)
            return appList[i];
    }

    return NULL;
}

stb::Component*
ComponentManager::load(std::string compName)
{
    stb::Component* retComp=isLoaded(compName);
    if(retComp)
        return retComp;
    //search demandList
    for(int i=0;i<(int)demandList.size();i++)
    {
        if(demandList[i]->getName()==compName)
        {
            Component* newComp=NULL;
            newComp=(Component*)compRetriever->getComponent(demandList[i]);
            if(!newComp){
                return NULL;
            }
            initComponent(newComp);
            return newComp;
        }

    }
    //search initList
    for(int i=0;i<(int)initList.size();i++)
    {
        if(initList[i]->getInfo()->getName()==compName)
        {
            Component* newComp=initList[i];
            initComponent(newComp);
            return newComp;
        }
    }

    logPrintE("unable to find " + compName + "\n");
    return NULL;
}


void
ComponentManager::connectComponents(Component* component)
{
	size_t i;

	// check if this component provides video
	//
	if(VideoProvider* videoProvider = component->getVideoProviderInterface())
	{
		// find all components that use video
		//
		for(i=0;i<compList.size();i++)
			if(VideoUser* videoUser = compList[i]->getVideoUserInterface())
				videoProvider->vp_registerVideoUser(videoUser);

		// find all applications that use video
		//
		for(i=0;i<appList.size();i++)
			if(VideoUser* videoUser = appList[i]->getVideoUserInterface())
				videoProvider->vp_registerVideoUser(videoUser);
	}

	// check if this component uses video
	//
	if(VideoUser* videoUser = component->getVideoUserInterface())
	{
		// find all components that use video
		//
		for(i=0;i<compList.size();i++)
			if(VideoProvider* videoProvider = compList[i]->getVideoProviderInterface())
				videoProvider->vp_registerVideoUser(videoUser);

		// find all applications that use video
		//
		for(i=0;i<appList.size();i++)
			if(VideoProvider* videoProvider = appList[i]->getVideoProviderInterface())
				videoProvider->vp_registerVideoUser(videoUser);
	}
}


void ComponentManager::shutDown()
{
    for(int i=0;i<(int)compList.size();i++)
    {
        stb::logPrintI("Shutting Down %s Component\n",compList[i]->getInfo()->getName().c_str());
        compList[i]->shutDown();
    }
    for(int i=0;i<(int)appList.size();i++)
    {
        stb::logPrintI("Shutting Down %s Application\n",appList[i]->getInfo()->getName().c_str());
        appList[i]->shutDown();
    }
}

END_NAMESPACE_STB
