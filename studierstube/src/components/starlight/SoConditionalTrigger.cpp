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
#endif

#include <Inventor/SoPath.h>

#include <stb/components/starlight/SoConditionalTrigger.h>

SO_ENGINE_SOURCE(SoConditionalTrigger);

void
SoConditionalTrigger::initClass()
{
    SO_ENGINE_INIT_CLASS(SoConditionalTrigger, SoEngine, "Engine");
}

SoConditionalTrigger::SoConditionalTrigger()
{
   SO_ENGINE_CONSTRUCTOR(SoConditionalTrigger);

   SO_ENGINE_ADD_INPUT(triggerBool, (true));
   boolIn.setNum(0);						
   SO_ENGINE_ADD_INPUT(triggerFloat, (0.0));
   boolIn.setNum(0);						
   SO_ENGINE_ADD_INPUT(triggerInt, (0));
   boolIn.setNum(0);						
   SO_ENGINE_ADD_INPUT(triggerString, (""));
   boolIn.setNum(0);						
   SO_ENGINE_ADD_INPUT(triggerNode, (NULL));
   boolIn.setNum(0);						


   SO_ENGINE_ADD_INPUT(boolIn, (false));
   boolIn.setNum(0);						// we cant initialize an empty MFField, so we have to use this hack
   SO_ENGINE_ADD_INPUT(floatIn, (0.0));
   floatIn.setNum(0);						// we cant initialize an empty MFField, so we have to use this hack
   SO_ENGINE_ADD_INPUT(intIn, (0));
   intIn.setNum(0);							// we cant initialize an empty MFField, so we have to use this hack
   SO_ENGINE_ADD_INPUT(stringIn, (""));
   stringIn.setNum(0);						// we cant initialize an empty MFField, so we have to use this hack
   SO_ENGINE_ADD_INPUT(nodeIn, (NULL));
   nodeIn.setNum(0);						// we cant initialize an empty MFField, so we have to use this hack
   SO_ENGINE_ADD_INPUT(triggerIn, ());

   SO_ENGINE_ADD_INPUT(forwardInput, (FALSE));
   SO_ENGINE_ADD_INPUT(retrigger, (FALSE));


   SO_ENGINE_ADD_INPUT(triggerAtInit, (false));
   SO_ENGINE_ADD_INPUT(token, (""));


   SO_ENGINE_DEFINE_ENUM_VALUE(comparetype, EQUAL);
   SO_ENGINE_DEFINE_ENUM_VALUE(comparetype, LESS_THAN);
   SO_ENGINE_DEFINE_ENUM_VALUE(comparetype, GREATER_THAN);
   SO_ENGINE_DEFINE_ENUM_VALUE(comparetype, LESS_OR_EQUAL);
   SO_ENGINE_DEFINE_ENUM_VALUE(comparetype, GREATER_OR_EQUAL);
   SO_ENGINE_DEFINE_ENUM_VALUE(comparetype, NOT_EQUAL);

   SO_ENGINE_SET_SF_ENUM_TYPE(comparison, comparetype);
   SO_ENGINE_ADD_INPUT(comparison, (EQUAL));


   SO_ENGINE_ADD_OUTPUT(trigger, SoSFTrigger);
   SO_ENGINE_ADD_OUTPUT(tokenOut, SoSFString);
   SO_ENGINE_ADD_OUTPUT(boolOut, SoSFBool);
   SO_ENGINE_ADD_OUTPUT(intValueOut, SoSFInt32);

   triggerNotified = false;
   tempBool = false;
}

SoConditionalTrigger::~SoConditionalTrigger()
{
}

void SoConditionalTrigger::inputChanged(SoField * whichField)
{
    if ((whichField == &token) | (whichField == &triggerAtInit))
    {
        trigger.enable(FALSE);
        tokenOut.enable(FALSE);
        boolOut.enable(FALSE);
		//SoDebugError::postInfo("SoConditionalTrigger::inputChanged()",
		//		"%s: token now set to: %s",
		//		this->getName().getString(), token.getValue().getString());
    }
    else {
	    bool localC = true;
        bool any = false;
	    int i,j, num, tnum;

        if (whichField == &triggerIn) {
            any = true;
            triggerNotified = false;
        }

	    // check all conditions.
	    if (((num = boolIn.getNum()) > 0) && ((tnum = triggerBool.getNum()) > 0)){
		    localC = false; any = true;
		    for (i=0; i<num; i++){
                for (j=0; j<tnum; j++){
			        if (boolIn[i] == triggerBool[j]){
				        localC = true;
				        break;
			        }
                }
                if (localC) break;
		    }
	    }

	    if (localC && ((num = floatIn.getNum()) > 0) && (triggerFloat.getNum() > 0)) {
		    localC = false; any = true;
		    for (i=0; i<num; i++){
			    if (compareFloat(floatIn[i])){
				    localC = true;
				    break;
			    }
		    }
	    }

	    if (localC && ((num = intIn.getNum()) > 0) && (triggerInt.getNum() > 0)) {
		    localC = false; any = true;
		    for (i=0; i<num; i++){
			    if (compareInt(intIn[i])){
				    localC = true;
				    break;
			    }
		    }
	    }

	    if (localC && ((num = stringIn.getNum()) > 0) && ((tnum = triggerString.getNum()) > 0)) {
		    localC = false; any = true;
            if (comparison.getValue() == EQUAL) {
                for (i=0; i<num; i++){
                    for (j=0; j<tnum; j++){
			            if (stringIn[i] == triggerString[j]){
				            localC = true;
				            break;
			            }
                    }
                    if (localC) break;
		        }
            }
            else {
                for (i=0; i<num; i++){
                    for (j=0; j<tnum; j++){
			            if (! stringIn[i] == triggerString[j]){
				            localC = true;
				            break;
			            }
                    }
                    if (localC) break;
		        }
            }
	    }
        
	    if (localC && ((num = nodeIn.getNum()) > 0) && ((tnum = triggerNode.getNum()) > 0)) {
		    localC = false; any = true;
		    for (i=0; i<num; i++){
                for (j=0; j<tnum; j++){
			        if (nodeIn[i] == triggerNode[j]){
				        localC = true;
				        break;
			        }
                }
                if (localC) break;
		    }
	    }

        if (!any) localC = false;
                
	    if (localC) {
            if (!retrigger.getValue())
            {
                if (!triggerNotified) {
				    //SoDebugError::postInfo("SoConditionalTrigger::inputChanged()",
				    //		"%s: condition == TRUE -> notify trigger!\n",this->getName().getString());
                    boolOut.enable(TRUE);
                    trigger.enable(TRUE);
                    tokenOut.enable(TRUE);
                    triggerNotified = true;
                    tempBool = true;
                }
                else {
                    trigger.enable(FALSE);
                    tokenOut.enable(FALSE);
                }
            }
            else
            {
                boolOut.enable(TRUE);
                trigger.enable(TRUE);
                tokenOut.enable(TRUE);
                triggerNotified = true;
                tempBool = true;
            }
	    }
        else {
            if (tempBool) {
				//SoDebugError::postInfo("SoConditionalTrigger::inputChanged()",
				//		"%s: condition == FALSE -> update boolOut!",
				//		this->getName().getString());
                tempBool = false;
                boolOut.enable(TRUE);
            }
            trigger.enable(FALSE);
            tokenOut.enable(FALSE);
            triggerNotified = false;
        }
    }
}

bool SoConditionalTrigger::compareFloat(const float val)
{
    int j = 0;

    switch (comparison.getValue()) {
    case EQUAL:
        for (j=0; j<triggerFloat.getNum(); j++)
            if (val == triggerFloat[j]) return true;
        break;
    case LESS_THAN: 
        for (j=0; j<triggerFloat.getNum(); j++)
            if (val < triggerFloat[j]) return true;
        break;
    case GREATER_THAN: 
        for (j=0; j<triggerFloat.getNum(); j++)
            if (val > triggerFloat[j]) return true;
        break;
    case LESS_OR_EQUAL: 
        for (j=0; j<triggerFloat.getNum(); j++)
            if (val <= triggerFloat[j]) return true;
        break;
    case GREATER_OR_EQUAL: 
        for (j=0; j<triggerFloat.getNum(); j++)
            if (val >= triggerFloat[j]) return true;
        break;
    case NOT_EQUAL:
        for (j=0; j<triggerFloat.getNum(); j++)
            if (val != triggerFloat[j]) return true;
        break;
    }
    return false;
}

bool SoConditionalTrigger::compareInt(const int val)
{
    int j = 0;

    switch (comparison.getValue()) {
    case EQUAL:
        for (j=0; j<triggerInt.getNum(); j++)
            if (val == triggerInt[j]) return true;
        break;
    case LESS_THAN: 
        for (j=0; j<triggerInt.getNum(); j++)
            if (val < triggerInt[j]) return true;
        break;
    case GREATER_THAN: 
        for (j=0; j<triggerInt.getNum(); j++)
            if (val > triggerInt[j]) return true;
        break;
    case LESS_OR_EQUAL: 
        for (j=0; j<triggerInt.getNum(); j++)
            if (val <= triggerInt[j]) return true;
        break;
    case GREATER_OR_EQUAL: 
        for (j=0; j<triggerInt.getNum(); j++)
            if (val >= triggerInt[j]) return true;
        break;
    case NOT_EQUAL:
        for (j=0; j<triggerInt.getNum(); j++)
            if (val != triggerInt[j]) return true;
        break;
    }
    return false;
}

void SoConditionalTrigger::evaluate()
{
    /*boolIn.getValues(0);
    floatIn.getValues(0);
    intIn.getValues(0);
    timeIn.getValues(0);
    stringIn.getValues(0);*/

	SO_ENGINE_OUTPUT(boolOut, SoSFBool, setValue(tempBool));

	//SoDebugError::postInfo("SoConditionalTrigger::evaluate()",
	//		"%s: boolOut: %i",
	//		this->getName().getString(),tempBool);

    if (trigger.isEnabled()) {
        //printf("About to output ... %d\n", tmp);
        SO_ENGINE_OUTPUT(tokenOut, SoSFString, setValue(token.getValue()));
	    SO_ENGINE_OUTPUT(trigger, SoSFTrigger, setValue());

        if (forwardInput.getValue())
        {
            SO_ENGINE_OUTPUT(intValueOut, SoSFInt32, setValue(intIn[0]));
        }
		//SoDebugError::postInfo("SoConditionalTrigger::evaluate()",
		//		"%s: Token: %s, trigger fired!",
		//		this->getName().getString(),token.getValue().getString());
    }

    trigger.enable(FALSE);
    tokenOut.enable(FALSE);
}



