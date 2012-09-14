/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
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
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The header file for the SoWindowHandling
*
* @author Erick Mendez
* @ingroup viewer
*
* $Id: SoFileEnv.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOWINDOWHANDLING_H_
#define _SOWINDOWHANDLING_H_


/*
* --------------------------------------------------------------------------------
* Includes
* --------------------------------------------------------------------------------
*/
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include "Viewer.h"

/** 
*
*
* @author Erick Mendez
* @ingroup viewer
* 
*/

class VIEWER_API SoWindowHandling: public SoBaseKit
{
    SO_KIT_HEADER(SoWindowHandling);
public:
    /// The constructor of the class
    SoWindowHandling();

    /// Destructor
    ~SoWindowHandling();

    /// Initializes the node kit
    static void initClass();

    SoSFTrigger exit;
    SoSFTrigger minimize;
    SoSFTrigger maximize;
    SoSFTrigger freezevideo;
    SoSFTrigger freezetracking;
    SoSFBool enableFreeze;
    SoMFName wildcardKey;
    SoMFString wildcardValue;

protected:

    /// Sensors
    SoFieldSensor *exitSensor;
    SoFieldSensor *minimizeSensor;
    SoFieldSensor *maximizeSensor;
    SoFieldSensor *freezevideoSensor;
    SoFieldSensor *freezetrackingSensor;
    SoFieldSensor *wildcardKeySensor;
    SoFieldSensor *wildcardValueSensor;


    /// Calls the function that rebuilds the object
    static void exitCB(void* data, SoSensor* sensor);
    static void minimizeCB(void* data, SoSensor* sensor);
    static void maximizeCB(void* data, SoSensor* sensor);
    static void freezevideoCB(void* data, SoSensor* sensor);
    static void freezetrackingCB(void* data, SoSensor* sensor);
    static void wildcardKeyCB(void* data, SoSensor* sensor);
    static void wildcardValueCB(void* data, SoSensor* sensor);

    void refreshPredicates();

    /// Attaches and detaches the sensors and does a couple of one time operations
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

};


#endif //_SOWINDOWHANDLING_H_
