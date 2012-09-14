
#ifndef __XML_SELECTION__
#define __XML_SELECTION__

#ifdef USE_TINYXML

#ifndef TINYXML_MOD
#  include <tinyxml/tinyxml.h>
#else
#  include <TinyXML/tinyxml.h>
#endif

#  if defined(TINYXML_MOD)

//   this is the modified TinyXML version modified by Daniel Wagner called TinyXML_Mod.
//   Under Windows can either be linked statically or as DLL.

#if defined (WIN32)
#    pragma message("XMLSelection: compiling against TinyXML_Mod")
#endif

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

#  define OT_DOMDOCUMENT TiXmlDocument
#  define OT_DOMELEMENT TiXmlElement

#else
#if defined (WIN32)
#pragma message("compiling against XERCESC")
#endif
#ifndef SWIG
#include <xercesc/dom/DOM.hpp>
#endif
#define OT_DOMDOCUMENT XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument
#define OT_DOMELEMENT XERCES_CPP_NAMESPACE_QUALIFIER DOMElement
#ifndef USE_XERCES
#define USE_XERCES 1
#endif
#ifdef WIN32
#ifdef _DEBUG
#pragma comment( lib, "xerces-c_2d.lib" )
#else
#pragma comment( lib, "xerces-c_2.lib" )
#endif 
#endif
#endif //USE_TINYXML


#endif //__XML_SELECTION__

/* 
 * ------------------------------------------------------------
 *   End of XMLSelection.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
