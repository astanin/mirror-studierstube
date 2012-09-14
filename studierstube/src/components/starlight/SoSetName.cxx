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
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The Source file for the SoSetName.
*
* @author Denis Kalkofen
*
* $Id: SoSetName.cpp 2006-04-26 sareika $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/starlight/SoSetName.h>
#include <Inventor/errors/SoError.h>

SO_NODE_SOURCE(SoSetName);

void
SoSetName::initClass()
{
    SO_NODE_INIT_CLASS(SoSetName,SoCommand , "SetName");
}

SoSetName::SoSetName()
{
    SO_NODE_CONSTRUCTOR(SoSetName);
	SO_NODE_ADD_FIELD(def, (""));
    SO_NODE_ADD_FIELD(name, (""));
	objectPtr=NULL;
}

SoSetName::~SoSetName()
{
	//nil
}

SbBool 
SoSetName::evaluateFieldValues()
{
	if (def.getValue().getString() == "")
	{
		SoError::post("ERROR: SoSetName -  'def' is empty.\n");		
		return false;
	}
	if (name.getValue().getString() == "")
	{
		name.setValue(def.getValue().getString());
	}
	return true;
}

SbBool 
SoSetName::setObjPtr()
{
	objectPtr=SoBase::getNamedBase(SbName(def.getValue().getString()),SoBase::getClassTypeId());
	if(objectPtr==NULL){
		SoError::post("ERROR: SoSetName -  Can't find '%s' ",def.getValue().getString());		
		return false;
	}
	return true;
}

SbBool 
SoSetName::setNameStr()
{
	nameStr=name.getValue();
	return true;
}

SbBool 
SoSetName::readInstance(SoInput *in, unsigned short flags )
{
	if(!SoCommand::readInstance(in,flags))
		return false;

	if(!setObjPtr())
		return false;		

	if(!setNameStr())
		return false;

	//schedule command
	SoCommand::priority--;
	this->schedule(process,this);
	SoCommand::priority++;
	SoCommand::priority++;

	return true;
}


void 
SoSetName::process(void *data, SoSensor* sensor)
{
	printf("INFO: processing SoSetName ... ");
	
	SoSetName* obj	=(SoSetName*)data;
	(obj->objectPtr)->setName(obj->nameStr);

	SoCommand::process(data,sensor);
	printf("done\n");
}
