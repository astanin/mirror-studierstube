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
/** The header file for the guiDefines.
*
* @author Daniel Wagner
*
* $Id$
* @file                                                                   */
/* ======================================================================= */


#ifndef _STB_SOSIMPLEOBJECT_H_
#define _STB_SOSIMPLEOBJECT_H_

#include "SoSimple.h"

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SoType.h>


/// Base clase for object hierarchy
class SOSIMPLE_API SoSimpleObject
{
	static SoType classTypeId;

public:
	static void initClass(void);
	static SoType getClassTypeId(void);
	virtual SoType getTypeId(void) const = 0;
	SbBool isOfType(SoType type) const;

	static void init(void);
};


#endif //_STB_SOSIMPLEOBJECT_H_
