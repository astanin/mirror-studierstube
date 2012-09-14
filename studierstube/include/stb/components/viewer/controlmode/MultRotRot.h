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
 * <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube 
 * ======================================================================== */
/** Definition of class MultRotRot
 *
 *
 * @author Rainer Splechtna
 *
 * $Id: MultRotRot.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _MULTROTROT_H_
#define _MULTROTROT_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFRotation.h>
#include <stb/base/macros.h>



/** This engine multiplies two rotations connected to the input fields: rotA and rotB.
 * The product is written to the output-field: product.
 * @ingroup util
 */


BEGIN_NAMESPACE_STB

class  MultRotRot : public SoEngine {

    SO_ENGINE_HEADER(MultRotRot);

public:
    /// input fields: two rotations
    SoMFRotation    rotationA;
    SoMFRotation    rotationB;

    /// output is a rotation
    SoEngineOutput product; // (SoMFRotation) product
    /// inverse of the resulting product
    SoEngineOutput inverse; 

    static void initClass();
    
    /// Constructor
    MultRotRot();

private:
    /// Destructor
    virtual ~MultRotRot();
    
    /// evaluation method
    virtual void evaluate();

};


END_NAMESPACE_STB


#endif

