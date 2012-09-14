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
/** SoContext source file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoContext.cpp 31 2005-04-04 15:46:47Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#ifdef __COIN__
#include <Inventor/actions/SoAudioRenderAction.h>
#endif


#include <stb/components/starlight/SoContextElement.h>
#include <stb/components/starlight/SoContext.h>

#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif

SO_NODE_SOURCE(SoContext);

void SoContext::initClass()
{
    SO_NODE_INIT_CLASS(SoContext, SoNode, "Node");

    // explicitly add to the SoHandle3DEventAction method table
    // SoHandle3DEventAction::addMethod(SoContext::getClassTypeId(), SoContext::callDoAction);
#ifdef __COIN__
    SoAudioRenderAction::addMethod(SoContext::getClassTypeId(), SoContext::callDoAction);
#endif
}

// constructor

SoContext::SoContext()
{
    SO_NODE_CONSTRUCTOR(SoContext);

    SO_NODE_DEFINE_ENUM_VALUE( Mode, ADD );
    SO_NODE_DEFINE_ENUM_VALUE( Mode, SET );
    SO_NODE_DEFINE_ENUM_VALUE( Mode, CLEAR );
    SO_NODE_DEFINE_ENUM_VALUE( Mode, CLEAR_ALL );
    SO_NODE_SET_SF_ENUM_TYPE(mode, Mode);
    SO_NODE_ADD_FIELD(mode, (ADD));
    SO_NODE_ADD_FIELD(index, (""));
    SO_NODE_ADD_FIELD(value, (""));

    index.deleteValues(0);
    value.deleteValues(0);
    index.setDefault(TRUE);
    value.setDefault(TRUE);
    isBuiltIn = TRUE;
}

SoContext::~SoContext()
{
}

void SoContext::GLRender(SoGLRenderAction *action)
{
    doAction(action);
}

void SoContext::callback(SoCallbackAction *action)
{
    doAction(action);
}

void SoContext::getBoundingBox(SoGetBoundingBoxAction *action)
{
    doAction(action);
}

void SoContext::handleEvent(SoHandleEventAction *action)
{
    doAction(action);
}

void SoContext::pick(SoPickAction *action)
{
    doAction(action);
}

void SoContext::getMatrix(SoGetMatrixAction *action)
{
    doAction(action);
}

void SoContext::search(SoSearchAction *action)
{
    doAction(action);
    SoNode::search(action);
}

void SoContext::callDoAction(SoAction * action, SoNode * node)
{
    node->doAction( action );
}

void SoContext::doAction(SoAction *action)
{
    switch( mode.getValue())
    {
    case ADD:
        {
            int32_t size = min( index.getNum(), value.getNum());
            SoContextElement::add(action->getState(), this, index.getValues(0), value.getValues(0), size );
            break;
        }
    case SET:
        {
            int32_t size = min( index.getNum(), value.getNum());
            SoContextElement::set(action->getState(), this, index.getValues(0), value.getValues(0), size );
            break;
        }
    case CLEAR:
        {
            SoContextElement::clear(action->getState(), this, index.getValues(0), index.getNum());
            break;
        }
    case CLEAR_ALL:
        {
            SoContextElement::clear(action->getState(), this );
            break;
        }
    }
}

