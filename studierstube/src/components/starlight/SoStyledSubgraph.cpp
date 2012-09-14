/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
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
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The Source file for the SoStyledSubgraph.
*
* @author Erick Mendez
*
* $Id: SoStyledSubgraph.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <stb/components/starlight/SoStyledSubgraph.h>
#include <stb/components/starlight/SoContextSeparator.h>
#include <stb/components/starlight/SoContextReport.h>
#include <stb/components/starlight/SoContextElement.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SbStringList.h>

using namespace std;

#pragma warning(disable:4996)

SO_KIT_SOURCE(SoStyledSubgraph);

/*
* --------------------------------------------------------------------------------
* Required by the node kit
* --------------------------------------------------------------------------------
*/
void SoStyledSubgraph::initClass(void)
{
	SO_KIT_INIT_CLASS(SoStyledSubgraph, SoBaseKit, "BaseKit");
}

/*
* --------------------------------------------------------------------------------
* Constructor and Destructor
* --------------------------------------------------------------------------------
*/
SoStyledSubgraph::SoStyledSubgraph()
{
	SO_KIT_CONSTRUCTOR(SoStyledSubgraph);
    SO_KIT_ADD_CATALOG_ENTRY(sepTop,				SoContextSeparator,     TRUE, this,	  "", FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(switchReport,	        SoContextReport,        TRUE, sepTop, "", TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(renderStyle,	        SoNodeContextReport,    TRUE, sepTop, "", TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(content,				SoNode,                 TRUE, sepTop, "", TRUE );

	SO_KIT_INIT_INSTANCE();

    // Set defaults for Context-Criteria
    //SO_GET_ANY_PART(this, "switchReport", SoContextReport)->
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(0,"__TemplateName");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(0,"__void");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(1,"__Style_0");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(1,"");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(2,"__Style_1");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(2,"");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(3,"__Style_2");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(3,"");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(4,"__Style_3");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(4,"");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(5,"__Style_4");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(5,"");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(6,"__Style_5");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(6,"");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(7,"__Style_6");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(7,"");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(8,"__Style_7");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(8,"");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->index.set1Value(9,"__Style_8");
    SO_GET_ANY_PART(this, "switchReport", SoContextReport)->defaultValue.set1Value(9,"");

	this->setUpConnections(TRUE, TRUE);
}

SoStyledSubgraph::~SoStyledSubgraph()
{
}

bool SoStyledSubgraph::isMappedTo(char * stylename, SoAction *action)
{
    int a;
    SbString keysandvalues;
    SbString compareKey, compareValue, compareTail, compareHead;
    char *stylestring=new char[255];
    char *mapkey=new char[255];

    // First, get the mapping key/values for this style
    sprintf(stylestring, "__Map_%s",stylename);
    keysandvalues=(SoContextElement::get(action->getState(), SbName(stylestring)));

    if (keysandvalues.getLength()>3)
    {

        // Wildcard
        if (keysandvalues.getString()=="*") return true;

        // Now check every key/value
        do
        {
            a=keysandvalues.find(";");
            if(a==-1)
            {
                compareHead=keysandvalues;
                compareTail="";
            }
            else
            {
                compareHead=keysandvalues.getSubString(0,a-1);
                compareTail=keysandvalues.getSubString(a+1);
            }
            a=compareHead.find("=");
            compareKey=compareHead.getSubString(0,a-1);
            compareValue=compareHead.getSubString(a+1);
            keysandvalues=compareTail;

            sprintf(mapkey, "__State_%s",compareKey.getString());
            if (compareValue!=(SoContextElement::get(action->getState(), SbName(mapkey))))
                return false;

        } while (compareTail.getLength()!=0);

        return true;
    }
    else return false;
}

void SoStyledSubgraph::refreshStyle(bool fromConnections, SoAction *action)
{
	char *strStyleName=new char[255];
    char *inputString=new char[255];
    bool flag=FALSE;

   if (SO_GET_ANY_PART(this, "switchReport", SoContextReport)->value.getNum()>0)
   {
        char *strTemplateName=new char[255];
        char *tmpString2=new char[255];
        int nb, i;

        strTemplateName=const_cast <char *>(SO_GET_ANY_PART(this, "switchReport", SoContextReport)->value[0].getString());

        // Check if there is a function defined
        if (strcmp(strTemplateName, "__void"))
        {
            // Just in case nothing is found
            sprintf(strStyleName, "Empty");

            nb=3;
            // Check if we are one of the mappings
            for (i=0;i<nb;i++)
            {
                tmpString2=const_cast <char *>(SO_GET_ANY_PART(this, "switchReport", SoContextReport)->value[i+1].getString());
                if (strlen(tmpString2)<=0) continue;
                if (isMappedTo(tmpString2, action))
                {
                    sprintf(strStyleName, "__Template_%s_%s", strTemplateName, tmpString2);
                    break;
                }
            }

            SO_GET_ANY_PART(this, "renderStyle", SoNodeContextReport)->index.setValue(strStyleName);
            SO_GET_ANY_PART(this, "renderStyle", SoNodeContextReport)->report=fromConnections;
        }
    }
}

void SoStyledSubgraph::GLRender(SoGLRenderAction * action)
{
    refreshStyle(FALSE, action);

	SoBaseKit::GLRender(action);
}
