/* ======================================================================== 
 * Copyright (C) 2008  Graz University of Technology
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
/** Header file for SoTrackedArtifact engine
  *
  * @author Gerhard Reitmayr, Istvan Barakonyi, Markus Sareika
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOCOLORBLOBTRAKENGINE_H_
#define _SOCOLORBLOBTRAKENGINE_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFTime.h>

#include <stb/components/event/SoInputEvent.h>

//#include <stb/components/event/dllinclude.h>

template <typename T> class EventAdapter;

class EVENT_API SoColorBlobTrakEngine : public SoEngine
{
    SO_ENGINE_HEADER(SoColorBlobTrakEngine);

public:
    SoMFName key;
    SoMFString value;

	SoSFBool		buttonHisteresis;  // Flag to allow buttons to depend on their immediate history
	SoSFBool		locked;
	
	// Engine Input
    SoSFVec3f       translationIn;     // input translation
    SoSFRotation    rotationIn;        // input rotation
    SoSFVec3f       scaleFactorIn;     // input scale
    SoSFShort       objectIdIn;        // input objectID
    SoSFFloat       confidenceIn;      // input confidence
    SoSFTime        timestampIn;       // input timestamp?
    
    SoSFBool        buttonIn0;         // input button 0
    SoSFBool        buttonIn1;         // input button 1
    SoSFBool        buttonIn2;         // input button 2
    SoSFBool        buttonIn3;         // input button 3
    SoSFBool        buttonIn4;         // input button 4
    SoSFBool        buttonIn5;         // input button 5
    SoSFBool        buttonIn6;         // input button 6
    SoSFBool        buttonIn7;         // input button 7
    
    SoSFBool        buttonInWrapper;   // input button wrapper
	
	// Engine Output
    SoEngineOutput  translation;       // (SoSFVec3f) tracker translation
    SoEngineOutput  rotation;          // (SoSFRotation) tracker rotation
    SoEngineOutput  scaleFactor;             // (SoSFVec3f) tracker scale
    SoEngineOutput  objectId;          // (SoSFShort) objectID
    SoEngineOutput  confidence;        // (SoSFFloat) confidence value //mf
    SoEngineOutput  button0;           // (SoSFBool) tracker button 0 value
    SoEngineOutput  button1;           // (SoSFBool) tracker button 1 value
    SoEngineOutput  button2;           // (SoSFBool) tracker button 2 value
    SoEngineOutput  button3;           // (SoSFBool) tracker button 3 value
    SoEngineOutput  button4;           // (SoSFBool) tracker button 4 value
    SoEngineOutput  button5;           // (SoSFBool) tracker button 5 value
    SoEngineOutput  button6;           // (SoSFBool) tracker button 6 value
    SoEngineOutput  button7;           // (SoSFBool) tracker button 7 value
    SoEngineOutput  buttonWrapper;     // (SoSFShort) wrapper with all the trackers in a mask
    SoEngineOutput  timestamp;         // event timestamp from opentracker

    SoColorBlobTrakEngine(void);
    static void initClass(void);

protected:
    virtual ~SoColorBlobTrakEngine();
    virtual void evaluate();        // engine's evaluate method
    virtual void inputChanged(SoField* whichField); // react to input changes field that has changed
    
    void processEvent(SoInputEvent *event);

    EventAdapter<SoColorBlobTrakEngine> *adapter;
    friend class EventAdapter<SoColorBlobTrakEngine>;

private:
	// FIXME: Should probably go as static inside evaluate(). Mendez 20060523

	// Used for histeresis
	SbBool        buttonHistory0;      // history of button 0
	SbBool        buttonHistory1;      // history of button 1
	SbBool        buttonHistory2;      // history of button 2
	SbBool        buttonHistory3;      // history of button 3
	SbBool        buttonHistory4;      // history of button 4
	SbBool        buttonHistory5;      // history of button 5
	SbBool        buttonHistory6;      // history of button 6
    SbBool        buttonHistory7;      // history of button 7

	// Used for detecting button changes 
	SbBool        buttonChange0;       // Change of button 0
	SbBool        buttonChange1;       // Change of button 1
	SbBool        buttonChange2;       // Change of button 2
	SbBool        buttonChange3;       // Change of button 3
	SbBool        buttonChange4;       // Change of button 4
	SbBool        buttonChange5;       // Change of button 5
	SbBool        buttonChange6;       // Change of button 6
    SbBool        buttonChange7;       // Change of button 7

    int lastObjectId;

};


#endif

