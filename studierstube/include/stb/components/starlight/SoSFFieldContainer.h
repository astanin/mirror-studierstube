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
 * $Id: SoSFFieldContainer.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */


#ifndef  _SO_SF_FIELDCONTAINER_
#define  _SO_SF_FIELDCONTAINER_

#include <Inventor/fields/SoSubField.h>

#include "starlight.h"

class SoSFFieldContainer;

/**
@ingroup util

  is a Field that supports both SoEngines and SoNodes (as both are SoFieldContainer). Note that you you are only able
  to access Engine Inputs over this SFField as engine Outputs are not part of the SoFieldContainer super class.
  
  The implementation is close to that of SoSFNode.

  Was used in @ see SoRouter.

*/

class STARLIGHT_API SoSFFieldContainer : public SoSField {

    // Use standard field stuff
    SO_SFIELD_HEADER(SoSFFieldContainer, SoFieldContainer *, SoFieldContainer *);

  SoINTERNAL public:
    static void		initClass();

    // Update a copied field to use the copy of the node if there is one
    virtual void	fixCopy(SbBool copyConnections);

    // Override this to also check the stored node
    virtual SbBool	referencesCopy() const;

  private:
    // Override this to maintain write-references in node
    virtual void	countWriteRefs(SoOutput *out) const;

    // Changes value in field without doing other notification stuff.
    // Keeps track of references and auditors.
    void		setVal(SoFieldContainer *newValue);
};

#endif /* _SO_SF_FIELDCONTAINER_ */
