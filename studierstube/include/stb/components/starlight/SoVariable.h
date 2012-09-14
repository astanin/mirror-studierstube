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
/* SoVariable.h contains the definition of SoVariable
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoVariable.h 8 2004-07-14 12:24:04Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _SOVARIABLE_H_
#define _SOVARIABLE_H_

#include <string>

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include "starlight.h"
#include "SoStringCalculator.h"

/** The SoVariable node implements a generalized calculator engine which behaves like 
a typed variable with a number of possible operations.

- The field <em>type</em> specifies the type of the variable, which can be one of the following
strings: "FLOAT" (float variable), "INT" (integer variable), "BOOL" (Boolean variable), "VEC3F" 
(3D vector variable) or "STRING" (for a string variable). It does not matter whether the type 
string is given in upper- or lowercase

- The fields <em>a, b, c and d</em> store strings containing the textual representation of string, 
float, integer or 3D vector values respectively. or can be connected to other fields.
If the connected fields do not store strings but - in case of number types - number values, 
they get automatically converted to strings by Inventor.

- The field <em>expression</em> holds the expression with the operations to be carried out 
on the input values. It does not matter whether it is in lower- or uppercase since all 
the expressions get converted to the appropriate case for the internal calculation 
engines privately stored internally in the node.

- The fields <em>oa, ob, oc and od</em> are the output fields holding again string values, 
which - in case of field connections - get automatically converted to the appropriate type 
if necessary. The initial value of input fields are "0" (INT and FLOAT), "FALSE" (BOOL), "0 0 0" (VEC3F) 
and "" (STRING), "" for the expression field. The output field default values are the same 
as the input fields with the respective type.

- If the <em>eval</em> trigger is fired, the expression gets evaluated. Unlike in the SoCalculator engine, 
the expression may contain output fields, which feeds them back and lets the user use their value in the 
calculations. For example "oa = a+b+oa" with a=5 and b=4 will yield 9; if <em>18</em> gets triggered once, 
yields 18; if twice, yields 36 etc. The expression "oa=oa+1" implements a simple way to 
increment a number value by again triggering <em>eval</em>.

Depending on its type various operations can be done on a variable:

    - FLOAT, INT, VEC3F: the class behaves like an SoCalculator engine, the same functions and
    operators can be used. For further details see the description of the SoCalculator class in 
    the Inventor documentation. You don't have to use uppercase for the variables when you use
    3D vectors (unlike in the SoCalculator engine, where you have to use A,B,... and oA,oB,..

    - STRING: the class behaves like an SoStringCalculator engine, the same functions and
    operators can be used. For further details see the description of the SoCalculator class in 
    the Studierstube documentation (in the stbapi/misc/ directory)

    - BOOL: it is possible to use Boolean operations in the SoCalculator engine, therefore the same
    operations (!,||,&&) can be used here as well. The result is always "TRUE" or "FALSE", not "1" or "0".
    Remark: senselessly using other, non-Boolean functions from SoCalculator (like sin(x), tan(x), 
    multiplication with 20 etc.) or other tricky ways won't crash the code but will always yield "FALSE" 
    unless the result is exactly 1.0.

@author Istvan Barakonyi
@ingroup util
*/
class STARLIGHT_API SoVariable : public SoNode {

    SO_NODE_HEADER(SoVariable);

public:

    /// Name of the variable
    SoSFString name;

    /// Type of the variable
    SoSFString type;

    /** @name Input value fields
     */
    /*@{*/

    SoMFString a;
    SoMFString b;
    SoMFString c;
    SoMFString d;

    /*@}*/

    /// Expression field (MULTIPLE string)
    SoMFString expression;

    /// Evaluation trigger
    SoSFTrigger eval;

    /** @name Internal, "read-only" fields, the user should NOT use them
     */
    /*@{*/

    SoMFString resultValue_a;
    SoMFString resultValue_b;
    SoMFString resultValue_c;
    SoMFString resultValue_d;
    SoMFString expressionValue;

    /*@}*/

    /** @name Output fields
     */
    /*@{*/

    SoMFString oa;
    SoMFString ob;
    SoMFString oc;
    SoMFString od;

    /*@}*/
    
    /// Inventor class initialization
    static void initClass();

    /// Constructor initializing the fields
    SoVariable();

protected:
    
    /// Flags indicating the input variables' presence in expressions
    bool inUsed[4];

    /// Maximum number of multiple field array size
    int maxNum;

    /// Destructor - does nothing
    virtual ~SoVariable();

    /// Field sensors
    SoFieldSensor *input_a_Sensor;
    SoFieldSensor *input_b_Sensor;
    SoFieldSensor *input_c_Sensor;
    SoFieldSensor *input_d_Sensor;
    SoFieldSensor *input_type_Sensor;
    SoFieldSensor *input_expression_Sensor;
    SoFieldSensor *input_eval_Sensor;

    /// Internal SoCalculator engine for numerical values (FLOAT, INT, VEC3F, BOOL)
    SoCalculator *calculator;

    /// Internal SoStringCalculator engine for string values (STRING)
    SoStringCalculator *strCalculator;

    /// Parses and checks expression syntax, reformats if necessary
    char* formatExpression(char *expr);

    /// Converts a Boolean field to an internal integer representation suitable for SoCalculator
    void convertBooleanField(SoMField *field,unsigned short num);

    /// Compares the parameter string with the built-in variable type strings
    bool isType(char *str);

    /// Gets triggered if any of the input fields changes
    static void inputChanged(void *data,SoSensor *sensor);

    /// Evaluates the output fields
    void evaluate();
};

#endif // _SOVARIABLE_H_
