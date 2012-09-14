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
/** The Source file for the SoMultiExtrusionKit.
*
* @author Erick Mendez
*
* $Id: SoPipeKit.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */



/*
* --------------------------------------------------------------------------------
* Includes
* --------------------------------------------------------------------------------
*/
#include <stb/components/starlight/SoMultiExtrusionKit.h>
#include <stb/components/starlight/SoPipeKit.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <iostream>

using namespace std;

SO_KIT_SOURCE(SoMultiExtrusionKit);

/*
* --------------------------------------------------------------------------------
* Required by the node kit
* --------------------------------------------------------------------------------
*/
void SoMultiExtrusionKit::initClass(void)
{
	SO_KIT_INIT_CLASS(SoMultiExtrusionKit, SoBaseKit, "BaseKit");
}

/*
* --------------------------------------------------------------------------------
* Constructor and Destructor
* --------------------------------------------------------------------------------
*/
SoMultiExtrusionKit::SoMultiExtrusionKit()
{
	SO_KIT_CONSTRUCTOR(SoMultiExtrusionKit);


	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,		    SoSeparator,		FALSE,	this,	        \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(coordsInternal,		SoCoordinate3,		FALSE,	topSeparator,	\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(facesInternal,			SoIndexedFaceSet,	FALSE,	topSeparator,	\x0, TRUE);

// Graph structure
// ---------------
// ¦ topSeparator ¦
// ---------------
//       ¦--------------------¦-------------------|
// ---------------     ---------------    ---------------
// ¦ shapeHints   ¦    ¦ coordsIntern ¦   ¦ facesInterna ¦
// ---------------     ---------------    ---------------


	// This is for the Fields
	SO_KIT_ADD_FIELD(vertices,	        (0,0));
    SO_KIT_ADD_FIELD(extrusionVectors,	(0,0,0));
    SO_KIT_ADD_FIELD(indices,	        (0));
    SO_KIT_ADD_FIELD(doubleSided,	    (FALSE));
    SO_KIT_ADD_FIELD(caps,	            (TRUE));

	SO_KIT_INIT_INSTANCE();

	// Create the Sensors
	verticesSensor=new SoFieldSensor(SoMultiExtrusionKit::refreshCB, this);
    extrusionVectorsSensor=new SoFieldSensor(SoMultiExtrusionKit::refreshCB, this);
    indicesSensor=new SoFieldSensor(SoMultiExtrusionKit::refreshCB, this);
    doubleSidedSensor=new SoFieldSensor(SoMultiExtrusionKit::refreshCB, this);
    capsSensor=new SoFieldSensor(SoMultiExtrusionKit::refreshCB, this);

    numOfInternalCoords=0;
    numOfInternalFaces=0;

	this->setUpConnections(TRUE, TRUE);
}

SoMultiExtrusionKit::~SoMultiExtrusionKit()
{
	delete verticesSensor;
    delete extrusionVectorsSensor;
    delete indicesSensor;
    delete doubleSidedSensor;
    delete capsSensor;
}

/*
* --------------------------------------------------------------------------------
* Protected functions including callbacks, and those requested by the nodekit
* --------------------------------------------------------------------------------
*/
SbBool SoMultiExtrusionKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		inherited::setUpConnections(onoff, doitalways);

		verticesSensor->attach(&this->vertices);
        extrusionVectorsSensor->attach(&this->extrusionVectors);
        indicesSensor->attach(&this->indices);
        doubleSidedSensor->attach(&this->doubleSided);
        capsSensor->attach(&this->caps);

		refresh();
	}
	else 
	{
		// We disconnect BEFORE base class.
		verticesSensor->detach();
        extrusionVectorsSensor->detach();
        indicesSensor->detach();
        doubleSidedSensor->detach();
        capsSensor->detach();

		inherited::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoMultiExtrusionKit::refreshCB(void *data, SoSensor * /*sensor*/)
{
	SoMultiExtrusionKit *tmp= (SoMultiExtrusionKit *)data;
	tmp->refresh();
}

void SoMultiExtrusionKit::refresh()
{
    SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coordsInternal", TRUE));
    SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("facesInternal", TRUE));

    unsigned int k, startIndex, endIndex;
    k=0; startIndex=0;

    coords->point.deleteValues(0);
    faces->coordIndex.deleteValues(0);
    numOfInternalCoords=0;
    numOfInternalFaces=0;

    // There must be at least three vertices
    if (vertices.getNum()<3) return;

    // Loop all the indices of the extrusions
    for (k=0;k<indices.getNum();k++)
    {
        // Loop through all the vertices on this extrusion
        endIndex=startIndex+indices[k];

        // Skip if only has 2 element or less
        if (indices[k]>=3)
        {
            createOneExtrusion(startIndex,endIndex, extrusionVectors[k]);
        }

        startIndex=endIndex;
    }

}

void SoMultiExtrusionKit::createOneExtrusion(int startIndex, int endIndex, SbVec3f extrusion)
{
    SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coordsInternal", TRUE));
    SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("facesInternal", TRUE));

    unsigned int nNumberOfVertices, i, k;
    SbVec3f tmpVec;


    SoMFVec2f cleanVertices;

    // Clean repeated vertices
    // Copy the first vertex
    k=0;
    cleanVertices.set1Value(k,vertices[startIndex]);
    // Then all other vertices except the last
    for (i=startIndex+1;i<endIndex-1;i++)
    {
        if (vertices[i]!=vertices[i-1])
        {
            k++;
            cleanVertices.set1Value(k,vertices[i]);
        }
    }
    // And now the last only if is not equal to the first
    if (vertices[endIndex]!=vertices[startIndex])
    {
        k++;
        cleanVertices.set1Value(k,vertices[endIndex]);
    }

    // The Number of Vertices to extrude
    nNumberOfVertices=k;

    // Attach footprint coordinates
    for (i=0;i<nNumberOfVertices;i++)
    {
        tmpVec.setValue(cleanVertices[i][0],cleanVertices[i][1],0);
        coords->point.set1Value(numOfInternalCoords+i,tmpVec);
    }

    // Attach extruded footprint coordinates
    for (i=0;i<nNumberOfVertices;i++)
    {
        tmpVec.setValue(cleanVertices[i][0],cleanVertices[i][1],0);
        coords->point.set1Value(numOfInternalCoords+i+nNumberOfVertices,tmpVec+extrusion.getValue());
    }

    if (caps.getValue())
    {
        // Create footprint face
        for (i=0;i<nNumberOfVertices;i++)
            faces->coordIndex.set1Value(numOfInternalFaces+i,numOfInternalCoords+i);
        faces->coordIndex.set1Value(numOfInternalFaces+nNumberOfVertices,-1);
        numOfInternalFaces=numOfInternalFaces+nNumberOfVertices+1;

        // Create footprint extruded face
        for (i=0;i<nNumberOfVertices;i++)
            faces->coordIndex.set1Value(numOfInternalFaces+i,numOfInternalCoords+((nNumberOfVertices*2)-i-1));
        faces->coordIndex.set1Value(numOfInternalFaces+nNumberOfVertices,-1);
        numOfInternalFaces=numOfInternalFaces+nNumberOfVertices+1;
    }

    // Create the rest of the cylinder faces except for the last
    for (i=0;i<nNumberOfVertices-1;i++)
    {
        faces->coordIndex.set1Value(numOfInternalFaces,	    numOfInternalCoords+i);
        faces->coordIndex.set1Value(numOfInternalFaces+1,	numOfInternalCoords+i+nNumberOfVertices);
        faces->coordIndex.set1Value(numOfInternalFaces+2,	numOfInternalCoords+i+nNumberOfVertices+1);
        faces->coordIndex.set1Value(numOfInternalFaces+3,	numOfInternalCoords+i+1);
        faces->coordIndex.set1Value(numOfInternalFaces+4,	-1);

        // Update the number of internal faces
        numOfInternalFaces=numOfInternalFaces+5;
    }

    // Create the last of the cylinder faces
    i=nNumberOfVertices-1;
    faces->coordIndex.set1Value(numOfInternalFaces,		numOfInternalCoords+nNumberOfVertices-1);
    faces->coordIndex.set1Value(numOfInternalFaces+1,	numOfInternalCoords+nNumberOfVertices*2-1);
    faces->coordIndex.set1Value(numOfInternalFaces+2,	numOfInternalCoords+nNumberOfVertices);
    faces->coordIndex.set1Value(numOfInternalFaces+3,	numOfInternalCoords);
    faces->coordIndex.set1Value(numOfInternalFaces+4,	-1);

    // Update the number of internal faces
    numOfInternalFaces=numOfInternalFaces+5;

    // Update the number of internal coordinates
    numOfInternalCoords=numOfInternalCoords+(nNumberOfVertices*2);
}
