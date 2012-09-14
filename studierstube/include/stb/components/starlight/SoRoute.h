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
 *
 * @author Thomas Psik
 *
 * $Id: SoRoute.h 24 2005-02-10 19:38:46Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SoRoute_H_
#define _SoRoute_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/SoInput.h>

#include "starlight.h"

class SoEngine;
class SoNode;
class SoNodeEngine;

/**
@ingroup util

This node allows the explicit construction of routes between two fields (or an engineoutput and a field, to be
exact) in the scenegraph. Source and target fields must be specified by a string in the format "<defname>.<fieldname>".

Note that if this node is removed from the scenegraph, the route persits. The fields are connected at read-in time. 
The defname part is evaluated in two steps. First it is evaluated within the currently read file and will result in
the last node before the SoRoute with the specified def name. If this fails, it evaluates the global name map and
will find the first node in the map with the specified def name. Changes to the fields after read-in from file will
not be evaluated !!

File Format / defaults:

@verbatim

SoRoute {
   SoSFString	from	""      # The full reference to the source field in "<defname>.<fieldname>" syntax 
   SoSFString	to		""		# The full reference to the destination field in "<defname>.<fieldname>" syntax
}
  
@endverbatim

Example

  DEF CUBE   SoCube		{}
  DEF CYLINDER   SoCylinder {}

  SoRoute { from "CUBE.height" to "CYLINDER.height" }


*/
class STARLIGHT_API SoRoute : public SoNode {

   SO_NODE_HEADER(SoRoute);

 public:
   // Fields:
   	
   /** The full reference to the 'from' field in "<defname>.<fieldname>" syntax. Use this as an alternative to REFing the
   source into the sourceNode/sourceEngine field */
   SoSFString         from;				    

   /** The full reference to the 'to' field in "<defname>.<fieldname>" syntax. Use this as an alternative to REFing the
   source into the sourceNode/sourceEngine field */
   SoSFString         to;				 
  	
   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   SoRoute();

 protected:

    /// Reads stuff into instance. Returns FALSE on error
    virtual SbBool	readInstance(SoInput *in, unsigned short flags);

    SoField *fromField;			///< used to keep a pointer to the from field (needed in SoRouter)
    SoEngineOutput *fromEngineOutput;	///< used to keep a pointer to the from EngineOutput(needed in SoRouter)
    SoFieldContainer *fromFieldContainer;
    SoField *toField;			///< used to keep a pointer to the to field (needed in SoRouter)
   	
    SbBool updateFromPtr(SoInput *in = NULL); ///< returns FALSE if string in from can not be resolved to a field or engineOutput
    SbBool updateToPtr(SoInput *in = NULL);	///< returns FALSE if string in from can not be resolved to a field 

 private:
	
    SbBool updateFromEngineOutput(SoEngine *, SbName engineOutputName, SoInput *in);  ///< 
    SbBool updateFromFieldPtr(SoFieldContainer *, SbName fieldName, SoInput *in	);				///<
    SbBool updateFromNodeEngineOutput(SoNodeEngine *fromNodeEngine, SbName engineOutputName, SoInput *in);
};

#endif // _SoRoute_H_
