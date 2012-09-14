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
/** Implementation of key toggle node
  *
  * @author Jan Prikryl, Gerhard Reitmayr
  *
  * $Id: SoKeyToggle.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  include <SoWinEnterScope.h>
#endif

#include <string.h>

#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoKeyboardEvent.h>

#ifdef STB_IS_WINDOWS
#  include <SoWinLeaveScope.h>
#endif

#include <stb/components/starlight/SoKeyToggle.h>

SO_NODE_SOURCE(SoKeyToggle);

//----------------------------------------------------------------------------
// class initialization method for a derived inventor node
//----------------------------------------------------------------------------
void SoKeyToggle::initClass() 
{
	SO_NODE_INIT_CLASS(SoKeyToggle, SoNode, "Node");
}

//----------------------------------------------------------------------------
// constructor of class SoKeyToggle 
//----------------------------------------------------------------------------
SoKeyToggle::SoKeyToggle() 
{
	SO_NODE_CONSTRUCTOR(SoKeyToggle);
    SO_NODE_ADD_FIELD(key,   (""));
    SO_NODE_ADD_FIELD(toggled,   (0));
    SO_NODE_ADD_FIELD(pressed, (""));
    SO_NODE_ADD_FIELD(isPressed, (FALSE));
    SO_NODE_ADD_FIELD(alt, (FALSE));
    SO_NODE_ADD_FIELD(shift, (FALSE));
    SO_NODE_ADD_FIELD(ctrl, (FALSE));
}

//----------------------------------------------------------------------------
// destructor of class SoKeyToggle 
//----------------------------------------------------------------------------
SoKeyToggle::~SoKeyToggle() {}

//----------------------------------------------------------------------------
// handle input event
//----------------------------------------------------------------------------

void SoKeyToggle::handleEvent(SoHandleEventAction *action)

{
    const SoEvent * event = action->getEvent();

    if (event->isOfType(SoKeyboardEvent::getClassTypeId()))
    {
        char c[2];
        c[0] = ((SoKeyboardEvent *) event)->getPrintableCharacter();
        c[1] = '\0';
        if (SO_KEY_PRESS_EVENT(event,ANY))
        {
            if (c[0] != '\0')
            {
                const char * keyString = key.getValue().getString();
                if (c[0] == keyString[0])
                {
                    isPressed.setValue(TRUE);
                    if (toggled.getValue() == 0)
                        toggled.setValue(1);
                    else
                        toggled.setValue(0);
                }
                // add c[0] to pressed, if it is not present in pressed
                SbString pressedStr = pressed.getValue();
                if( strchr(pressedStr.getString(), c[0]) == NULL )
                {                    
                    pressedStr += c;
                    pressed.setValue(pressedStr);
                }
            }
        }
        else
        {
            if( c[0] != '\0' )
            {
                // remove c from pressed, if present
                SbString pressedStr = pressed.getValue();
                const char * pressedChr = pressedStr.getString();
                char * found = (char *) strchr(pressedChr, c[0]);
                if( found != NULL )
                {
                    int index =  (int) (found - pressedChr);
                    pressedStr.deleteSubString(index,index);
                    pressed.setValue(pressedStr);
                }
            }
            pressed.setValue("");
            isPressed.setValue(FALSE);
        }
        if( shift.getValue() != event->wasShiftDown())
            shift.setValue(event->wasShiftDown());
        if( alt.getValue() != event->wasAltDown())
            alt.setValue(event->wasAltDown());
        if( ctrl.getValue() != event->wasCtrlDown())
            ctrl.setValue(event->wasCtrlDown());
    }
}
