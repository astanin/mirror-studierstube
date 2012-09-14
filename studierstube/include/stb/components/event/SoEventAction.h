/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Header file for SoEventAction node
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: SoEventAction.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOEVENTACTION_H__
#define __SOEVENTACTION_H__

#include <Inventor/actions/SoCallbackAction.h>

#include "event.h"

class SoInputEvent;

class EVENT_API SoEventAction : public SoCallbackAction
{
    SO_ACTION_HEADER(SoEventAction);

public:
    SoEventAction(void);
    virtual ~SoEventAction(void);
    static void initClass(void);

    void setEvent(const SoInputEvent * ev)
    {
        event = ev;
    };

    const SoInputEvent * getEvent(void) const
    {
        return event;
    };

    void setHandled(void)
    {
        setTerminated(TRUE);
    };

    SbBool isHandled(void) const
    {
        return hasTerminated();
    };

    void setGrabberPath( SoPath * aPath) {};
    void setTailAsGrabber(void);
    void releaseGrabber(void);
    SoNode * getGrabber(void) const;
    SoPath * getGrabberPath(void) const;

protected:
    SoPath * grabbedPath;
    const SoInputEvent * event;

    // action method for event type nodes will be instantiated for all different event aware base types
    template <class T> static void nodeS( SoAction * action, SoNode * node );
};
#endif //__SOEVENTACTION_H__
