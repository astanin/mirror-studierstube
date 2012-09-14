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
/** The Source file for the SoPrismKit.
*
* @author Erick Mendez
*
* $Id: SoPrismKit.cpp 2006-03-10 mendez $
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
#include <stb/components/starlight/SoPrismKit.h>
#include <iostream>

using namespace std;

SO_KIT_SOURCE(SoPrismKit);

/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoPrismKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoPrismKit, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoPrismKit::SoPrismKit()
{
	SO_KIT_CONSTRUCTOR(SoPrismKit);

	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,	SoSeparator,		FALSE,	this,			\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(coords,		SoCoordinate3,		FALSE,	topSeparator,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(faces,			SoIndexedFaceSet,	FALSE,	topSeparator,	\x0, TRUE);

    // This is for the Fields
    SO_KIT_ADD_FIELD(numFaces,  (3));
    SO_KIT_ADD_FIELD(radius,    (1.0f));
    SO_KIT_ADD_FIELD(height,    (2.0f));
    SO_KIT_ADD_FIELD(caps,      (FALSE));

    SO_KIT_INIT_INSTANCE();

	// Create the Sensors
    numFacesSensor=new SoFieldSensor(SoPrismKit::refreshCB, this);
    heightSensor=new SoFieldSensor(SoPrismKit::refreshCB, this);
    radiusSensor=new SoFieldSensor(SoPrismKit::refreshCB, this);
    capsSensor=new SoFieldSensor(SoPrismKit::refreshCB, this);

	this->setUpConnections(TRUE, TRUE);
}

SoPrismKit::~SoPrismKit()
{
    delete numFacesSensor;
    delete heightSensor;
    delete radiusSensor;
    delete capsSensor;
}

/*
 * --------------------------------------------------------------------------------
 * Protected functions including callbacks, and those requested by the nodekit
 * --------------------------------------------------------------------------------
 */
SbBool SoPrismKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

		// Attach the field sensors, always make sure to change first the 
		// extrusion vector
        numFacesSensor->attach(&this->numFaces);
        heightSensor->attach(&this->height);
        radiusSensor->attach(&this->radius);
        capsSensor->attach(&this->caps);

		refresh();
    }
    else 
    {
        // We disconnect BEFORE base class.
        numFacesSensor->detach();
        heightSensor->detach();
        radiusSensor->detach();
        capsSensor->detach();

        SoBaseKit::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}

void SoPrismKit::refreshCB(void *data, SoSensor * /*sensor*/)
{
    SoPrismKit *tmp= (SoPrismKit *)data;
	tmp->refresh();
}


void SoPrismKit::refresh()
{
    SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coords", TRUE));
    SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("faces", TRUE));
    unsigned int i, k, nNumberOfVertices, nNumberOfCoordinates, nNextLimit, nNextIndex;
    k=0;

	// There must be at least three
	if (numFaces.getValue()<3)
        numFaces.setValue(3);

    nNumberOfVertices=numFaces.getValue();

    /// Generate vertices
    float angleSlope, angle, halfHeight;
    float x, y;
    SbVec3f nextPoint;
    angleSlope=(2*3.141592654)/nNumberOfVertices;
    angle=0;
    halfHeight=height.getValue()/2.0;
    for (i=0;i<nNumberOfVertices;i++)
    {
        x=radius.getValue()*(sin(angle));
        y=radius.getValue()*(cos(angle));
        nextPoint.setValue(x,-halfHeight,y);
        coords->point.set1Value(i,nextPoint);
        nextPoint.setValue(x,halfHeight,y);
        coords->point.set1Value(i+nNumberOfVertices,nextPoint);
        angle-=angleSlope;
    }

    nNextIndex=-1;
    nNextLimit=-1;
    
    // Create Bottom Face
    if (caps.getValue())
    {
        nNextIndex=0;
        nNextLimit=nNumberOfVertices;
        for (i=nNextIndex;i<nNextLimit;i++)
            faces->coordIndex.set1Value(i,i);
        faces->coordIndex.set1Value(nNextLimit,-1);
    
        // Create Top Face
        nNextIndex=nNextLimit+1;
        nNextLimit=nNextIndex+nNumberOfVertices;
        for (i=0;i<nNumberOfVertices;i++)
            faces->coordIndex.set1Value(i+nNextIndex,nNextLimit-i-2);
        faces->coordIndex.set1Value(nNextLimit,-1);
    }

    // This is the total of coordinates in 'coords'
    nNumberOfCoordinates=nNumberOfVertices*2;

	// Create the rest of the cylinder faces except for the last
	nNextIndex=nNextLimit+1;
	for (i=0;i<nNumberOfVertices-1;i++)
	{
		nNextLimit=(i*5+nNextIndex);
		faces->coordIndex.set1Value(nNextLimit,	    i);
		faces->coordIndex.set1Value(nNextLimit+1,	i+nNumberOfVertices);
		faces->coordIndex.set1Value(nNextLimit+2,	i+nNumberOfVertices+1);
		faces->coordIndex.set1Value(nNextLimit+3,   i+1);
		faces->coordIndex.set1Value(nNextLimit+4,	-1);
	}

	// Create the last of the cylinder faces
	i=nNumberOfVertices-1;
	nNextLimit=(i*5+nNextIndex);
	faces->coordIndex.set1Value(nNextLimit,	    nNumberOfVertices-1);
	faces->coordIndex.set1Value(nNextLimit+1,	nNumberOfVertices*2-1);
	faces->coordIndex.set1Value(nNextLimit+2,	nNumberOfVertices);
	faces->coordIndex.set1Value(nNextLimit+3,	0);
	faces->coordIndex.set1Value(nNextLimit+4,	-1);

}

