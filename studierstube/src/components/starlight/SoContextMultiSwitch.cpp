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
/**
 * implementation of the SoContextMultiSwitch node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoContextMultiSwitch.cpp 31 2005-04-04 15:46:47Z daniel $
 * @file                                                                   */
/* ======================================================================= */

#include <vector>
#include <set>
#include <algorithm>

#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <stb/components/starlight/SoContextElement.h>
#include <stb/components/starlight/SoContextMultiSwitch.h>
#include <stb/components/starlight/SoMultiSwitchElement.h>

using namespace std;

SO_NODE_SOURCE(SoContextMultiSwitch);

SoContextMultiSwitch::SoContextMultiSwitch()
{
    SO_NODE_CONSTRUCTOR(SoContextMultiSwitch);
    SO_NODE_ADD_FIELD(numChildren, (-1));
    SO_NODE_ADD_FIELD(index,(""));
    SO_NODE_ADD_FIELD(defaultChildren, (-1));
    numChildren.deleteValues(0);
    defaultChildren.deleteValues(0);
    isBuiltIn = TRUE;
}

SoContextMultiSwitch::~SoContextMultiSwitch()
{

}

void SoContextMultiSwitch::initClass()
{
    if( SoType::fromName("SoContextMultiSwitch").isBad())
    {
        SoMultiSwitch::initClass();
        SO_NODE_INIT_CLASS(SoContextMultiSwitch, SoMultiSwitch, "SoMultiSwitch");
    }
}

// Overrides method in SoMultiSwitch to return default value
// because we can only determine within an action which nodes to process

SbBool SoContextMultiSwitch::affectsState() const
{
    return SoNode::affectsState();
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

void SoContextMultiSwitch::doAction(SoAction *action)
{
    const SbName & ind = index.getValue();
    if( index.isIgnored() || ind == "" || whichChildren.isIgnored() ||
        numChildren.isIgnored() || getNumChildren() == 0 )
        return;

    int	numIndices = numChildren.getNum();
    const int32_t *indices = numChildren.getValues(0);
    int lastChild = getNumChildren() - 1;
    int switchCase;

    int32_t val;
    const int32_t * children = whichChildren.getValues(0);
    int32_t childrenNum = whichChildren.getNum();
    vector<int32_t> indexVector;

    if(SoContextElement::isSet(action->getState(), ind))  // element index set
    {
        // get the group to traverse from the element
        val = atoi(SoContextElement::get(action->getState(), ind ).getString());
        if( val < -3 )
            switchCase = SO_SWITCH_NONE;
        else
        {
            switchCase = val;
            // compute switchCase and store the set of children to traverse
            if( val < 0 )
            {
                // If index is inherited from state, get value from there
                if (switchCase == SO_SWITCH_INHERIT)
                {
                    children = SoMultiSwitchElement::get(action->getState(), &childrenNum );
                    if( childrenNum == 0 )
                        switchCase = SO_SWITCH_NONE;
                    else
                        switchCase = (*min_element( children, children+childrenNum));

                    indexVector.insert( indexVector.begin(), children, children+childrenNum );
                }
            }
            else
            {
                if( val >= numIndices)
                    val %= numIndices;

                int count = 0;
                int j;

                for( j = 0; j <= val -1; j ++ )
                    count += indices[j];

                indexVector.insert( indexVector.begin(), children+count, children+count+indices[val] );
                // Store resulting index in state if not already inherited from there
                SoMultiSwitchElement::set(action->getState(), this, children + count, indices[val]);
            }
        }
    }
    else // otherwise
    {
        if( defaultChildren.isIgnored() != TRUE && defaultChildren.getNum() > 0 )
        {
            const int32_t * defaults = defaultChildren.getValues(0);
            int32_t defaultsNum = defaultChildren.getNum();
            switchCase = (*min_element( defaults, defaults+defaultsNum));

            // If index is inherited from state, get value from there
            if (switchCase == SO_SWITCH_INHERIT)
            {
                children = SoMultiSwitchElement::get(action->getState(), &childrenNum );
                if( childrenNum == 0 )
                    switchCase = SO_SWITCH_NONE;
                else
                    switchCase = (*min_element( children, children+childrenNum));

                indexVector.insert( indexVector.begin(), children, children+childrenNum );
            }
            else
            {
                indexVector.insert( indexVector.begin(), defaults, defaults + defaultsNum);
                // Store resulting index in state if not already inherited from there
                SoMultiSwitchElement::set(action->getState(), this, defaults, defaultsNum);
            }
        }
        else
        {
            switchCase = SO_SWITCH_NONE;
        }
    }

    // if we need them ordered, do so and remove any duplicates
    // note this should work on the emtpy indexVector as well
    if (ordered.getValue() == TRUE )
    {
        sort(indexVector.begin(), indexVector.end());
        indexVector.resize( unique(indexVector.begin(), indexVector.end()) - indexVector.begin());
    }
    // if we are in path remove all children that need not to be traversed
    // note this should work on the emtpy indexVector as well
    if(action->getPathCode(numIndices, indices) == SoAction::IN_PATH)
    {
        lastChild = indices[numIndices -1];

        // remove the ones that
        // are not in the path set, however don't change the order etc
        std::set<int32_t> pathSet( indices, indices+numIndices );
        NotInSet<int32_t> test( pathSet );
        indexVector.resize(remove_if(indexVector.begin(), indexVector.end(),test) - indexVector.begin());
    }

    switch (switchCase) {
        case SO_SWITCH_NONE:
	        break;

        case SO_SWITCH_ALL:
            if( lastChild < 0 )
                break;
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
                    doChildren(bba, ((*it) > getNumChildren())?((*it) % getNumChildren()):(*it));
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
                    doChildren(action, ((*it) > getNumChildren())?((*it) % getNumChildren()):(*it));
                }
	        }
	        break;
    }
}
