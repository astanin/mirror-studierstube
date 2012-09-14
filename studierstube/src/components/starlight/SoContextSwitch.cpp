/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoContextSwitch - implementation
  *
  * @author  Clemens Pecinovsky, Gerhard Reitmayr
  *
  * $Id: SoContextSwitch.cpp 31 2005-04-04 15:46:47Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>

#include <stb/components/starlight/SoContextElement.h>
#include <stb/components/starlight/SoContextSwitch.h>


SO_NODE_SOURCE(SoContextSwitch);

/***********************************************************************************************
Function  * void initClass()
Purpose   * Class initializer.
Arguments * -
Return    * -
Notes     * static.
***********************************************************************************************/
void SoContextSwitch::initClass()
{
    SO_NODE_INIT_CLASS(SoContextSwitch, SoGroup, "Group");
}

/***********************************************************************************************
Function  * SoContextSwitch()
Purpose   * Constructor
Arguments * -
Return    * -
Notes     * -
***********************************************************************************************/
SoContextSwitch::SoContextSwitch()
{
    SO_NODE_CONSTRUCTOR(SoContextSwitch);
    SO_NODE_ADD_FIELD( index, (""));
    SO_NODE_ADD_FIELD( defaultChild, (-1));
}

/***********************************************************************************************
Function  * ~SoContextSwitch()
Purpose   * Destructor. 
Arguments * -
Return    * -
Notes     * -
***********************************************************************************************/
SoContextSwitch::~SoContextSwitch()
{
  //nothing do be done
}

/***********************************************************************************************
Function  * void GLRender(SoGLRenderAction *action)
Purpose   * does the render action
Arguments * renderAction
Return    * -
Notes     * -
***********************************************************************************************/
void SoContextSwitch::GLRender(SoGLRenderAction *action)
{
  SoContextSwitch::doAction(action);
}

/***********************************************************************************************
Function  * void handleEvent(SoHandleEventAction *action);
Purpose   * does the handleEvent action
Arguments * callback
Return    * -
Notes     * -
***********************************************************************************************/
void SoContextSwitch::handleEvent(SoHandleEventAction *action)
{
  SoContextSwitch::doAction(action);
}

/***********************************************************************************************
Function  * void getBoundingBox(SoGetBoundingBoxAction *action)
Purpose   * does the boundingbox action
Arguments * boundingboxAction
Return    * -
Notes     * -
***********************************************************************************************/
void SoContextSwitch::getBoundingBox(SoGetBoundingBoxAction *action)
{
  SoContextSwitch::doAction(action);
}

/***********************************************************************************************
Function  * void pick(SoPickAction *action)
Purpose   * does the pick action
Arguments * SoPickAction
Return    * -
Notes     * -
***********************************************************************************************/
void SoContextSwitch::pick(SoPickAction *action)
{
  SoContextSwitch::doAction(action);
}

/***********************************************************************************************
Function  * void getMatrix(SoGetMatrixAction *action)
Purpose   * does the getMatrixk action
Arguments * SoGetMatrixAction
Return    * -
Notes     * -
***********************************************************************************************/
void SoContextSwitch::getMatrix(SoGetMatrixAction *action)
{
  int numIndices;
  const int *indices;

  switch (action->getPathCode(numIndices,indices))
  {
    case SoAction::NO_PATH:
    case SoAction::BELOW_PATH:
      //If there is no path, or we are off the end, do nothing
      break;
    case SoAction::OFF_PATH:
    case SoAction::IN_PATH:
      //If we are in the path chain or we affect nodes in the path chain, traverse the children
      SoContextSwitch::doAction(action);  
      break;
  }
  
}

/***********************************************************************************************
Function  * void search(SoSearchAction *action)
Purpose   * does the SoSearchAction action
Arguments * SoGetMatrixAction
Return    * -
Notes     * -
***********************************************************************************************/
void SoContextSwitch::search(SoSearchAction *action)
{
  //If the action is searching everything, then traverse all children like SoGroup would
  if (action->isSearchingAll())
    SoGroup::search(action);
  else
  {
    //First make sure this node is found we are searching for alternate (or group) nodes.
    SoNode::search(action);

    //traverse the children in the usual way
    SoContextSwitch::doAction(action);
  }
}



/***********************************************************************************************
Function  * void doAction(SoAction *action)
Purpose   * sets the state elements
Arguments * Action
Return    * -
Notes     * -
***********************************************************************************************/
void SoContextSwitch::doAction(SoAction *action)
{
//    if( action->isOfType(SoGLRenderAction::getClassTypeId()))
//        SoCacheElement::invalidate(action->getState());

    if( index.isIgnored())
        return;

    int	numIndices;
    const int *indices;

    if (action->getPathCode(numIndices, indices) == SoAction::IN_PATH) {
	int	i;
	for (i = 0; i < numIndices; i++)
	    doChild(action, indices[i]);
    }
    else
	    doChild(action, -1);
}

// actual switch implementation

void SoContextSwitch::doChild(SoAction *action, int matchIndex)
{
    int32_t	which;
    const SbName & ind = index.getValue();

    if( ind == "" || !SoContextElement::isSet( action->getState(), ind ))
    {
        which = defaultChild.getValue();
    }
    else
    {
        which = atoi(SoContextElement::get( action->getState(), ind ).getString());
    }
    
    // If index is inherited from state, get value from there
    if (which == SO_SWITCH_INHERIT)        
        which = SoSwitchElement::get(action->getState());        
    // Store resulting index in state if not already inherited from there
    else
        SoSwitchElement::set(action->getState(), which);        
        
    // nothing to traverse anyway
    if( getNumChildren() == 0 )
        return;

    // Make sure it is in range
    if (which >= getNumChildren())
        which %= getNumChildren();

//    printf("SoContextSwitch %x: %i->%i : %i\n", this, ind,SoContextElement::get( action->getState(), ind ), which );

    // Now we have the real value to deal with
    switch (which) {
        
    case SO_SWITCH_NONE:
    case SO_SWITCH_INHERIT:
        break;
        
    case SO_SWITCH_ALL:
        // If traversing to compute bounding box, we have to do some
        // work in between children
        if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId())) {
            SoGetBoundingBoxAction *bba = (SoGetBoundingBoxAction *) action;
            SbVec3f	totalCenter(0.0, 0.0, 0.0);
            int		numCenters = 0;
            int 	lastChild = (matchIndex >= 0 ? matchIndex :
            getNumChildren()  - 1);
            
            for (int i = 0; i <= lastChild; i++) {
                children->traverse(bba, i, i);
                if (bba->isCenterSet()) {
                    totalCenter += bba->getCenter();
                    numCenters++;
                    bba->resetCenter();
                }
            }
            // Now, set the center to be the average:
            if (numCenters != 0)
                bba->setCenter(totalCenter /(float)numCenters, FALSE);
        }
        else {
            if (matchIndex >= 0)
                children->traverse(action, 0, matchIndex);
            else
                children->traverse(action);
        }
        break;
        
    default:                
        // Traverse indicated child
        if (matchIndex < 0 || matchIndex == which)
            children->traverse(action, (int) which);
    }
}

// Implements write action. Copied from the SGI SoSwitch implementation.

void SoContextSwitch::write(SoWriteAction *action)
{
    SoOutput *out = action->getOutput();
    
    // When writing out a switch that is in a path, we always want to
    // write out ALL children of the switch. If we did the default
    // thing of writing out just those children that affect the nodes
    // in the path, we could screw up. Consider a switch that has two
    // child separators and whichChild set to 1. If a path goes
    // through the switch to the second child, the first child, being
    // a separator, does not affect the path. But if we don't write
    // out the separator, the whichChild will reference a
    // nonexistent child. So we always write out all children.
    
    // NOTE: SoChildList::traverse() checks the current path code and
    // skips children off the path that do not affect the
    // state. Because of this we have to avoid calling it. Instead, we
    // do its work here.
    
    // This code is stolen and modified from SoGroup::write() and
    // SoChildList::traverse()
    
    int lastChild = getNumChildren() - 1;
    SoAction::PathCode pc = action->getCurPathCode();
    
    // In write-reference counting phase
    if (out->getStage() == SoOutput::COUNT_REFS) 
    {
        // Increment our write reference count
        addWriteReference(out);
        
        // If this is the first reference (i.e., we don't now have
        // multiple references), also count all appropriate children
        if (! hasMultipleWriteRefs()) 
        {
            for (int i = 0; i <= lastChild; i++) 
            {
                action->pushCurPath(i);
                action->traverse(getChild(i));
                action->popCurPath(pc);
            }
        }
    }
    // In writing phase, we have to do some more work
    else if (! writeHeader(out, TRUE, FALSE)) 
    {
        // Write fields
        const SoFieldData *fieldData = getFieldData();
        fieldData->write(out, this);
        
        // We KNOW that all children should be written, so don't
        // bother calling shouldWrite()
        
        // If writing binary format, write out number of children
        // that are going to be written
        if (out->isBinary())
            out->write(getNumChildren());
        
        for (int i = 0; i <= lastChild; i++) {
            action->pushCurPath(i);
            action->traverse(getChild(i));
            action->popCurPath(pc);
        }
        
        // Write post-children stuff
        writeFooter(out);
    }
}
