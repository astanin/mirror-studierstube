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
/** The header file for the SoStyleMap
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoCSMLFamilies.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOSTYLEMAP_H_
#define _SOSTYLEMAP_H_

/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#endif

#include <Inventor/actions/SoGLRenderAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stb/components/starlight/starlight.h>

class SoStyleMap: public SoBaseKit
{
    SO_KIT_HEADER(SoStyleMap);

public:

    /// Hey! it's a Singleton
    static SoStyleMap *getInstance();

    /// The constructor of the class, initializes the catalog
    SoStyleMap();

    /// Destructor, deletes the sensors
    ~SoStyleMap();

    SoSFString templatename;
    SoSFString stylename;
    SoMFString keys;
    SoMFString values;
    SoSFString keysAndValuesString;

    /// Flag
    SoSFBool enable;

	///  Initializes the node kit
    static void initClass();

	/// Refreshes the styles
	void refresh();

private:
    static SoStyleMap* pinstance;
protected:

	/// Sensors
    SoFieldSensor *templatenameSensor;
    SoFieldSensor *stylenameSensor;
    SoFieldSensor *keysSensor;
    SoFieldSensor *valuesSensor;
    SoFieldSensor *keysAndValuesStringSensor;
    SoFieldSensor *enableSensor;

	/// The parts of the catalog
	SO_KIT_CATALOG_ENTRY_HEADER(sepTop);
	SO_KIT_CATALOG_ENTRY_HEADER(cxActives);

	/// Attaches and detaches the sensors and does a couple of one time operations
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Calls the function that refreshes the styles
	static void refreshCB(void* data, SoSensor* sensor);

};


#endif //_SOSTYLEMAP_H_

