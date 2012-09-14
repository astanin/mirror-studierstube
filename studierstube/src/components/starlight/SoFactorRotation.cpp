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
 * $Id: SoFactorRotation.cpp 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <cassert>

#include <Inventor/SbLinear.h>

#include <stb/components/starlight/SoFactorRotation.h>
#include <stb/components/starlight/math2.h>


SO_ENGINE_SOURCE(SoFactorRotation);

void SoFactorRotation::initClass() 
{
	SO_ENGINE_INIT_CLASS(SoFactorRotation, SoEngine, "Engine");
}

SoFactorRotation::SoFactorRotation()
{
    SO_ENGINE_CONSTRUCTOR(SoFactorRotation);

	// define input fields and their default values
	SO_ENGINE_ADD_INPUT( rotation, (SbVec3f(0.0,0.0,1.0),0.0));
    SO_ENGINE_ADD_INPUT( axis, (X));
    SO_ENGINE_ADD_INPUT( order,(PRE));

    SO_ENGINE_DEFINE_ENUM_VALUE(Axis, X);
    SO_ENGINE_DEFINE_ENUM_VALUE(Axis, Y);
    SO_ENGINE_DEFINE_ENUM_VALUE(Axis, Z);

    SO_ENGINE_SET_MF_ENUM_TYPE( axis, Axis );

    SO_ENGINE_DEFINE_ENUM_VALUE(Order, PRE);
    SO_ENGINE_DEFINE_ENUM_VALUE(Order, POST);

    SO_ENGINE_SET_MF_ENUM_TYPE( order, Order );

	// define the output
	SO_ENGINE_ADD_OUTPUT(factor, SoMFRotation);
    SO_ENGINE_ADD_OUTPUT(remainder, SoMFRotation);
    SO_ENGINE_ADD_OUTPUT(factorInverse, SoMFRotation);
    SO_ENGINE_ADD_OUTPUT(remainderInverse, SoMFRotation);
}

SoFactorRotation::~SoFactorRotation()
{
}

void SoFactorRotation::evaluate(){
    int nrotation = rotation.getNum();
    int naxis = axis.getNum();
    int norder = order.getNum();
    int nout = (nrotation && naxis && norder) ? m2max(m2max( nrotation, naxis),norder) : 0;

    SO_ENGINE_OUTPUT(factor, SoMFRotation, setNum( nout ));
    SO_ENGINE_OUTPUT(remainder, SoMFRotation, setNum( nout ));

    if( factor.getNumConnections() || remainder.getNumConnections())
    {
        for( int i = 0; i < nout; i++ )
        {
            const SbRotation &r = rotation[m2min(i,nrotation-1)];
            SbRotation fac, rem;
            int ax = axis[m2min(i,naxis-1)];
            int ord = order[m2min(i,norder-1)];

            computeFactorization( r, fac, rem, ax, ord );
            SbRotation facInv = fac.inverse();
            SbRotation remInv = rem.inverse();
            SO_ENGINE_OUTPUT(factor, SoMFRotation, set1Value(i,fac));
            SO_ENGINE_OUTPUT(remainder, SoMFRotation, set1Value(i,rem));
            SO_ENGINE_OUTPUT(factorInverse, SoMFRotation, set1Value(i,facInv));
            SO_ENGINE_OUTPUT(remainderInverse, SoMFRotation, set1Value(i,remInv));
        }
    }
}

void SoFactorRotation::computeFactorization( const SbRotation & rotation, SbRotation & factor, SbRotation & remainder, int axis, int order)
{
    const int W = 3;
    const float * rot = rotation.getValue();
    float * fac = (float *) factor.getValue();
//     float * rem = (float *) remainder.getValue();

    // actually these three cases would not strictly be necessary with
    // the right kind of indexing functions... go knock yourself out !
    switch( axis )
    {
    case X:
        if( rot[X] != 0 )
        {
            // compute the factor
            double d = rot[W] / rot[X];            
            fac[X] = (float)(sqrt(1/(1+d*d)));
            fac[W] = (float)(fac[X] * d);
            fac[Y] = 0;
            fac[Z] = 0;
        }
        else 
        {
            // compute the factor
            factor = SbRotation::identity();
        }
        break;
    case Y:
        if( rot[Y] != 0 )
        {
            // compute the factor
            double d = rot[W] / rot[Y];            
            fac[Y] = (float)(sqrt(1/(1+d*d)));
            fac[W] = (float)(fac[Y] * d);
            fac[X] = 0;
            fac[Z] = 0;
        }
        else 
        {
            // compute the factor
            factor = SbRotation::identity();
        }
        break;
    case Z:
        if( rot[Z] != 0 )
        {
            // compute the factor
            double d = rot[W] / rot[Z];
            fac[Z] = (float)(sqrt(1/(1+d*d)));
            fac[W] = (float)(fac[Z] * d);
            fac[Y] = 0;
            fac[X] = 0;
        }
        else 
        {
            // compute the factor
            factor = SbRotation::identity();
        }
        break;
    default :
        assert("SoFactorRotation undefined Axis case !" == NULL);
    }
    // compute the remainder
    if( order == PRE )
    {
        remainder = rotation * factor.inverse();
    }
    else
    {
        remainder = factor.inverse() * rotation;
    }
}
