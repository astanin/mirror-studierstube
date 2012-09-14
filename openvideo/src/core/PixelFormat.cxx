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
/** The source file for the all PixelFormat implementations.
*
* @author Daniel Wagner
*
* $Id: PixelFormat.cxx 238 2007-05-03 13:24:57Z bornik $
* @file                                                                   */
/* ======================================================================= */


#include <openvideo/PixelFormat.h>

#ifdef OV_IS_WINDOWS
#  include <windows.h>
#endif

#include <GL/gl.h>
#include <map>


#ifndef GL_BGR
#  define GL_BGR 0x80E0
#endif

#ifndef GL_BGRA
#  define GL_BGRA 0x80E1
#endif


namespace openvideo 
{


    typedef std::map<std::string, PIXEL_FORMAT>	StringFormatMap;
    typedef std::pair<std::string, PIXEL_FORMAT> StringFormatPair;


    const char* formatNames[] = {
        "R8G8B8",
        "B8G8R8",
        "R8G8B8X8",
        "B8G8R8X8",
        "R5G6B5",
        "L8",
        "YUY2",
        "UYVY",
        "YV12",
        "UNKNOWN"
    };

    static StringFormatMap stringFormatMap;


    static void
    fillStringFormatMap()
    {
	stringFormatMap.clear();

	for(int i=0; i<FORMAT_UNKNOWN; i++)
            stringFormatMap.insert( StringFormatPair(formatNames[i], static_cast<PIXEL_FORMAT>(i)) );
    }


    PIXEL_FORMAT
    PixelFormat::StringToFormat(const std::string& formatName)
    {
	if(stringFormatMap.empty())
            fillStringFormatMap();

	StringFormatMap::iterator it = stringFormatMap.find(formatName);

	if(it == stringFormatMap.end())
            return FORMAT_UNKNOWN;

	return it->second;
    }


    std::string
    PixelFormat::FormatToString(PIXEL_FORMAT format)
    {
	if(format>=0 && format<FORMAT_UNKNOWN)
            return formatNames[format];

	return formatNames[FORMAT_UNKNOWN];
    }


    int
    PixelFormat::getBitsPerPixel(PIXEL_FORMAT format)
    {
	switch(format)
	{
            case FORMAT_R8G8B8X8:
            case FORMAT_B8G8R8X8:
                return 32;

            case FORMAT_R8G8B8:
            case FORMAT_B8G8R8:
                return 24;

            case FORMAT_YV12:
                return 12;

            case FORMAT_YUY2:
            case FORMAT_UYVY:
            case FORMAT_R5G6B5:
		return 16;

            case FORMAT_L8:
		return 8;

            default:
		return 0;
	}
    }


    PIXEL_FORMAT
    PixelFormat::fromOGL(int format)
    {
	switch(format)
	{
            case GL_RGB:
		return FORMAT_R8G8B8;
            case GL_BGR:
		return FORMAT_B8G8R8;
            case GL_RGBA:
		return FORMAT_R8G8B8X8;
            case GL_BGRA:
		return FORMAT_B8G8R8X8;
            case GL_LUMINANCE:
		return FORMAT_L8;
            default:
		return FORMAT_UNKNOWN;
	}
    }


    bool
    PixelFormat::toOGL(PIXEL_FORMAT format, unsigned int& oglFormat, int& oglInternalFormat)
    {
	switch(format)
	{
            case FORMAT_R8G8B8:
            case FORMAT_YUY2:
            case FORMAT_UYVY:
            case FORMAT_YV12:
		oglFormat=GL_RGB;
		oglInternalFormat=3;
		return true;

            case FORMAT_B8G8R8:
		oglFormat=GL_BGR_EXT;
		oglInternalFormat=3;
		return true;

            case FORMAT_R8G8B8X8:
		oglFormat=GL_RGBA;
		oglInternalFormat=4;
		return true;

            case FORMAT_B8G8R8X8:
		oglFormat=GL_BGRA_EXT;
		oglInternalFormat=4;
		return true;

            case FORMAT_L8:
		oglFormat=GL_LUMINANCE;
		oglInternalFormat=GL_LUMINANCE8;
		return true;

            case FORMAT_R5G6B5:
                oglFormat = GL_RGB;
                oglInternalFormat = GL_RGB16;
                return true;
            case FORMAT_UNKNOWN:
                oglFormat = GL_RGB;
                oglFormat = GL_RGB;
                return false;
	}

	return false;
    }


}  // namespace openvideo

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
