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
/** The Source file for the SoStyleTemplate.
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
#include <stb/components/starlight/SoStyleTemplate.h>
#include <stb/components/starlight/starlight.h>
#include <stb/components/starlight/SoNodeContext.h>
#include <stb/components/starlight/SoContext.h>
#include <Inventor/nodes/SoGroup.h>

using namespace std;

//#pragma warning(disable:4996)

SO_KIT_SOURCE(SoStyleTemplate);

/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoStyleTemplate::initClass(void)
{
    SO_KIT_INIT_CLASS(SoStyleTemplate, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoStyleTemplate::SoStyleTemplate()
{
	SO_KIT_CONSTRUCTOR(SoStyleTemplate);

	// Create the nodekit
	SO_KIT_ADD_CATALOG_ENTRY(sepTop,			SoGroup,		FALSE,	this,	\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(cxnRenderStyles,	SoNodeContext,  TRUE,	sepTop,	\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(cxStyles,	        SoContext,	    TRUE,	sepTop,	\x0, TRUE);
	SO_KIT_ADD_FIELD(name,              (""));
    SO_KIT_ADD_FIELD(stylenames,	    (0));
    SO_KIT_ADD_FIELD(styles,	        (0));
    SO_KIT_INIT_INSTANCE();


	// Create the sensors
    nameSensor=new SoFieldSensor(SoStyleTemplate::stylesCB, this);
    stylenamesSensor=new SoFieldSensor(SoStyleTemplate::stylesCB, this);
    stylesSensor=new SoFieldSensor(SoStyleTemplate::stylesCB, this);

    nNumberOfStyles=-1;

	this->setUpConnections(true, true);
}

SoStyleTemplate::~SoStyleTemplate()
{
    delete nameSensor;
    delete stylenamesSensor;
    delete stylesSensor;
}

SbBool SoStyleTemplate::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onoff, doitalways);

        nameSensor->attach(&this->name);
        stylenamesSensor->attach(&this->stylenames);
        stylesSensor->attach(&this->styles);

		refresh();
	}
	else 
	{
		// We disconnect BEFORE base class.
        nameSensor->detach();
        stylenamesSensor->detach();
        stylesSensor->detach();

		SoBaseKit::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoStyleTemplate::stylesCB(void *data, SoSensor *sensor)
{
	SoStyleTemplate *self= (SoStyleTemplate *)data;
	self->refresh();
}

void SoStyleTemplate::refresh()
{
    if (name.getValue().getLength()==0) return;

    int i;
    char *nameString=new char[255];
    char *stylenameString=new char[255];
    char *tmpString=new char[255];

    strcpy(nameString, (char *) name.getValue().getString());

    for (i=0;i<stylenames.getNum();i++)
    {
        strcpy(stylenameString, (char *) stylenames[i].getString());

        sprintf(tmpString, "__Template_%s_%s", nameString, stylenameString);
        setStyle(tmpString, styles[i]);

        // This has to be generalized later: (this is a static list of the available style names)
        sprintf(tmpString, "__Style_%d", i);
        SO_GET_PART(this, "cxStyles", SoContext)->index.set1Value(i, tmpString);
        SO_GET_PART(this, "cxStyles", SoContext)->value.set1Value(i, stylenameString);
        sprintf(tmpString, "__Map_%s", stylenameString);
        SO_GET_PART(this, "cxStyles", SoContext)->index.set1Value(i+stylenames.getNum(), tmpString);
        SO_GET_PART(this, "cxStyles", SoContext)->value.set1Value(i+stylenames.getNum(), "");
    }
}

void SoStyleTemplate::setStyle(char *strFamily, SoNode *style)
{
	int nIndex=SO_GET_PART(this, "cxnRenderStyles", SoNodeContext)->index.find(strFamily);

	if (nIndex>=0)
		SO_GET_PART(this, "cxnRenderStyles", SoNodeContext)->value.set1Value(nIndex, style);
	else 
	{
		nNumberOfStyles++;
		SO_GET_PART(this, "cxnRenderStyles", SoNodeContext)->index.set1Value(nNumberOfStyles, strFamily);
		SO_GET_PART(this, "cxnRenderStyles", SoNodeContext)->value.set1Value(nNumberOfStyles, style);
	}
}
