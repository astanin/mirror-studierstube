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
/** The Source file for the SoStyleMap.
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
#include <stb/components/starlight/SoStyleMap.h>
#include <stb/components/starlight/starlight.h>
#include <stb/components/starlight/SoContext.h>
#include <Inventor/nodes/SoGroup.h>

using namespace std;

#pragma warning(disable:4996)

SO_KIT_SOURCE(SoStyleMap);

SoStyleMap *SoStyleMap::pinstance=0;

SoStyleMap *SoStyleMap::getInstance()
{
    if (pinstance==0)
    {
        pinstance=new SoStyleMap;
    }
    return pinstance;
}


/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoStyleMap::initClass(void)
{
    SO_KIT_INIT_CLASS(SoStyleMap, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoStyleMap::SoStyleMap()
{
	SO_KIT_CONSTRUCTOR(SoStyleMap);

	// Create the nodekit
	SO_KIT_ADD_CATALOG_ENTRY(sepTop,	SoGroup,		FALSE,	this,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(cxActives,	SoContext,      TRUE,	sepTop,	\x0, TRUE);
    SO_KIT_ADD_FIELD(templatename,          (""));
    SO_KIT_ADD_FIELD(stylename,             (""));
    SO_KIT_ADD_FIELD(keys,                  (0));
    SO_KIT_ADD_FIELD(values,                (0));
    SO_KIT_ADD_FIELD(keysAndValuesString,   (""));
    SO_KIT_ADD_FIELD(enable,                (TRUE));
    SO_KIT_INIT_INSTANCE();


	// Create the sensors
    templatenameSensor=new SoFieldSensor(SoStyleMap::refreshCB, this);
    stylenameSensor=new SoFieldSensor(SoStyleMap::refreshCB, this);
    keysSensor=new SoFieldSensor(SoStyleMap::refreshCB, this);
    valuesSensor=new SoFieldSensor(SoStyleMap::refreshCB, this);
    keysAndValuesStringSensor=new SoFieldSensor(SoStyleMap::refreshCB, this);
    enableSensor=new SoFieldSensor(SoStyleMap::refreshCB, this);
    keysAndValuesString.setValue("");

	this->setUpConnections(true, true);
}

SoStyleMap::~SoStyleMap()
{
    delete templatenameSensor;
    delete stylenameSensor;
    delete keysSensor;
    delete valuesSensor;
    delete keysAndValuesStringSensor;
    delete enableSensor;
}

SbBool SoStyleMap::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onoff, doitalways);

        templatenameSensor->attach(&this->templatename);
        stylenameSensor->attach(&this->stylename);
        keysSensor->attach(&this->keys);
        valuesSensor->attach(&this->values);
        keysAndValuesStringSensor->attach(&this->keysAndValuesString);
        enableSensor->attach(&this->enable);

		refresh();
	}
	else 
	{
		// We disconnect BEFORE base class.
        templatenameSensor->detach();
        stylenameSensor->detach();
        keysSensor->detach();
        valuesSensor->detach();
        keysAndValuesStringSensor->detach();
        enableSensor->detach();

		SoBaseKit::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoStyleMap::refreshCB(void *data, SoSensor *sensor)
{
	SoStyleMap *self= (SoStyleMap *)data;
	self->refresh();
}

void SoStyleMap::refresh()
{

    int i;
    char *tmpString=new char[255];
    char *keyvalueString=new char[255];

    // If no criteria name given the set it to __void
    if (templatename.getValue().getLength()==0)
    {
        SO_GET_PART(this, "cxActives", SoContext)->index.set1Value(0, "__TemplateName");
        SO_GET_PART(this, "cxActives", SoContext)->value.set1Value(0, "__void");
    }
    else
    {
        // Set the used criteria name
        SO_GET_PART(this, "cxActives", SoContext)->index.set1Value(0, "__TemplateName");
        SO_GET_PART(this, "cxActives", SoContext)->value.set1Value(0, templatename.getValue().getString());

        if (keysAndValuesString.getValue().getLength()>0)
        {
            sprintf(tmpString, "__Map_%s", stylename.getValue().getString());
            SO_GET_PART(this, "cxActives", SoContext)->index.set1Value(1, tmpString);
            SO_GET_PART(this, "cxActives", SoContext)->value.set1Value(1, keysAndValuesString.getValue().getString());
        }
        else
        {
            // Set the bindings
            for (i=0;i<keys.getNum();i++)
            {
                if (i!=0)
                    sprintf(keyvalueString, "%s;%s=%s", keyvalueString, keys[i].getString(), values[i].getString());
                else
                    sprintf(keyvalueString, "%s=%s", keys[i].getString(), values[i].getString());
            }

            sprintf(tmpString, "__Map_%s", stylename.getValue().getString());
            SO_GET_PART(this, "cxActives", SoContext)->index.set1Value(1, tmpString);
            SO_GET_PART(this, "cxActives", SoContext)->value.set1Value(1, keyvalueString);
        }
    }
}
