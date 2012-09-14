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
/** SoMultiSwitch implementation file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoMultiSwitch.cpp 31 2005-04-04 15:46:47Z daniel $
  * @file                                                                   */
 /* ======================================================================= */


#include <vector>
#include <set>
#include <algorithm>

#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>

#include <stb/components/starlight/SoMultiSwitchElement.h>
#include <stb/components/starlight/SoMultiSwitch.h>


using namespace std;

SO_NODE_SOURCE(SoMultiSwitch);

void SoMultiSwitch::initClass()
{
    if( SoType::fromName("SoMultiSwitch").isBad())
    {
        SoMultiSwitchElement::initClass();

        SO_NODE_INIT_CLASS(SoMultiSwitch, SoGroup, "Group");

        SO_ENABLE(SoCallbackAction, SoMultiSwitchElement);
        SO_ENABLE(SoGLRenderAction, SoMultiSwitchElement);
        SO_ENABLE(SoGetBoundingBoxAction, SoMultiSwitchElement);
        SO_ENABLE(SoGetMatrixAction, SoMultiSwitchElement);
        SO_ENABLE(SoHandleEventAction, SoMultiSwitchElement);
        SO_ENABLE(SoPickAction, SoMultiSwitchElement);
        SO_ENABLE(SoSearchAction, SoMultiSwitchElement);
    //SO_ENABLE(SoHandle3DEventAction, SoMultiSwitchElement);
    }
}

// constructor

SoMultiSwitch::SoMultiSwitch() : cbFunc( NULL ), duringSearchAll( FALSE )
{
    SO_NODE_CONSTRUCTOR(SoMultiSwitch);
    SO_NODE_ADD_FIELD(whichChildren, (SO_SWITCH_NONE));
    SO_NODE_ADD_FIELD(ordered, (TRUE));
    isBuiltIn = TRUE;
}

// constructor that takes approximate number of children.

SoMultiSwitch::SoMultiSwitch(int nChildren) : SoGroup(nChildren), cbFunc( NULL ), duringSearchAll( FALSE )
{
    SO_NODE_CONSTRUCTOR(SoMultiSwitch);
    SO_NODE_ADD_FIELD(whichChildren, (SO_SWITCH_NONE));
    SO_NODE_ADD_FIELD(ordered, (TRUE));
    isBuiltIn = TRUE;
}

//    Destructor

SoMultiSwitch::~SoMultiSwitch()
{}

//    Overrides method in SoNode to return FALSE if there is no
//    selected child or the selected child does not affect the state.

SbBool SoMultiSwitch::affectsState() const
{

    return TRUE;

    /*
     * FIXME: without children it would return false. However the
     *        transfer of the whichChildren field as element CAN
     *        influence later nodes. This was lost then.
     *        gerhard 20030324
    int32_t which;

	if(getNumChildren() == 0)
		return FALSE;

    if (whichChildren.isIgnored())
	    which = SO_SWITCH_NONE;
    else
	    which = (*min_element( whichChildren.getValues(0), whichChildren.getValues(0)+whichChildren.getNum()));

    // Special case-- if called during application of an
    // SoSearchAction that is searching all:
    if (this->duringSearchAll == TRUE)
	    which = SO_SWITCH_ALL;

    if (which == SO_SWITCH_NONE)
	    return FALSE;

    if (which == SO_SWITCH_ALL || which == SO_SWITCH_INHERIT)
	    return TRUE;

    if ( which >= 0 )
    {
        SbBool result = FALSE;
        for( int i = 0; i < whichChildren.getNum(); i++ )
        {
            which = whichChildren[i];
            if (which >= getNumChildren())
            {
	            which %= getNumChildren();
            }
            result |= getChild(which)->affectsState();
        }
        return result;
    }
    return FALSE;

    */
}

// function object that tests membership of an element in a set
template <class T>
class NotInSet {
private:
    const set<T> & theSet;
public:
    NotInSet( const set<T> & s) : theSet(s) {}

    inline bool operator() (T & elem) const {
        return theSet.find( elem ) == theSet.end();
    }
};

// Typical action traversal. this does the real work

void SoMultiSwitch::doAction(SoAction *action)
{
    int	numIndices = -1;
    const int *indices;
    int lastChild = getNumChildren() - 1;
    int switchCase;

    const int32_t * helper = whichChildren.getValues(0);
    int32_t helperNum = whichChildren.getNum();
    vector<int32_t> indexVector;

    // strange case, but hit it once ?!
    if( helper == NULL || helperNum == 0 )
        return;

    // either process no children or get the smallest index (makes -3 override the others)
    if (whichChildren.isIgnored())
    	switchCase = SO_SWITCH_NONE;
    else
    {
        switchCase = (*min_element( helper, helper+helperNum));

        // If index is inherited from state, get value from there
        if (switchCase == SO_SWITCH_INHERIT)
        {
            helper = SoMultiSwitchElement::get(action->getState(), &helperNum );

            if( helperNum == 0 )
                switchCase = SO_SWITCH_NONE;
            else
                switchCase = (*min_element( helper, helper+helperNum));
        }
        // Store resulting index in state if not already inherited from there
        else
        {
	        SoMultiSwitchElement::set(action->getState(), this, helper, helperNum);
        }

        // copy the children field over into the vector
        indexVector.reserve( helperNum );
        indexVector.insert(indexVector.begin(), helper, helper+helperNum );

        // if we need them ordered, do so and remove any duplicates
        if (ordered.getValue() == TRUE )
        {
            sort(indexVector.begin(), indexVector.end());
            indexVector.resize( unique(indexVector.begin(), indexVector.end()) - indexVector.begin());
        }

        // if we are in path remove all children that need not to be traversed
        if(action->getPathCode(numIndices, indices) == SoAction::IN_PATH)
        {
            lastChild = indices[numIndices -1];

            // remove the ones that
            // are not in the path set, however don't change the order etc
            std::set<int32_t> pathSet( indices, indices+numIndices );
            NotInSet<int32_t> test( pathSet );
            indexVector.resize(remove_if(indexVector.begin(), indexVector.end(),test) - indexVector.begin());
        }
    }

	// after doing all the regular stuff, lets see if we actually have children to traverse.
	// we do this here, because we still want to set the SoMultiSwitchElement and so on.
	if( getNumChildren() == 0)
		return;

    switch (switchCase) {
        case SO_SWITCH_NONE:
	        break;

        case SO_SWITCH_ALL:
	        // If traversing to compute bounding box, we have to do some
	        // work in between children
	        if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId()))
            {
	            SoGetBoundingBoxAction *bba = (SoGetBoundingBoxAction *) action;
	            SbVec3f	totalCenter(0.0, 0.0, 0.0);
	            int numCenters = 0;

	            for (int i = 0; i <= lastChild; i++)
                {
		            doChildren(bba, i);
		            if (bba->isCenterSet())
                    {
		                totalCenter += bba->getCenter();
		                numCenters++;
		                bba->resetCenter();
		            }
	            }
	            // Now, set the center to be the average:
	            if (numCenters != 0)
		            bba->setCenter(totalCenter /(float) numCenters, FALSE);
	        }
	        else {
                doChildren(action, 0, lastChild );
	        }
	        break;

       default:
            // IN_PATH or not, in indexVector only the right values are stored.
            // now we can easily iterate over them and do our thing

            // If traversing to compute bounding box, we have to do some
	        // work in between children
	        if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId()))
            {
	            SoGetBoundingBoxAction *bba = (SoGetBoundingBoxAction *) action;
	            SbVec3f	totalCenter(0.0, 0.0, 0.0);
	            int numCenters = 0;

                for( vector<int32_t>::iterator it = indexVector.begin(); it != indexVector.end(); it++ )
                {
                    doChildren(bba, ((*it) >= getNumChildren())?((*it) % getNumChildren()):(*it));
                    if (bba->isCenterSet())
                    {
		                totalCenter += bba->getCenter();
		                numCenters++;
		                bba->resetCenter();
		            }
                }
	            // Now, set the center to be the average:
	            if (numCenters != 0)
		            bba->setCenter(totalCenter /(float) numCenters, FALSE);
	        }
	        else {
                for( vector<int32_t>::iterator it = indexVector.begin(); it != indexVector.end(); it++ )
                {
                    doChildren(action, ((*it) >= getNumChildren())?((*it) % getNumChildren()):(*it));
                }
	        }
	        break;
    }
}

// calls the callback for the child and traverses it

void SoMultiSwitch::doChildren( SoAction * action, int start, int end )
{
    if( end == -1 )
        end = start;
    if( cbFunc != NULL )
    {
        for( int i = start; i <= end; i++ )
            if( (*cbFunc)( userData, this, action, i, (*children)[i]) == TRUE )
                children->traverse(action, i);
    }
    else
        children->traverse( action, start, end );

}

//  Traversal for callback action.

void SoMultiSwitch::callback(SoCallbackAction *action)
{
    doAction(action);
}

//  Traversal for GL rendering

void SoMultiSwitch::GLRender(SoGLRenderAction *action)
{
    doAction(action);
}

//  Traversal for picking

void SoMultiSwitch::pick(SoPickAction *action)
{
    doAction(action);
}

//  Traversal for get bounding box

void SoMultiSwitch::getBoundingBox(SoGetBoundingBoxAction *action)
{
    doAction(action);
}

//  Traversal for handle event

void SoMultiSwitch::handleEvent(SoHandleEventAction *action)
{
    doAction(action);
}

// Implements getMatrix action.

void SoMultiSwitch::getMatrix(SoGetMatrixAction *action)
{
    int	numIndices;
    const int *indices;

    // Only need to compute matrix if switch is a node in middle of
    // current path chain or is off path chain (since the only way
    // this could be called if it is off the chain is if the switch is
    // under a group that affects the chain).

    switch (action->getPathCode(numIndices, indices))
    {
        //If there is no path, or we are off the end, do nothing
        case SoAction::NO_PATH:
        case SoAction::BELOW_PATH:
        	break;
        //If we are in the path chain or we affect nodes in the path chain, traverse the children
        case SoAction::OFF_PATH:
        case SoAction::IN_PATH:
	        doAction(action);
	        break;
    }
}

//  Implements search action.

void SoMultiSwitch::search(SoSearchAction *action)
{
    // if the action is searching everything, then do so...
    // we set the local duringSearchAll flag to get the right behavior in affectsState !
    if (action->isSearchingAll()) {
        this->duringSearchAll = TRUE;
	    SoGroup::search(action);
        this->duringSearchAll = FALSE;
    }
    else // Otherwise, traverse according to the regular switch node rules
    {
        // First, make sure this node is found if we are searching for switches
    	SoNode::search(action);

        // Recurse
	    if (! action->isFound())
	        doAction(action);
    }
}

// Implements write action. Copied from the SGI SoSwitch implementation.

void SoMultiSwitch::write(SoWriteAction *action)
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

void SoMultiSwitch::setCallback( SoMultiSwitchCB * func, void * data )
{
    cbFunc = func;
    userData = data;
}

void SoMultiSwitch::removeCallback()
{
    cbFunc = NULL;
}
