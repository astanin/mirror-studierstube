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
/** The header file for the SoPrismKit.
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoPrismKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SODETAILCYLINDERKIT_H_
#define _SODETAILCYLINDERKIT_H_



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "starlight.h"

/** 
* Allows the creation of a cylinder given an specific number of faces
*
* @author Erick Mendez
* @ingroup starlight
*/

class STARLIGHT_API SoPrismKit: public SoBaseKit
{
    SO_KIT_HEADER(SoPrismKit);

public:
	 /// Initializes the node kit
    static void initClass();

	/// The constructor of the class, initializes the catalog
    SoPrismKit();

	 /// Destructor, deletes the sensors
    ~SoPrismKit();

    /// Number of faces to be displayed (minimum 3)
	SoSFInt32 numFaces;

    /// Height of the cylinder
    SoSFFloat height;

    /// Intended radius
    SoSFFloat radius;

    /// Parts to render, bottom, sides and top
    SoSFBool caps;

protected:

    /// Sensors
    SoFieldSensor *numFacesSensor;
    SoFieldSensor *heightSensor;
    SoFieldSensor *radiusSensor;
    SoFieldSensor *capsSensor;

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


#endif //_SODETAILCYLINDERKIT_H_
