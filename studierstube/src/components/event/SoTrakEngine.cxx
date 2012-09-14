/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Source file for SoTrakEngine engine
  *
  * @author Gerhard Reitmayr, Istvan Barakonyi
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/engines/SoSubEngine.h>
#include <stb/components/event/EventContextFilter.h>
#include <stb/components/event/SoTrakEngine.h>

SO_ENGINE_SOURCE(SoTrakEngine);

void SoTrakEngine::initClass(void)
{    
    SO_ENGINE_INIT_CLASS(SoTrakEngine,SoEngine,"Engine");
}

SoTrakEngine::SoTrakEngine(void) : adapter(NULL)
{
    SO_ENGINE_CONSTRUCTOR(SoTrakEngine);

    SO_ENGINE_ADD_INPUT(key,(""));
	SO_ENGINE_ADD_INPUT(value,(""));
	SO_ENGINE_ADD_INPUT(buttonHisteresis,(FALSE));
	SO_ENGINE_ADD_INPUT(locked, (FALSE));
	
    SO_ENGINE_ADD_INPUT(translationIn, (0,0,0));
    SO_ENGINE_ADD_INPUT(rotationIn, (0,0,0,1));
    SO_ENGINE_ADD_INPUT(confidenceIn, (0.0)); //mf
    SO_ENGINE_ADD_INPUT(timestampIn, (0.0)); //eev
    SO_ENGINE_ADD_INPUT(buttonIn0, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn1, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn2, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn3, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn4, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn5, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn6, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn7, (FALSE));

    SO_ENGINE_ADD_OUTPUT(translation, SoSFVec3f);
    SO_ENGINE_ADD_OUTPUT(rotation, SoSFRotation);
    SO_ENGINE_ADD_OUTPUT(confidence, SoSFFloat); //mf
    SO_ENGINE_ADD_OUTPUT(timestamp, SoSFTime); //eev
    SO_ENGINE_ADD_OUTPUT(button0, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button1, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button2, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button3, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button4, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button5, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button6, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button7, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(buttonWrapper, SoSFShort);


	buttonHistory0=FALSE;
	buttonHistory1=FALSE;
	buttonHistory2=FALSE;
	buttonHistory3=FALSE;
	buttonHistory4=FALSE;
	buttonHistory5=FALSE;
	buttonHistory6=FALSE;
	buttonHistory7=FALSE;

	buttonChange0=TRUE;
	buttonChange1=TRUE;
	buttonChange2=TRUE;
	buttonChange3=TRUE;
	buttonChange4=TRUE;
	buttonChange5=TRUE;
	buttonChange6=TRUE;

    SO_ENGINE_OUTPUT(buttonWrapper,SoSFShort,setValue(0));
    this->enableNotify(FALSE);
}

SoTrakEngine::~SoTrakEngine()
{
    if (adapter!=NULL) delete adapter;
}


void SoTrakEngine::inputChanged(SoField* whichField)
{
    if (whichField==&key || whichField==&value){
        delete adapter;
        adapter=new EventAdapter<SoTrakEngine>(this);
    }
}

void SoTrakEngine::processEvent(SoInputEvent *event)
{

    if (event){

		/// FIXME: Why is the schema not used here? Mendez 20060315
		if (event->containsKey("event.position")){
			SbVec3f pos=event->getSFVec3f("event.position");
			translationIn.setValue(pos);
		}
		if (event->containsKey("event.orientation")){
			SbRotation rot=event->getSFRotation("event.orientation");
			rotationIn.setValue(rot);
		}
      // map confidence
      if (event->containsKey("event.confidence")){
         float conf = event->getSFFloat("event.confidence");
         confidenceIn.setValue(conf);
      }
      
      if (event->containsKey("event.time")){
	
	timestampIn.setValue(	((event->getSFTime("event.time")).getValue()));
      }

        short wrapper=0;
        for (int i=0;i<8;i++){
            char fieldName[255],buttonEventName[255];
            sprintf(fieldName,"buttonIn%d",i);
            SoSFBool *field=(SoSFBool*)(this->getField(fieldName));
            if (field){
                sprintf(buttonEventName,"event.button.%d",i);
				if (event->containsKey(buttonEventName))
                {
					field->setValue(event->getSFBool(buttonEventName));
                    wrapper=wrapper|(event->getSFBool(buttonEventName) << i);
				}
            }
        }
        buttonInWrapper.setValue(wrapper);
		
		evaluate();
    }

}

void SoTrakEngine::evaluate() 
{
	if(locked.getValue())return;
	
	SO_ENGINE_OUTPUT(translation,SoSFVec3f,setValue(translationIn.getValue()));
	SO_ENGINE_OUTPUT(rotation,SoSFRotation,setValue(rotationIn.getValue()));
	SO_ENGINE_OUTPUT(confidence,SoSFFloat,setValue(confidenceIn.getValue()));

	if (!buttonHisteresis.getValue())
	{
		SO_ENGINE_OUTPUT(button0,SoSFBool,setValue(buttonIn0.getValue()));
		SO_ENGINE_OUTPUT(button1,SoSFBool,setValue(buttonIn1.getValue()));
		SO_ENGINE_OUTPUT(button2,SoSFBool,setValue(buttonIn2.getValue()));
		SO_ENGINE_OUTPUT(button3,SoSFBool,setValue(buttonIn3.getValue()));
		SO_ENGINE_OUTPUT(button4,SoSFBool,setValue(buttonIn4.getValue()));
		SO_ENGINE_OUTPUT(button5,SoSFBool,setValue(buttonIn5.getValue()));
		SO_ENGINE_OUTPUT(button6,SoSFBool,setValue(buttonIn6.getValue()));
		SO_ENGINE_OUTPUT(button7,SoSFBool,setValue(buttonIn7.getValue()));
		SO_ENGINE_OUTPUT(buttonWrapper,SoSFShort,setValue(buttonInWrapper.getValue()));
    
	}
	else  // if Histeresis is on
	{
		// Button 0
		if ((buttonChange0==FALSE)&&(buttonIn0.getValue()==TRUE))
		{
			buttonHistory0=!buttonHistory0;
			SO_ENGINE_OUTPUT(button0,SoSFBool,setValue(buttonHistory0));
		}
		buttonChange0=buttonIn0.getValue();

		// Button 1
		if ((buttonChange1==FALSE)&&(buttonIn1.getValue()==TRUE))
		{
			buttonHistory1=!buttonHistory1;
			SO_ENGINE_OUTPUT(button1,SoSFBool,setValue(buttonHistory1));
		}
		buttonChange1=buttonIn1.getValue();

		// Button 2
		if ((buttonChange2==FALSE)&&(buttonIn2.getValue()==TRUE))
		{
			buttonHistory2=!buttonHistory2;
			SO_ENGINE_OUTPUT(button2,SoSFBool,setValue(buttonHistory2));
		}
		buttonChange2=buttonIn2.getValue();

		// Button 3
		if ((buttonChange3==FALSE)&&(buttonIn3.getValue()==TRUE))
		{
			buttonHistory3=!buttonHistory3;
			SO_ENGINE_OUTPUT(button3,SoSFBool,setValue(buttonHistory3));
		}
		buttonChange3=buttonIn3.getValue();

		// Button 4
		if ((buttonChange4==FALSE)&&(buttonIn4.getValue()==TRUE))
		{
			buttonHistory4=!buttonHistory4;
			SO_ENGINE_OUTPUT(button4,SoSFBool,setValue(buttonHistory4));
		}
		buttonChange4=buttonIn4.getValue();

		// Button 5
		if ((buttonChange5==FALSE)&&(buttonIn5.getValue()==TRUE))
		{
			buttonHistory5=!buttonHistory5;
			SO_ENGINE_OUTPUT(button5,SoSFBool,setValue(buttonHistory5));
		}
		buttonChange5=buttonIn5.getValue();

		// Button 6
		if ((buttonChange6==FALSE)&&(buttonIn6.getValue()==TRUE))
		{
			buttonHistory6=!buttonHistory6;
			SO_ENGINE_OUTPUT(button6,SoSFBool,setValue(buttonHistory6));
		}
		buttonChange6=buttonIn6.getValue();

		// Button 7
		if ((buttonChange7==FALSE)&&(buttonIn7.getValue()==TRUE))
		{
			buttonHistory7=!buttonHistory7;
			SO_ENGINE_OUTPUT(button7,SoSFBool,setValue(buttonHistory7));
		}
		buttonChange7=buttonIn7.getValue();

	}

}


