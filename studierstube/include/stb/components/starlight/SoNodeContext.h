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
/** SoNodeContext property node header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoNodeContext.h 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SONODECONTEXT_H_
#define _SONODECONTEXT_H_

#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/nodes/SoSubNode.h>

#include "starlight.h"

class SoCallbackAction;
class SoGLRenderAction;
class SoGetBoundingBoxAction;
class SoGetMatrixAction;
class SoHandleEventAction;
class SoPickAction;
class SoSearchAction;

/**
 * This property node modifies the SoContextElement in the state of the current action.
 * It adds a node to the context which can be traversed or reported by a
 * corresponding SoNodeContextReport node. This functionality makes nodes and
 * scene graphs first-class citizens of the context. See the @ref context module
 * description for an example using this node.
 *
 * File format :
@code 
SoNodeContext {
  MFName index  []
  MFNode value  []
}
@endcode
 *
 * @author Gerhard Reitmayr
 * @ingroup context
 */
class STARLIGHT_API SoNodeContext : public SoNode
{
    SO_NODE_HEADER(SoNodeContext);

public:

    /// list of indices to affect
    SoMFName index;
    /// list of operands for modification
    SoMFNode value;

    /// static class initialization method of OpenInventor
    static void initClass();

    /// Constructor
    SoNodeContext();

protected:
    virtual void GLRender(SoGLRenderAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void handleEvent(SoHandleEventAction *action);
    virtual void pick(SoPickAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void search(SoSearchAction *action);

    /** does the actual action work. It modifies the SoNodeContextElement
     * state.
     */
    virtual void doAction(SoAction *action);

    static void  callDoAction(SoAction *, SoNode *);

    /// destructor
    virtual ~SoNodeContext();
};

#endif
