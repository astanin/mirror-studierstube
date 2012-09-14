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
/** The header file for the OS class.
*
* @author Denis Kalkofen
*
* $Id: OS.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _STB_OS_H
#define _STB_OS_H

#include <stb/base/string.h>


#if defined(_WIN32_WCE)

#  define STB_IS_WINDOWS
#  define STB_IS_WINCE
#  if defined(_DEBUG) || defined(DEBUG)
#    define STB_IS_DEBUG
#  endif

#elif defined(WIN32)

#  define STB_IS_WINDOWS
#  define STB_IS_WINXP
#  if defined(_DEBUG) || defined(DEBUG)
#    define STB_IS_DEBUG
#  endif

#elif defined(LINUX) || defined(__linux__)

#  define STB_IS_LINUX

#elif defined(__APPLE__)

#  define STB_IS_APPLE

#else

#  error("unknown OS!!!")

#endif


#ifdef STB_IS_WINDOWS

#  define WIN32_LEAN_AND_MEAN
#  define OS_SEP "\\"
#  include <windows.h>

#endif


#ifdef STB_IS_LINUX

#  include <dlfcn.h>
#  define OS_SEP "/"

#endif

#ifdef STB_IS_APPLE

#  include <dlfcn.h>
#  define OS_SEP "/"

#endif

BEGIN_NAMESPACE_STB


#ifdef STB_IS_WINDOWS

   typedef HMODULE hModule;

#elif defined(STB_IS_LINUX) || defined (STB_IS_APPLE)

   typedef void* hModule; 

#endif


//-------------------------------
//functions
hModule os_LoadLibrary(stb::string fileName);

int*    os_GetProcAddress(hModule moduleHandle,const char*   procName);

bool    os_FreeLibrary(hModule libHandle);

bool    os_correctModuleName(stb::string& fileName, bool nMakeDebug, bool nMakeES);


END_NAMESPACE_STB


#ifdef STB_IS_WINDOWS
#  define CREATE_COMPONENT_FUNC(CLASSNAME) \
	extern "C" {																						\
	\
	__declspec(dllexport) stb::CLASSNAME* createComponent()\
{																									\
	return new stb::CLASSNAME;																	\
}																									\
	\
}
#else  // !STB_IS_WINDOWS
#  define CREATE_COMPONENT_FUNC(CLASSNAME) \
	extern "C" { \
	stb::CLASSNAME* createComponent() \
{ \
	return new stb::CLASSNAME; \
} \
}
#endif // STB_IS_WINDOWS



#endif //OS_H


/* ========================================================================
 * End of file
 * ========================================================================
 * Local Variables:
 * mode: c++
 * c-basic-offset: 4
 * eval: (c-set-offset 'substatement-open 0)
 * eval: (c-set-offset 'case-label '+)
 * eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 * eval: (setq indent-tabs-mode nil)
 * End:
 * ========================================================================
 */

