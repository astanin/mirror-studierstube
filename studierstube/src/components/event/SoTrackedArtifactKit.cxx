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
/** SoTrackedArtifactKit.h contains the implementation of class SoTrackedArtifactKit
  *
  * @author Rainer Splechtna, Istvan Barakonyi
  *
  * $Id: SoTrackedArtifactKit.cpp 47 2005-07-18 17:54:04Z bara $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include <stb/components/event/SoTrakEngine.h>
#include <stb/components/event/SoTrackedArtifactKit.h>

SO_KIT_SOURCE(SoTrackedArtifactKit);

/// Inventor class initialization
void SoTrackedArtifactKit::initClass()
{
    SO_KIT_INIT_CLASS(SoTrackedArtifactKit,SoBaseKit,"BaseKit");
}

/// The constructor of the kit inits the fields and connects the tracker
SoTrackedArtifactKit::SoTrackedArtifactKit()
{
    SO_KIT_CONSTRUCTOR(SoTrackedArtifactKit);

    SO_KIT_ADD_CATALOG_ENTRY(artifactRoot,SoSeparator,FALSE,this,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(mover,SoTransform,FALSE,artifactRoot,\0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(offset,SoTransform,TRUE,artifactRoot,\0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(onOffSwitch,SoSwitch,FALSE,artifactRoot,\0,FALSE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(geometry,SoNode,SoSeparator,FALSE,onOffSwitch,\0,TRUE);

    SO_KIT_ADD_FIELD(timeOut,(0.0));
    SO_KIT_ADD_FIELD(isVisible,(TRUE));
    SO_KIT_ADD_FIELD(key,(""));
    SO_KIT_ADD_FIELD(value,(""));

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    moveSensor.setFunction(moved);
    moveSensor.setData(this);
    moveSensor.setPriority(1);   

    timer.setFunction(timerFired);
    timer.setData(this);
    
    tracker=NULL;

    visibleSensor.setFunction(visibleChanged);
    visibleSensor.setData(this);
    visibleSensor.setPriority(1);
    visibleSensor.attach(&isVisible);

    setUpConnections(TRUE,TRUE);

    showGeometry(TRUE);
    setUpConnections(TRUE,TRUE);
}


/// The destructor of the kit does nothing
SoTrackedArtifactKit::~SoTrackedArtifactKit()
{
    // tracker is an engine and will be deleted if all outputs are disconnected !!
    // if (tracker!= NULL) delete tracker;
}

SbBool SoTrackedArtifactKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);

        // do your own special setup here !
        tracker = new SoTrakEngine;
        tracker->key.connectFrom(&key);
        tracker->value.connectFrom(&value);
        SoTransform *mover=getMover();   
        mover->translation.connectFrom(&tracker->translation);
        mover->rotation.connectFrom(&tracker->rotation);
        moveSensor.attach(&getMover()->translation);        
        
        // if a timeout is set, the timer sensor should be scheduled here already.
        // otherwise the geometry will not become invisible, if no movement happens.
        /*
        if( timeOut.getValue().getValue() > 0.0 )
        {
            if( timer.isScheduled())
                timer.unschedule();
            timer.setTimeFromNow( timeOut.getValue());
            timer.schedule();
        }*/
    }
    else {
        // We disconnect BEFORE base class.
        // Disconnect the field sensors.
        moveSensor.detach();

        SoTransform *mover = getMover();   
        mover->translation.disconnect();
        mover->rotation.disconnect();

        SoBaseKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
}

/// Returns the SoTransform node which moves the object
SoTransform* SoTrackedArtifactKit::getMover()
{
    return SO_GET_ANY_PART(this,"mover",SoTransform);
}

/// Returns the offset of this object
SoTransform* SoTrackedArtifactKit::getOffset()
{
    return SO_GET_PART(this,"offset",SoTransform);
}

/// Returns the root of the geometry-graph of this object
SoNode* SoTrackedArtifactKit::getGeometry()
{
    return SO_GET_PART(this,"geometry",SoNode);
}

/// Set the geometry-graph of this object
void 
SoTrackedArtifactKit::setGeometry(SoNode *newGeometry)
{
    setPart("geometry", newGeometry);
}

/// Use this method to switch the object's geometry on or off
void SoTrackedArtifactKit::showGeometry(SbBool onOff)
{
    SO_GET_ANY_PART(this,"onOffSwitch",SoSwitch)->whichChild = onOff ? -3 : -1;
    visibleSensor.detach();
    isVisible.setValue( onOff );
    visibleSensor.attach( &isVisible );
}

/// Returns the state of the geometry switch
SbBool SoTrackedArtifactKit::geometryIsVisible()
{
    int switchStatus=SO_GET_ANY_PART(this,"onOffSwitch",SoSwitch)->whichChild.getValue();
    if (switchStatus==-3) return TRUE;
    else return FALSE;
}

void SoTrackedArtifactKit::movedCB() 
{
    // to be used in subclasses
}

void SoTrackedArtifactKit::moved( void *data, SoSensor * /*sensor*/ )
{
    // reset timeOut timer
    SoTrackedArtifactKit *self=(SoTrackedArtifactKit*)data;
    if(self->timeOut.getValue().getValue()>0.0){
        if( self->timer.isScheduled()) self->timer.unschedule();
        self->timer.setTimeFromNow(self->timeOut.getValue());
        self->timer.schedule();
    }
    if(FALSE==self->geometryIsVisible()){
        self->showGeometry( TRUE );
    }
    self->movedCB();
}
    
void SoTrackedArtifactKit::timerFired( void *data, SoSensor * /*sensor*/ )
{
    SoTrackedArtifactKit *self=(SoTrackedArtifactKit*)data;
    self->showGeometry(FALSE);
}

void SoTrackedArtifactKit::visibleChanged( void * data, SoSensor * /*sensor*/ )
{
    SoTrackedArtifactKit *self=(SoTrackedArtifactKit*)data;
    self->showGeometry(self->isVisible.getValue());
}
