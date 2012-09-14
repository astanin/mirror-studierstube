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
/** The header file for the KernelLoader class.
*
* @author Bernhard Reitinger
*
* $Id$
* @file                                                                   */
/* ======================================================================= */

#ifndef _KERNELLOADER_H_
#define _KERNELLOADER_H_

#include <stb/base/string.h>
#include <stb/base/macros.h>

BEGIN_NAMESPACE_STB

class KernelLoader {

public:
    /**
     *	Constructor()
     */
    KernelLoader();

    /**
     *	The destructor.
     */
    ~KernelLoader();
   
    /**
     *	set the name of the library which is going to be loaded.
     */
    void setLibName(stb::string aLibName);
   
    /**
     *	set the name of the function which is going to be called.
     */
    void setExecFuncName(stb::string aFuncName);
   
    /**
     *	loads the lib and calls the function defined by 'execFuncName'.
     */
    bool runKernel(int argc, char* argv[]);
   
protected:	
   
    stb::string libName;
    stb::string execFuncName;
   
};

END_NAMESPACE_STB

#endif//_KERNELLOADER_H_

//========================================================================
// End of KernelLoader.h
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
