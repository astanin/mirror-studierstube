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
 * header file for the SoContextMultiSwitch node, a multi switch node based
 * on the context.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoContextMultiSwitch.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOCONTEXTMULTISWITCH_H_
#define _SOCONTEXTMULTISWITCH_H_

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFName.h>

#include "SoMultiSwitch.h"

/**
 * The SoContextMultiSwitch is an extension to the SoMultiSwitch node to allow some
 * experiements with the context sensitive scene graph concept. It allows to
 * define subsets of its children and to use the SoContextElement to select during 
 * traversal which sets of children will be traversed. The ordered field behaves as in the SoMultiSwitch node.
 *
 * Note that the values SO_SWITCH_NONE, SO_SWITCH_INHERIT and SO_SWITCH_ALL have
 * their usual meaning in the whichChildren field. They override any other indices given in the
 * same set of nodes. Their individual order is SO_SWITCH_ALL, SO_SWITCH_INHERIT and SO_SWITCH_NONE.
 *
 * The defaultChildren field is used, if the index specified is not part of the context.
 * This is also interpreted as the context value of that index is NIL. Then the
 * value of the defaultChild is used to define the behaviour of the Switch.
 *
 * File format :
<pre>SoContextMultiSwitch {
  SFBool  ordered       TRUE
  SFInt32 index         INT32_MIN
  MFInt32 whichChildren []
  MFInt32 numChildren   []
  MFInt32 defaultChildren [-1]
}</pre>
 *
 * @author Gerhard Reitmayr
 * @ingroup context
 */
class STARLIGHT_API SoContextMultiSwitch : public SoMultiSwitch
{
    SO_NODE_HEADER(SoContextMultiSwitch);

public:
	SoContextMultiSwitch();

	virtual ~SoContextMultiSwitch();

    /** 
     * this field stores the index in the context to be used to select the index of the set
     * of children to be traversed. if it is smaller than 0 or greater than the number
     * of sets, the switch will traverse no children at all. 
     */
    SoSFName index;

    /** 
     * this field stores the number of indices in the whichChildren field
     * that belong to each subset of children. It works similar to the
     * fields in SoFaceSet.
     */
    SoMFInt32 numChildren;

    /** 
     * The defaultChildren field is used, if the index specified is not part of the context.
     * This is also interpreted as the context value of that index is NIL. Then the
     * value of the defaultChild is used to define the behaviour of the Switch.
     */
    SoMFInt32 defaultChildren;

    /** static method to initialize class for OpenInventor. It also
     * initializes SoGroupSwitchElement and enables it for all necessary
     * actions.  
     */
    static void initClass();

    /** Overrides method in SoMultiSwitch to return default value
     * because we can only determine within an action which nodes to process. */
    virtual SbBool affectsState() const;

protected:
    
    /** does the actual action work. It computes which children to traverse
     * and implements some special behaviour for bounding box actions. 
     * @param action the action to apply to the children
     */
    virtual void doAction(SoAction *action);
};

#endif // _SOCONTEXTMULTISWITCH_H_

