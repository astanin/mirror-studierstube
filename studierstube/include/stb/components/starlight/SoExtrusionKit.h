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
/** The header file for the SoExtrusionKit.
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoExtrusionKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOEXTRUSIONKIT_H_
#define _SOEXTRUSIONKIT_H_



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "starlight.h"

/** 
* Header for an Open Inventor Class that implements an extrusion 
* Kit given a set of points and a direction vector.
*
* Notes: The set of coordinates must be a closed loop, this is, the last
*	     element must be the same as the first
*
* @author Erick Mendez
* @ingroup starlight
*/

class STARLIGHT_API SoExtrusionKit: public SoBaseKit
{
    SO_KIT_HEADER(SoExtrusionKit);
	typedef SoBaseKit inherited;

public:
	 /// Initializes the node kit
    static void initClass();

	/// The constructor of the class, initializes the catalog
    SoExtrusionKit();

	 /// Destructor, deletes the sensors
    ~SoExtrusionKit();

	/// The input vertices that will be extruded
	SoMFVec2f vertices;

	/// The extrusion vector, giving the direction and magnitude of extrusion
	SoSFVec3f extrusionVector;

    /// Flag to let the kit know that it has to create the top and bottom faces, default value is TRUE
    SoSFBool caps;

protected:

	/// Sensors
    SoFieldSensor *capsSensor;
    SoFieldSensor *verticesSensor;
	SoFieldSensor *extrusionVectorSensor;

	/// Parts of the catalog
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(coords);
	SO_KIT_CATALOG_ENTRY_HEADER(faces);

	/// Attaches and detaches the sensors and does a couple of one time operations
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Calls the function that rebuilds the object
    static void refreshCB(void* data, SoSensor* sensor);

	/// Rebuilds the Extrusion Object
	void refresh();

};


#endif //_SOEXTRUSIONKIT_H_
