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
/** Class implementation for SoAbortGroup
  *
  * @author Hermann Wurnig, Gerhard Reitmayr
  *
  * $Id: SoAbortGroup.cpp 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */
#include <stb/components/starlight/SoAbortGroup.h>

SO_NODE_SOURCE(SoAbortGroup);

/////////////////////////////////////////////////////////////////////
// Name:		initClass
// Purpose:		sets up type- and file-format name-information for
//				the class
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoAbortGroup::initClass()
{
	// use initialization-macro
	SO_NODE_INIT_CLASS(SoAbortGroup, SoGroup, "Group");

    //SoHandle3DEventAction::addMethod(SoAbortGroup::getClassTypeId(), SoAbortGroup::handle3DEventS);
}

/////////////////////////////////////////////////////////////////////
// Name:		SoAbortGroup
// Purpose:		Constructor of class SoAbortGroup
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoAbortGroup::SoAbortGroup()
{
	SO_NODE_CONSTRUCTOR(SoAbortGroup);

    SO_NODE_DEFINE_ENUM_VALUE(Actions, NONE );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, RENDER );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, CB );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, BBOX );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, EVENT );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, PICK );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, MATRIX );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, SEARCH );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, WRITE );
    SO_NODE_DEFINE_ENUM_VALUE(Actions, OTHER );
    SO_NODE_SET_SF_ENUM_TYPE(abortActions, Actions);

	// declare fields and their default-values
	SO_NODE_ADD_FIELD(abortActions, (NONE));
}

/////////////////////////////////////////////////////////////////////
// Name:		~SoAbortGroup
// Purpose:		Destructor of class SoAbortGroup
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SoAbortGroup::~SoAbortGroup()
{
}

void SoAbortGroup::GLRender(SoGLRenderAction *action)
{
    if( abortActions.getValue() & RENDER )
        SoNode::GLRender( action );
    else
        SoGroup::GLRender( action );
}

void SoAbortGroup::callback(SoCallbackAction *action)
{
    if( abortActions.getValue() & CB )
        SoNode::callback( action );
    else
        SoGroup::callback( action );    
}

void SoAbortGroup::getBoundingBox(SoGetBoundingBoxAction *action)
{
    if( abortActions.getValue() & BBOX )
        SoNode::getBoundingBox( action );
    else
        SoGroup::getBoundingBox( action );    
}

void SoAbortGroup::handleEvent(SoHandleEventAction *action)
{
    if( abortActions.getValue() & EVENT )
        SoNode::handleEvent( action );
    else
        SoGroup::handleEvent( action );
}

void SoAbortGroup::pick(SoPickAction *action)
{
    if( abortActions.getValue() & PICK )
        SoNode::pick( action );
    else
        SoGroup::pick( action );
}

void SoAbortGroup::getMatrix(SoGetMatrixAction *action)
{
    if( abortActions.getValue() & MATRIX )
        SoNode::getMatrix( action );
    else
        SoGroup::getMatrix( action );
}

void SoAbortGroup::search(SoSearchAction *action)
{
    if( abortActions.getValue() & SEARCH )
        SoNode::search( action );
    else
        SoGroup::search( action );
}

void SoAbortGroup::write(SoWriteAction *action)
{
    if( abortActions.getValue() & WRITE )
        SoNode::write( action );
    else
        SoGroup::write( action );
}

/// @todo (priority:1,effort:2h,reporter:flo) find way to register this for Handle3DEventAction, or generic actions! 

//void SoAbortGroup::handle3DEventS(SoAction *action, SoNode *node)
//{
//    SoAbortGroup * group = (SoAbortGroup *)node;
//    if( group->abortActions.getValue() & EVENT3D )
//        node->SoNode::doAction( action );
//    else
//        group->SoGroup::doAction( action );
//}
