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
/** SoNodeContextReport source file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoNodeContextReport.cpp 24 2005-02-10 19:38:46Z reitmayr $
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
#include <Inventor/misc/SoChildList.h>

#include <stb/components/starlight/SoNodeContextReport.h>
#include <stb/components/starlight/SoContextElement.h>

SO_NODE_SOURCE(SoNodeContextReport);

void SoNodeContextReport::initClass()
{
    SO_NODE_INIT_CLASS(SoNodeContextReport, SoNode, "Node");

    // explicitly add to the SoHandle3DEventAction method table
    // SoHandle3DEventAction::addMethod(SoNodeContextReport::getClassTypeId(), SoNodeContextReport::callDoAction);
#ifdef __COIN__
    SoAudioRenderAction::addMethod(SoNodeContextReport::getClassTypeId(), SoNodeContextReport::callDoAction);
#endif
}

// constructor

SoNodeContextReport::SoNodeContextReport()
{
    SO_NODE_CONSTRUCTOR(SoNodeContextReport);

    SO_NODE_ADD_FIELD(index, (""));
    SO_NODE_ADD_FIELD(value, (NULL));
    SO_NODE_ADD_FIELD(traverse, (TRUE));
    SO_NODE_ADD_FIELD(report, (FALSE));

    index.deleteValues(0);
    value.deleteValues(0);
    index.setDefault(TRUE);
    value.setDefault(FALSE);
    isBuiltIn = TRUE;   

    children = new SoChildList( this );
}

SoNodeContextReport::~SoNodeContextReport()
{
    delete children;
}

void SoNodeContextReport::GLRender(SoGLRenderAction *action)
{
    doAction(action);
}

void SoNodeContextReport::callback(SoCallbackAction *action)
{
    doAction(action);
}

void SoNodeContextReport::getBoundingBox(SoGetBoundingBoxAction *action)
{
    doAction(action);
}

void SoNodeContextReport::handleEvent(SoHandleEventAction *action)
{
    doAction(action);
}

void SoNodeContextReport::pick(SoPickAction *action)
{
    doAction(action);
}

void SoNodeContextReport::getMatrix(SoGetMatrixAction *action)
{
    doAction(action);
}

void SoNodeContextReport::search(SoSearchAction *action)
{
    doAction(action);
    SoNode::search(action);
}

void SoNodeContextReport::callDoAction(SoAction * action, SoNode * node)
{
    node->doAction( action );
}

void SoNodeContextReport::doAction(SoAction *action)
{
    // short cut
    if( traverse.getValue() == FALSE && report.getValue() == FALSE )
        return;

    int i = -1;
    SoState * state = action->getState();
    SbBool refresh = FALSE;

    if( index.getNum() == children->getLength())
    {
        for( i = 0; i < index.getNum(); i++ )
        {
            if( SoContextElement::getNode(state, index[i]) != (*children)[i] )
                break;
        }
    }
    if((index.getNum() != children->getLength()) || ( i != index.getNum()))
    {
        SbBool oldFlag = enableNotify(FALSE);
        children->truncate(0);
        for(i = 0; i < index.getNum(); i++ )
        {
            SoNode * node = SoContextElement::getNode(state, index[i]);
            if( node != NULL )
            {
                children->append(node);
            }
        }
        enableNotify( oldFlag );
        touch();
        refresh = TRUE;
    }
    // otherwise set value from the context
    if( refresh == TRUE && report.getValue() == TRUE )
    {
        value.setNum( index.getNum());
        SoNode ** data = value.startEditing();
        for( i = 0; i < index.getNum(); i++ )
        {
            data[i] = SoContextElement::getNode(state, index[i]);
        }
        value.finishEditing();
    }
    if( traverse.getValue() == TRUE )
    {
        children->traverse(action);
    }
}
