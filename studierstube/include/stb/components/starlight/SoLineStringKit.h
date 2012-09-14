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
/** The header file for the SoLineStringKit.
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoLineStringKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOLINESTRINGKIT_H_
#define _SOLINESTRINGKIT_H_



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include "starlight.h"

class STARLIGHT_API SoLineStringKit: public SoBaseKit
{
    SO_KIT_HEADER(SoLineStringKit);
	typedef SoBaseKit inherited;

public:
	/// Initializes the node kit
    static void initClass();

	/// The constructor of the class, initializes the catalog
    SoLineStringKit();

	/// Destructor, deletes the sensors
    ~SoLineStringKit();

	/// The input vertices of the line string
	SoMFVec2f vertices;

	/// Width of the line
	SoSFFloat width;

protected:

	/// Sensors
	SoFieldSensor *verticesSensor;
	SoFieldSensor *widthSensor;

	/// Parts of the catalog
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(coords);
	SO_KIT_CATALOG_ENTRY_HEADER(faces);
	SO_KIT_CATALOG_ENTRY_HEADER(shapeHints);

	/// Attaches and detaches the sensors and does a couple of one time operations
	virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Calls the function that rebuilds the object
    static void verticesCB(void* data, SoSensor* sensor);

	/// Calls the function that rebuilds the object
    static void widthCB(void* data, SoSensor* sensor);

	/// Rebuilds the Extrusion Object
	void refresh();

};



#endif

