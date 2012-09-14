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
/** Class definition for SoAbortGroup
  *
  * @author Hermann Wurnig, Gerd Hesina, Gerhard Reitmayr
  *
  * $Id: SoAbortGroup.h 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOABORTGROUP_H__
#define __SOABORTGROUP_H__

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoSFBitMask.h>

#include "starlight.h"

class SoGLRenderAction;
class SoCallbackAction;
class SoGetBoundingBoxAction;
class SoHandleEventAction;
class SoPickAction;
class SoGetMatrixAction;
class SoSearchAction;
class SoWriteAction;

/**
 *  This class implements a group-node that stops the
 *  any action from traversing its children, depending
 *  on the value of the field abortActions. This bit field allows you
 *  to set for each action individually whether it should traverse the
 *  children or not. Setting a flag will not traverse the children !
 * @ingroup camera
 * @ingroup util
 * @author Hermann Wurnig, Gerd Hesina, Gerhard Reitmayr
 */
class STARLIGHT_API SoAbortGroup : public SoGroup {

	SO_NODE_HEADER(SoAbortGroup);

public:

    static void initClass();	

    SoAbortGroup();

    /// the field defining which actions will not traverse children
    SoSFBitMask abortActions;
        
    /// the flags used in the bit field to define during which actions not to traverse
    enum MyActions {
        NONE = 0,
		RENDER = 1,
        CB = 2,
        BBOX = 4,
        EVENT = 8,
        PICK = 16,
        MATRIX = 32,
        SEARCH = 64,
        WRITE = 128,
        OTHER = 256
    };

protected:

    virtual void GLRender(SoGLRenderAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void handleEvent(SoHandleEventAction *action);
    virtual void pick(SoPickAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void search(SoSearchAction *action);
    virtual void write(SoWriteAction *action);
    
private:
	virtual    ~SoAbortGroup();

};

#endif
