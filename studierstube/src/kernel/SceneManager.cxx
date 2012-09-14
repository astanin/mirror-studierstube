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
/* @author Denis Kalkofen
*
* $Id: SceneManager.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/kernel/SceneManager.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoGroup.h>

BEGIN_NAMESPACE_STB

SceneManager::SceneManager()
{
   root=new SoSeparator();

   sceneRoot=new SoSeparator();

   //  parent for 'SoOpenTrackerSource' (see event)
   trackersourceParent=new SoSeparator();

   displayRoot=new SoSeparator();

   //set up the scene graph
   //
   //              root
   //               |
   //       |------------------------------------------|
   //    sceneRoot                                 displayRoot	
   //       |                                          |
   //  |--------------------|---------|            SoDisplay
   // trackRoot        app-graph  app-graph
   //  |
   // trackersource (optional)
   //

   root->ref();
   root->addChild(sceneRoot);
   sceneRoot->addChild(trackersourceParent);
   root->addChild(displayRoot);
   touchRoot=root;
}

SceneManager::~SceneManager()
{
   //nil
}

void
SceneManager::update()
{
    touchRoot->touch();
}

void 
SceneManager::registerApp(SoSeparator *appRoot)
{
    sceneRoot->addChild(appRoot);
}

SoSeparator* 
SceneManager::getDisplayRoot()
{
	return displayRoot;
}

SoSeparator* 
SceneManager::getSceneRoot()
{
	return sceneRoot;
}

void 
SceneManager::setTrackerSource(SoNode *otSource)
{
	trackersourceParent->removeAllChildren();
	trackersourceParent->addChild(otSource);

    //sceneRoot->replaceChild(0,otSource);
}


SoNode*
SceneManager::getTrackerSource()
{
	if(trackersourceParent->getNumChildren()>0)
	{
		assert(trackersourceParent->getNumChildren()==1);
		return trackersourceParent->getChild(0);
	}

	return NULL;

	//return sceneRoot->getChild(0);
}


void 
SceneManager::setDisplay(SoGroup *display)
{
    displayRoot->addChild(display);
   
    if(display->getNumChildren()>0)
    {
         touchRoot=(SoNode*)display->getChild(0);
    }
}


END_NAMESPACE_STB
