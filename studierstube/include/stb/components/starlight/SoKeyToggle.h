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
/** class definition SoKeyToggle
  *
  * This node makes it possible for your Inventor code to simply react on key
  * press events.
  *
  * @author  Jan Prikryl, Gerhard Reitmayr
  *
  * $Id: SoKeyToggle.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOKEYTOGGLE_H_
#define _SOKEYTOGGLE_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFBool.h>
#include <stb/base/OS.h>
#include "starlight.h"

#ifdef STB_IS_WINDOWS
#  include <SoWinLeaveScope.h>
#endif

/**
 * This node makes it possible for your Inventor code to simply react on key 
 * press events.
 * @verbatim
SoKeyToggle {
    key     ""
    toggled FALSE/TRUE

    pressed ""
    shift FALSE
    alt FALSE
    ctrl FALSE
}@endverbatim
 *
 * The value stored in 'key' has to be a string of lenght at least one. Empty
 * strings will be ignored, only the first character in the string has
 * any meaning. Values are case-sensitive, control keys (F1-F12, BSP and so
 * on) can't be used.
 *
 * pressed contains the characters for all currently pressed keys. It will
 * only contain keys with printable representations (obviously). shift, ctrl and
 * alt contain the state of the Shift, Control and Alt keys and are TRUE, if
 * these keys are pressed.
 *
 * There are some issues related to key repetition, which can lead to key releases.
 *
 * @author Jan Prikryl, Gerhard Reitmayr
 * @ingroup starlight
 */
class STARLIGHT_API SoKeyToggle : public SoNode {

    SO_NODE_HEADER(SoKeyToggle);

public:
    // class initialization
    static void initClass();
    // default constructon
    SoKeyToggle();
    virtual void handleEvent(SoHandleEventAction *action);

	/// defines the key to react on. Only the first character is used
    SoSFString key;
    /// indicates whether the selected key is currently pressed.
    SoSFBool isPressed;
	/// is set according to the key press events received
    SoSFBool   toggled;
    /// contains list of all currently pressed keys
    SoSFString pressed;
    /// TRUE if shift is pressed, FALSE otherwise
    SoSFBool shift;
    /// TRUE if alt is pressed, FALSE otherwise
    SoSFBool alt;
    /// TRUE if ctrl is pressed, FALSE otherwise
    SoSFBool ctrl;

private:
    virtual ~SoKeyToggle();
};

#ifdef STB_IS_WINDOWS
#  include <SoWinEnterScope.h>
#endif

#endif //_SOKEYTOGGLE_H_
