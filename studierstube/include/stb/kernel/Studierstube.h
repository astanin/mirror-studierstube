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
/** 
*
* @author Denis Kalkofen
*
* $Id: Studierstube.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _STUDIERSTUBE_H_
#define _STUDIERSTUBE_H_


#if defined(WIN32) || defined(_WIN32_WCE)
#  pragma warning(disable:4251)
#  pragma warning(disable:4290)
#  ifdef KERNEL_EXPORTS
#    define KERNEL_API __declspec(dllexport)
#  else
#    define KERNEL_API __declspec(dllimport)
#  endif
#else
#  define KERNEL_API
#endif


#if defined(WIN32) || defined(_WIN32_WCE)
#  pragma warning(disable:4251)
#  pragma warning(disable:4290)
#  ifdef COMPONENT_EXPORTS
#    define COMPONENT_API __declspec(dllexport)
#  else
#    define COMPONENT_API __declspec(dllimport)
#  endif
#else
#  define COMPONENT_API
#endif


#define STUDIERSTUBE_VERSION_STRING "Studierstube v4.2"
#define STUDIERSTUBE_YEAR_STRING "2007"

namespace stb{
	class Kernel;
}
// C-style interface for initial instance creation
//
extern "C" 
{
	KERNEL_API void executeStudierstube(int argc, char** argv);
}

/**
* @defgroup kernel Kernel Classes
*/

#endif//
