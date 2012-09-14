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
/** SoRoute.cxx  - implementation of class SoRouter (version 2).
  * SoRoute is the implementation of a Node that allows to create persistent 
  * field connections at read in time.
  *
  * @author  Thomas Psik
  *
  * $Id: SoRoute.cpp 24 2005-02-10 19:38:46Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <string>
#include <cstdarg>

#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoSFNode.h>

#include <stb/components/starlight/SoRoute.h>


using namespace std;

SO_NODE_SOURCE(SoRoute);

void
SoRoute::initClass()
{
    SO_NODE_INIT_CLASS(SoRoute, SoNode, "Node");
}

SoRoute::SoRoute()
{
    SO_NODE_CONSTRUCTOR(SoRoute);

    SO_NODE_ADD_FIELD(from, (""));
    SO_NODE_ADD_FIELD(to, (""));
  
	fromField = NULL;
	fromEngineOutput = NULL;
    fromFieldContainer = NULL;
	toField = NULL;
}

SbBool SoRoute::updateFromFieldPtr(SoFieldContainer *fromNode, SbName fieldName, SoInput *in)
{
  	// from is a node 
	fromField = fromNode->getField(fieldName);
	if (fromField == NULL)
	{
		SoReadError::post(in, "SoRoute::updateFromFieldPtr could not resolve field \"%s\" in \"%s\" node",
			fieldName.getString(), fromNode->getName().getString());
		return FALSE;
	}
	return TRUE;
}

SbBool SoRoute::updateFromEngineOutput(SoEngine *fromEngine, SbName engineOutputName, SoInput *in)
{
	// from is an engine
	fromEngineOutput = fromEngine->getOutput(engineOutputName);
	if (fromEngineOutput != NULL)
	{
		return TRUE;
	} 
	else 
	{
		fromField = fromEngine->getField(engineOutputName);
		if (fromField != NULL)
		{
			return TRUE;
		}
	
	}
	SoReadError::post(in, "SoRoute::updateFromEngineOutput could not resolve EngineOutput or EngineField \"%s\" in \"%s\" engine.",
		engineOutputName.getString(), fromEngine->getName().getString());
	return FALSE;
}

SbBool SoRoute::updateFromNodeEngineOutput(SoNodeEngine *fromNodeEngine, SbName engineOutputName, SoInput *in)
{
	// from is an engine
	fromEngineOutput = fromNodeEngine->getOutput(engineOutputName);
	if (fromEngineOutput != NULL)
	{
		return TRUE;
	} 
	else 
	{
		fromField = fromNodeEngine->getField(engineOutputName);
		if (fromField != NULL)
		{
			return TRUE;
		}
	
	}
    SoReadError::post(in, "SoRoute::updateFromNodeEngineOutput could not resolve EngineOutput or EngineField \"%s\" in \"%s\" node engine.",
		engineOutputName.getString(), fromNodeEngine->getName().getString());
	return FALSE;
}

SbBool SoRoute::updateFromPtr(SoInput *in)
{
	if (from.getValue().getString() == "")
	{
		SoReadError::post(in, "SoRoute::updateFromPtr from is empty.");
		return FALSE;
	}
	
	// check from name, could also be a engine output !!
    SbName firstPart;
	SbName secondPart;
    SbBool nodeinput = false;
	string fromName =  from.getValue().getString();
    int idx = fromName.find('.');
    if (idx > -1) {
        firstPart = SbName(fromName.substr(0, idx).c_str());
	    secondPart = SbName(fromName.substr(idx+1).c_str());
    }
    else {
        firstPart = SbName(fromName.c_str());
        nodeinput = true;
    }
	
	// clean pointers 
	fromEngineOutput = NULL;
	fromField = NULL;
    fromFieldContainer = NULL;
	
	SbBool ret = TRUE;

	// at read in time use in->findReference()
	if (in != NULL) 
	{
		SoBase* fromBase = NULL;
		fromBase  = in->findReference(firstPart);
		if (fromBase  == NULL) 
		{
			SoReadError::post(in, "Unknown reference \"%s\"", firstPart.getString());
			return FALSE;
		}
		
		// handle SoEngine / SoNode difference
        if ((nodeinput) && (fromBase->isOfType(SoFieldContainer::getClassTypeId())) ) {
            fromFieldContainer = (SoFieldContainer*)fromBase;
        }

		else if (fromBase->isOfType(SoEngine::getClassTypeId()))
		{
			ret = updateFromEngineOutput((SoEngine*) fromBase, secondPart, in);
		}
        else if (fromBase->isOfType(SoNodeEngine::getClassTypeId()))
        {
            ret = updateFromNodeEngineOutput((SoNodeEngine*) fromBase, secondPart, in);
        }
		else if (fromBase->isOfType(SoFieldContainer::getClassTypeId()))
		{
			ret = updateFromFieldPtr((SoFieldContainer *)fromBase, secondPart, in);
		}
		else
		{
			SoReadError::post(in, "fromBase \"%s\"is neither an SoEngine nor a SoFieldContainer",firstPart.getString());
			ret = FALSE;
		}

		// error is explained above. 
		return ret;
	}

	// at runtime use SoNode::getByName() / SoEngine::getByName()
	SoNode *tempNode = SoNode::getByName(firstPart);
	
	if (tempNode == NULL) 
	{
		// not a node ??
		SoEngine* fromEngine = NULL;
		fromEngine = SoEngine::getByName(firstPart);
		if (fromEngine == NULL) 
		{
			SoReadError::post(in, "SoRoute::updateFromPtr could not resolve field \"%s\", because no \"%s\" node/engine was found.",
				secondPart.getString(), firstPart.getString());
			return FALSE;
		}

		ret = updateFromEngineOutput(fromEngine, secondPart, in);
	}
    else if (nodeinput) {
        fromFieldContainer = tempNode;
    }
	else {
		ret = updateFromFieldPtr(tempNode, secondPart, in);
	}
	
	//check error code	
	if (!ret)
	{
		// error is explained above. 
		//displayError(in, "");		
		return FALSE;
	}
	return TRUE;
}

SbBool SoRoute::updateToPtr(SoInput *in)
{
	string toName = to.getValue().getString();
    int idx = toName.find('.');
    
	SbName firstPart = SbName(toName.substr(0, idx).c_str());
	SbName secondPart = SbName(toName.substr(idx+1).c_str());
  
	SoFieldContainer* toContainer; 

	if (in != NULL) 
	{
		toContainer  = (SoFieldContainer*)(in->findReference(firstPart));
		if (toContainer  == NULL) 
		{
			SoReadError::post(in, "Unknown reference \"%s\"",
					  firstPart.getString());
			return FALSE;
		}
	}
	else
	{
		toContainer = SoNode::getByName(firstPart);
    
		if (toContainer == NULL) 
			toContainer = SoEngine::getByName(firstPart);
	}

	if (toContainer == NULL) 
	{
		SoReadError::post(in, "SoRoute::updateToPtr could not resolve to field  \"%s\", because no \"%s\" node/engine was found",
			secondPart.getString(), firstPart.getString());
		return FALSE;
	}

    toField = toContainer->getField(secondPart);
	if (toField == NULL)
	{
		SoReadError::post(in, "SoRoute::updateToPtr could not resolve to field \"%s\" in \"%s\" node/engine",
			secondPart.getString(),firstPart.getString());
		return FALSE;
	}
	return TRUE;
}

// Reads value(s) of node
SbBool SoRoute::readInstance(SoInput *in, unsigned short flags )
{
	SoFieldContainer::readInstance(in, flags);

	// now check if values are correct !
    SbBool valuesOK = TRUE;
	//in->
    if( !updateFromPtr(in))
	    valuesOK &= updateFromPtr(NULL);

    if( !updateToPtr(in))
	    valuesOK &= updateToPtr(NULL);

	// if either function is FALSE return with failure
	if (!valuesOK) return FALSE;

	// establish connection if route is TRUE
	if (fromField != NULL)
	{
		toField->connectFrom(fromField);
	}
	else if (fromEngineOutput != NULL)
	{
		toField->connectFrom(fromEngineOutput);
	}
    else if (toField->isOfType(SoSFNode::getClassTypeId())) {
        ((SoSFNode*)toField)->setValue((SoNode*)fromFieldContainer);
    }

	return TRUE;
}

