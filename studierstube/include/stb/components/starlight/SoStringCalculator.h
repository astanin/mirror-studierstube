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
/* SoStringCalculator.h contains the definition of SoStringCalculator
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoStringCalculator.h 8 2004-07-14 12:24:04Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _SOSTRINGCALCULATOR_H_
#define _SOSTRINGCALCULATOR_H_

#include <string>

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/SbPList.h>

#include "starlight.h"

/** SoStringCalculator is a calculator engine for string operations

Operations stored in field <em>expression</em> can be done on the input fields <em>a,b,c and d</em>
to yield outputs <em>oa,ob,oc and od</em>. The following operations can be used and combined in the 
expressions:

  - Concatenation: concatenates the specified strings and writes it into the given output field.
    E.g. if expression = "oa = a + b + c", a = "How", b = "are" and c = "you?" then oa yields "How are you?"

  - Substring: just like in SbString.getSubString(start,end) this function takes the substring from character #start 
    to character #end (inclusive!) and writes it into the given output field. If the 2nd index is -1, the substring
    starting with character #start to the end of the string is taken. If the 1st index is -1, the substring function
    is ignored and the whole string is taken. If any of the indices is higher than or equal to the length of the
    string, an empty string is taken.

    E.g. if expression = "ob = c(1,4) + d(5,-1)", c = "abcdefgh" and d = "ABCDEFGH" then ob yields "bcdeFGH".

    WARNING: the usage of this substring function differs from the std::string.substr(start,length) syntax as the 2nd
    parameter specified the number of characters to copy after character #start and here in out function the end index
    of the character is specified.

Just like in SoCalculator, temporary variables can be used and the already calculated output values can
be reused in another calculation expression, for instance

    if expression = ["ta = a(1,2) + b(2,6)", "oa = ta + ta(1,6)", "oa = oa(1,-1)"]
    and a = "Test", b = "Example"
    then oa yields "samplesample"

*** TODO:
Currently there is NO support for brackets to group expressions like "oa = (a+b(3,4))(4,7)".
You need to use the temporary variables instead and create multiple strings inside the expression
field value e.g. "ta = a+b(3,4)", "oa = ta (4,7)"

@author Istvan Barakonyi
@ingroup util
*/
class STARLIGHT_API SoStringCalculator : public SoEngine
{
    SO_ENGINE_HEADER(SoStringCalculator);

public:

    /** @name Input fields
     */
    /*@{*/

    SoMFString a;
    SoMFString b;
    SoMFString c;
    SoMFString d;
    SoMFString e;
    SoMFString f;
    SoMFString g;
    SoMFString h;

    /*@}*/

    /// Expression field
    SoMFString expression;

    /** @name Output fields
     */
    /*@{*/

    SoEngineOutput oa;
    SoEngineOutput ob;
    SoEngineOutput oc;
    SoEngineOutput od;

    /*@}*/
    
    /// constructor initializing all the fields
    SoStringCalculator();

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:
    
    /// destructor - does nothing
    virtual ~SoStringCalculator();

    /// The number of input value fields. The number of temporary variables also equals to this number.
    enum { INPUT_NUM = 8 };

    /// The number of output fields
    enum { OUTPUT_NUM = 4 };

    /** Auxiliary variables for the internal calculation structure, which is the result of
        the parsing of the expression strings.
      */
    SbPList inputVarList[OUTPUT_NUM+INPUT_NUM];
    SbIntList outputVarList;
    SbPList indexList0[OUTPUT_NUM+INPUT_NUM];
    SbPList indexList1[OUTPUT_NUM+INPUT_NUM];
    SoMFString value[OUTPUT_NUM+INPUT_NUM];
    int occurence[OUTPUT_NUM+INPUT_NUM];
    int maxFieldLength;
    /// flags indicating the input variables' presence in expressions
    bool inUsed[INPUT_NUM];

    /// gets triggered if any of the input fields changes
    virtual void inputChanged(SoField *whichField);

    /// evaluates the engine's output
    virtual void evaluate();

    /// clears the internal calculation structure i.e. temp and output variable lists and substring index lists
    void clearAll();

    /// parses and checks expression syntax, build up calculation structure
    bool parseExpression(std::string exp,SbStringList *inputVar,SbIntList *index0,SbIntList *index1);
};

#endif //_SOSTRINGCALCULATOR_H_

