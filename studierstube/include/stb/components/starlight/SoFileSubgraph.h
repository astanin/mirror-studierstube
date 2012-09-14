/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* SoFileSubgraph.h contains the definition of class SoFileSubgraph
 *
 * @author Istvan Barakonyi
 * 
 * $Id: SoFileSubgraph.h 8 2004-07-14 12:24:04Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _SOFILESUBGRAPH_H_
#define _SOFILESUBGRAPH_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "starlight.h"

/** Nodekit to load in a scenegraph from a file that can be attached to a node in 
    another scenegraph and - unlike SoFile - be searched afterwards
    
    @author Istvan Barakonyi
    @ingroup util
 */
class STARLIGHT_API SoFileSubgraph: public SoBaseKit
{
    SO_KIT_HEADER(SoFileSubgraph);
    /// Separator node to attach the loaded scenegraph to
    SO_KIT_CATALOG_ENTRY_HEADER(root);

public:

    /// the name of the Inventor file including the absolute or relative file path
    SoSFString fileName;

    /// the constructor initializes the filename field
    SoFileSubgraph();

    /// detaches the field sensor
    ~SoFileSubgraph();

SoINTERNAL public:

    static void initClass();

protected:

    /// filename sensors
    SoFieldSensor *fileNameSensor;

    /// attribute change static callback function
    static void fileNameCB(void *data,SoSensor *sensor);

    /// attribute change callback function
    void fileNameChanged();

    /// sets up field connections
    SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// reads in the scenegraph from the given Inventor file
    void readFile(const char *fileName);
};

#endif //_SOFILESUBGRAPH_H_
