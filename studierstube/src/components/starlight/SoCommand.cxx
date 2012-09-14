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
/** The Source file for the SoCommand.
*
* @author Denis Kalkofen
*
* $Id: SoSetName.cpp 2006-04-26 sareika $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/starlight/SoCommand.h>
#include <Inventor/errors/SoError.h>

SO_NODE_ABSTRACT_SOURCE(SoCommand);
uint32_t SoCommand::priority=2;

void
SoCommand::initClass()
{
    SO_NODE_INIT_ABSTRACT_CLASS(SoCommand, SoNode, "SoNode");
}


SoCommand::SoCommand(void)
{
	SO_NODE_CONSTRUCTOR(SoCommand);
	
}

SoCommand::~SoCommand(void)
{
	//nil
}


SbBool 
SoCommand::readInstance(SoInput *in, unsigned short flags )
{
	if(!SoFieldContainer::readInstance(in, flags))
		return false;
	if(!evaluateFieldValues())
		return false;

	return true;
}

void 
SoCommand::schedule(SoSensorCB *  func,  void *data)
{
	idleSensor= new SoIdleSensor(func,data);
	idleSensor->setPriority(SoCommand::priority);
	idleSensor->schedule();
	((SoBase*)data)->ref();
}

void 
SoCommand::process(void *data,SoSensor* /*sensor*/)
{
	delete ((SoCommand*)data)->idleSensor;
	((SoBase*)data)->unref();
}
