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
/*
 * Header file for the Multiple Engine Fader SoFaderVec3fEngine
 * @author Erick Mendez
 */

#ifndef _SOFADERVEC3FENGINE_H_
#define _SOFADERVEC3FENGINE_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/engines/SoOneShot.h>
#include <Inventor/engines/SoInterpolateVec3f.h>
#include <stb/components/starlight/SoEaseIn.h>
#include <stb/components/starlight/SoConditionalTrigger.h>
#include <Inventor/fields/SoSFBitMask.h>

#include "starlight.h"

/** 
@ingroup util
@author Erick Mendez

*/
class STARLIGHT_API SoFaderVec3fEngine : public SoEngine {

   SO_ENGINE_HEADER(SoFaderVec3fEngine);

 public:
     enum MyStyles {
         EASE = 0,
         LOGARITHMIC = 1,
         PULSE = 2
     } Styles;

     SoSFBitMask style;             ///< Fading Style: EASE, LOGARITHMIC or PULSE
     SoSFFloat ease;                ///< Interpolation parameter
     SoSFBool signal;               ///< The trigger signal that starts the engine
     SoSFBool fireOn;               ///< Whether the engine should start on true or false
     SoSFFloat duration;            ///< Animation time
     SoSFVec3f interpolate0;        ///< The start value to interpolate
     SoSFVec3f interpolate1;        ///< The end value to interpolate
     SoSFVec3f in;                  ///< For internal usage
     SoEngineOutput out;            ///< The output of the engine

   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void initClass();

   // Constructor
   SoFaderVec3fEngine();

 private:

     SoFieldSensor *signalSensor;
     SoFieldSensor *fireOnSensor;
     SoFieldSensor *interpolate0Sensor;
     SoFieldSensor *interpolate1Sensor;

     SoConditionalTrigger *conditional;
     SoOneShot *oneshot;
     SoEaseIn *easein;
     SoInterpolateVec3f *interpolatefloat;

     static void refreshCB(void* data, SoSensor* sensor);
     static void fieldsCB(void* data, SoSensor* sensor);

     void updateEngines();

   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoFaderVec3fEngine();

   virtual void evaluate();

};

#endif // _SOFADERVEC3FENGINE_H_
