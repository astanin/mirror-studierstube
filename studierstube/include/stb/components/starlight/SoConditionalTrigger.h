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

#ifndef _SOCONDITIONALTRIGGER_H_
#define _SOCONDITIONALTRIGGER_H_

#include <Inventor/engines/SoSubEngine.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFNode.h>

#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFNode.h>

#include "starlight.h"

/** 
@ingroup util

A general purpose trigger, that can be used in various ways for OIV scripting.

SoConditionalTrigger takes multiple values of different types as input, and triggers
if a certain condition is met. Use the *In fields to connect fields of various types
to this engine, set the trigger* fields to the value that these inputs are compared to.
Use the comparison field to set the comparison type for numeric values, if you don't want
to test for equality.

The semantics of the SoConditionalTrigger is as follows: If <em>any</em> of the incoming values
of a given type match the comparison, the comparison evaluates true for this type. If <em>all</em>
comparisons for types that are connected evaluate true, the overall comparison evaluates true. This
value is written to boolOut.

The trigger field fires if the comparison changes from false to true - in this case, also
the string set in token is copied to tokenOut - this is for using strings as triggers for other engines, e. g.
to distinguish between various SoConditionalTriggers.
*/
class STARLIGHT_API SoConditionalTrigger : public SoEngine {

   SO_ENGINE_HEADER(SoConditionalTrigger);

 public:
    /// incoming bool values are compared to this value
    SoMFBool triggerBool;       
    /// incoming float values are compared to this value
    SoMFFloat triggerFloat;     
    /// incoming int values are compared to this value
    SoMFInt32 triggerInt;       
    /// incoming string values are compared to this value
    SoMFString triggerString;
    /// if the node is contained in the nodeIn, the trigger fires
    SoMFNode triggerNode;

    /// bool input
    SoMFBool boolIn;            
    /// float input
    SoMFFloat floatIn;          
    /// int input
    SoMFInt32 intIn;            
    /// string input
    SoMFString stringIn;
    /// node input
    SoMFNode nodeIn;  
    /// pass-through trigger input
    SoSFTrigger triggerIn;
    
    /// whether to fire the trigger on application start (not implemented)
    SoSFBool triggerAtInit;      

    enum comparetype {
        EQUAL = 0,
        LESS_THAN,
        GREATER_THAN,
		LESS_OR_EQUAL,
		GREATER_OR_EQUAL,
        NOT_EQUAL
    };
	
	/// comparison type for numeric comparisons (EQUAL | LESS_THAN | GREATER_THAN | LESS_OR_EQUAL | GREATER_OR_EQUAL)
	SoSFEnum comparison; 
    
    /// A string token that will be copied to tokenOut each time the trigger fires.
    SoSFString token;           

    // Whether to forward the input values
    SoSFBool forwardInput;
    // Whether to output regardless of previous state
    SoSFBool retrigger;

    /// fires if the condition changes from false to true
    SoEngineOutput trigger;         
    /// is set to token whenever trigger fires
    SoEngineOutput tokenOut;        
    /// current state of condition
    SoEngineOutput boolOut;     
    /// The incoming value is output if it meets the condition
    SoEngineOutput intValueOut;     

   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   SoConditionalTrigger();

 private:

   /// we save the last state here to fire "events" only once
   bool triggerEvaluated;
   bool triggerNotified;
   bool tempBool;

   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoConditionalTrigger();

   virtual void inputChanged(SoField * whichField);
   virtual void evaluate();

   bool compareFloat(const float val);
   bool compareInt(const int val);
   bool compareTime(const double val);
};

#endif // _SOCONDITIONALTRIGGER_H_
