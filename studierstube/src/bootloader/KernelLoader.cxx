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

/** The impl file for the KernelLoader class.
*
* @author Bernhard Reitinger
*
* $Id$
* @file                                                                   */
/* ======================================================================= */


#include <stb/base/fixWinCEIssues.h>
#include <stb/bootloader/KernelLoader.h>

#include <iostream>

#include <ace/DLL.h>
#include <ace/Log_Msg.h>

BEGIN_NAMESPACE_STB

KernelLoader::KernelLoader() {    
}

KernelLoader::~KernelLoader() {    
}

void KernelLoader::setLibName(stb::string aLibName) 
{
    libName = aLibName;
}

void KernelLoader::setExecFuncName(stb::string aFuncName) 
{
    execFuncName = aFuncName;
}

bool KernelLoader::runKernel(int argc, char* argv[]) 
{
    using namespace std;
    ACE_DLL kernel_dll;

    if(kernel_dll.open(ACE_TEXT_CHAR_TO_TCHAR(libName.c_str())))
	{
        ACE_DEBUG((LM_ERROR, "could not load %s library\n", libName.c_str()));
        return false;        
    }

    // get function pointer
    void (*startKernel)(int, char**);

    startKernel = (void(*)(int, char**))kernel_dll.symbol(ACE_TEXT_CHAR_TO_TCHAR(execFuncName.c_str()));

    if (!startKernel) {
        ACE_DEBUG((LM_ERROR, "could not get entry point of %s\n", libName.c_str()));
        return false;        
    }

    //call startkernel
    (*startKernel)(argc,argv);

    kernel_dll.close();
    
    return true;
}

END_NAMESPACE_STB

//========================================================================
// End of KernelLoader.cxx
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
