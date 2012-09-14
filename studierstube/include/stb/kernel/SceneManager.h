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
/** The header file for the SceneManager class.  
 *  
 * @author Denis Kalkofen  
 *  
 * $Id: SceneManager.h 25 2005-11-28 16:11:59Z denis $  
 * @file                                                                   */  
/* ======================================================================= */  

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
#include <stb/base/macros.h>
#include <stb/kernel/Studierstube.h>


class SoSeparator;
class SoGroup;
class SbName;
class SoNode;


BEGIN_NAMESPACE_STB


/**@ingroup kernel
 * The SceneManager holds studierstube's main scene graph and implements functions to 
 * get parts of it and set specific nodes in designated subgraphs. 
 *    
 *              root(SoSeparator*)	
 *               |
 *       |--------------------------------|
 *    sceneRoot(SoSeparator*)	   displayRoot(SoSeparator*)	
 *       |                                |
 *  |-----------|-----|                 SoDisplay
 * trackRoot   
 *
 */
class KERNEL_API SceneManager
{
public:
    /**
     *     The Constructor	
     */
    SceneManager();

    /**
     *     The destructor.
     */
    ~SceneManager();

    /************************************************************************
     * Calls touch() on the node defined by the 'touchRoot' variable. 
     ************************************************************************/
    void update();
    
    /************************************************************************
     * Adds display to the 'displayRoot' node. 
     * Sets the 'touchRoot' pointer to point to the 'displayRoot' node.
     ************************************************************************/
    void setDisplay(SoGroup *display);
    
    /************************************************************************
     * Returns a pointer to all displays root nodeS
     ************************************************************************/
    SoSeparator* getDisplayRoot();
    
    /************************************************************************
     * Adds 'appRoot' to the applications sceneRoot node.
     ************************************************************************/
    void registerApp(SoSeparator *appRoot);

    /************************************************************************
     * Returns a pointer to the applications scene root node.
     ************************************************************************/
    SoSeparator* getSceneRoot();

    /************************************************************************
     * Replaces the first child of the applications scene root node. 
     ************************************************************************/
    void setTrackerSource(SoNode *otSource);

	SoNode *getTrackerSource();

protected:	
    /************************************************************************
     * Studierstube's root node
     ************************************************************************/
    SoSeparator* root;
    
    /************************************************************************
     * A pointer to the node which is 'touched' during an update. 
     *  Default = root. 
     ************************************************************************/
    SoNode* touchRoot;

    /************************************************************************
     * A pointer to all the applications root node.
     ************************************************************************/
    SoSeparator* sceneRoot;

    /************************************************************************
     *  A pointer to all the SoDisplay's (see viewer) root node.
     ************************************************************************/
    SoSeparator* displayRoot;


	/// Parent for the (optional) tracker source instance
	SoSeparator* trackersourceParent;

private:
	
};// class 


END_NAMESPACE_STB


#endif//_SCENEMANAGER_H_
