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
/** The header file for the SoSetName.
*
* @author Denis Kalkofen
*
* $Id: SoSetName.h 2006-04-26 sareika $
* @file                                                                   */
/* ======================================================================= */

////////////////////////// use example:
//DEF BLA Material {
//    diffuseColor 0 1 0
//       transparency 1
//   }
//   SoSetName{
//       name "TEST"    #can be left away
//       def "BLA"
//   }

#ifndef _SOSETNAME_H_
#define _SOSETNAME_H_

#include "SoCommand.h"
#include "starlight.h"

class STARLIGHT_API SoSetName : public SoCommand {
	SO_NODE_HEADER(SoSetName);
 public:
	SoSetName();
	SoSFString def;
	SoSFString name;
	static void  initClass();
 
 protected:
	~SoSetName();
	
	SoBase		*objectPtr;
	SbString    nameStr;
	SbBool setObjPtr();
	SbBool setNameStr();
	
	SbBool evaluateFieldValues();
	virtual SbBool readInstance(SoInput *in, unsigned short flags);
	static void process(void *data,SoSensor* sensor);   
};

#endif //_SOSETNAME_H_
