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
 * ======================================================================== */
/**
 * declaration of generic SoEngineWrapper node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoEngineWrapper.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOENGINEWRAPPER_H_
#define _SOENGINEWRAPPER_H_

#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFEngine.h>

#include "starlight.h"

/** 
 * This node wraps an engine so that it can be used within the scene graph. This 
 * is important to be able to reuse Open Inventor engines in a VRML scene graph.
 * The SoEngineWrapper node features a single field called @c engine that contains 
 * the engine to wrap. It will further expose any fields and outputs of the engine
 * so that they can be used within the file format like fields and outputs of the
 * node. 
 *
 * Note: still crashes upon exit, if it is used with PROTOs and debug mode !
 * 
 * Note: probably still have to fix something on output, to get the DEF name from the node
 *
 * File format syntax :
@code
SoEngineWrapper {
    SoSFEngine engine NULL
}
@endcode
 *
 * Example for Inventor 2.1 format:
@code
DEF Test SoEngineWrapper {
    engine SoTimeCounter { } # needs to go first
    min 0                    # fields from the engine
    max 10
    frequency 0.1            
}
DEF Display SoText2 {
    string "hello" = USE Test.output # references an engine output
}
@endcode
 *
 * Example for VRML 2.0 format:
 @code
DEF Test SoEngineWrapper {
    engine SoTimeCounter {}
    min 0
    max 10
    frequency 0.1            
}
DEF Display SoText2 {
    string "hello"
}        
ROUTE Test.output TO Display.string
@endcode
 *
 * Example for using PROTOs:
 @code
PROTO WrapperTest [
    exposedField SFShort min -2
    eventOut     SFShort out
]
{
    Group {
        children [
            DEF Test SoEngineWrapper {
                engine SoTimeCounter {}
                min IS min
                max 10
                frequency 0.1            
                output IS out
            }
            DEF Display SoText2 {
                string "hello"
            }
            ROUTE Test.output TO Display.string
        ]
    }
}
 @endcode
 *
 *
@author Gerhard Reitmayr
@ingroup util
*/
class STARLIGHT_API SoEngineWrapper : public SoNodeEngine {
    
	typedef SoNode inherited;

    SO_NODEENGINE_HEADER(SoEngineWrapper);
    
public:
    /// field containing the engine that is wrapped, must always
    /// go first in the file format.
    SoSFEngine engine;
    
protected:
    /// We have to store these per instance, because of changing fields.
    SoFieldData * instanceFieldData;
    /// We have to store these per instance, because of changing outputs.
    SoEngineOutputData * instanceEngineOutputData;
        
public:
    
    // Initializes this class for use in scene graphs. This
    // should be called after database initialization and before
    // any instance of this node is constructed.
    static void initClass();
    
    // Constructor
    SoEngineWrapper();   

    virtual void copyContents(const SoFieldContainer * from, SbBool copyconnections);

protected:

    virtual void evaluate(void);
    
    // these engines must write out their input type before writing
    // their fields, so these are overridden:
    virtual SbBool	readInstance(SoInput *in, unsigned short flags);
    
private:
    
    /// Destructor. Private to keep people from trying to delete nodes, rather than using the reference count mechanism.
    virtual ~SoEngineWrapper();

    /// overrides SoNodeEngine::destroy() to avoid crashes
    virtual void destroy(void); 
       
    /// sets up the dynamic fields and outputs whenever the engine changes
    void setup(void);
};

#endif // _SOENGINEWRAPPER_H_
