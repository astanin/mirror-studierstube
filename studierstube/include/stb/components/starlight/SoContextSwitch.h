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
 * header for SoContextSwitch
 *
 * @author Clemens Pecinovsky, Gerhard Reitmayr
 *
 * $Id: SoContextSwitch.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOCONTEXTSWITCH_H_
#define _SOCONTEXTSWITCH_H_

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFName.h>

#include "starlight.h"

class SoCallbackAction;
class SoGLRenderAction;
class SoGetBoundingBoxAction;
class SoGetMatrixAction;
class SoHandleEventAction;
class SoPickAction;
class SoSearchAction;
class SoHandle3DEventAction;
class SoWriteAction;

/**
* switch that traverses differently based on the context element.
* This is the simplest version of a switch based on the context element.
* It checks on a single index and takes the value of the index to 
* behave like a typical switch element. The index field defines which index 
* to use. The value of the index in the context will be interpreted as for an
* ordinary switch node. If the value of the index is out of the typical switch
* range [-3, inf), or there is no value for that index, the node does not traverse
* any children. 
*
* The defaultChild field is used, if the index specified is not part of the context.
* This is also interpreted as the context value of that index is NIL. Then the
* value of the defaultChild is used to define the behaviour of the Switch.
* 
* Note also that SO_SWITCH_INHERIT interacts with the same element as the original
* Switch node. That way ContextSwitches and Switches can interact.
* 
* The implementation is taken largely from the original OpenInventor Switch node.
*
* @author Gerhard Reitmayr
* 
* Fileformat :
<pre>
SoContextSwitch {
    SFInt32 index INT32_MIN
    SFInt32 defaultChild -1
}
</pre>
* @ingroup context
*/
class STARLIGHT_API SoContextSwitch : public SoGroup
{
    SO_NODE_HEADER(SoContextSwitch);
    
public:
  
    /// the index in the Context to be used as switch value
    SoSFName index;
    
    /// the default switch behaviour, if the index is not set
    SoSFInt32 defaultChild;
    
    /// OIV class initialization
    static void initClass();

    /// constructor
    SoContextSwitch();
    
protected:
    virtual void GLRender(SoGLRenderAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void handleEvent(SoHandleEventAction *action);
    virtual void pick(SoPickAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void search(SoSearchAction *action);
    virtual void write(SoWriteAction *action);

    virtual void doAction(SoAction *action);
    
    /** Traverses correct child based on value of whichChild field. If
     * matchIndex is non-negative, the indicated child must match it
     * for it to be traversed. Sets switch element in state unless we
     * are inheriting the value. 
     */
    void doChild(SoAction *action, int matchIndex);

    virtual ~SoContextSwitch();
};

#endif
