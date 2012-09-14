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
/** The header file for the SoPipeKit
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoPipeKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOPIPEKIT_H_
#define _SOPIPEKIT_H_


/*
* --------------------------------------------------------------------------------
* Includes
* --------------------------------------------------------------------------------
*/
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoSeparator.h>

#include "starlight.h"

/** 
*
* @author Erick Mendez
* @ingroup starlight
*/

float getAngle(SbVec3f alpha, SbVec3f beta);

class STARLIGHT_API SoPipeKit: public SoBaseKit
{
	SO_KIT_HEADER(SoPipeKit);
	typedef SoBaseKit inherited;

public:
	/// Initializes the node kit
	static void initClass();

	/// The constructor of the class, initializes the catalog
	SoPipeKit();

	/// Destructor, deletes the sensors
	~SoPipeKit();

    /// Number of faces to be displayed (minimum 3)
    SoSFInt32 numFaces;

	/// The input vertices that will be extruded
	SoMFVec3f coords;

	/// Radius of the pipe
	SoSFFloat radius;

	/// Set if starting and ending spheres are desired
	SoSFBool caps;

protected:
	/// Sensors
	SoFieldSensor *coordsSensor;
	SoFieldSensor *radiusSensor;

	/// Parts of the catalog
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(pipes);

	/// Attaches and detaches the sensors and does a couple of one time operations
	virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Calls the function that rebuilds the object
	static void coordsCB(void* data, SoSensor* sensor);

	/// Calls the function that rebuilds the object
	static void radiusCB(void* data, SoSensor* sensor);

	/// Creates a Cylinder of a radius set by the class between the two given points
	SoSeparator *createCylinder(SbVec3f start, SbVec3f end);

	/// Creates a Sphere of a radius set by the class at the given position
	SoSeparator *createSphere(SbVec3f position);

	/// Rebuilds the Object
	void refresh();

};


#endif
