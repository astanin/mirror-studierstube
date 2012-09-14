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
/** some OpenGL utility routines
 *
 * $Id: GLtools.cpp 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <cstdio>
#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include <Inventor/errors/SoDebugError.h>

char _checkGLErrors(char *file, int line)
{
    int i = 0;
    GLenum e;
    
    while ((e = glGetError ()) != GL_NO_ERROR)
    {
		SoDebugError::post("checkGLErrors()",
			"%s:%i: GL error: %s\n", file, line, gluErrorString(e));
        i++;
    }
    return i;
}

/* ===========================================================================
   End of GLtools.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
