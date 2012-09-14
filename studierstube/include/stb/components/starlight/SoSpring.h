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

#ifndef _SOSPRING_H_
#define _SOSPRING_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <stb/components/starlight/SoMFVec2s.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include "starlight.h"


/*!
 *@ingroup starlight
 * \brief
 * SoSpring is an engine to simulate a simplified one point spring.
 * 
 * This spring needs initial and target position and physical proprieties, then 
 * you can trigger it.
 *
 * \author Erik Mendez
 * \author Michele Fiorentino
 * Example
 * 
 *  DEF SPRING SoSpring
 *  {
 *  startpos = 0 0 10
 *  endpos = 0 0 0
 *  vel  -100 0 0
 *  acc 10 0 0
 *  mass1 10
 *  stiffness 5055.50
 *  damp 0.9
 *  timeStep 0.002
 *  treshold 1.5
 *  trigger = USE -+- use your trigger here -+-
 *  }
 * 
 */
class STARLIGHT_API SoSpring : public SoEngine 
{
   SO_ENGINE_HEADER(SoSpring);

 public:

     // Particles proprieties
     SoSFVec3f startpos;	/// Initial position
     SoSFVec3f endpos;		/// Target position
	 SoSFFloat mass;		/// Mass value
	 SoSFVec3f vel;			/// Initial velocity
	 SoSFVec3f acc;			/// Initial acceleration

	 // Spring proprieties
	 SoSFFloat stiffness;	/// Spring Rigidity
     SoSFFloat treshold;	/// Acceleration and velocity treshold
	 SoSFFloat damp;		/// Damp factor 
     
	 SoSFFloat timeStep;	/// update rate
     SoSFTrigger trigger;	/// Trigger

	 // engine outputs
     SoEngineOutput posout; /// instant position
   
	
   static void initClass();

   SoSpring();

 private:
	 	 

    void updateSystem();
    void applyForce();

    static void timerFunc(void *data, SoSensor *);

    SoTimerSensor *timer;                                            // This is a physics model, so its based in time
                                                     

    // Backup of everything defined by the user, for internal usage
    
    virtual ~SoSpring();

    virtual void inputChanged(SoField * whichField);
    virtual void evaluate();

};

#endif // _SOSPRING_H_
