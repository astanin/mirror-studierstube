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
/** The header file for the SoStyledSubgraph
*
* @author Erick Mendez
* @ingroup starlight
*
* $Id: SoStyledSubgraph.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */


#ifndef _SOSTYLEDSUBGRAPH_H_
#define _SOSTYLEDSUBGRAPH_H_


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
#include <Inventor/fields/SoSFString.h>
#include <stb/components/starlight/SoNodeContextReport.h>
#include <stb/components/starlight/starlight.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoFieldSensor.h>

/**
 * This class is the kit the user will be using for defining a the scene of a magic lens
 *
 * @author Erick Mendez
 * @ingroup starlight
 */

class SoStyledSubgraph: public SoBaseKit
{
	/// Parts of the catalog
	SO_KIT_HEADER(SoStyledSubgraph);
    SO_KIT_CATALOG_ENTRY_HEADER(sepTop);
    SO_KIT_CATALOG_ENTRY_HEADER(switchReport);
	SO_KIT_CATALOG_ENTRY_HEADER(renderStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(content); 

public:

	/// The constructor of the class, initializes the catalog
	SoStyledSubgraph();

	/// Destructor, deletes the sensors
	~SoStyledSubgraph();

	/// Initializes the node kit
	static void initClass();

	/// Sets what style should we use for rendering
	void refreshStyle(bool fromConnections, SoAction *action);
    bool isMappedTo(char *stylename, SoAction *action);

protected:
	/// Sets the Cg parameters and renders down its tree
	virtual void GLRender(SoGLRenderAction * action);

};


#endif //_SOSTYLEDSUBGRAPH_H_
