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
* $Id: OS.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */


#include <stb/base/fixWinCEIssues.h>
#include <stb/base/OS.h>
#include <stb/base/fixWinXPIssues.h>
#include <ace/OS.h>

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#include <cstdio>
#include <sstream>
#include <iostream>
#include <assert.h>


BEGIN_NAMESPACE_STB


//---------------------------------------
/**
*/
hModule 
os_LoadLibrary(stb::string fileName)
{
#ifdef STB_IS_WINDOWS
    //ostringstream name;
    //name  << fileName;
    //#ifdef _DEBUG
    //    name << "d";
    //#endif
    //    name <<".dll";

    //FILE* fp = fopen(name.str().c_str(), "rb");
    //if(!fp)
    //{
    //    printf("can't open %s\n",name.str().c_str());
    //    return NULL;
    //}

	std::string fName = fileName;
	fName += ".dll";

	const char* libNameStr = fName.c_str();
	assert(libNameStr);

	return ::LoadLibrary(ACE_TEXT_CHAR_TO_TCHAR(libNameStr));
#else //!STB_IS_WINDOWS
    using namespace std;
    stb::string pathName, libName;
    stb::string::size_type pos = fileName.rfind('/');

    if(pos != stb::string::npos) {
        ++pos;
        pathName = fileName.substr(0, pos);
        fileName = fileName.substr(pos, fileName.length() - pos);
    }

#ifndef __APPLE__
    libName = pathName + "lib" + fileName + ".so";
#else
    libName = pathName + "lib" + fileName + ".dylib";
#endif 
    cerr << "Dynamically loading >" << libName << "< ...";
    
    // load the library
    hModule p = dlopen(libName.c_str(), RTLD_LAZY);
    if (!p) {
        cerr << " failed: cannot load library: " << dlerror() << '\n';
        return 0;
    }
    cerr << " done." << endl;
    return p;
#endif
}

//----------------------------------------
/**
*/
int*
os_GetProcAddress(hModule moduleHandle,const char*   procName)
{
#ifdef STB_IS_WINDOWS
    return (int*)GetProcAddress(moduleHandle, ACE_TEXT_CHAR_TO_TCHAR(procName));
#else
    return (int*)dlsym(moduleHandle, procName);
#endif
}

bool    
os_FreeLibrary(hModule libHandle)
{
#ifdef STB_IS_WINDOWS
    if(FreeLibrary(libHandle))//if else to avoid warning C4800:forcing value to bool 'true' or 'false' 
	return true;
    else 
	return false;
#else
    dlclose(libHandle);
    return true;
#endif
}


bool
os_correctModuleName(stb::string& fileName, bool nMakeDebug, bool nMakeES)
{

	// only WinXP uses '_es' to mark OpenGL ES version...
	//
#if defined(_IS_KLIMTES_) && defined(STB_IS_WINXP)
	if(nMakeES)
		fileName += "_es";
#endif

	// Windows debug DLLs are post-fixed with a 'd'
	//
#if defined(STB_IS_WINDOWS) && defined(STB_IS_DEBUG)
	if(nMakeDebug)
		fileName += "d";
#endif

	return true;
}


END_NAMESPACE_STB


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

