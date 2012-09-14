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
 * $Id: DLL.cpp 212 2007-05-09 13:11:53Z bornik $
 * @file
 * ======================================================================= */


#include <ARToolKitPlus/TrackerSingleMarkerImpl.h>
#include <ARToolKitPlus/TrackerMultiMarkerImpl.h>

/// This file should only be compiled when using ARToolKitPlus as a DLL.

// fix bug in intel compiler which forgets to link
// against the standard run-time library.
//
/*#ifndef IS_EVC4
#  if defined(__INTEL_COMPILER) && defined(_WIN32_WCE)
#    ifdef _DEBUG
#      pragma comment(linker, "/defaultlib:MSVCRTd.lib")
#    else
#      pragma comment(linker, "/defaultlib:MSVCRT.lib")
#    endif
#  endif
#endif*/
//#pragma comment(linker, "/disallowlib:oldnames.lib")


namespace ARToolKitPlus
{


#ifdef ARTOOLKITPLUS_DLL

ARTOOLKITPLUS_API_IMPL ARToolKitPlus::TrackerSingleMarker*
createTrackerSingleMarker(int nWidth, int nHeight,
                          int nMarkerSizeX, int nMarkerSizeY, int nMarkerSampleNum,
                          ARToolKitPlus::PIXEL_FORMAT nPixelFormat)
{

	if(nMarkerSizeX==6 && nMarkerSizeY==6 && nMarkerSampleNum==6)
	{
		typedef ARToolKitPlus::TrackerSingleMarkerImpl<6,6,6, 1, 8> TrackerSingleMarker_6_6_6_1_8;

#ifndef _ARTKP_NO_MEMORYMANAGER_
		if(memManager && !memManager->didInit())
			memManager->init(TrackerSingleMarker_6_6_6_1_8::getMemoryRequirements());
#endif

		TrackerSingleMarker* tracker = new TrackerSingleMarker_6_6_6_1_8(nWidth, nHeight);
		tracker->setPixelFormat(nPixelFormat);
		return tracker;
	}

	return NULL;
}

ARTOOLKITPLUS_API_IMPL ARToolKitPlus::TrackerMultiMarker*
createTrackerMultiMarker(int nWidth, int nHeight,
                         int nMarkerSizeX, int nMarkerSizeY, 
			 int nMarkerSampleNum,
                         ARToolKitPlus::PIXEL_FORMAT nPixelFormat)
{

	if(nMarkerSizeX==6 && nMarkerSizeY==6 && nMarkerSampleNum==6)
	{
		typedef ARToolKitPlus::TrackerMultiMarkerImpl<6,6,6, 1, 16> TrackerMultiMarker_6_6_6_1_16;

#ifndef _ARTKP_NO_MEMORYMANAGER_
		if(memManager && !memManager->didInit())
			memManager->init(TrackerMultiMarker_6_6_6_1_16::getMemoryRequirements());
#endif

		TrackerMultiMarker* tracker = new TrackerMultiMarker_6_6_6_1_16(nWidth, nHeight);
		tracker->setPixelFormat(nPixelFormat);
		return tracker;
	}

	return NULL;
}

#endif //ARTOOLKITPLUS_DLL


//#ifdef __SYMBIAN32__
//
//#  include <e32uid.h>
//
//#  ifdef EKA2
//GLDEF_C TInt E32Dll() 
//#  else 
//GLDEF_C TInt E32Dll(TDllReason /*aReason*/) 
//#  endif EKA2
//// DLL entry point
//{
//	return(KErrNone);
//}
//
//#endif //__SYMBIAN32__


}  // namespace ARToolKitPlus



