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
/** Class definition for SoContextSeparator
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoContextSeparator.h 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOCONTEXTSEPARATOR_H__
#define __SOCONTEXTSEPARATOR_H__

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/fields/SoSFBool.h>

#include "starlight.h"

/**
 * This node implements a Separator that stops notification that occur during 
 * traversal. Such notifications only happen if SoContextReport or SoNodeContextReport
 * nodes are used, but can trigger constant re-rendering and so on. Therefore
 * it is best to encapsulate a context sensitive scene graph with such a Separator
 * node. See the @ref context module description for an example of this node.
 *
 @code
 SoContextSeparator {
    SFBool blockNotify   TRUE   # flag to disable notification, 
 }
 @endcode
 * 
 * @ingroup context
 * @author Gerhard Reitmayr
 */
class STARLIGHT_API SoContextSeparator : public SoSeparator {
    typedef SoSeparator inherited;

	SO_NODE_HEADER(SoContextSeparator);

public:

    static void initClass();

    SoContextSeparator();

    /// flag to disable notification, TRUE by default
    SoSFBool blockNotify;

protected:
    
    virtual void doAction(SoAction * action);
    virtual void GLRender(SoGLRenderAction * action);
    virtual void GLRenderBelowPath(SoGLRenderAction * action);
    virtual void GLRenderInPath(SoGLRenderAction * action);
    virtual void GLRenderOffPath(SoGLRenderAction * action);
    virtual void callback(SoCallbackAction * action);
    virtual void getBoundingBox(SoGetBoundingBoxAction * action);
    virtual void getMatrix(SoGetMatrixAction * action);
    virtual void rayPick(SoRayPickAction * action);
    virtual void search(SoSearchAction * action);
#ifdef __COIN__
    virtual void audioRender(SoAudioRenderAction * action);
#endif
    
    /// changed notification to block it, if traversal and blockNotify say so.
    virtual void notify(SoNotList * nl);

    /// internal flag to denote, if there is currently a traversal under way.
    SbBool inTraversal;
private:
	virtual    ~SoContextSeparator();

};

#endif
