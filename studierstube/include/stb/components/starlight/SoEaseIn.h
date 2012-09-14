 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== */

#ifndef _SOEASEIN_H_
#define _SOEASEIN_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFBitMask.h>

#include "starlight.h"

/** 
@ingroup util

A helper class for smooth animations. Converts float inputs in the range from 0..1 (e.g. from a
SoOneShot engine) to float values in the range form 0..1 (or 1 to 0 if reversed), applying a non-linear laws that smoothens the start
and end velocity. 

*/
class STARLIGHT_API SoEaseIn : public SoEngine {

   SO_ENGINE_HEADER(SoEaseIn);

 public:

     SoSFBitMask style;

     enum MyStyles {
         EASE = 0,         ///< Normal ease function (0 -> 1)
         LOGARITHMIC = 1,  ///< Logaricmic function (0 -> 1)
         PULSE = 2         ///< Sinusoidal function (0 -> 1 ->0)
     } Styles;

    SoSFFloat ease;        ///<  parameter for configuring ease functions. determines how big the effect will be.
    SoSFFloat in;          ///< input 0->1 float  
    SoSFBool reverse;      ///< results in a 1->0 law FALSE as Default
    SoEngineOutput out; 	///< output 0->1 float or inverse if reverse field is TRUE
	
   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void initClass();

   // Constructor
   SoEaseIn();

 private:

   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoEaseIn();

   virtual void evaluate();

};

#endif // _SOEASEIN_H_
