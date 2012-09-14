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
/** The header file for the SoMultiPipeDL
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoPipeKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOMULTIPIPEDL_H_
#define _SOMULTIPIPEDL_H_


/*
* --------------------------------------------------------------------------------
* Includes
* --------------------------------------------------------------------------------
*/
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "starlight.h"

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
/** 
*
* @author Erick Mendez
* @ingroup starlight
*/

class STARLIGHT_API SoMultiPipeDL: public SoNode
{
	SO_NODE_HEADER(SoMultiPipeDL);
	typedef SoNode inherited;

public:
	/// Initializes the node kit
	static void initClass();

	/// The constructor of the class, initializes the catalog
	SoMultiPipeDL();

	/// Destructor, deletes the sensors
	~SoMultiPipeDL();

    /// Number of faces to be displayed (minimum 3)
    SoSFInt32 numFaces;

	/// The input vertices that will be extruded
	SoMFVec3f coords;

    /// Number of faces to be displayed (minimum 3)
    SoMFInt32 lineIndices;

	/// Radius of the pipe
	SoSFFloat radius;

    /// Set if starting and ending spheres are desired
    SoSFBool caps;

    /// Useful for shadow generation
    SoSFBool flat;

private:
    int numOfInternalCoords;
    int numOfInternalFaces;
    bool initialized;

protected:
	/// Sensors
	SoFieldSensor *coordsSensor;
    SoFieldSensor *radiusSensor;
    SoFieldSensor *capsSensor;

    SoCoordinate3 *coordsInternal;
    SoIndexedFaceSet *facesInternal;
    GLuint internalDisplayList;

	/// Calls the function that rebuilds the object
	static void refreshCB(void* data, SoSensor* sensor);

	/// Creates a Cylinder of a radius set by the class between the two given points
	SoSeparator *createCylinder(SbVec3f start, SbVec3f end);

	/// Rebuilds the Object
	void refresh();

    void createOneLineString(int startIndex, int endIndex);
    void createOneSegment(SbVec3f start, SbVec3f end);

    void GLRender(SoGLRenderAction * action);
};


#endif //_SOMULTIPIPEDL_H_
