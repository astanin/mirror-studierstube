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
/** The Source file for the SoLineStringKit.
*
* @author Erick Mendez
*
* $Id: SoLineStringKit.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <stb/components/starlight/SoLineStringKit.h>
#include <iostream>

using namespace std;

SO_KIT_SOURCE(SoLineStringKit);

/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoLineStringKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoLineStringKit, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoLineStringKit::SoLineStringKit()
{
	SO_KIT_CONSTRUCTOR(SoLineStringKit);

	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,	SoSeparator,		FALSE,	this,			\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(shapeHints,	SoShapeHints,		FALSE,	topSeparator,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(coords,		SoCoordinate3,		FALSE,	topSeparator,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(faces,			SoIndexedFaceSet,	FALSE,	topSeparator,	\x0, TRUE);

	// This is for the Fields
	SO_KIT_ADD_FIELD(vertices,		(0,0));
	SO_KIT_ADD_FIELD(width,			(0));

    SO_KIT_INIT_INSTANCE();

	// Create the Sensors
	verticesSensor=new SoFieldSensor(SoLineStringKit::verticesCB, this);
	widthSensor=new SoFieldSensor(SoLineStringKit::widthCB, this);

	// This has to be done only once, we know all the vertices are in
	// counterclockwise order and that they could be concave
	SoShapeHints *shapeHints=(SoShapeHints *)(this->getPart("shapeHints", TRUE));
	shapeHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	shapeHints->faceType=SoShapeHints::UNKNOWN_FACE_TYPE;

	this->setUpConnections(TRUE, TRUE);
}

SoLineStringKit::~SoLineStringKit()
{
	delete verticesSensor;
	delete widthSensor;
}

/*
 * --------------------------------------------------------------------------------
 * Protected functions including callbacks, and those requested by the nodekit
 * --------------------------------------------------------------------------------
 */
SbBool SoLineStringKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        inherited::setUpConnections(onoff, doitalways);

		// Attach the field sensors, always make sure to change first the 
		// extrusion vector
		verticesSensor->attach(&this->vertices);
		widthSensor->attach(&this->width);

		refresh();
    }
    else 
    {
        // We disconnect BEFORE base class.
		verticesSensor->detach();
		widthSensor->detach();

        inherited::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}

void SoLineStringKit::verticesCB(void *data, SoSensor *)
{
    SoLineStringKit *exkTmp= (SoLineStringKit *)data;
	exkTmp->refresh();
}

void SoLineStringKit::widthCB(void *data, SoSensor * /*sensor*/)
{
    SoLineStringKit *exkTmp= (SoLineStringKit *)data;
	exkTmp->refresh();
}

void SoLineStringKit::refresh()
{
	unsigned int i, nNumberOfVertices;

	nNumberOfVertices=vertices.getNum();
	if (nNumberOfVertices<2) return;

	SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coords", TRUE));
	SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("faces", TRUE));

	// These will be used as normals to the current line
	SbVec3f normalPrevious, normalNext, tmpVertex;

	// Remember, the points are the center line, we use the normal to make the "width"
	// of the line to follow the normal to itself

	// The first point
	normalNext[0]=-(vertices[1][1]-vertices[0][1]);
	normalNext[1]=vertices[1][0]-vertices[0][0];
	normalNext[2]=0;
	normalNext.normalize();
	normalNext*=width.getValue();
	tmpVertex[0]=vertices[0][0];
	tmpVertex[1]=vertices[0][1];
	tmpVertex[2]=0;
	coords->point.set1Value(0,tmpVertex+normalNext);
	coords->point.set1Value((nNumberOfVertices*2)-1,tmpVertex-normalNext);

	// The second to the second to last
	for (i=1;i<nNumberOfVertices-1;i++)
	{
		normalPrevious[0]=-(vertices[i][1]-vertices[i-1][1]);
		normalPrevious[1]=vertices[i][0]-vertices[i-1][0];
		normalPrevious[2]=0;

		normalNext[0]=-(vertices[i+1][1]-vertices[i][1]);
		normalNext[1]=vertices[i+1][0]-vertices[i][0];
		normalNext[2]=0;

		normalPrevious+=normalNext;
		normalPrevious.normalize();
		normalPrevious*=width.getValue();

		tmpVertex[0]=vertices[i][0];
		tmpVertex[1]=vertices[i][1];
		coords->point.set1Value(i,tmpVertex+normalPrevious);
		coords->point.set1Value((nNumberOfVertices*2)-i-1,tmpVertex-normalPrevious);
	}

	// The last point
	normalPrevious[0]=-(vertices[nNumberOfVertices-1][1]-vertices[nNumberOfVertices-2][1]);
	normalPrevious[1]=vertices[nNumberOfVertices-1][0]-vertices[nNumberOfVertices-2][0];
	normalPrevious[2]=0;
	normalPrevious.normalize();
	normalPrevious*=width.getValue();
	tmpVertex[0]=vertices[nNumberOfVertices-1][0];
	tmpVertex[1]=vertices[nNumberOfVertices-1][1];
	coords->point.set1Value(nNumberOfVertices-1,tmpVertex+normalPrevious);
	coords->point.set1Value(nNumberOfVertices,tmpVertex-normalPrevious);

	// Create footprint face
	for (i=0;i<nNumberOfVertices*2;i++)
		faces->coordIndex.set1Value(i,i);
	faces->coordIndex.set1Value(nNumberOfVertices*2,-1);
}


