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
/** Source file for SoMultimodalEngine engine
  *
  * @author Eduardo Veas
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/engines/SoSubEngine.h>
#include <stb/components/event/EventContextFilter.h>
#include <stb/components/event/SoMultimodalEngine.h>

#include <iostream>
#include <vector>


SO_ENGINE_SOURCE(SoMultimodalEngine);

void SoMultimodalEngine::initClass(){
   SO_ENGINE_INIT_CLASS(SoMultimodalEngine, SoEngine, "Engine");
};


//Constructor
SoMultimodalEngine::SoMultimodalEngine(): adapter(NULL)
{
  //	printf("SOMULTIMODALENGINE::CONSTRUCTOR\n");
  // Do Standard Constructor Stuff
  SO_ENGINE_CONSTRUCTOR(SoMultimodalEngine);

  SO_ENGINE_ADD_INPUT(key,(""));
  SO_ENGINE_ADD_INPUT(value,(""));
  SO_ENGINE_ADD_INPUT(histeresis,(FALSE));
  SO_ENGINE_ADD_INPUT(calculateAccumulatedPosition,(FALSE));

  // define input fields
  SO_ENGINE_ADD_INPUT( attrName, (""));
  SO_ENGINE_ADD_INPUT( attrType, (""));
  SO_ENGINE_ADD_INPUT( attrResetSignal, (""));
  SO_ENGINE_ADD_INPUT( vec3fIn, (0,0,0));
  SO_ENGINE_ADD_INPUT( accumulatedPositionIn, (0,0,0));
  SO_ENGINE_ADD_INPUT( rotationIn, (0, 0, 0, 1));
  //basic Types
  SO_ENGINE_ADD_INPUT( boolIn, (TRUE));
  SO_ENGINE_ADD_INPUT( floatIn, (0.0));
  SO_ENGINE_ADD_INPUT( intIn, (0));
  SO_ENGINE_ADD_INPUT( shortIn, (0));

  SO_ENGINE_ADD_INPUT( uintIn, (0));
  SO_ENGINE_ADD_INPUT( ushortIn, (0));
  SO_ENGINE_ADD_INPUT( stringIn, (""));
  SO_ENGINE_ADD_INPUT( mffloatIn, (0));

  /* just in case 
  SO_ENGINE_ADD_INPUT( imageIn, (0));
  SO_ENGINE_ADD_INPUT(  image3In, (0));*/
  SbMatrix identity;
  identity.makeIdentity();
  SO_ENGINE_ADD_INPUT( matrixIn, (identity)); 


  // define output fields specifying types
  SO_ENGINE_ADD_OUTPUT( vec3fValue,   SoSFVec3f);
  SO_ENGINE_ADD_OUTPUT( accumulatedPosition,   SoSFVec3f);
  SO_ENGINE_ADD_OUTPUT( rotationValue,   SoSFRotation);
  // basic Types
  SO_ENGINE_ADD_OUTPUT( boolValue, SoSFBool );
  SO_ENGINE_ADD_OUTPUT( floatValue,   SoSFFloat);
  SO_ENGINE_ADD_OUTPUT( intValue,   SoSFInt32);
  SO_ENGINE_ADD_OUTPUT( shortValue,   SoSFShort);

  SO_ENGINE_ADD_OUTPUT( uintValue,   SoSFUInt32);
  SO_ENGINE_ADD_OUTPUT( ushortValue,   SoSFUShort);
  SO_ENGINE_ADD_OUTPUT( stringValue,   SoSFString);
  SO_ENGINE_ADD_OUTPUT( mffloatValue,   SoMFFloat);

  /* just in case 
  SO_ENGINE_ADD_OUTPUT( imageValue,   SoSFImage);
  SO_ENGINE_ADD_OUTPUT( image3Value,   SoSFImage3); */
  SO_ENGINE_ADD_OUTPUT( matrixValue,   SoSFMatrix);

  buttonHistory = FALSE;
  buttonChange  = TRUE;
  boolHistory=FALSE;
  boolChange=TRUE;

  this->enableNotify(false);
  
};

// Destructor does nothing
SoMultimodalEngine::~SoMultimodalEngine(){
    if (adapter!=NULL) delete adapter;	
};


void SoMultimodalEngine::evaluate() {
	//printf("SOMULTIMODALENGINE::EVALUATE evaluating station%d, attr %s\n", station.getValue(), ((attrName.getValue()).getString()));

    if (!histeresis.getValue())
    {
        SO_ENGINE_OUTPUT(vec3fValue, SoSFVec3f, setValue(
            vec3fIn.getValue()));
        SO_ENGINE_OUTPUT(accumulatedPosition, SoSFVec3f, setValue(
            accumulatedPositionIn.getValue()));
        SO_ENGINE_OUTPUT(stringValue, SoSFString, setValue(
            stringIn.getValue()));
        SO_ENGINE_OUTPUT(rotationValue, SoSFRotation, setValue(
            rotationIn.getValue()));
        SO_ENGINE_OUTPUT(boolValue, SoSFBool, setValue(
            boolIn.getValue()));
        SO_ENGINE_OUTPUT(floatValue, SoSFFloat, setValue(
            floatIn.getValue()));
        SO_ENGINE_OUTPUT(intValue, SoSFInt32, setValue(
            intIn.getValue()));
        SO_ENGINE_OUTPUT(shortValue, SoSFShort, setValue(
            shortIn.getValue()));
        SO_ENGINE_OUTPUT(uintValue, SoSFUInt32, setValue(
            uintIn.getValue()));

        SO_ENGINE_OUTPUT(ushortValue, SoSFUShort, setValue(
            ushortIn.getValue()));
	     SO_ENGINE_OUTPUT(mffloatValue, SoMFFloat, copyFrom(
            mffloatIn));
		 SO_ENGINE_OUTPUT(matrixValue, SoSFMatrix, setValue(
			 matrixIn.getValue()));
    }
    else
    {
        // FIXME: At the moment only implemented for bool values. Mendez 20070628
        // Button 0
        if ((boolChange==FALSE)&&(boolIn.getValue()==TRUE))
        {
            boolHistory=!boolHistory;
            SO_ENGINE_OUTPUT(boolValue, SoSFBool, setValue(boolHistory));
        }
        boolChange=boolIn.getValue();



        SO_ENGINE_OUTPUT(vec3fValue, SoSFVec3f, setValue(
            vec3fIn.getValue()));
        SO_ENGINE_OUTPUT(accumulatedPosition, SoSFVec3f, setValue(
            accumulatedPositionIn.getValue()));
        SO_ENGINE_OUTPUT(stringValue, SoSFString, setValue(
            stringIn.getValue()));
        SO_ENGINE_OUTPUT(rotationValue, SoSFRotation, setValue(
            rotationIn.getValue()));
        SO_ENGINE_OUTPUT(floatValue, SoSFFloat, setValue(
            floatIn.getValue()));
        SO_ENGINE_OUTPUT(intValue, SoSFInt32, setValue(
            intIn.getValue()));
        SO_ENGINE_OUTPUT(shortValue, SoSFShort, setValue(
            shortIn.getValue()));
        SO_ENGINE_OUTPUT(uintValue, SoSFUInt32, setValue(
            uintIn.getValue()));

        SO_ENGINE_OUTPUT(ushortValue, SoSFUShort, setValue(
            ushortIn.getValue()));
        SO_ENGINE_OUTPUT(mffloatValue, SoMFFloat, copyFrom(
            mffloatIn));
		SO_ENGINE_OUTPUT(matrixValue, SoSFMatrix, setValue(
			matrixIn.getValue()));
	}

};
void SoMultimodalEngine::inputChanged(SoField * whichField){
  //printf("SOMULTIMODALENGINE::INPUTCHANGED \n");
    if (whichField==&key || whichField==&value){
        delete adapter;
        adapter=new EventAdapter<SoMultimodalEngine>(this);
    }
};

void SoMultimodalEngine::processEvent(SoInputEvent *event)
{
    if (event){
      SbString key = attrName.getValue();
     
      SbString eventstr = key.getSubString(0, 5);
      if (eventstr != "event."){
	key = "event.";
	key += attrName.getValue();
      }

      //      printf("SOMULTIMODALENGINE::PROCESSEVENT: multimodal %s\n", key.getString());	
      
      if (event->containsKey(key.getString())){
	// evaluate the type of the event in order to decide in which output to 
	// place it
	//	printf("SOMULTIMODALENGINE::PROCESSEVENT: of type %s\n", attrType.getValue().getString());	
	if(attrType.getValue()=="string"){

	  if (event->isOfType(key, typeid(SbString))){

	    stringIn.setValue(event->getSFString(key));
	    evaluate();
	  }

	} else if (attrType.getValue() == "int"){
	  if (event->isOfType(key, typeid(int32_t))){
	    intIn.setValue(event->getSFInt32(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "short"){
	  if (event->isOfType(key, typeid(short))){
	    shortIn.setValue(event->getSFShort(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "uint"){
	  if (event->isOfType(key, typeid(uint32_t))){
	    uintIn.setValue(event->getSFUint32(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "ushort"){
	  if (event->isOfType(key, typeid(unsigned short))){
	    ushortIn.setValue(event->getSFUshort(key));
	    evaluate();
	  }
	} else if (attrType.getValue() =="ulong"){
	  if (event->isOfType(key, typeid(uint32_t))){
	    uintIn.setValue(event->getSFUint32(key));
	    evaluate();
	  }
	} else if ((attrType.getValue() =="float") || (attrType.getValue() =="double")){
	  if ((event->isOfType(key, typeid(float))) || (event->isOfType(key, typeid(double)))){

	    floatIn.setValue(event->getSFFloat(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "bool"){
	  if (event->isOfType(key, typeid(SbBool))){
	    boolIn.setValue(event->getSFBool(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "vec3f"){
	  if (event->isOfType(key, typeid(SbVec3f))){
	    vec3fIn.setValue(event->getSFVec3f(key));
        if (calculateAccumulatedPosition.getValue())
        {
            accumulatedPositionIn.setValue(accumulatedPositionIn.getValue()+vec3fIn.getValue());
        }
	    evaluate();
	  }
	  else if (event->isOfType(key, typeid(std::vector<float>))){
		 std::vector<float> vec = event->getVector(key);
	    if (vec.size() == 3) {
			 vec3fIn.setValue(vec[0], vec[1], vec[2]);
             if (calculateAccumulatedPosition.getValue())
             {
                 accumulatedPositionIn.setValue(accumulatedPositionIn.getValue()+vec3fIn.getValue());
             }
		    evaluate();
		 }
	  }
	} else if (attrType.getValue() == "rotation") {
	  if (event->isOfType(key, typeid(SbRotation))){
	    rotationIn.setValue(event->getSFRotation(key));
	    evaluate();
	  }
	  else if (event->isOfType(key, typeid(std::vector<float>))){
		 std::vector<float> vec = event->getVector(key);
	    if (vec.size() == 4) {
			 rotationIn.setValue(vec[0], vec[1], vec[2], vec[3]);
		    evaluate();
		 }
	  }
	} else if (attrType.getValue() == "mffloat"){
	  if (event->isOfType(key, typeid(std::vector<float>))){
	    std::vector<float> vec = event->getVector(key);
		 for (unsigned int i=0; i<vec.size(); i++)
		    mffloatIn.set1Value(i,vec[i]);
	    evaluate();
	  }
	} else if (attrType.getValue() == "time"){
	  if (event->isOfType(key, typeid(SbTime))){
	    timeIn.setValue(event->getSFTime(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "matrix"){
		if (event->isOfType(key, typeid(std::vector<float>))){
			const std::vector<float> &vec = event->getVector(key);
			matrixIn.setValue(vec[0],vec[1],vec[2],vec[3],
							  vec[4], vec[5],vec[6],vec[7],
							  vec[8],vec[9],vec[10],vec[11],
							  vec[12],vec[13],vec[14],vec[15]);
			evaluate();
		}
	  
	} else if (attrType.getValue() == "image"){
	  
    }
    if (attrResetSignal.getValue().getLength()>0){      // Verify if a reset signal is provided
        SbString keyReset = attrResetSignal.getValue();
        SbString eventstrReset = keyReset.getSubString(0, 5);
        if (eventstrReset != "event."){
            keyReset = "event.";
            keyReset += attrResetSignal.getValue();
        }

        if (event->isOfType(keyReset, typeid(int32_t))) // it is assumed to be an int
        {
            if (event->getSFInt32(keyReset))    // If the signal arrives
            {
                // Reset everything
                vec3fIn.setValue(0,0,0);
                accumulatedPositionIn.setValue(0,0,0);
                rotationIn.setValue(0,0,0,1);
                boolIn.setValue(TRUE);
                floatIn.setValue(0.0);
                intIn.setValue(0);
                shortIn.setValue(0);
                uintIn.setValue(0);
                ushortIn.setValue(0);
                stringIn.setValue("");
                mffloatIn.setValue(0);
            }
        }
    }
      }
      //      evaluate(); 
    }
      
}




