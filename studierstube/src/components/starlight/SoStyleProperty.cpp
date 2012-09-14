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
/** The Source file for the SoStyleProperty.
*
* @author Erick Mendez
*
* $Id: SoCSMLFamilies.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <iostream>
#include <stb/components/starlight/SoStyleProperty.h>
#include <stb/components/starlight/starlight.h>
#include <stb/components/starlight/SoContext.h>
#include <Inventor/nodes/SoGroup.h>

using namespace std;

#pragma warning(disable:4996)

SO_KIT_SOURCE(SoStyleProperty);

SoStyleProperty *SoStyleProperty::pinstance=0;

SoStyleProperty *SoStyleProperty::getInstance()
{
    if (pinstance==0)
    {
        pinstance=new SoStyleProperty;
    }
    return pinstance;
}


/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoStyleProperty::initClass(void)
{
    SO_KIT_INIT_CLASS(SoStyleProperty, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoStyleProperty::SoStyleProperty()
{
	SO_KIT_CONSTRUCTOR(SoStyleProperty);

	// Create the nodekit
	SO_KIT_ADD_CATALOG_ENTRY(sepTop,	SoGroup,		FALSE,	this,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(cxActives,	SoContext,      TRUE,	sepTop,	\x0, TRUE);
    SO_KIT_ADD_FIELD(keys,                  (0));
    SO_KIT_ADD_FIELD(values,                (0));
    SO_KIT_INIT_INSTANCE();


	// Create the sensors
    keysSensor=new SoFieldSensor(SoStyleProperty::refreshCB, this);
    valuesSensor=new SoFieldSensor(SoStyleProperty::refreshCB, this);

	this->setUpConnections(true, true);
}

SoStyleProperty::~SoStyleProperty()
{
    delete keysSensor;
    delete valuesSensor;
}

SbBool SoStyleProperty::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onoff, doitalways);

        keysSensor->attach(&this->keys);
        valuesSensor->attach(&this->values);

		refresh();
	}
	else 
	{
		// We disconnect BEFORE base class.
        keysSensor->detach();
        valuesSensor->detach();

		SoBaseKit::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoStyleProperty::refreshCB(void *data, SoSensor *sensor)
{
	SoStyleProperty *self= (SoStyleProperty *)data;
	self->refresh();
}

void SoStyleProperty::refresh()
{

    int i;
    char *tmpString=new char[255];

    // Set the bindings
    for (i=0;i<keys.getNum();i++)
    {
        sprintf(tmpString, "__State_%s", keys[i].getString());
        SO_GET_PART(this, "cxActives", SoContext)->index.set1Value(i, tmpString);
        SO_GET_PART(this, "cxActives", SoContext)->value.set1Value(i, values[i].getString());
    }
}
