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
/** SoTrackedArtifactKit.h contains the definition of class SoTrackedArtifactKit
  *
  * @author Rainer Splechtna, Istvan Barakonyi
  *
  * $Id: SoTrackedArtifactKit.h 47 2005-07-18 17:54:04Z bara $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOTRACKEDARTIFACTKIT_H_
#define _SOTRACKEDARTIFACTKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>


/** SoTrackedArtifactKit is a nodekit representing a tracked object.

This class can be used as base-class for any tracked object. 
The object will be automatically moved by (connected to) a SoTrakEngine with
the key-value filters specified in the kit's field.
The class offers some simple access-methods for setting/getting the geometry of
the object and getting an offset. The geometry of the object can be
turned on and off by a calling the showGeometry() method. 

@author Rainer Splechtna, Istvan Barakonyi
@ingroup tracking
*/

class SoTrakEngine;

class EVENT_API SoTrackedArtifactKit : public SoBaseKit
{
    SO_KIT_HEADER(SoTrackedArtifactKit);
    SO_KIT_CATALOG_ENTRY_HEADER(artifactRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(mover);
    SO_KIT_CATALOG_ENTRY_HEADER(offset);
    SO_KIT_CATALOG_ENTRY_HEADER(onOffSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);

public:

    /// These fields define the (key,value) event filters, they are connected to the trak engine's key & value fields
    SoMFName key;
    SoMFString value;

    /// The constructor of the kit inits the fields and connects the tracker to the object
    SoTrackedArtifactKit();

    /// The destructor of the kit does nothing
    ~SoTrackedArtifactKit();

    /// Inventor class initialization
    static void initClass();

    /** Returns the offset of this object
        @return the SoTransform-node representing the offset of the object
      */
    SoTransform* getOffset();

    /** Returns the root of the geometry graph of this object
        @return the SoNode representing the object's geometry root
      */
    SoNode*      getGeometry();
    
    /** Set the geometry-graph of this object
        @param newGeometry the object's new geometry 
      */
    void         setGeometry(SoNode *newGeometry);
    
    /** Use this method to switch the object's geometry on or off.
        @param onOff TRUE: the geometry is switched on, 
                     FALSE: the geometry is switched off
      */
    void         showGeometry(SbBool onOff);

    /** Returns the state of the geometry switch
        @return TRUE: if the geometry is switched on
                FALSE: if the geometry is switched off
    */
    SbBool       geometryIsVisible();

    /** Returns the SoTransform node which moves the object (i.e. which is
        updated by a SoTrakEngine)
        @return the SoTransform-node, which moves the object
      */
    SoTransform* getMover();

    /** Defines a timeout to set the geometry invisible, if it didn't move. 
      * 0 is no timeout. */
	SoSFTime timeOut;

    /// Denotes if the geometry is currently visible
    SoSFBool isVisible;

protected:

    /// Internal tracker engine object to get tracking events
    SoTrakEngine *tracker;

    /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Sensor to check on movements to reset timer for timeout
	SoFieldSensor moveSensor;

    /// Sensor to trigger a time out
	SoAlarmSensor timer;

    /// Sensor to check on isVisible
    SoFieldSensor visibleSensor;

    /** Sensor callback method for movement sensor. it rescedules
     * the timeout sensor and sets the geometry to visible if 
     * necessary.
     */
	static void moved( void *data, SoSensor *sensor );

	/// Callback from within moved, that can be used to do additional processing if the TrackedAritfact has been moved
	virtual void movedCB();

	/// Sensor callback method for timeout. it sets the geometry to invisible
	static void timerFired( void *data, SoSensor *sensor );

    /// Sensor callback method for showing/hiding the geometry
    static void visibleChanged( void * data, SoSensor * sensor );
};

#endif //_SOTRACKEDARTIFACTKIT_H_

