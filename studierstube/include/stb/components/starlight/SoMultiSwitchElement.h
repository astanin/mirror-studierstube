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
/** SoMultiSwitchElement header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoMultiSwitchElement.h 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOMULTISWITCHELEMENT_H_
#define _SOMULTISWITCHELEMENT_H_

#include <cstring>
#include <vector>

#include <Inventor/elements/SoReplacedElement.h>

#include "starlight.h"

/** This class implements the element to store the value of a SoMultiSwitch
 * node for inheritance of the children value. Its class is initalized by the
 * initClass method of SoMultiSwitch, which also enables the element with all
 * actions necessary. 
 * @author Gerhard Reitmayr
 * @ingroup util
 */
class STARLIGHT_API SoMultiSwitchElement : public SoReplacedElement 
{
    SO_ELEMENT_HEADER(SoMultiSwitchElement);

public:
    
    /// Initializes the element and sets a default value
    virtual void init(SoState *state);

    /// Initializes the SoMultiSwitchElement class
    static void initClass();

    /// Prints element (for debugging)
    virtual void print(FILE *fp) const;

    /// sets the current element value. 
    static void set(SoState *state, SoNode * node, const int32_t * values, const int32_t num )
    {
        SoMultiSwitchElement *elt;
        // Get an instance we can change (pushing if necessary)
        elt = (SoMultiSwitchElement *) getElement(state, classStackIndex,node);
        elt->indices.assign( values, values+num );
    }

    /** gets the current element value. The data pointed to is not guaranteed
     *  to stay valid until the next call to the OpenInventor library or the
     *  end of the calling function. Therefore copy the data first, if you need
     * that.
     */
    static const int32_t * get( SoState *state, int32_t * num )
    {
        const SoMultiSwitchElement * el = (const SoMultiSwitchElement *)getConstElement(state, classStackIndex);
        *num = (int32_t) el->indices.size();
        return & el->indices.front();
    }

protected:
    /// Destructor
    virtual ~SoMultiSwitchElement();
    
    /// stores the vector of indices
    std::vector<int32_t> indices;
};

#endif

