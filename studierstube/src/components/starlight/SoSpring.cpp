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
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  include <windows.h>
#  include <math.h>
#endif

#include <stb/components/starlight/SoSpring.h>


// *********************** SoSpring **************************


SO_ENGINE_SOURCE(SoSpring);

void
SoSpring::initClass()
{
    SO_ENGINE_INIT_CLASS(SoSpring, SoEngine, "Engine");
}

SoSpring::SoSpring()
{
    SO_ENGINE_CONSTRUCTOR(SoSpring);

    SO_ENGINE_ADD_INPUT(startpos,(-5,0,0));
    SO_ENGINE_ADD_INPUT(endpos,(0,0,0));
	SO_ENGINE_ADD_INPUT(mass,(1.0f));
	SO_ENGINE_ADD_INPUT(vel,(0,0,0));
	SO_ENGINE_ADD_INPUT(acc,(0,0,0));

	
    SO_ENGINE_ADD_INPUT(stiffness,(250.0f));
    SO_ENGINE_ADD_INPUT(damp,(0.5f));
    SO_ENGINE_ADD_INPUT(timeStep,(0.002f));
    SO_ENGINE_ADD_INPUT(trigger,());
	SO_ENGINE_ADD_INPUT(treshold,(0.01f));

    SO_ENGINE_ADD_OUTPUT(posout, SoSFVec3f);

	evaluate();

	
	
    // This timer will be used to update the system once it is been triggered
    timer=new SoTimerSensor(timerFunc,this);
}

SoSpring::~SoSpring()
{
}

void SoSpring::timerFunc(void *data, SoSensor *)
{
    SoSpring *self=(SoSpring *) data;

	self->updateSystem();
	self->evaluate();
}



void SoSpring::applyForce()
{
    // Calculate the forces according to Hooke's Law
    // k * (DISTij-LENij) * Vij
	SbVec3f resultingForces;
 	SbVec3f deltaVector = endpos.getValue()-startpos.getValue();
    float elongation; 
	
	SbVec3f acc1 = acc.getValue();
	SbVec3f vel1 = vel.getValue();

	elongation = deltaVector.length();
	deltaVector.normalize();
	resultingForces=stiffness.getValue()*elongation*deltaVector;

	// debug 
	// printf("a1%f,a2%f,a3%f, v1%f,v2%f,v3%f,p1%f,p2%f,p3%f \n", acc1[0],acc1[1],acc1[2],vel1[0],vel1[1],vel1[2],startpos.getValue()[0],startpos.getValue()[1],startpos.getValue()[2] );
	acc1 = resultingForces*(1.0f/mass.getValue());
 	vel1 = vel1 + (acc1*timeStep.getValue());
	vel1  *= damp.getValue();

	vel.setValue(vel1);
	acc.setValue(acc1);

	if ( (vel1.length()<treshold.getValue())& (acc1.length()<treshold.getValue() ) )
		{
		// debug
		//printf("INFO: treasold reached!! \n");
		
		timer->unschedule();
		}
	else // update
		{
	startpos.setValue(startpos.getValue()+ vel1*timeStep.getValue()) ;
		}
}



void SoSpring::updateSystem()
{
    
    applyForce();
    
}


void SoSpring::inputChanged(SoField * whichField)
{
    if (whichField == &trigger)
    {
        // Start the thing!
        
        updateSystem();
        timer->setInterval(timeStep.getValue());
        timer->schedule();
      evaluate(); 
    } 
	
}

void SoSpring::evaluate()
{

SO_ENGINE_OUTPUT(posout, SoSFVec3f, setValue(startpos.getValue()));
}
