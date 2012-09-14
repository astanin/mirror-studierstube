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
/** Implementation of class SoMultMatrix
  *
  * @author Dieter Schmalstieg, Gerd Hesina
  *
  * $Id: SoMultMatrix.cpp 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/components/starlight/SoMultMatrix.h>


SO_ENGINE_SOURCE(SoMultMatrix);

// ---------------------------------------------------------------------------
// Initializes the SoMultMatrix class.

void
SoMultMatrix::initClass()
{
   SO_ENGINE_INIT_CLASS(SoMultMatrix, SoEngine, "Engine");
}

// ---------------------------------------------------------------------------
// Constructor

SoMultMatrix::SoMultMatrix()
{
   // Do standard constructor stuff
   SO_ENGINE_CONSTRUCTOR(SoMultMatrix);

   // Define input fields and their default values
   SO_ENGINE_ADD_INPUT(matrix1,  (SbMatrix::identity()));
   SO_ENGINE_ADD_INPUT(matrix2,  (SbMatrix::identity()));

   // Define the output, specifying its type
   SO_ENGINE_ADD_OUTPUT(product, SoSFMatrix);
   SO_ENGINE_ADD_OUTPUT(inverse, SoSFMatrix);
}

// ---------------------------------------------------------------------------
// Destructor. Does nothing.

SoMultMatrix::~SoMultMatrix()
{
}

// ---------------------------------------------------------------------------
// This is the evaluation routine.
// computes product = matrix1 * matrix2, and
//          inverse = inverse(product)

void
SoMultMatrix::evaluate()
{
    SbMatrix pr = matrix1.getValue()*matrix2.getValue();
    
    SO_ENGINE_OUTPUT(product, SoSFMatrix, setValue(pr));
    SO_ENGINE_OUTPUT(inverse, SoSFMatrix, setValue(pr.inverse()));
}

//eof
