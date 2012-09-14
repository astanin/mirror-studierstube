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


#ifndef _SOSIMPLE_MAIN_H
#define _SOSIMPLE_MAIN_H


#include <stb/base/OS.h>


#ifdef WIN32
#  ifdef SOSIMPLE_NODLL
#    define SOSIMPLE_API
#  else //!SOSIMPLE_NODLL
#    ifdef SOSIMPLE_EXPORTS
#      define SOSIMPLE_API __declspec(dllexport)
#    else //SOSIMPLE_EXPORTS
#      define SOSIMPLE_API __declspec(dllimport)
#    endif //SOSIMPLE_EXPORTS
#  endif //SOSIMPLE_NODLL
#else
#  define SOSIMPLE_API
#endif //WIN32




#define SOSIMPLE_OBJECT_ABSTRACT_HEADER(classname, parentname) \
public: \
	static void initClass(void); \
	static SoType getClassTypeId(void); \
	virtual SoType getTypeId(void) const /* = 0 (see comment above) */; \
private: \
	typedef parentname inherited; \
	static SoType classTypeId

#define SOSIMPLE_OBJECT_HEADER(classname, parentname) \
public: \
	static void initClass(void); \
	static SoType getClassTypeId(void); \
	virtual SoType getTypeId(void) const; \
	static void * createInstance(void); \
private: \
	typedef parentname inherited; \
	static SoType classTypeId

#define SOSIMPLE_OBJECT_ABSTRACT_SOURCE(classname) \
	void classname::initClass(void) { \
	assert(classname::classTypeId == SoType::badType()); \
	classname::classTypeId = \
	SoType::createType(inherited::getClassTypeId(), \
	SO__QUOTE(classname)); \
} \
	SoType classname::getClassTypeId(void) { \
	return classname::classTypeId; \
} \
	SoType classname::getTypeId(void) const { \
	return classname::classTypeId; \
} \
	SoType classname::classTypeId SOSIMPLE_STATIC_SOTYPE_INIT

#define SOSIMPLE_OBJECT_SOURCE(classname) \
	void classname::initClass(void) { \
	assert(classname::classTypeId == SoType::badType()); \
	classname::classTypeId = \
	SoType::createType(inherited::getClassTypeId(), \
	SO__QUOTE(classname), \
	classname::createInstance); \
} \
	SoType classname::getClassTypeId(void) { \
	return classname::classTypeId; \
} \
	SoType classname::getTypeId(void) const { \
	return classname::classTypeId; \
} \
	void * classname::createInstance(void) { \
	assert(classname::classTypeId != SoType::badType()); \
	return (void *) new classname; \
} \
	SoType classname::classTypeId SOSIMPLE_STATIC_SOTYPE_INIT



#endif //_SOSIMPLE_MAIN_H
