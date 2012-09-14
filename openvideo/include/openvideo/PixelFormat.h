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
/** The header file for all PixelFormat implementations.
*
* @author Daniel Wagner
*
* $Id: PixelFormat.h 238 2007-05-03 13:24:57Z bornik $
* @file                                                                   */
/* ======================================================================= */

#ifndef _PIXELFORMAT_H
#define _PIXELFORMAT_H

#include <openvideo/openVideo.h>
#include <string>


namespace openvideo {


    enum PIXEL_FORMAT {
	FORMAT_R8G8B8	= 0,
	FORMAT_B8G8R8	= 1,
	FORMAT_R8G8B8X8	= 2,
	FORMAT_B8G8R8X8	= 3,
	FORMAT_R5G6B5	= 4,
	FORMAT_L8	= 5,
	FORMAT_YUY2	= 6,
	FORMAT_UYVY	= 7,
	FORMAT_YV12	= 8,

	FORMAT_UNKNOWN	= 9
    };


    class OPENVIDEO_API PixelFormat
    {
    public:
	static PIXEL_FORMAT StringToFormat(const std::string& formatName);

	static std::string FormatToString(PIXEL_FORMAT format);

	static PIXEL_FORMAT fromOGL(int format);

	static bool toOGL(PIXEL_FORMAT format, unsigned int& oglFormat, int& oglInternalFormat);

	static int getBitsPerPixel(PIXEL_FORMAT format);

    };  // namespace PixelFormat


}  // namespace openvideo


#endif //_PIXELFORMAT_H

//========================================================================
// End of $FILENAME$
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
