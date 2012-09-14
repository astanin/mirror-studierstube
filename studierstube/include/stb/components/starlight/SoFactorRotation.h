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
/** Definition of class SoFactorRotation
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoFactorRotation.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOFACTORROTATION_H_
#define _SOFACTORROTATION_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/fields/SoMFEnum.h>

#include "starlight.h"

/** This engine factors incoming rotations into two outgoing rotation.
 * The factor output returns a rotation around one axis only and the
 * remainder output the rest of the rotation. The enum field axis defines 
 * which rotation axis to factor out and the enum field order in which order
 * the two factors need to be multiplied to yield the initial rotation :
 * @li @c PRE defines the factorization as Q = Qremainder * Qfactor
 * @li @c POST defines the factorization as Q = Qfactor * Qremainder
 *
 * The inverse outputs return the inverse of the factors. Observe that
 * the order of mulitplication is reversed for the inverse.
 *
 * The engine has only MF field inputs and outputs and behaves like a common
 * OpenInventor engine. It outputs as many entries as the maximum length input
 * field contains. For the other input fields the last value is repeated.
File format syntax / defaults:
 
@verbatim
SoFactorRotation {
   # general parameters
   SoMFEnum  axis X     # Y | Z
   SoMFEnum  order PRE  # POST

   # input
   SoMFRotation rotation 0 0 1 0 

   # outputs
   SoMFRotation factor
   SoMFRotation remainder

   SoMFRotation factorInverse
   SoMFRotation remainderInverse
}
@endverbatim
 * @author Gerhard Reitmayr
 * @ingroup util
 */
class STARLIGHT_API SoFactorRotation : public SoEngine  
{
    SO_ENGINE_HEADER(SoFactorRotation);

public:

    /// the input field containing the composite rotation
    SoMFRotation rotation;

    enum Axis {
        X = 0, Y = 1, Z = 2
    };
    /// the enum defines a rotation about which axis to factor out
    SoMFEnum axis; 

    enum Order {
        PRE, POST
    };
    /// order defines the order of the factorization
    SoMFEnum order;

    /// this output contains the factor as a rotation around the defined prinzipal axis
    SoEngineOutput factor;
    /// this output contains the remainder rotation
    SoEngineOutput remainder;
    /// this output contains the inverse of the factor
    SoEngineOutput factorInverse;
    /// this output contains the inverse of the remainder
    SoEngineOutput remainderInverse;

	SoFactorRotation();
	static void initClass();

    /** this method computes the actual factorization and returns it in factor and remainder.
     * Its public and static, so it can be reused by other code. 
     * @param rotation rotation to factor
     * @param factor rotation to store the factor in
     * @param remainder rotation to store the remainder in
     * @param axis X,Y,Z as defined in this class
     * @param order PRE,POST as defined in this class
     */
    static void computeFactorization( const SbRotation & rotation, SbRotation & factor, SbRotation & remainder, int axis, int order);
    
private:
	/// Destructor
	virtual ~SoFactorRotation();
	
	/// evaluation method
	virtual void evaluate();
};

#endif // !defined(_SOFACTORROTATION_H_)
