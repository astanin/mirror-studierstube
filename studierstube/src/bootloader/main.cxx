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
/** The source file for the studierstube bootloader.
*
* @author Denis Kalkofen
*
* $Id: main.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/bootloader/KernelLoader.h>
#include <stb/base/OS.h>


#ifdef STB_IS_WINCE


bool
preloadModules()
{

	printf("INFO: preloading modules\n");

	const char* libNames[] = { "TinyXML_Mod", "ACEmini", "ARToolKitPlusDll", "opentracker", "libGLES_CL", "KlimtES", "coin2es", "openVideo", "SoSimple", "stbkernel", "stbevent", "stbvideo", "stbviewer_simple" };
	bool libDebugs[] = {       true,          true,      false,              true,          false,        true,      true,      true,        true,       true,        true,       true,       true,              };
	const size_t numLibs = sizeof(libNames) / sizeof(const char*);

	for(size_t i=0; i<numLibs; i++)
	{
		std::string libName = libNames[i];
		stb::os_correctModuleName(libName, libDebugs[i], false);
		printf("trying to load module '%s'\n", libName.c_str());
		if(!stb::os_LoadLibrary(libName))
		{
			//flo
			WCHAR str[256];
			LPVOID lpMsgBuf;
			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, (LPWSTR)&lpMsgBuf, 0, NULL);
			swprintf(str, L"Failed to load library:\n%S\n\nWindows reports: %s", libName.c_str(), lpMsgBuf);
			MessageBoxW(NULL, str, L"STARTUP ERROR", MB_OK);
			LocalFree(lpMsgBuf);

			printf("ERROR: failed to load module '%s'\n", libName.c_str());
			return false;
		}
	}

	printf("INFO: all modules preloaded correctly\n");
	return true;
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	const int argc = 2;
#ifdef STB_IS_WINCE
	char* argv[argc] = { "studierstube.exe", "Program Files/stb4/data" };
#else
	char* argv[argc] = { "studierstube.exe", "/data" };
#endif
	stb::string libName="stbkernel";

	if(!preloadModules())
		return -1;

	// make sure we load debug version in case of debug build...
	//
	stb::os_correctModuleName(libName, true, true);

	const char execFunc[]="executeStudierstube";
	stb::KernelLoader loader;
	loader.setExecFuncName(execFunc);
	loader.setLibName(libName);
	loader.runKernel(argc,argv);

	return 0;
}


#else //STB_IS_WINCE


int 
main(int argc,char* argv[])
{
    stb::string libName="stbkernel";

	// make sure we load debug version in case of debug build...
	//
	stb::os_correctModuleName(libName, true, true);

    const char execFunc[]="executeStudierstube";
    stb::KernelLoader loader;
    loader.setExecFuncName(execFunc);
    loader.setLibName(libName);
    loader.runKernel(argc,argv);
    
    return 0;
}


#endif //STB_IS_WINCE
