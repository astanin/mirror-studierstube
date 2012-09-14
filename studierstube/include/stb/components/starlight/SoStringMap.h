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
/** header file for SoInt32FanIn engine
 *
 * @author   Florian Ledermann, Gerhard Reitmayr
 *
 * $Id: SoStringMap.h 25 2005-02-10 19:40:35Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _SOSTRINGMAP_H_
#define _SOSTRINGMAP_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>

#include "starlight.h"

/** 
 * The SoStringMap is a utility engine to map strings onto other strings. Three modes
 * are supported MAPPING, UNION and INTERSECTION. 
 *
 * In MAPPING mode the mapping is given as two lists of strings that are
 * matched by the same index. Two inputs provide either values of the a or b domain
 * and two outputs will then provide the mapping of the input data. Therefore data
 * in aIn will map to bOut and bIn to aOut. It is assumed that every data item appears
 * only once and only the first one is always selected to compute the mapping.
 *
 * In UNION mode the values with the same indices in the fields a and b provide
 * the ordered pairs of a relation. The inputs aIn and bIn will be mapped to any
 * values in the b or a domain that match a pair together with values from aIn and bIn.
 * The outputs will be put into bOut and aOut respectively. In this mode the 
 * outputs are treated as sets and every value will only appear once in the output, 
 * as a single input can generate more than one output etc. This computes the UNION
 * of the cylinder sets associated with each input value.
 *
 * In INTERSECTION modes a relation is assumed as in UNION, however only values
 * related to all input values are generated. This basically computes the intersection
 * of the cylinder sets associated with each input value.
 *
 * In INDEX mode, only a and aIn to bOut is used. It maps the incoming strings to the
 * respective indices they have in the field a. If a string is not found, no output value
 * is created for it. This mode simply allows to compute
 * the indices of strings in lists and can be combined with SoMultiSwitch to build
 * more intelligent switches.
 * 
 * The compact flag controls what to do with empty outputs which are created if 
 * inputs in MAPPING or INDEX more are not found in the map. If compact is TRUE,
 * these are filtered out and not added to the output list. This is also the 
 * default setting.
 *
 * Don't worry about the SoMFString fields, remember that OpenInventor does
 * automatic conversion between SoMFFields an SoSFFields of the same type.
 *
 * File format syntax :
@verbatim
SoStringMap {
    SoMFString a   []
    SoMFString b   []

    SoMFString aIn []
    SoMFString bIn []
  
    SoEngineOutput aOut
    SoEngineOutput bOut  

    SoSFEnum mode MAPPING | UNION | INTERSECTION | INDEX

    SoSFBool compact FALSE
}
@endverbatim
 * @ingroup util
 * @author Gerhard Reitmayr
 */
class STARLIGHT_API SoStringMap : public SoEngine  
{
 SO_ENGINE_HEADER(SoStringMap);

public:
    /** Initializes this class for use in scene graphs. This
     * should be called after database initialization and before
     * any instance of this node is constructed. */
    static void initClass();    

    /// provides the domain of input a
    SoMFString a;
    /// provides the domain of input b
    SoMFString b;

    /// input values in the domain a
    SoMFString aIn;
    /// input values in the domain b
    SoMFString bIn;

    /// mapped input values from bIn in domain a
    SoEngineOutput aOut;
    /// mapped input values from aIn in domain b
    SoEngineOutput bOut;
        
    enum Mode { MAPPING = 0, UNION, INTERSECTION, INDEX };

    /// the mode to operate in
    SoSFEnum mode;

    /// filter empty results in the output lists
    SoSFBool compact;

    // Constructor
    SoStringMap();
    
private:
    
    // Destructor. Private to keep people from trying to delete
    // nodes, rather than using the reference count mechanism.
    // Makes newer GCC complaining about destructor not being
    // avaliable as public function.
    virtual ~SoStringMap();
    virtual void evaluate();
};


#endif // !defined(_SOSTRINGMAP_H_)
