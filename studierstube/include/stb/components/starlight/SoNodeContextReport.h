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
/** SoNodeContextReport property node header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoNodeContextReport.h 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SONODECONTEXTREPORT_H_
#define _SONODECONTEXTREPORT_H_

#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoSubNode.h>

#include "starlight.h"

class SoCallbackAction;
class SoGLRenderAction;
class SoGetBoundingBoxAction;
class SoGetMatrixAction;
class SoHandleEventAction;
class SoPickAction;
class SoSearchAction;
class SoChildList;

/**  
 * This property node reports and/or traversed nodes values from the SoContextElement 
 * set by a SoNodeContext node. It will only report values, if they differ from the ones
 * in the current state to avoid constant notification and re-rendering. However,
 * it will always traverse the nodes. The flags traverse and report enable the
 * respective behaviours.
 * 
 * File format :
@code
SoNodeContextReport {
  MFName index  []
  MFNode value  []
  SFBool traverse TRUE
  SFBool report   FALSE
}
@endcode
 *
 * @author Gerhard Reitmayr
 * @ingroup context
 */
class STARLIGHT_API SoNodeContextReport : public SoNode 
{
    SO_NODE_HEADER(SoNodeContextReport);

public:

    /// the indexes to retrieve the node references from
    SoMFName index;
    /// output field to report the node references
    SoMFNode value;

    /// flag to enable traversal of the nodes
    SoSFBool traverse;
    /// flag to enable report of the nodes
    SoSFBool report;

    /// static class initialization method of OpenInventor
    static void initClass();
    
    /// Constructor
    SoNodeContextReport();

protected:
    virtual void GLRender(SoGLRenderAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void handleEvent(SoHandleEventAction *action);
    virtual void pick(SoPickAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void search(SoSearchAction *action);

    /** does the actual action work. It modifies the SoContextElement
     * state.
     */
    virtual void doAction(SoAction *action);

    static void  callDoAction(SoAction *, SoNode *);    

    /// the internal children list which takes care of traversal
    SoChildList * children;

    /// destructor
    virtual ~SoNodeContextReport();
};

#endif

