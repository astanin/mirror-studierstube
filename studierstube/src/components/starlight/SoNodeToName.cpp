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
/** Implementation of SoNodeToName engine
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoNodeToName.cpp 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/fields/SoMFString.h>
#include <Inventor/nodes/SoNode.h>

#include <stb/components/starlight/SoNodeToName.h>

SO_ENGINE_SOURCE(SoNodeToName);

void SoNodeToName::initClass() 
{
	SO_ENGINE_INIT_CLASS(SoNodeToName, SoEngine, "Engine");
}

SoNodeToName::SoNodeToName() 
{
	SO_ENGINE_CONSTRUCTOR(SoNodeToName);

	// define input fields and their default values
	SO_ENGINE_ADD_INPUT(input, (NULL));
	SO_ENGINE_ADD_INPUT(compact,(TRUE));

	// define the output
	SO_ENGINE_ADD_OUTPUT(output, SoMFString);
}

SoNodeToName:: ~SoNodeToName()
{
	// NIL
}

void SoNodeToName::evaluate()
{   
    // make room as necessary
    SO_ENGINE_OUTPUT(output, SoMFString, setNum(input.getNum()));
    int next = 0;
    for( int i = 0; i < input.getNum(); i++ )
    {
        SoNode * node = input[i];
        if( node == NULL || node->getName() == "" )
        {
            if(  compact.getValue() == FALSE )
            {
                SO_ENGINE_OUTPUT(output, SoMFString, set1Value(next, ""));
                next++;
            }
        }
        else
        {
            SO_ENGINE_OUTPUT(output, SoMFString, set1Value(next, node->getName().getString()));
            next++;
        }        
    }
    // truncate as necessary
    SO_ENGINE_OUTPUT(output, SoMFString, setNum(next));
}
