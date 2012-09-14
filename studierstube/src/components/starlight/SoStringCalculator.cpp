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
/* SoStringCalculator.cxx contains the implementation of SoStringCalculator
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoStringCalculator.cpp 8 2004-07-14 12:24:04Z tamer $
 * @file
 */
/* ======================================================================== */

#include <string>

#include <Inventor/errors/SoDebugError.h>


#include <stb/components/starlight/SoStringCalculator.h>


SO_ENGINE_SOURCE(SoStringCalculator);

SoStringCalculator::SoStringCalculator()
{
    SO_ENGINE_CONSTRUCTOR(SoStringCalculator);

    SO_ENGINE_ADD_INPUT(a, (""));
    SO_ENGINE_ADD_INPUT(b, (""));
    SO_ENGINE_ADD_INPUT(c, (""));
    SO_ENGINE_ADD_INPUT(d, (""));
    SO_ENGINE_ADD_INPUT(e, (""));
    SO_ENGINE_ADD_INPUT(f, (""));
    SO_ENGINE_ADD_INPUT(g, (""));
    SO_ENGINE_ADD_INPUT(h, (""));
    SO_ENGINE_ADD_INPUT(expression, (""));
    
    SO_ENGINE_ADD_OUTPUT(oa, SoMFString);
    SO_ENGINE_ADD_OUTPUT(ob, SoMFString);
    SO_ENGINE_ADD_OUTPUT(oc, SoMFString);
    SO_ENGINE_ADD_OUTPUT(od, SoMFString);

    maxFieldLength=0;
}

SoStringCalculator::~SoStringCalculator()
{
}

void SoStringCalculator::initClass()
{
    SO_ENGINE_INIT_CLASS(SoStringCalculator, SoEngine, "Engine");
}

void SoStringCalculator::inputChanged(SoField *whichField)
{
    if (whichField==&expression){
        std::string exp;
        int index;
        int i;
	std::string::size_type marker1=0,marker2=0;
        bool isOutputVar=false;
        bool isTempVar=false;
        SbStringList *inputVar;
        SbIntList *index0;
        SbIntList *index1;

        clearAll();

        // go through all the expression strings of the 'expression' SoMFString field value
        for (i=0;i<expression.getNum();i++){
            // check whether expression is not empty
            if (expression[i]=="") break;
            exp=expression[i].getString();
            if ((marker1=exp.find_first_not_of(" "))==exp.npos){
				SoDebugError::postWarning("SoStringCalculator::inputChanged()",
						"parse error: empty expression");
                return;
            }
            // check whether the left hand side is an output variable or a temporary variable
            isOutputVar=exp[marker1]=='o' && exp[marker1+1]>='a' && exp[marker1+1]<=('a'+OUTPUT_NUM-1);
            isTempVar=exp[marker1]=='t' && exp[marker1+1]>='a' && exp[marker1+1]<=('a'+INPUT_NUM-1);
            if (!isOutputVar && !isTempVar) {
				SoDebugError::post("SoStringCalculator::inputChanged()",
						"parse error: output or temp variable not found");
                return;
            }

            /* get the appropriate index in the variable array and add it to the output variable 
               list to store the calculation order */
            index=exp[marker1+1]-'a'+(isTempVar ? OUTPUT_NUM : 0);
            outputVarList.append(index);

            // setup structure for parsing the new expression string
            inputVar=new SbStringList;
            index0=new SbIntList;
            index1=new SbIntList;            

            // parse expression
            if ((marker1=exp.find("=",marker1+2))==exp.npos &&
                (marker1=exp.find_first_not_of(" ",marker1+1))==exp.npos) {
				SoDebugError::post("SoStringCalculator::inputChanged()",
						"parse error: missing right hand side");
                return;
            }
            do {
                // parse subexpressions --> strings separated by '+' marks
                marker2=exp.find("+",marker1+1);
                if ((marker1=exp.find_first_not_of(" ",marker1+1))==marker2){
					SoDebugError::post("SoStringCalculator::inputChanged()",
						"parse error: syntax error");
                    return;
                }
                if (marker2==exp.npos) marker2=exp.length();

                // if an error occured while parsing, remove all the results coming from this incorrect expression
                if (!parseExpression(exp.substr(marker1,marker2-marker1),inputVar,index0,index1)) {
                    clearAll();
                    return;
                }
                marker1=marker2;
            } while (marker2!=exp.length());

            // add parsed variable list and substring index lists
            inputVarList[index].append(inputVar);
            indexList0[index].append(index0);
            indexList1[index].append(index1);
        } // end of for
    }
}

void SoStringCalculator::evaluate()
{
    char outputName[3]="oa";
    char varName[3];
    char inputName[2];
    int i,j,k;

    // determine the maximum multiple field variable length
    maxFieldLength=0;
    inputName[1]=0;
    for (i=0;i<INPUT_NUM;i++){
        inputName[0]='a'+i;
        SoMField *field = (SoMField*)this->getField(inputName);
        if (inUsed[i] && field->getNum()>maxFieldLength) maxFieldLength=field->getNum();
    }

    for (i=0;i<OUTPUT_NUM+INPUT_NUM;i++){
        occurence[i]=0;
        for (j=0;j<maxFieldLength;j++) value[i].set1Value(j,"");
    }

    for (i=0;i<outputVarList.getLength();i++){

        SoMFString result; result.setNum(maxFieldLength);
        SbStringList *varList=(SbStringList*)(inputVarList[outputVarList[i]][occurence[outputVarList[i]]]);
        SbIntList *index0=(SbIntList*)(indexList0[outputVarList[i]][occurence[outputVarList[i]]]);
        SbIntList *index1=(SbIntList*)(indexList1[outputVarList[i]][occurence[outputVarList[i]]]);

        for (j=0;j<varList->getLength();j++){
            strcpy(varName,(*varList)[j]->getSubString(0,1).getString());
            if (varName[0]=='o' || varName[0]=='t'){
                int ix=varName[1]-'a'+(varName[0]=='t' ? OUTPUT_NUM : 0);
                for (k=0;k<maxFieldLength;k++) {
                    SbString temp=result[k];
                    if ((*index0)[j]>=0) {
                        if ((*index0)[j]<value[ix][k].getLength() && (*index1)[j]<value[ix][k].getLength()){
                            temp+=value[ix][k].getSubString((*index0)[j],(*index1)[j]);
                        }
                        //else printf("STRINGCALCULATOR: warning: index overflow on input %s\n",varName);
                    }
                    else temp+=value[ix][k];
                    result.set1Value(k,temp);
                }
            }
            else {
                SoMFString *field=(SoMFString*)this->getField((*varList)[j]->getString());
                for (k=0;k<maxFieldLength;k++) {
                    SbString temp=result[k];
                    if (field->getNum()<=k) field->set1Value(k,"");
                    if ((*index0)[j]>=0) {
                        if ((*index0)[j]<((*field)[k]).getLength() && (*index1)[j]<((*field)[k]).getLength()){
                            temp+=((*field)[k]).getSubString((*index0)[j],(*index1)[j]);
                        }
                        //else printf("STRINGCALCULATOR: warning: index overflow on input %s\n",varName);
                    }
                    else temp+=((*field)[k]);
                    result.set1Value(k,temp);
                }
            }
        }
        for (j=0;j<maxFieldLength;j++) value[outputVarList[i]].set1Value(j,result[j]);

        occurence[outputVarList[i]]++;
    }

    for (i=0;i<OUTPUT_NUM;i++){
        outputName[1]='a'+i;
        SoEngineOutput *output=this->getOutput(outputName);
        for (int j=0;j<maxFieldLength;j++){
            SO_ENGINE_OUTPUT((*output), SoMFString, set1Value(j,value[i][j]));
        }
    }
}

bool SoStringCalculator::parseExpression(std::string exp,SbStringList *inputVar,SbIntList *index0,SbIntList *index1)
{
    std::string::size_type marker0=0,marker1=0;
    int id0=-1,id1=-1;
    SbString *str=NULL;
    
    bool twoLetterVar=
        (exp.length()>1) && (exp[0]=='o' || exp[0]=='t') && 
        (exp[1]>='a' && exp[1]<=('a'+OUTPUT_NUM-1));
    bool oneLetterVar=!twoLetterVar && exp[0]>='a' && exp[0]<=('a'+INPUT_NUM-1);

    if (!oneLetterVar && !twoLetterVar){
		SoDebugError::post("SoStringCalculator::parseExpression()",
			"incorrect variable name: %c",exp[0]);
        return false;
    }

    // extracting single variable name
    str=new SbString(exp.substr(0,oneLetterVar ? 1 : 2).c_str());
    inputVar->append(str);

    if (oneLetterVar) inUsed[exp[0]-'a']=true;

    // searching for substring expression "varName(indexList0,indexList1)" 
    if ((marker0=exp.find_first_not_of(" ",oneLetterVar ? 1 : 2))!=exp.npos){
        if (exp[marker0]!='(') {
			SoDebugError::post("SoStringCalculator::parseExpression()",
				"syntax error: %s",exp.c_str());
            return false;
        }
        // number found
        if ((marker0=exp.find_first_not_of(" ",marker0+1))!=exp.npos &&
            (marker1=exp.find_first_of(",",marker0+1))!=exp.npos){
            sscanf(exp.substr(marker0,marker1-marker0).c_str(),"%d",&id0);         
        }
        else {
			SoDebugError::post("SoStringCalculator::parseExpression()",
				"error parsing 1st substring index: %s",exp.c_str());
            return false;
        }
        if ((marker0=exp.find_first_not_of(" ",marker1+1))!=exp.npos &&
            (marker1=exp.find_first_of(")",marker0+1))!= exp.npos){
            sscanf(exp.substr(marker0,marker1-marker0).c_str(),"%d",&id1);
        }
        else {
			SoDebugError::post("SoStringCalculator::parseExpression()",
				"error parsing 2nd substring index: %s",exp.c_str());
            return false;
        }
    }

    index0->append(id0);
    index1->append(id1);

    return true;
}

// clear temp and output variable lists from the previous expression
void SoStringCalculator::clearAll()
{   
    int i;
    for (i=0;i<OUTPUT_NUM+INPUT_NUM;i++){
        while (inputVarList[i].getLength()>0) {
            while (((SbStringList*)(inputVarList[i][0]))->getLength()>0) ((SbStringList*)(inputVarList[i][0]))->remove(0);
            inputVarList[i].remove(0);
        }
        while (indexList0[i].getLength()>0){
            while (((SbIntList*)(indexList0[i][0]))->getLength()>0) ((SbIntList*)(indexList0[i][0]))->remove(0);
            indexList0[i].remove(0);
        }
        while (indexList1[i].getLength()>0){
            while (((SbIntList*)(indexList1[i][0]))->getLength()>0) ((SbIntList*)(indexList1[i][0]))->remove(0);
            indexList1[i].remove(0);
        }
        while (outputVarList.getLength()>0) outputVarList.remove(0);
        value[i].setValue("");
        occurence[i]=0;
    }

    // reset flags indicating the input variables' presence in expressions
    for (i=0;i<INPUT_NUM;inUsed[i++]=false);
}
