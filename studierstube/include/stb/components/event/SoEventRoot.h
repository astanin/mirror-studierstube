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
/** Header file for SoEventRoot node
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoEventRoot.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOEVENTROOT_H__
#define __SOEVENTROOT_H__

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include "SoInputEvent.h"

class SoEventAction;
template <typename T> class EventAdapter;

class EVENT_API SoEventRoot : public SoBaseKit
{
    SO_KIT_HEADER( SoEventRoot );

    SO_KIT_CATALOG_ENTRY_HEADER(context);
    SO_KIT_CATALOG_ENTRY_HEADER(attributes);
    SO_KIT_CATALOG_ENTRY_HEADER(scene);

public:
    SoMFName key;
    SoMFString value;

    SoEventRoot(void);
    static void initClass(void);

protected:
    virtual ~SoEventRoot();

    static void dataChanged( void * data, SoSensor * sensor );
    SoFieldSensor keySensor;
    SoFieldSensor valueSensor;

    static void sceneChanged( void * data, SoSensor * sensor );
    SoNodeSensor sceneSensor;

    SbBool setUpConnections(SbBool onOff, SbBool doItAlways);
    
    void processEvent( SoInputEvent * event );
    void applyAction();

    SoEventAction * localAction;
    EventAdapter<SoEventRoot> * adapter;
    SoInputEvent lastEvent;

    friend class EventAdapter<SoEventRoot>;
};
#endif
