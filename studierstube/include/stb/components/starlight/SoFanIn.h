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
 * declaration of generic SoFanIn engine
 *
 * @author Florian Ledermann, Gerhard Reitmayr
 *
 * $Id: SoFanIn.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOFANIN_H_
#define _SOFANIN_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFInt32.h>

#include "starlight.h"

/** 
 * This engine allows to connect up to ten output fields to a single field. The
 * type of the field is specified in a type parameter given in the file format
 * or at instantiation. Any SoMField derived type can be used ! 
 * The use of the 'index' field:
 *     index >= 0  : only the updates of input field in#index will be considered, 
 *                   the update of other fields will be disregarded
 *     index == -1 : the updates of all fields are considered (default)
 *     index<-1 || index>9 (out of range): the updates of all fields are considered
 *
 * File format syntax :
@verbatim
SoFanIn {
    type <type>
    SoSFInt32 index
    <type> in0 
    <type> in1 
    <type> in2 
    <type> in3 
    <type> in4 
    <type> in5 
    <type> in6 
    <type> in7 
    <type> in8 
    <type> in9 

    SoEngineOutput out
}
@endverbatim
 *
@author Florian Ledermann, Gerhard Reitmayr
@ingroup util
*/
class STARLIGHT_API SoFanIn : public SoEngine {
    
    SO_ENGINE_HEADER(SoFanIn);
    
public:
    
    SoFieldList in;
    
    SoEngineOutput * out;

    SoSFInt32 index;
    
protected:
    SoField * lastChangedField;
    /// We have to store these per instance, because of changing type.
    SoFieldData * instanceFieldData;
    SoEngineOutputData * instanceEngineOutputData;
        
public:
    
    // Initializes this class for use in scene graphs. This
    // should be called after database initialization and before
    // any instance of this node is constructed.
    static void initClass();
    
    // Constructor
    SoFanIn(SoType inputType);
    
protected:
    
    // i don't know if we need this.. this is present in e.g. SoGate in OSOIV, but not in COIN
    // virtual SoFieldContainer * copyThroughConnection() const;
    
    virtual void copyContents(const SoFieldContainer * from, SbBool copyconnections);
    
private:
    
    /// Private constructor. Only used when reading from file.
    SoFanIn();
    /// Destructor. Private to keep people from trying to delete nodes, rather than using the reference count mechanism.
    virtual ~SoFanIn();
    
    virtual void inputChanged(SoField * whichField);
    virtual void evaluate();
        
    // these engines must write out their input type before writing
    // their fields, so these are overridden:
    virtual SbBool	readInstance(SoInput *in, unsigned short flags);
    virtual void	writeInstance(SoOutput *out);
    
    void setup(SoType inputType);

    int conversionCase;
};

#endif // _SOFANIN_H_
