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
//  PROJECT:    Studierstube
//  CONTENT:    Class definition of SoMultMatrix
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:03:54  gh      last modification
// ===========================================================================


#ifndef _SOMULTMATRIX_H_
#define _SOMULTMATRIX_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFMatrix.h>

#include "starlight.h"

/**
 * SoMultMatrix is an engine that multiplies matrices. Documented at
 * http://www.studierstube.org/stbapi/SoMultMatrix.html
 * SoMultMatrix is a convenience class created for cases where one needs the
 * product of two matrices or the inverse thereof. By not connecting matrix2,
 * one may also compute the inverse of matrix1 alone (as matrix2 defaults to
 * identity). This class is useful for setting up transformation hierarchies
 * that cannot be done with e. g., SoTransform nodes alone. In particular, this
 * class has been uses with SoSeamKits that can be connected to an SoTrakEngine
 * @ingroup util
 */
class STARLIGHT_API SoMultMatrix 
:
    public SoEngine 
{

   SO_ENGINE_HEADER(SoMultMatrix);

public:
	// Inputs:
	SoSFMatrix matrix1;      //matrix input 1
	SoSFMatrix matrix2;      //matrix input 2

	// Output:
	SoEngineOutput product;  // (SoSFMatrix) = matrix1 * matrix2
	SoEngineOutput inverse;  // (SoSFMatrix) = inverse(product)

	// Initialization
	static void initClass(); //static class initialization

	// Constructor
	SoMultMatrix();          //constructor

 private:
	// Destructor
	virtual ~SoMultMatrix(); //destructor

	// Evaluation method
	virtual void evaluate(); //compute product, inverse
};

#endif//_SOMULTMATRIX_H_
