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
/** SoContextReport source file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoContextReport.cpp 31 2005-04-04 15:46:47Z daniel $
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
#include <stb/components/starlight/SoContextReport.h>


SO_NODE_SOURCE(SoContextReport);

void SoContextReport::initClass()
{
    SO_NODE_INIT_CLASS(SoContextReport, SoNode, "Node");

    // explicitly add to the SoHandle3DEventAction method table
    // SoHandle3DEventAction::addMethod(SoContextReport::getClassTypeId(), SoContextReport::callDoAction);
#ifdef __COIN__
    SoAudioRenderAction::addMethod(SoContextReport::getClassTypeId(), SoContextReport::callDoAction);
#endif
}

// constructor

SoContextReport::SoContextReport()
{
    SO_NODE_CONSTRUCTOR(SoContextReport);

    SO_NODE_ADD_FIELD(index, (""));
    SO_NODE_ADD_FIELD(value, (""));
    SO_NODE_ADD_FIELD(defaultValue, (""));

    index.deleteValues(0);
    value.deleteValues(0);
    defaultValue.deleteValues(0);

    index.setDefault(TRUE);
    value.setDefault(TRUE);
    defaultValue.setDefault(TRUE);

    isBuiltIn = TRUE;
}

SoContextReport::~SoContextReport()
{
}

void SoContextReport::GLRender(SoGLRenderAction *action)
{
    doAction(action);
}

void SoContextReport::callback(SoCallbackAction *action)
{
    doAction(action);
}

void SoContextReport::getBoundingBox(SoGetBoundingBoxAction *action)
{
    doAction(action);
}

void SoContextReport::handleEvent(SoHandleEventAction *action)
{
    doAction(action);
}

void SoContextReport::pick(SoPickAction *action)
{
    doAction(action);
}

void SoContextReport::getMatrix(SoGetMatrixAction *action)
{
    doAction(action);
}

void SoContextReport::search(SoSearchAction *action)
{
    doAction(action);
    SoNode::search(action);
}

void SoContextReport::callDoAction(SoAction * action, SoNode * node)
{
    node->doAction( action );
}

void SoContextReport::doAction(SoAction *action)
{
    int i;
    SoState * state = action->getState();

    if( index.getNum() == value.getNum() )
    {
        for( i = 0; i < index.getNum(); i++ )
        {
            if( SoContextElement::isSet(state, index[i]) == TRUE )
            {
                if( SoContextElement::get(state, index[i]) != value[i] )
                    break;
            }
            else
            {
                if( defaultValue.getNum() < i )
                {
                    if( value[i] != "" )
                        break;
                }
                else
                {
                    if( value[i] != defaultValue[i] )
                        break;
                }
            }
        }
        // if all are equal then do nothing
        if( i == index.getNum())
            return;
    }
    // otherwise set value from the context
    value.setNum( index.getNum());
    SbString * data = value.startEditing();
    for( i = 0; i < index.getNum(); i++ )
    {
        if( SoContextElement::isSet(state, index[i]) == TRUE )
        {
            data[i] = SoContextElement::get(state, index[i]);
        }
        else
        {
            data[i] = ( defaultValue.getNum() < i )?(""):(defaultValue[i]);
        }
    }
    value.finishEditing();
}
