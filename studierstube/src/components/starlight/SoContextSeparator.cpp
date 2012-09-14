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
/** Class implementation for SoContextSeparator
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoContextSeparator.cpp 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/components/starlight/SoContextSeparator.h>

SO_NODE_SOURCE(SoContextSeparator);

void SoContextSeparator::initClass()
{
	// use initialization-macro
	SO_NODE_INIT_CLASS(SoContextSeparator, SoSeparator, "Separator");
}

SoContextSeparator::SoContextSeparator()
{
	SO_NODE_CONSTRUCTOR(SoContextSeparator);
	SO_NODE_ADD_FIELD(blockNotify, (TRUE));
    inTraversal = FALSE;
    // FIXME gr: a workaround around some caching problems with Coin, probably have to 
    // investigate further.
    renderCaching.setValue( SoSeparator::OFF );
}

SoContextSeparator::~SoContextSeparator()
{
}

void SoContextSeparator::doAction(SoAction * action)
{
    // printf("A(");
    inTraversal = TRUE;
    inherited::doAction( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::GLRender(SoGLRenderAction *action)
{
    // printf("G(");
    inTraversal = TRUE;
    inherited::GLRender( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::GLRenderBelowPath(SoGLRenderAction * action)
{
    // printf("GB(");
    inTraversal = TRUE;
    inherited::GLRenderBelowPath( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::GLRenderInPath(SoGLRenderAction * action)
{
    // printf("GI(");
    inTraversal = TRUE;
    inherited::GLRenderInPath( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::GLRenderOffPath(SoGLRenderAction * action)
{
    // printf("GO(");
    inTraversal = TRUE;
    inherited::GLRenderOffPath( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::callback(SoCallbackAction *action)
{
    // printf("C(");
    inTraversal = TRUE;
    inherited::callback( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::getBoundingBox(SoGetBoundingBoxAction *action)
{
    // printf("B(");
    inTraversal = TRUE;
    inherited::getBoundingBox( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::rayPick(SoRayPickAction * action)
{
    // printf("P(");
    inTraversal = TRUE;
    inherited::rayPick( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::getMatrix(SoGetMatrixAction *action)
{
    // printf("M(");
    inTraversal = TRUE;
    inherited::getMatrix( action );
    inTraversal = FALSE;
    // printf(")");
}

void SoContextSeparator::search(SoSearchAction *action)
{
    // printf("S(");
    inTraversal = TRUE;
    inherited::search( action );
    inTraversal = FALSE;
    // printf(")");
}

#ifdef __COIN__
void SoContextSeparator::audioRender(SoAudioRenderAction * action)
{
    // printf("AR(");
    inTraversal = TRUE;
    inherited::audioRender( action );
    inTraversal = FALSE;
    // printf(")");
}
#endif

void SoContextSeparator::notify(SoNotList * nl)
{
    if(!(inTraversal && blockNotify.getValue()))
    {
        inherited::notify(nl);
        // printf("|");
    }
    else
    {
        // printf(".");
    }
}
