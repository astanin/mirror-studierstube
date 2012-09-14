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


#ifndef   	_STB_DATATYPES_
#define   	_STB_DATATYPES_


#include <stb/base/macros.h>


BEGIN_NAMESPACE_STB


/// Chosen to be compatible to OpenVideo's pixel formats
enum PIXEL_FORMAT
{
	FORMAT_R8G8B8	= 0,
	FORMAT_B8G8R8	= 1,
	FORMAT_R8G8B8X8	= 2,
	FORMAT_B8G8R8X8	= 3,
	FORMAT_R5G6B5	= 4,
	FORMAT_L8		= 5,

	FORMAT_UNKNOWN	= 6
};


/*struct VIDEO_FRAME
{
	VIDEO_FRAME() : buffer(0)
	{
		width = height = 0;
	}

	PIXEL_FORMAT			format;
	int						width, height;
	const unsigned char*	buffer;
};*/


END_NAMESPACE_STB



#endif // _STB_DATATYPES_
