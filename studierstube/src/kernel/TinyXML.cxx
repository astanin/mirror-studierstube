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
* $Id: TinyXML.cxx 25 2005-11-28 16:11:59Z  $
* @file                                                                   */
/* ======================================================================= */


#ifndef USE_TINYXMLMOD
#include <tinyxml/tinyxml.h>
#else
#include <TinyXML/tinyxml.h>
#endif
#  if defined(TINYXML_MOD)

//   this is the modified TinyXML version modified by Daniel Wagner called TinyXML_Mod.
//   Under Windows can either be linked statically or as DLL.

#    pragma message("XMLSelection: compiling against TinyXML_Mod")

#    if defined(WIN32) || defined (_WIN32_WCE)
#      ifdef TINYXML_MOD_STATIC


#        if defined(_DEBUG) || defined(DEBUG)
#          pragma message("XMLSelection: linking against static-version: TinyXML_ModLibd.lib")
#          pragma comment( lib, "TinyXML_ModLibd.lib" )
#        else
#          pragma message("XMLSelection: linking against static-version: TinyXML_ModLib.lib")
#          pragma comment( lib, "TinyXML_ModLib.lib" )
#        endif 

#      elif defined(TINYXML_MOD_DLL)

//       this is the dynamic linking version of the TinyXML library
//       modified by Daniel Wagner

#        if defined(_DEBUG) || defined(DEBUG)
#          pragma message("XMLSelection: linking against DLL-version: TinyXML_Modd.lib")
#          pragma comment( lib, "TinyXML_Modd.lib" )
#        else
#          pragma message("XMLSelection: linking against DLL-version: TinyXML_Mod.lib")
#          pragma comment( lib, "TinyXML_Mod.lib" )
#        endif 

#      else  //!defined(TINYXML_MOD_DLL)

#        pragma message("XMLSelection: TinyXML_Mod must be linked either static or dynamic.")
#        pragma message("              Please define either TINYXML_MOD_STATIC or TINYXML_MOD_DLL")
#        pragma error

#      endif  //!defined(TINYXML_MOD_DLL)

#    endif  //defined(WIN32) || defined (_WIN32_WCE)

#  else			// defined(TINYXML_MOD)

//   this is the default version of the TinyXML library
#    pragma message("XMLSelection: compiling against standard TinyXML")

#    if defined(_DEBUG) || defined(DEBUG)
#      pragma message("XMLSelection: linking against tinyxmld.lib")
#      pragma comment( lib, "tinyxmld.lib" )
#    else
#      pragma message("XMLSelection: linking against tinyxml.lib")
#      pragma comment( lib, "tinyxml.lib" )
#    endif 

#  endif		// defined(TINYXML_MOD)
