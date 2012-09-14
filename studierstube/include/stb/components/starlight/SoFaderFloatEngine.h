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
 * Header file for the Multiple Engine Fader SoFaderFloatEngine
 * @author Erick Mendez
 */

#ifndef _SOFADERFLOATENGINE_H_
#define _SOFADERFLOATENGINE_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/engines/SoOneShot.h>
#include <Inventor/engines/SoInterpolateFloat.h>
#include <stb/components/starlight/SoEaseIn.h>
#include <stb/components/starlight/SoConditionalTrigger.h>
#include <Inventor/fields/SoSFBitMask.h>

#include "starlight.h"

/** 
@ingroup starlight
@author Erick Mendez

*/
class STARLIGHT_API SoFaderFloatEngine : public SoEngine {

   SO_ENGINE_HEADER(SoFaderFloatEngine);

 public:
     enum MyStyles {
         EASE = 0,
         LOGARITHMIC = 1,
         PULSE = 2
     } Styles;

     SoSFBool enable;           
     SoSFBitMask style;             // The style of the fading, EASE, LOGARITHMIC or PULSE
     SoSFFloat ease;                // A weight factor
     SoSFBool signalForward;     // The trigger signal that starts the engine from 0 to 1
     SoSFBool signalBackward;    // The trigger signal that starts the engine from 1 to 0
     SoSFBool signalReset;       // The trigger signal that tells the output to be set to the interpolate0
     SoSFBool triggerForward;     // The trigger that starts the engine from 0 to 1
     SoSFBool triggerBackward;    // The trigger that starts the engine from 1 to 0
     SoSFBool triggerReset;       // The trigger that tells the output to be set to the interpolate0
     SoSFBool fireOn;               // Whether the engine should start on true or false
     SoSFFloat duration;            // How long should the animation last
     SoSFFloat interpolate0;        // The start value to interpolate
     SoSFFloat interpolate1;        // The end value to interpolate
     SoSFFloat in;                  // For internal usage
     SoEngineOutput out;            // The output of the engine

   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void initClass();

   // Constructor
   SoFaderFloatEngine();

 private:

     SoFieldSensor *signalForwardSensor;
     SoFieldSensor *signalBackwardSensor;
     SoFieldSensor *signalResetSensor;
     SoFieldSensor *triggerForwardSensor;
     SoFieldSensor *triggerBackwardSensor;
     SoFieldSensor *triggerResetSensor;
     SoFieldSensor *fireOnSensor;

     SoConditionalTrigger *conditional;
     SoOneShot *oneshot;
     SoEaseIn *easein;
     SoInterpolateFloat *interpolatefloat;

     static void refreshForwardCB(void* data, SoSensor* sensor);
     static void refreshBackwardCB(void* data, SoSensor* sensor);
     static void refreshResetCB(void* data, SoSensor* sensor);
     static void fireOnCB(void* data, SoSensor* sensor);

     void updateEngines();
     void reset();

   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoFaderFloatEngine();

   virtual void evaluate();

};

#endif // _SOFADERFLOATENGINE_H_
