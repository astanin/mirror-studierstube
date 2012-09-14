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
/* SoVariable.cxx contains the implementation of SoVariable
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoVariable.cpp 8 2004-07-14 12:24:04Z tamer $
 * @file
 */
/* ======================================================================== */

#include <ctype.h>

#include <Inventor/errors/SoDebugError.h>

#include <stb/components/starlight/SoVariable.h>
#include <stb/base/OS.h>

#ifndef STB_IS_WINDOWS
#  define _strdup strdup
#endif


SO_NODE_SOURCE(SoVariable);

void
SoVariable::initClass()
{
    SO_NODE_INIT_CLASS(SoVariable, SoNode, "Node");
}

SoVariable::SoVariable()
{
    SO_NODE_CONSTRUCTOR(SoVariable);

    // public input fields
    SO_NODE_ADD_FIELD(name, (""));
    SO_NODE_ADD_FIELD(type, ("STRING"));
    SO_NODE_ADD_FIELD(a, (""));
    SO_NODE_ADD_FIELD(b, (""));
    SO_NODE_ADD_FIELD(c, (""));
    SO_NODE_ADD_FIELD(d, (""));
    SO_NODE_ADD_FIELD(expression, (""));
    SO_NODE_ADD_FIELD(eval, ());

    // private, internal fields
    SO_NODE_ADD_FIELD(resultValue_a, (""));
    SO_NODE_ADD_FIELD(resultValue_b, (""));
    SO_NODE_ADD_FIELD(resultValue_c, (""));
    SO_NODE_ADD_FIELD(resultValue_d, (""));
    SO_NODE_ADD_FIELD(expressionValue, (""));

    // output fields
    SO_NODE_ADD_FIELD(oa, (""));
    SO_NODE_ADD_FIELD(ob, (""));
    SO_NODE_ADD_FIELD(oc, (""));
    SO_NODE_ADD_FIELD(od, (""));

    calculator=NULL;
    strCalculator=NULL;
    maxNum=0;

    // field sensors
    input_a_Sensor=new SoFieldSensor(inputChanged,this);
    input_b_Sensor=new SoFieldSensor(inputChanged,this);
    input_c_Sensor=new SoFieldSensor(inputChanged,this);
    input_d_Sensor=new SoFieldSensor(inputChanged,this);
    input_type_Sensor=new SoFieldSensor(inputChanged,this);
    input_expression_Sensor=new SoFieldSensor(inputChanged,this);
    input_eval_Sensor=new SoFieldSensor(inputChanged,this);

    input_a_Sensor->attach(&a);
    input_b_Sensor->attach(&b);
    input_c_Sensor->attach(&c);
    input_d_Sensor->attach(&d);
    input_type_Sensor->attach(&type);
    input_expression_Sensor->attach(&expression);
    input_eval_Sensor->attach(&eval);
}

SoVariable::~SoVariable()
{
}

void SoVariable::inputChanged(void *data,SoSensor *sensor)
{
    SoVariable *variable=(SoVariable*)data;
    
    int i;
    if (sensor==variable->input_expression_Sensor) {
        // reset flags indicating the input variables' presence in expressions
        for (i=0;i<4;variable->inUsed[i++]=false);
        for (i=0;i<variable->expression.getNum();i++){
            // duplicates string
            char *str=_strdup(variable->expression[i].getString());
            variable->expressionValue.set1Value(i,variable->formatExpression(str));
        }
    }
    else if (sensor==variable->input_type_Sensor) {
        // const char *typeString=variable->type.getValue().getString();

        // set up calculator field connections
        if (variable->isType("STRING")) {
            if (variable->strCalculator==NULL) variable->strCalculator=new SoStringCalculator;

            variable->strCalculator->a.connectFrom(&variable->a);
            variable->strCalculator->b.connectFrom(&variable->b);
            variable->strCalculator->c.connectFrom(&variable->c);
            variable->strCalculator->d.connectFrom(&variable->d);
            variable->strCalculator->e.connectFrom(&variable->oa);
            variable->strCalculator->f.connectFrom(&variable->ob);
            variable->strCalculator->g.connectFrom(&variable->oc);
            variable->strCalculator->h.connectFrom(&variable->od);
            variable->strCalculator->expression.connectFrom(&variable->expressionValue);
            variable->resultValue_a.connectFrom(&variable->strCalculator->oa);
            variable->resultValue_b.connectFrom(&variable->strCalculator->ob);
            variable->resultValue_c.connectFrom(&variable->strCalculator->oc);
            variable->resultValue_d.connectFrom(&variable->strCalculator->od);
        }
        else if (variable->isType("FLOAT") || variable->isType("INT")){
            if (variable->calculator==NULL) variable->calculator=new SoCalculator;
            
            variable->calculator->a.connectFrom(&variable->a);
            variable->calculator->b.connectFrom(&variable->b);
            variable->calculator->c.connectFrom(&variable->c);
            variable->calculator->d.connectFrom(&variable->d);
            variable->calculator->e.connectFrom(&variable->oa);
            variable->calculator->f.connectFrom(&variable->ob);
            variable->calculator->g.connectFrom(&variable->oc);
            variable->calculator->h.connectFrom(&variable->od);
            variable->calculator->expression.connectFrom(&variable->expressionValue);
            variable->resultValue_a.connectFrom(&variable->calculator->oa);
            variable->resultValue_b.connectFrom(&variable->calculator->ob);
            variable->resultValue_c.connectFrom(&variable->calculator->oc);
            variable->resultValue_d.connectFrom(&variable->calculator->od);
        }
        else if (variable->isType("BOOL")){
            if (variable->calculator==NULL) variable->calculator=new SoCalculator;

            variable->calculator->a.disconnect();
            variable->calculator->b.disconnect();
            variable->calculator->c.disconnect();
            variable->calculator->d.disconnect();
            variable->calculator->e.disconnect();
            variable->calculator->f.disconnect();
            variable->calculator->g.disconnect();
            variable->calculator->h.disconnect();
            variable->calculator->expression.connectFrom(&variable->expressionValue);
            variable->resultValue_a.connectFrom(&variable->calculator->oa);
            variable->resultValue_b.connectFrom(&variable->calculator->ob);
            variable->resultValue_c.connectFrom(&variable->calculator->oc);
            variable->resultValue_d.connectFrom(&variable->calculator->od);
        }
        else if (variable->isType("VEC3F")) {
            if (variable->calculator==NULL) variable->calculator=new SoCalculator;
            
            variable->calculator->A.connectFrom(&variable->a);
            variable->calculator->B.connectFrom(&variable->b);
            variable->calculator->C.connectFrom(&variable->c);
            variable->calculator->D.connectFrom(&variable->d);
            variable->calculator->E.connectFrom(&variable->oa);
            variable->calculator->F.connectFrom(&variable->ob);
            variable->calculator->G.connectFrom(&variable->oc);
            variable->calculator->H.connectFrom(&variable->od);
            variable->calculator->expression.connectFrom(&variable->expressionValue);
            variable->resultValue_a.connectFrom(&variable->calculator->oA);
            variable->resultValue_b.connectFrom(&variable->calculator->oB);
            variable->resultValue_c.connectFrom(&variable->calculator->oC);
            variable->resultValue_d.connectFrom(&variable->calculator->oD);
        }
        //else if (variable->isType(SbString("VEC4F"))  {}
        else {
			SoDebugError::post("SoVariable::inputChanged()",
				"unknow variable type %s",variable->type.getValue().getString());
        }
    }
    // it's one of the input fields a/b/c/d or eval
    else {
        variable->maxNum=0;
        char fieldname[3];
        for (i=0;i<8;i++){
            if (i<4){
                fieldname[1]=0;
                if (variable->inUsed[i]) {
                    fieldname[0]='a'+ i;
                    SoMField *field=(SoMField*)variable->getField(fieldname);
                    if (field->getNum()>variable->maxNum) variable->maxNum=field->getNum();
                }
            }
            else {
                fieldname[0]='o';
                fieldname[1]='a'+i-4;
                fieldname[2]=0;
                SoMFString *field=(SoMFString*)variable->getField(fieldname);
                if (field->getNum()>=variable->maxNum) variable->maxNum=field->getNum();
            }
        }

        // evaluate
        if (sensor==variable->input_eval_Sensor) {
            if (variable->isType("BOOL")){
                variable->convertBooleanField(&variable->a,0);
                variable->convertBooleanField(&variable->b,1);
                variable->convertBooleanField(&variable->c,2);
                variable->convertBooleanField(&variable->d,3);
            }
            variable->evaluate();
        }
    }
}

char* SoVariable::formatExpression(char *expr)
{
    std::string result;
    bool found_oa_ta=false,found_a=false,after_variable_noalpha=false;
    bool first_output_parsed=false;

    // converts all the alpha characters of the expression to lowercase
    for (size_t i=0; i<strlen(expr); i++) { expr[i]=tolower(expr[i]); }

    // first_output_parsed = have we parsed the left side of the expression yet? (e.g. the variable "oa" from expression "oa=a+b-c")
    first_output_parsed=false;
    for (unsigned int p=0;p<strlen(expr);p++){
        bool wrapAround=false;
        // look for the first occurence of characters [a,b,c,d] to parse variables a,b,c,d,oa,ob,oc,od,ta,tb,tc,td
        if ((expr[p]>='a' && expr[p]<='d')){
            // check preceding characters for output and temp variables
            found_oa_ta=
                // there is at least one character before
                (p>0) &&
                // there is a 't' or 'o' before
                (
                    (expr[p-1]=='t' || expr[p-1]=='o') &&
                    // before the 't' or 'o' there is no alpha character or no character at all
                    (
                        p==1 ||
                        (
                            p>1 && 
                            (expr[p-2]<'a' || expr[p-2]>'z')
                        )
                    )
                );
            // checking individual
            found_a=expr[p-1]<'a' || expr[p-1]>'z';

            // check succeeding character: if it's not alphanumeric or end of string, we found a real variable, 
            // we can convert it to uppercase if it's necessary (i.e. it's a VEC3F and the internal SoCalculator needs it)
            after_variable_noalpha=
                (size_t)(p+1)==strlen(expr) ||
                (
                    (size_t)(p+1)<strlen(expr) && 
                    (expr[p+1]<'a' || expr[p+1]>'z') 
                );

            found_oa_ta&=after_variable_noalpha;
            found_a&=after_variable_noalpha;

            // if vector variable, convert [a|b|c|d] variables to [A|B|C|D] and 
            // [o|t][a|b|c|d] variables to [o|t][A|B|C|D]
            if ((found_oa_ta || found_a) && isType("VEC3F")){
                expr[p]+='A'-'a';
            }
            // if the first output variable (the left side in the expression) has already been parsed (e.g. "oa" 
            // in expr. "oa=a+b-c"), replace o[a|b|c|d] by [e|f|g|h] for feeding it back to itself as inputs
            if (found_oa_ta && expr[p-1]=='o'){
                if (first_output_parsed){
                    expr[p-1]=' ';
                    expr[p]+='e'-'a';
                    result=result.substr(0,result.length()-1);
                    if (isType("BOOL")) wrapAround=true;
                }
                else first_output_parsed=true;
            }
            if (found_a) {
                inUsed[tolower(expr[p])-'a']=true;
                if (isType("BOOL")) wrapAround=true;
            }
        }
        if (wrapAround){
            result+="(";
            result+=expr[p];
            result+="==1)";
        }
        else result+=expr[p]; 
    }
    if (isType("BOOL")) result+=" ? 1 : 0";

    return _strdup(result.c_str());
}

void SoVariable::convertBooleanField(SoMField *field,unsigned short num)
{
    if (calculator==NULL) return;

    SoMFString *varInputField=(SoMFString*)field;
    SoMFFloat *calcInputField = NULL;
    if (num<8){
        char fieldname[2];
        fieldname[0]='a'+num;
        fieldname[1]=0;
        calcInputField=(SoMFFloat*)calculator->getField(fieldname);
    }
    
    SbString value;
    for (int i=0;i<varInputField->getNum();i++){
        value=(*varInputField)[i];
        if (value=="TRUE" || value=="true" || value=="1") calcInputField->set1Value(i,1);
        else calcInputField->set1Value(i,0);
    }
}

bool SoVariable::isType(char *str)
{
    char *typeStr=_strdup(type.getValue().getString());
    for (unsigned int i=0;i<strlen(typeStr);i++){
        typeStr[i]=toupper(typeStr[i]);
    }
    return !strcmp(typeStr,str);
}

void SoVariable::evaluate()
{
    int i;
    float floatValue;

    for (i=0;i<maxNum;i++){
        if (isType("STRING") || isType("FLOAT") || isType("VEC3F")){
            oa.set1Value(i,i<resultValue_a.getNum() ? resultValue_a[i] : "");
            ob.set1Value(i,i<resultValue_b.getNum() ? resultValue_b[i] : "");
            oc.set1Value(i,i<resultValue_c.getNum() ? resultValue_c[i] : "");
            od.set1Value(i,i<resultValue_d.getNum() ? resultValue_d[i] : "");
        }
        else if (isType("INT")){
            if (i<resultValue_a.getNum() && sscanf(resultValue_a[i].getString(),"%f",&floatValue)>0){
                oa.set1Value(i,(int)floatValue);
            }
            else oa.set1Value(i,0);
            if (i<resultValue_b.getNum() && sscanf(resultValue_b[i].getString(),"%f",&floatValue)>0){
                ob.set1Value(i,(int)floatValue);
            }
            else ob.set1Value(i,0);
            if (i<resultValue_c.getNum() && sscanf(resultValue_c[i].getString(),"%f",&floatValue)>0){
                oc.set1Value(i,(int)floatValue);
            }
            else oc.set1Value(i,0);
            if (i<resultValue_d.getNum() && sscanf(resultValue_d[i].getString(),"%f",&floatValue)>0){
                od.set1Value(i,(int)floatValue);
            }
            else od.set1Value(i,0);
        }
        else if (isType("BOOL")){
            convertBooleanField(&oa,4);
            convertBooleanField(&ob,5);
            convertBooleanField(&oc,6);
            convertBooleanField(&od,7);
            oa.set1Value(i,i<resultValue_a.getNum() ? (resultValue_a[i]=="1" ? "TRUE" : "FALSE") : "");
            ob.set1Value(i,i<resultValue_b.getNum() ? (resultValue_b[i]=="1" ? "TRUE" : "FALSE") : "");
            oc.set1Value(i,i<resultValue_c.getNum() ? (resultValue_c[i]=="1" ? "TRUE" : "FALSE") : "");
            od.set1Value(i,i<resultValue_d.getNum() ? (resultValue_d[i]=="1" ? "TRUE" : "FALSE") : "");
        }
        //else if (isType("VEC4F")){}
    }
}
