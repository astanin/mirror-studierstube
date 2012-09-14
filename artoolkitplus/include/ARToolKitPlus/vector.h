/* ========================================================================
* PROJECT: ARToolKitPlus
* ========================================================================
* This work is based on the original ARToolKit developed by
*   Hirokazu Kato
*   Mark Billinghurst
*   HITLab, University of Washington, Seattle
* http://www.hitl.washington.edu/artoolkit/
*
* Copyright of the derived and new portions of this work
*     (C) 2006 Graz University of Technology
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
* For further information please contact 
*   Dieter Schmalstieg
*   <schmalstieg@icg.tu-graz.ac.at>
*   Graz University of Technology, 
*   Institut for Computer Graphics and Vision,
*   Inffeldgasse 16a, 8010 Graz, Austria.
* ========================================================================
** @author   Daniel Wagner
*
* $Id: vector.h 210 2007-05-09 12:26:23Z bornik $
* @file
* ======================================================================== */


#ifndef __ARTOOLKITVECTOR_HEADERFILE__
#define __ARTOOLKITVECTOR_HEADERFILE__

#include <cmath>
#include <ARToolKitPlus/config.h>

#include <ARToolKitPlus/math/artkpFixedBase_generic.h>
#include <ARToolKitPlus/math/artkpFloat_fixed.h>


namespace ARToolKitPlus {


struct ARMat;

namespace Vector {
//#ifndef _WIN32_WCE
//	typedef artkpFloat_fixed<artkpFixedBase_generic<16,1> > FLOAT;
//#else
	typedef ARFloat FLOAT;
//#endif
}


struct ARVec {
	Vector::FLOAT *v;
    int    clm;
} ;


namespace Vector {
	ARVec  *alloc( int clm );
	int    free( ARVec *v );
        // int    disp( ARVec *v );
	FLOAT household( ARVec *x );
	FLOAT innerproduct( ARVec *x, ARVec *y );
	int    tridiagonalize( ARMat *a, ARVec *d, ARVec *e );
}


}  // namespace ARToolKitPlus


#endif // __ARTOOLKITVECTOR_HEADERFILE__
