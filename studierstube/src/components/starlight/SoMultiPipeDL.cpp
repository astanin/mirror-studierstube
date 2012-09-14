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
/** The Source file for the SoMultiPipeDL.
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
#include <stb/components/starlight/SoMultiPipeDL.h>
#include <stb/components/starlight/SoPipeKit.h>
#include <iostream>

using namespace std;

SO_NODE_SOURCE(SoMultiPipeDL);

/*
* --------------------------------------------------------------------------------
* Required by the node kit
* --------------------------------------------------------------------------------
*/
void SoMultiPipeDL::initClass(void)
{
	SO_NODE_INIT_CLASS(SoMultiPipeDL, SoNode, "Node");
}

/*
* --------------------------------------------------------------------------------
* Constructor and Destructor
* --------------------------------------------------------------------------------
*/
SoMultiPipeDL::SoMultiPipeDL()
{
	SO_NODE_CONSTRUCTOR(SoMultiPipeDL);

	// This is for the Fields
	SO_NODE_ADD_FIELD(coords,	    (0,0,0));
    SO_NODE_ADD_FIELD(radius,	    (0));
    SO_NODE_ADD_FIELD(numFaces,	    (3));
    SO_NODE_ADD_FIELD(lineIndices,	(0));
    SO_NODE_ADD_FIELD(caps,	        (FALSE));
    SO_NODE_ADD_FIELD(flat,  	    (FALSE));

	// Create the Sensors
	coordsSensor=new SoFieldSensor(SoMultiPipeDL::refreshCB, this);
    radiusSensor=new SoFieldSensor(SoMultiPipeDL::refreshCB, this);
    capsSensor=new SoFieldSensor(SoMultiPipeDL::refreshCB, this);
    coordsSensor->attach(&this->coords);
    radiusSensor->attach(&this->radius);
    capsSensor->attach(&this->caps);

    coordsInternal=new SoCoordinate3;
    facesInternal=new SoIndexedFaceSet;

    numOfInternalCoords=0;
    numOfInternalFaces=0;
    initialized=false;
}

SoMultiPipeDL::~SoMultiPipeDL()
{
    glDeleteLists(internalDisplayList, 1);
    delete coordsSensor;
    delete radiusSensor;
    delete capsSensor;
}

/*
* --------------------------------------------------------------------------------
* Protected functions including callbacks, and those requested by the nodekit
* --------------------------------------------------------------------------------
*/

void SoMultiPipeDL::GLRender(SoGLRenderAction * action)
{
    if (!initialized)  // We do this here because we have to wait till the OpenGL Context is available
    {
        refresh();
    }

    glCallList(internalDisplayList);
    inherited::GLRender(action);
}

void SoMultiPipeDL::refreshCB(void *data, SoSensor * /*sensor*/)
{
	SoMultiPipeDL *tmp= (SoMultiPipeDL *)data;
	//tmp->refresh();
}

void SoMultiPipeDL::refresh()
{
    unsigned int k, startIndex, endIndex;
    k=0; startIndex=0;

    coordsInternal->point.deleteValues(0);
    facesInternal->coordIndex.deleteValues(0);
    numOfInternalCoords=0;
    numOfInternalFaces=0;

    // There must be at least three faces
    if (numFaces.getValue()<3) numFaces.setValue(3);

    // Loop all the indices of line strings
    for (k=0;k<lineIndices.getNum();k++)
    {
        // Loop through all the vectors on this line string
        endIndex=startIndex+lineIndices[k];

        // Skip if only has one element or less
        if (lineIndices[k]>=2)
        {
            createOneLineString(startIndex,endIndex);
        }

        startIndex=endIndex;
    }

    // create one display list
    internalDisplayList = glGenLists(1);

    // compile the display list, store a triangle in it
    glNewList(internalDisplayList, GL_COMPILE);
        glBegin(GL_QUADS);
            for (int i=0;i<numOfInternalFaces;i=i+5)
            {
                glVertex3fv(coordsInternal->point[facesInternal->coordIndex[i]].getValue());
                glVertex3fv(coordsInternal->point[facesInternal->coordIndex[i+1]].getValue());
                glVertex3fv(coordsInternal->point[facesInternal->coordIndex[i+2]].getValue());
                glVertex3fv(coordsInternal->point[facesInternal->coordIndex[i+3]].getValue());
            }
        glEnd();
    glEndList();


    initialized=true;
}

void SoMultiPipeDL::createOneLineString(int startIndex, int endIndex)
{
    for (int i=startIndex;i<endIndex-1;i++)
        createOneSegment(coords[i],coords[i+1]);
}


void SoMultiPipeDL::createOneSegment(SbVec3f start, SbVec3f end)
{
    unsigned int nNextIndex, nNumberOfVertices, i;
    float magnitude, angle, theta, angleSlope, x, y, z;
    SbVec3f tmpVec, diffVec, axisVec, nextVertexBottom, nextVertexTop;

    /// ------------------------- Prepare everything

    nNumberOfVertices=numFaces.getValue();

    // Get the magnitude of the difference vector
    diffVec=end-start;
    magnitude=diffVec.length();

    // Get a rotation vector and its angle
    tmpVec.setValue(0, 0, 1);
    axisVec=diffVec.cross(tmpVec);
    axisVec.normalize();
    theta=getAngle(diffVec, tmpVec);

    // Prepare the angle and its slope
    angle=3.141592654f/2.0f;
    angleSlope=(2.0f*3.141592654f)/nNumberOfVertices;

    /// ------------------------- Add the vertices

    SbVec3f firstTerm, secondTerm, thirdTerm;
    // Add all the vertices
    for (i=0;i<nNumberOfVertices;i++)
    {

        x=radius.getValue()*(sin(angle));
        y=radius.getValue()*(cos(angle));
        z=0;

        // Calculate bottom vertex (without rotation)
        nextVertexBottom.setValue(x,y,z);

        // Rotate bottom about angle/axis
            // xcos(theta)
            firstTerm=nextVertexBottom;
            firstTerm*=cos(theta);

            // n(n.x)(1-cos(theta))
            secondTerm=axisVec;
            secondTerm*=axisVec.dot(nextVertexBottom);
            secondTerm*=(1-cos(theta));

            // (xXn)sin(theta)
            tmpVec=nextVertexBottom;
            thirdTerm=tmpVec.cross(axisVec);
            thirdTerm*=sin(theta);

            nextVertexBottom=firstTerm+secondTerm+thirdTerm;

        // Translate bottom vertex to start position
        nextVertexBottom=nextVertexBottom+start;

        // Add bottom vertex
        if (flat.getValue()) nextVertexBottom[2]=0;
        coordsInternal->point.set1Value(numOfInternalCoords+i,nextVertexBottom);

        // Calculate top vertex (without rotation)
        z=magnitude;
        nextVertexTop.setValue(x,y,z);


        // Rotate top about angle/axis
            // xcos(theta)
            firstTerm=nextVertexTop;
            firstTerm*=cos(theta);

            // n(n.x)(1-cos(theta))
            secondTerm=axisVec;
            secondTerm*=axisVec.dot(nextVertexTop);
            secondTerm*=(1-cos(theta));

            // (xXn)sin(theta)
            tmpVec=nextVertexTop;
            thirdTerm=tmpVec.cross(axisVec);
            thirdTerm*=sin(theta);

            nextVertexTop=firstTerm+secondTerm+thirdTerm;

        // Translate top vertex to start position
        nextVertexTop=nextVertexTop+start;

        // Add top vertex
        if (flat.getValue()) nextVertexTop[2]=0;
        coordsInternal->point.set1Value(numOfInternalCoords+i+nNumberOfVertices,nextVertexTop);

        // Change the angle for the next vertex
        angle-=angleSlope;
    }

    // ------------------------- Add the faces

    int old=numOfInternalFaces;

    // Create Bottom Face
    if (caps.getValue())
    {
        // Loop the number of all the new bottom vertices
        // Then add them at the appropriate GLOBAL positions of faces
        for (i=0;i<nNumberOfVertices;i++)
            facesInternal->coordIndex.set1Value(numOfInternalFaces+i,numOfInternalCoords+i);

        // Finish this face
        facesInternal->coordIndex.set1Value(numOfInternalFaces+nNumberOfVertices,-1);

        // Update the number of internal faces
        numOfInternalFaces=numOfInternalFaces+nNumberOfVertices;

        // Loop the number of all the new bottom vertices
        // Then add them at the appropriate GLOBAL positions of faces
        for (i=0;i<nNumberOfVertices;i++)
            facesInternal->coordIndex.set1Value(numOfInternalFaces+i,numOfInternalCoords+nNumberOfVertices+i);

        // Finish this face
        facesInternal->coordIndex.set1Value(numOfInternalFaces+nNumberOfVertices,-1);

        // Update the number of internal faces
        numOfInternalFaces=numOfInternalFaces+nNumberOfVertices;
    }

    // Create the rest of the cylinder faces except for the last
    for (i=0;i<nNumberOfVertices-1;i++)
    {
        nNextIndex=numOfInternalCoords+i;
        facesInternal->coordIndex.set1Value(numOfInternalFaces,		nNextIndex);
        facesInternal->coordIndex.set1Value(numOfInternalFaces+1,	nNextIndex+nNumberOfVertices);
        facesInternal->coordIndex.set1Value(numOfInternalFaces+2,	nNextIndex+nNumberOfVertices+1);
        facesInternal->coordIndex.set1Value(numOfInternalFaces+3,	nNextIndex+1);
        facesInternal->coordIndex.set1Value(numOfInternalFaces+4,	-1);

        // Update the number of internal faces
        numOfInternalFaces=numOfInternalFaces+5;
    }

    // Create the last of the cylinder faces
    facesInternal->coordIndex.set1Value(numOfInternalFaces,		numOfInternalCoords+(nNumberOfVertices-1));
    facesInternal->coordIndex.set1Value(numOfInternalFaces+1,	numOfInternalCoords+(nNumberOfVertices*2)-1);
    facesInternal->coordIndex.set1Value(numOfInternalFaces+2,	numOfInternalCoords+nNumberOfVertices);
    facesInternal->coordIndex.set1Value(numOfInternalFaces+3,	numOfInternalCoords);
    facesInternal->coordIndex.set1Value(numOfInternalFaces+4,	-1);

    // Update the number of internal faces
    numOfInternalFaces=numOfInternalFaces+5;

    // Update the number of internal coordinates
    numOfInternalCoords=numOfInternalCoords+(nNumberOfVertices*2);
}
