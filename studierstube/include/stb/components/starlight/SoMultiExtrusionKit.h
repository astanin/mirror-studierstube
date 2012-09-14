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
/** The header file for the SoMultiExtrusionKit
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoPipeKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOMULTIEXTRUSIONKIT_H_
#define _SOMULTIEXTRUSIONKIT_H_


/*
* --------------------------------------------------------------------------------
* Includes
* --------------------------------------------------------------------------------
*/
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoSeparator.h>

#include "starlight.h"

/** 
*
* @author Erick Mendez
* @ingroup starlight
*/

class STARLIGHT_API SoMultiExtrusionKit: public SoBaseKit
{
	SO_KIT_HEADER(SoMultiExtrusionKit);
	typedef SoBaseKit inherited;

public:
	/// Initializes the node kit
	static void initClass();

	/// The constructor of the class, initializes the catalog
	SoMultiExtrusionKit();

	/// Destructor, deletes the sensors
	~SoMultiExtrusionKit();

    /// The input vertices that will be extruded
    SoMFVec2f vertices;

    /// The extrusion vector, giving the direction and magnitude of extrusion
    SoMFVec3f extrusionVectors;

    /// The extrusion vector, giving the direction and magnitude of extrusion
    SoMFInt32 indices;

    /// Set if starting and ending spheres are desired
    SoSFBool doubleSided;

    /// Set if starting and ending spheres are desired
    SoSFBool caps;

private:
    int numOfInternalCoords;
    int numOfInternalFaces;

protected:
	/// Sensors
	SoFieldSensor *verticesSensor;
    SoFieldSensor *extrusionVectorsSensor;
    SoFieldSensor *indicesSensor;
    SoFieldSensor *doubleSidedSensor;
    SoFieldSensor *capsSensor;

    /// Parts of the catalog
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(coordsInternal);
    SO_KIT_CATALOG_ENTRY_HEADER(facesInternal);

	/// Attaches and detaches the sensors and does a couple of one time operations
	virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Calls the function that rebuilds the object
	static void refreshCB(void* data, SoSensor* sensor);

	/// Creates a Cylinder of a radius set by the class between the two given points
	SoSeparator *createCylinder(SbVec3f start, SbVec3f end);

	/// Rebuilds the Object
	void refresh();

    void createOneExtrusion(int startIndex, int endIndex, SbVec3f extrusion);

};


#endif
