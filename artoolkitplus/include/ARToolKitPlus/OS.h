/* ========================================================================
* PROJECT: ARToolKitPlus
* ========================================================================
* This work is based on the original ARToolKit developed by
*   Hirokazu Kato
*   Mark Billinghurst
*   HITLab, University of Washington, Seattle
* http://www.hitl.washington.edu/artoolkit/
*
* Copyright of the derived and new portions of this work
*     (C) 2006 Graz University of Technology
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
* For further information please contact 
*   Dieter Schmalstieg
*   <schmalstieg@icg.tu-graz.ac.at>
*   Graz University of Technology, 
*   Institut for Computer Graphics and Vision,
*   Inffeldgasse 16a, 8010 Graz, Austria.
* ========================================================================
** @author   Daniel Wagner
*
* $Id: OS.h 181 2006-09-07 09:49:27Z daniel $
* @file
* ======================================================================== */


#ifndef __ARTOOLKITPLUS_OS_HEADERFILE__
#define __ARTOOLKITPLUS_OS_HEADERFILE__

//
// This header file provide basic support for various
// operating systems at preprocessor level.
//

//#define DEBUG_DIV_RANGE
//#define VIGNETTING_ENABLED

#define DIV_TABLE_MIN 0
#define DIV_TABLE_MAX 2500
#define DIV_TABLE_BASE_FACT 64
#define DIV_TABLE_PREC 16
#define DIV_TABLE_IDX_SHIFT 12
#define DIV_TABLE_TO_FIXED_SHIFT 2
#define DIV_TABLE_SIZE (DIV_TABLE_MAX-DIV_TABLE_MIN)*DIV_TABLE_PREC

#define DIV_TABLE_FIXED_FROM_FIXED(VAL) DIV_TABLE[((VAL)>>DIV_TABLE_IDX_SHIFT)-DIV_TABLE_PREC*DIV_TABLE_MIN]>>DIV_TABLE_TO_FIXED_SHIFT
#define DIV_TABLE_FIXEDx4_FROM_FIXED(VAL) DIV_TABLE[((VAL)>>DIV_TABLE_IDX_SHIFT)-DIV_TABLE_PREC*DIV_TABLE_MIN]


#if defined(WIN32) || defined(_WIN32_WCE)

// Windows can compile as static LIB or
// DLL, depending on whether ARTOOLKITPLUS_DLL is
// defined or not. We rely ARTOOLKITPLUS_EXPORTS
// to detect if ARToolKitPlus itself or an application
// compiling against it is currently being built.
//
#  ifdef ARTOOLKITPLUS_DLL
#    ifdef ARTOOLKITPLUS_EXPORTS
#      define ARTOOLKITPLUS_API_DECL __declspec(dllexport)
#      define ARTOOLKITPLUS_API_IMPL __declspec(dllexport)
#    else
#      define ARTOOLKITPLUS_API_DECL __declspec(dllimport)
#      define ARTOOLKITPLUS_API_IMPL __declspec(dllimport)
#    endif // ARTOOLKITPLUS_EXPORTS
#  else
#    define ARTOOLKITPLUS_API_DECL
#    define ARTOOLKITPLUS_API_IMPL
#  endif // ARTOOLKITPLUS_DLL

#ifdef _WIN32_WCE
// Hardcoded support for fixed-point and no
// LibRPP support under Symbian.
//
#  define _USEFIXED_
#  define _USE_DIV_TABLE_
#  define _NO_LIBRPP_
#  define LIBRPP_STATIC
//#  define __ARTKP_NO_STL_VECTOR__
#else

// WinXP
//#define _USEFIXED_

#endif //_WIN32_WCE

#elif __SYMBIAN32__

// ARToolKitPlus for Symbian always builds into a DLL.
// There is also no difference in pre-processor definitions
// between building the DLL and building an application.
//
#  define ARTOOLKITPLUS_DLL
#  define ARTOOLKITPLUS_API_DECL IMPORT_C
#  define ARTOOLKITPLUS_API_IMPL EXPORT_C

// Hardcoded support for fixed-point and no
// LibRPP support under Symbian.
//
#  define _USEFIXED_
#  define _NO_LIBRPP_
#  define _USE_DIV_TABLE_
#  define _NO_LIBRPP_
#  define LIBRPP_STATIC
#  define __ARTKP_NO_STL_VECTOR__
#  define _ARTKP_NO_MEMORYMANAGER_

//#include <cassert>

#else

// All other platforms don't require imports or exports
// for now...
//
#  define ARTOOLKITPLUS_API_DECL
#  define ARTOOLKITPLUS_API_IMPL

#endif


#endif //__ARTOOLKITPLUS_OS_HEADERFILE__
