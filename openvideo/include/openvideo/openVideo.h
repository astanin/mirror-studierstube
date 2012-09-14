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
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The OPENVIDEO_API definition.
  *
  * @author Denis Kalkofen
  *
  * $Id: openVideo.h 184 2006-06-02 14:58:35Z breiting $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _OPENVIDEO_H_
#define _OPENVIDEO_H_

// The configOV.h file is only used on windows. On linux command line
// arguments are used instead.
#ifdef HAVE_CONFIGOV_H
#include <openvideo/configOV.h>
#endif


#define OPENVIDEO_VERSION_MAJOR 0
#define OPENVIDEO_VERSION_MINOR 2


namespace openvideo {

enum VERSION {
	VERSION_MAJOR = OPENVIDEO_VERSION_MAJOR,
	VERSION_MINOR = OPENVIDEO_VERSION_MINOR
};

}  // namespace openvideo



#if defined(_WIN32_WCE)
#  define OV_IS_WINCE
#  define OV_IS_WINDOWS
#elif defined(WIN32)
#  define OV_IS_WINXP
#  define OV_IS_WINDOWS
#endif

#ifdef OV_IS_WINDOWS
#  if defined(DEBUG) || defined(_DEBUG)
#    define OV_IS_DEBUG
#  endif
#endif

#ifdef LINUX
#  define OV_IS_LINUX
#endif


#if defined(WIN32) || defined(_WIN32_WCE)
#pragma warning(disable:4251)
#endif

#if defined(WIN32) || defined(_WIN32_WCE)
    #ifdef OPENVIDEO_EXPORTS
        #define OPENVIDEO_API __declspec(dllexport)
    #else
        #define OPENVIDEO_API __declspec(dllimport)
    #endif
#else
	#define OPENVIDEO_API
#endif

/// comfortable namespace setting

#define BEGIN_NAMESPACE_OV namespace openvideo {
#define END_NAMESPACE_OV }; // namespace


/**
 * @defgroup core Core Classes
 *	The 'Core' classes provide OpenVideo with its basic functionality to construct and update a 
 *	directed acyclic graph like data structure, which consists of OpenVideo nodes and is given via 
 *	an xml based configuration file. To easily extend OpenVideo’s set of nodes the core classes 
 *	also implement an interface to derive new nodes from. 
 *	The Interface implements an ‘Abstract Factory Pattern[POSA96]’ which is outlined in the following chart.
 *	\image html abstractfactory.gif
 *	
 *	Refering to this terminology the 'AbstractFactory' is implemented in the Core module under the name of "NodeFactory" and 
 *	the 'NodeFactory' is implemented by the class 'Node'.
 *
 *	For details about the functionality of the Core module refer to the various class documentations of the core members. 
 *  \n
 *	\n
 *	\n
 *	[POSA96]F. Buschmann, R. Meunier, H. Rohnert, P. Sommerlad, M. Stal
 *  Pattern-Oriented Software Architecture - A System Of Patterns John Wiley & Sons, Chichester, 1996
 */

/**
 * @defgroup nodes Node Classes
 *	The ‘Nodes’ module collects all node implementations. All the nodes here are derived from the interface the ‘Core’ module implements.
 *	Since OpenVideo’s interface implements an ‘Abstract Factory Pattern[Ref]’ 
 *	there are two classes needed to provide OpenVideo with a new node implementation. These nodes are in particular 
 *	a ‘Factory’ (which is able to create a specific type of nodes) and the actual 'Node' implementation. For details on 
 *	the interface please see the interface documentation in the ‘Core’ module. \n
 *  \n
 *	\n
 *	\n
 *	[POSA96]F. Buschmann, R. Meunier, H. Rohnert, P. Sommerlad, M. Stal
 *  Pattern-Oriented Software Architecture - A System Of Patterns John Wiley & Sons, Chichester, 1996
 */


#endif //_OPENVIDEO_H_
