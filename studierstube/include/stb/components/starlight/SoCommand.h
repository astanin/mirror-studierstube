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
/** The header file for the SoCommand.
*
* @author Denis Kalkofen
*
* $Id: SoCommand.cpp 2006-04-26 sareika $
* @file                                                                   */
/* ======================================================================= */

#ifndef  _SOCOMMAND_H_
#define  _SOCOMMAND_H_

#include <Inventor/fields/SoSFString.h> 
#include "Inventor/nodes/SoSubNode.h"
#include <Inventor/SoInput.h> 
#include "Inventor/sensors/SoIdleSensor.h"
#include "Inventor/sensors/SoOneShotSensor.h"

#include "starlight.h"

class STARLIGHT_API SoCommand : public SoNode
{
	SO_NODE_ABSTRACT_HEADER(SoCommand);
public:
	SoCommand(void);
	static void initClass();

protected:
	~SoCommand(void);

	virtual SbBool readInstance(SoInput *in, unsigned short flags);
	virtual SbBool evaluateFieldValues()=0;
	
	SoIdleSensor *idleSensor;
	static uint32_t  priority;
	
	void schedule(SoSensorCB *  func,  void *data);
	static void process(void *data, SoSensor* sensor);//idle callback func
};


#endif //_SOCOMMAND_H_
