/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** source file for SoEventAction node
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: SoEventAction.cpp 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#if !defined(__INTEL_COMPILER) && _MSC_VER==1200
#  pragma message(" ")
#  pragma message("daniel2all: i can not compile this file with VC6, need to switch to Intel compiler for this file!")
#  pragma message(" ")
#endif

#include "Inventor/SbString.h"

//#include <stb/components/starlight/SoContextElement.h>


#include <stb/components/event/SoEventAction.h>
#include <stb/components/event/SoEventNode.h>
#include <stb/components/event//SoEventGroup.h>
#include <stb/components/event/SoEventSeparator.h>
#include <stb/components/event/SoEventKit.h>

SO_ACTION_SOURCE(SoEventAction);

void SoEventAction::initClass(void)
{
    SO_ACTION_INIT_CLASS(SoEventAction, SoCallbackAction );

    SO_ACTION_ADD_METHOD(SoEventNode, SoEventAction::nodeS<SoEventNode>);
    SO_ACTION_ADD_METHOD(SoEventGroup, SoEventAction::nodeS<SoEventGroup>);
    SO_ACTION_ADD_METHOD(SoEventSeparator, SoEventAction::nodeS<SoEventSeparator>);
    SO_ACTION_ADD_METHOD(SoEventKit, SoEventAction::nodeS<SoEventKit>);
}

SoEventAction::SoEventAction(void)
{   
    SO_ACTION_CONSTRUCTOR(SoEventAction);

    event = NULL;
    grabbedPath = NULL;
}

SoEventAction::~SoEventAction(void)
{
    if( grabbedPath != NULL )
    {
        grabbedPath->unref();   
        grabbedPath = NULL;
    }
}

void SoEventAction::setTailAsGrabber(void)
{   
    releaseGrabber();
    grabbedPath = getCurPath()->copy();
    grabbedPath->ref();
    grabbedPath->getTail()->grabEventsSetup();
}

void SoEventAction::releaseGrabber(void)
{
    if( grabbedPath != NULL )
    {
        grabbedPath->unref();
        grabbedPath->getTail()->grabEventsCleanup();
        grabbedPath = NULL;
    }
}

SoNode * SoEventAction::getGrabber(void) const
{
    if( grabbedPath != NULL )
    {
        return grabbedPath->getTail();
    }
    return NULL;
}

SoPath * SoEventAction::getGrabberPath(void) const
{
    return grabbedPath;
}


template <class T> void SoEventAction::nodeS( SoAction * action, SoNode * node )
{
    assert( action && node );
    assert( action->isOfType(SoEventAction::getClassTypeId()));
    assert( node->isOfType(T::getClassTypeId()));

    SoEventAction * eventAction = (SoEventAction *) action;
    T * eventNode = (T *) node;

    if( eventAction->hasTerminated())
        return;
    
    eventNode->preOrder( eventAction );
    eventNode->doAction( eventAction );
    eventNode->postOrder( eventAction );
}
