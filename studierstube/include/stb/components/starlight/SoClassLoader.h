/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoClassLoader implementation
  *
  * @author  Andreas Zajic
  *
  * $Id: SoClassLoader.h 8 2004-07-14 12:24:04Z tamer $
  * @file  SoClassLoader.h                                                  */
 /* ======================================================================= */

#ifndef _SOCLASSLOADER_H_
#define _SOCLASSLOADER_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFString.h>
#include <stb/base/OS.h>

#include "starlight.h"

#ifdef STB_IS_WINDOWS
#  include <windows.h>
#elif defined(STB_IS_LINUX) || defined(STB_IS_APPLE)
#  include <ltdl.h>
#endif


/** SoClassLoader enhances Open Inventors loading mechanism.
 * When a SoClassLoader node is read from a file, it loads the DLL defined in the Field 
 * fileName using standard operating system functionality (e.g. WIN32API:loadlibrary()). 
 * After successfully loading the library the initClass() method of the class defined in 
 * the className field is called to initialize the class and add it to the OIV databases 
 * list of known classes. From the point where the given class is registered to the OIV 
 * database in this way, it is possible to read an instance of such a class from a file 
 * without further arrangements. It tries to find the DLL in the current input directories
 * of OpenInventor. Therefore the same rules for file names apply as for images and
 * SoFile nodes. For instance, file names can be relative to the current .iv file !
 * Make sure to use only slashes '/' in the file names.
 *
 * The SoClassLoader node is usually only used inside a .iv-file.
 * @author  Andreas Zajic
 * @ingroup workspace
 * @ingroup util
 */
class STARLIGHT_API SoClassLoader
:
public SoNode
{
    SO_NODE_HEADER(SoClassLoader);

public:
    /// field containing the name of the class to load.
    SoSFString className;
    /// file name of the DLL or SO that contains the class.
    SoSFString fileName;

    static void initClass();

    SoClassLoader();

    SbBool loadClass( const SbStringList & directories );
    SbBool loadClass(SbString classname, SbString filename);

#if defined(STB_IS_WINDOWS)
    HINSTANCE getObjectHandle() { return objectHandle; };
#elif defined(STB_IS_LINUX)
    void* getObjectHandle() { return objectHandle; };
#elif defined (STB_IS_APPLE)
    void* getObjectHandle() { return objectHandle; };
#endif

    
protected:
    virtual SbBool readInstance(SoInput *in, unsigned short flags);


private:

    SbBool linkDll(SbString filename);
    SbBool findEntryPoint(SbString classname);
    SbBool callInitClass();

    virtual ~SoClassLoader();

#ifdef STB_IS_WINDOWS
    HINSTANCE objectHandle;
#elif defined(STB_IS_LINUX)
    lt_dlhandle objectHandle;
#elif defined(STB_IS_APPLE)
    lt_dlhandle objectHandle;
#endif

    void* messageHandler;

    void (*initClassFunc)();

};

#endif // _SOCLASSLOADER_H_

