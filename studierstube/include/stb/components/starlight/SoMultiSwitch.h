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
/** SoMultiSwitch header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoMultiSwitch.h 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOMULTISWITCH_H_
#define _SOMULTISWITCH_H_

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFBool.h>

#include "starlight.h"

class SoCallbackAction;
class SoGLRenderAction;
class SoGetBoundingBoxAction;
class SoGetMatrixAction;
class SoHandleEventAction;
class SoPickAction;
class SoSearchAction;
class SoWriteAction;

/* @def Don't traverse any children	*/
#define SO_SWITCH_NONE		(-1)	
/* @def Inherit value from state	*/
#define SO_SWITCH_INHERIT	(-2)	
/* @def Traverse all children	    */
#define SO_SWITCH_ALL		(-3)	

class  SoMultiSwitch;

/**
 * callback type for the test callback called by a SoMultiSwitch node.
 * @param userdata pointer to the user data registered with the callback
 * @param switchNode the switch calling the function
 * @param action the action traversing the scene graph
 * @param number the number of the tested child in the multiswitch node
 * @param child a pointer to the child about to be traversed.
 * @return flag indicating whether the child should be traversed (TRUE) or 
 *         not (FALSE).
 */
typedef SbBool SoMultiSwitchCB( void * userdata, SoMultiSwitch * switchNode, 
                                SoAction * action, int number, SoNode * child );

/**
 * The behaviour of the MultiSwitch node is similar to the normal Switch node
 * in OpenInventor. However it allows to specify a subset of the children to 
 * be traversed. In addition to that it allows to register a callback that is 
 * called for each child that is about to be traversed. If such a callback is 
 * registered, it finally decides whether the child is really traversed.
 * The ordered field determines how the children are traversed. If it is TRUE they
 * are processed in the same order
 * as they appear in the scene graph (left-to-right) and only once, otherwise
 * they are traversed as they appear in the whichChildren field. This allows
 * some fancy tricks with reusing of children several times.
 *
 * This node is basically a reimplementation of the OpenInventor Switch node
 * with different functionality. It was done this way to avoid having an 
 * empty whichChild field lying around and to avoid some inheritance issues 
 * in the original code.
 *
 * A lot of different things are possible :
 * @li just specify a subset using whichChildren
 * @li a subset with additional testing for action type, child type, modes etc.
 *  using the callback
 * @li only doing the testing for all children by using -3 and a callback
 *
 * @note Finally a good idea would be to derive the currently implemented 
 * SoContextSwitch from this node as the former offers just additional prefiltering. 
 *
 * File format :
<pre>MultiSwitch {
  SFBool  ordered       TRUE
  MFInt32 whichChildren [-1]
}</pre>
 *
 * @author Gerhard Reitmayr
 * @ingroup util
 */
class STARLIGHT_API SoMultiSwitch : public SoGroup
{
    SO_NODE_HEADER(SoMultiSwitch);

public:

    /** this field stores the set of children to be traversed. Multiple values
     * are accepted and ignored. For values larger then the number of children 
     * the remainder of the division with the number of children will be used
     * (as in the original SoSwitch node :). If the special values -1, -2 or -3
     * appear, they override the set and show no children for -1 (SO_SWITCH_NONE),
     * use the state inherited from a former SoMultiSwitch for -2 (SO_SWITCH_INHERIT)
     * or show all children for -3 (SO_SWITCH_ALL). 
     */
    SoMFInt32 whichChildren;

    SoSFBool ordered;

    /** static method to initialize class for OpenInventor. It also
     * initializes SoMultiSwitchElement and enables it for all necessary
     * actions.  
     */
    static void initClass();

    /// constructor
    SoMultiSwitch();

    /// constructor that takes approximate number of children.
    SoMultiSwitch(int numChildren);

    /**
     * sets the test callback to be used for testing children to be traversed.
     * @param func pointer to the callback function
     * @param data void pointer that is passed to the user function */
    void setCallback( SoMultiSwitchCB * func, void * data = NULL );
    
    /**
     * removes a set callback again. Now children will be traversed again without 
     * testing. */
    void removeCallback();

    /** Overrides method in SoNode to return FALSE if there is no
     * selected child or the selected child does not affect the state. */
    virtual SbBool affectsState() const;

protected:
    virtual void GLRender(SoGLRenderAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void handleEvent(SoHandleEventAction *action);
    virtual void pick(SoPickAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void search(SoSearchAction *action);
    virtual void write(SoWriteAction *action);
    
    /** does the actual action work. It computes which children to traverse
     * and implements some special behaviour for bounding box actions. 
     * @param action the action to apply to the children
     */
    virtual void doAction(SoAction *action);    

    /** traverses a range of children and calls the callback function
     * for each of them, if it exists. If the callback function returns 
     * TRUE or does not exist, then the child is traversed, otherwise
     * not. If no end is given, then only the one child specified by
     * start is traversed.
     * @param action the action to apply to the children
     * @param start the start of the children range
     * @param end the end of the range ( is also traversed ). If it
     *        is -1 then it is set equal to start.
     */
    void doChildren( SoAction * action, int start, int end = -1 );

    /// destructor
    virtual ~SoMultiSwitch();

    /// stores the function pointer or NULL, if no callback function is set.
    SoMultiSwitchCB * cbFunc;

    /// stores the user data pointer or NULL, if no callback function is set.
    void * userData;

    /// stores a flag to signal that a search all SearchAction is underway.
    SbBool duringSearchAll;

};

#endif
