/* ========================================================================
 * Copyright (C) 2004-2005  Graz University of Technology
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
 * Graz University of Technology, Institut für Maschinelles Sehen und Darstellen,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: 
 * ======================================================================== */
/** @author   Daniel Wagner
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */


#ifndef _PLATFORM_TOOLS_HEADERFILE_
#define _PLATFORM_TOOLS_HEADERFILE_

#include <string.h>

inline const unsigned short* convertCharToUnicode(const char* nStr)
{
	static size_t wlen = 0;
	static unsigned short* wstr=0;

	size_t i, len = strlen(nStr);

	if(len>wlen)
	{
		delete wstr;
		wlen = len;
		wstr = new unsigned short[wlen+1];
	}

	for(i=0; i<len; i++)
		wstr[i] = (unsigned short)nStr[i];
	wstr[len] = 0;

	return wstr;
}


inline const char* convertUnicodeToChar(const unsigned short* nWStr)
{
	static size_t len = 0;
	static char* str=0;

	size_t i, wlen = wcslen((wchar_t*)nWStr);

	if(wlen>len)
	{
		delete str;
		len = wlen;
		str = new char[len+1];
	}

	for(i=0; i<wlen; i++)
		str[i] = (char)nWStr[i];
	str[wlen] = 0;

	return str;
}



#if (_MSC_VER<1300) && defined(_WIN32_WCE)			// eVC4

#  define NATIVE_TO_CHAR(str) convertUnicodeToChar(str)
#  define CHAR_TO_NATIVE(str) convertCharToUnicode(str)
#  define CHAR_TO_DLLFUNC_STR(str) convertCharToUnicode(str)

#endif



#if (_MSC_VER<1300) && !defined(_WIN32_WCE)			// VC6

#  define NATIVE_TO_CHAR(str) str
#  define CHAR_TO_NATIVE(str) str
#  define CHAR_TO_DLLFUNC_STR(str) str

#endif



#if (_MSC_VER>=1400) && !defined(_WIN32_WCE)		// VC8.XP

#  define NATIVE_TO_CHAR(str) str
#  define CHAR_TO_NATIVE(str) str
#  define CHAR_TO_DLLFUNC_STR(str) str

#endif



#if (_MSC_VER>=1400) && defined(_WIN32_WCE)		// VC8.CE

#  define NATIVE_TO_CHAR(str) convertUnicodeToChar(str)
#  define CHAR_TO_NATIVE(str) (const wchar_t*)convertCharToUnicode(str)
#  define CHAR_TO_DLLFUNC_STR(str) (const wchar_t*)convertCharToUnicode(str)

#endif

#endif //_PLATFORM_TOOLS_HEADERFILE_
