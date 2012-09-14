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
/** The Source file for the SoExtrusionKit.
*
* @author Erick Mendez
*
* $Id: SoExtrusionKit.cpp 2006-03-10 mendez $
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
#include <stb/components/starlight/SoExtrusionKit.h>
#include <iostream>

using namespace std;

SO_KIT_SOURCE(SoExtrusionKit);

/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoExtrusionKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoExtrusionKit, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoExtrusionKit::SoExtrusionKit()
{
	SO_KIT_CONSTRUCTOR(SoExtrusionKit);

	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,	SoSeparator,		FALSE,	this,			\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(coords,		SoCoordinate3,		FALSE,	topSeparator,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(faces,			SoIndexedFaceSet,	FALSE,	topSeparator,	\x0, TRUE);

	// This is for the Fields
	SO_KIT_ADD_FIELD(vertices,			(0,0));
    SO_KIT_ADD_FIELD(extrusionVector,	(0,0,0));
    SO_KIT_ADD_FIELD(caps,	            (TRUE));

    SO_KIT_INIT_INSTANCE();

	// Create the Sensors
    capsSensor=new SoFieldSensor(SoExtrusionKit::refreshCB, this);
    verticesSensor=new SoFieldSensor(SoExtrusionKit::refreshCB, this);
	extrusionVectorSensor=new SoFieldSensor(SoExtrusionKit::refreshCB, this);

	this->setUpConnections(TRUE, TRUE);
}

SoExtrusionKit::~SoExtrusionKit()
{
    delete capsSensor;
    delete verticesSensor;
	delete extrusionVectorSensor;
}

/*
 * --------------------------------------------------------------------------------
 * Protected functions including callbacks, and those requested by the nodekit
 * --------------------------------------------------------------------------------
 */
SbBool SoExtrusionKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        inherited::setUpConnections(onoff, doitalways);

		// Attach the field sensors, always make sure to change first the 
		// extrusion vector
        capsSensor->attach(&this->caps);
        verticesSensor->attach(&this->vertices);
		extrusionVectorSensor->attach(&this->extrusionVector);

		refresh();
    }
    else 
    {
        // We disconnect BEFORE base class.
        capsSensor->detach();
        verticesSensor->detach();
		extrusionVectorSensor->detach();

        inherited::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}

void SoExtrusionKit::refreshCB(void *data, SoSensor * /*sensor*/)
{
    SoExtrusionKit *exkTmp= (SoExtrusionKit *)data;
	exkTmp->refresh();
}

void SoExtrusionKit::refresh()
{

	// There must be at least three
	if (vertices.getNum()<3) return;

	SoMFVec2f cleanVertices;
	unsigned int i, k, nNumberOfVertices, nNumberOfCoordinates, nNextLimit, nNextIndex;
	k=0;

	// Clean repeated vertices
	// Copy the first vertex
	cleanVertices.set1Value(k,vertices[k]);
	// Then all other vertices except the last
	for (i=1;i<(unsigned int)vertices.getNum()-1;i++)
	{
		if (vertices[i]!=vertices[i-1])
		{
			k++;
			cleanVertices.set1Value(k,vertices[i]);
		}
	}
	// And now the last only if is not equal to the first
	if (vertices[vertices.getNum()-1]!=vertices[0])
	{
		k++;
		cleanVertices.set1Value(k,vertices[vertices.getNum()-1]);
	}

	// Afterwards there should still be at least three
	nNumberOfVertices=cleanVertices.getNum();
	if (nNumberOfVertices<3) return;

	SbVec3f tmpVec;
	SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coords", TRUE));
	SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("faces", TRUE));

	// Attach footprint coordinates
	for (i=0;i<nNumberOfVertices;i++)
	{
		tmpVec.setValue(cleanVertices[i][0],cleanVertices[i][1],0);
		coords->point.set1Value(i,tmpVec);
	}

	// Attach extruded footprint coordinates
	for (i=0;i<nNumberOfVertices;i++)
	{
		tmpVec.setValue(cleanVertices[i][0],cleanVertices[i][1],0);
		coords->point.set1Value(i+nNumberOfVertices,tmpVec+extrusionVector.getValue());
	}

	// This is the total of coordinates in 'coords'
	nNumberOfCoordinates=nNumberOfVertices*2;

    if (caps.getValue())
    {
	    // Create footprint face
	    nNextIndex=0;
	    nNextLimit=nNumberOfVertices;
	    for (i=nNextIndex;i<nNextLimit;i++)
		    faces->coordIndex.set1Value(i,i);
	    faces->coordIndex.set1Value(nNextLimit,-1);

	    // Create footprint extruded face
	    nNextIndex=nNextLimit+1;
	    nNextLimit=nNextIndex+nNumberOfVertices;
	    for (i=0;i<nNumberOfVertices;i++)
		    faces->coordIndex.set1Value(i+nNextIndex,nNextLimit-i-2);
	    faces->coordIndex.set1Value(nNextLimit,-1);

	    // Create the rest of the cylinder faces except for the last
        nNextIndex=nNextLimit+1;
    }
    else
    {
        nNextIndex=0;
        nNextLimit=nNumberOfVertices;
    }

	for (i=0;i<nNumberOfVertices-1;i++)
	{
		nNextLimit=(i*5+nNextIndex);
		faces->coordIndex.set1Value(nNextLimit,		i);
		faces->coordIndex.set1Value(nNextLimit+1,	i+nNumberOfVertices);
		faces->coordIndex.set1Value(nNextLimit+2,	i+nNumberOfVertices+1);
		faces->coordIndex.set1Value(nNextLimit+3,	i+1);
		faces->coordIndex.set1Value(nNextLimit+4,	-1);
	}

	// Create the last of the cylinder faces
	i=nNumberOfVertices-1;
	nNextLimit=(i*5+nNextIndex);
	faces->coordIndex.set1Value(nNextLimit,		nNumberOfVertices-1);
	faces->coordIndex.set1Value(nNextLimit+1,	nNumberOfVertices*2-1);
	faces->coordIndex.set1Value(nNextLimit+2,	nNumberOfVertices);
	faces->coordIndex.set1Value(nNextLimit+3,	0);
	faces->coordIndex.set1Value(nNextLimit+4,	-1);

}

