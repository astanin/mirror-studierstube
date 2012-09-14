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
/** 
 * Creates a string list from the DEF names of nodes
 * @author  Gerhard Reitmayr
 *
 * $Id: SoNodeToName.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SONODETONAME_H_
#define _SONODETONAME_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFBool.h>

#include "starlight.h"

/**
 * This engine converts a list of nodes into a list of strings containing the
 * names of these nodes ('DEF names'). It either operates in a direct mode
 * where NULL entries or nodes without names are represented as empty strings "",
 * or in compact mode where these nodes are left out of the resulting string. This
 * behaviour can be set with the @ref compact field. It can be useful together
 * with a @ref SoRaypicker and a @ref SoStringMap to compute things based on
 * the picked path.
 *
 * File format syntax / defaults:
@verbatim
SoNodeToName {
    SoMFNode    input    NULL      # input field
    SoSFBool    compact  TRUE      # leave out NULL entries or unnamed nodes
    SoEngineOutput output          # type SoMFString
}
@endverbatim
 *
 * @ingroup util
 * @author Gerhard Reitmayr
 */
class STARLIGHT_API SoNodeToName : public SoEngine {
public:
	SO_ENGINE_HEADER(SoNodeToName);
	
public:

    /// a list of nodes
    SoMFNode input;

    /// whether nodes without names are represented by empty strings or not at all.
    SoSFBool compact;

	/**
     * The engine output of type SoMFString.
     */
	SoEngineOutput output; // (SoMFString)


	/**
     * Static initializer. Registers the converter with the OIV runtime.
     * Call this in your program before you load an .iv file using this converter.
     */
	static void initClass();

	/**
     * Default constructor.
     */
	SoNodeToName();

private:

	/**
     * Default destructor.
     */
	virtual ~SoNodeToName();

	/**
     * Evaluation method.
     */
	virtual void evaluate();
};

#endif//_SONODETONAME_H_
