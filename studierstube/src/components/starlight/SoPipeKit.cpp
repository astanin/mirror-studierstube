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
/** The Source file for the SoPipeKit.
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
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <stb/components/starlight/SoPipeKit.h>
#include <stb/components/starlight/SoPrismKit.h>
#include <iostream>

using namespace std;

SO_KIT_SOURCE(SoPipeKit);

/*
* --------------------------------------------------------------------------------
* Required by the node kit
* --------------------------------------------------------------------------------
*/
void SoPipeKit::initClass(void)
{
	SO_KIT_INIT_CLASS(SoPipeKit, SoBaseKit, "BaseKit");
}

/*
* --------------------------------------------------------------------------------
* Constructor and Destructor
* --------------------------------------------------------------------------------
*/
SoPipeKit::SoPipeKit()
{
	SO_KIT_CONSTRUCTOR(SoPipeKit);

	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(pipes,		SoSeparator,		FALSE,	this,	\x0, TRUE);

	// This is for the Fields
	SO_KIT_ADD_FIELD(coords,	(0,0,0));
    SO_KIT_ADD_FIELD(radius,	(0));
    SO_KIT_ADD_FIELD(numFaces,	(3));
	SO_KIT_ADD_FIELD(caps,		(FALSE));

	SO_KIT_INIT_INSTANCE();

	// Create the Sensors
	coordsSensor=new SoFieldSensor(SoPipeKit::coordsCB, this);
	radiusSensor=new SoFieldSensor(SoPipeKit::radiusCB, this);

	this->setUpConnections(TRUE, TRUE);
}

SoPipeKit::~SoPipeKit()
{
	delete coordsSensor;
	delete radiusSensor;
}

/*
* --------------------------------------------------------------------------------
* Protected functions including callbacks, and those requested by the nodekit
* --------------------------------------------------------------------------------
*/
SbBool SoPipeKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		inherited::setUpConnections(onoff, doitalways);

		coordsSensor->attach(&this->coords);
		radiusSensor->attach(&this->radius);

		refresh();
	}
	else 
	{
		// We disconnect BEFORE base class.
		coordsSensor->detach();
		radiusSensor->detach();

		inherited::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoPipeKit::coordsCB(void *data, SoSensor * /*sensor*/)
{
	SoPipeKit *tmp= (SoPipeKit *)data;
	tmp->refresh();
}

void SoPipeKit::radiusCB(void *data, SoSensor * /*sensor*/)
{
	SoPipeKit *tmp= (SoPipeKit *)data;
	tmp->refresh();
}

float getAngle(SbVec3f alpha, SbVec3f beta)
{
	return (acos(alpha.dot(beta)/(alpha.length()*beta.length())));
}

SoSeparator *SoPipeKit::createCylinder(SbVec3f start, SbVec3f end)
{
	SbVec3f tmpVec, diffVec, rotVec;
	tmpVec.setValue(0, 1, 0);
	diffVec=end-start;
	float magnitude=diffVec.length();
	rotVec=diffVec.cross(tmpVec);
	float angle=getAngle(diffVec, tmpVec);

	SoPrismKit *myCylinder=new SoPrismKit;
    myCylinder->numFaces=numFaces;
	myCylinder->radius=radius.getValue();
	myCylinder->height.setValue(magnitude);

	SoTransform *myTranslation1=new SoTransform;
	myTranslation1->translation.setValue(SbVec3f(0,(magnitude/2),0));

	SoTransform *myTranslation2=new SoTransform;
	myTranslation2->translation.setValue(start.getValue());

	SoTransform *myRotation=new SoTransform;
	myRotation->rotation.setValue(rotVec,-angle);

	SoSeparator *mySeparator=new SoSeparator;
	mySeparator->addChild(myTranslation2);
	mySeparator->addChild(myRotation);
	mySeparator->addChild(myTranslation1);
	mySeparator->addChild(myCylinder);

	return mySeparator;
}

SoSeparator *SoPipeKit::createSphere(SbVec3f position)
{
	SoSphere *mySphere=new SoSphere;
	mySphere->radius.setValue(radius.getValue());

	SoTransform *myTransform=new SoTransform;
	myTransform->translation.setValue(position.getValue());

	SoSeparator *mySeparator=new SoSeparator;
	mySeparator->addChild(myTransform);
	mySeparator->addChild(mySphere);

	return mySeparator;
}

void SoPipeKit::refresh()
{
	// There must be at least two
	if (coords.getNum()<2) return;

	SoMFVec3f cleanCoords;
	unsigned int i, k, nNumberOfCoordinates;
	k=0;

	// Clean repeated coordinates
	cleanCoords.set1Value(k,coords[k]);
	for (i=1;i<(unsigned int)coords.getNum();i++)
	{
		if (coords[i]!=coords[i-1])
		{
			k++;
			cleanCoords.set1Value(k,coords[i]);
		}
	}

	// Afterwards there should still be at least two
	nNumberOfCoordinates=cleanCoords.getNum();
	if (nNumberOfCoordinates<2)
		return;

	SoSeparator *pipes=(SoSeparator *)(this->getPart("pipes", TRUE));

	pipes->removeAllChildren();

	// Add an initial sphere if desired
	if (caps.getValue())
		pipes->addChild(createSphere(cleanCoords[0]));

	// Add an initial section
	pipes->addChild(createCylinder(cleanCoords[0],cleanCoords[1]));
	// Add the rest
	for (i=1;i<nNumberOfCoordinates-1;i++)
	{
        if (caps.getValue())
		    pipes->addChild(createSphere(cleanCoords[i]));
		pipes->addChild(createCylinder(cleanCoords[i],cleanCoords[i+1]));
	}

	// Add an ending sphere if desired
	if (caps.getValue())
		pipes->addChild(createSphere(cleanCoords[nNumberOfCoordinates-1]));

}


