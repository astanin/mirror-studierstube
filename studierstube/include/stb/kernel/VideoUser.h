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
 * @author Daniel Wagner
 *  
 * $Id$
 * @file                                                                   */  
/* ======================================================================= */  


#ifndef _STB_VIDEOUSER_H_
#define _STB_VIDEOUSER_H_

#include <stb/base/OS.h>
#include <stb/kernel/Studierstube.h>
#include <stb/base/macros.h>
#include <stb/base/datatypes.h>
#include <vector>


namespace openvideo {
	class Buffer;
};


BEGIN_NAMESPACE_STB


class VideoUser
{
public:
	enum UPDATE_MODE {
		UPDATE_IMMEDIATELY = 1,
		UPDATE_BEFORE_RENDER = 2
	};

    virtual ~VideoUser()
	{}

    /// Called before the first frame arrives.
    /**
     *  The video frame setup is guarantied to never change.
     */
    virtual void vu_init(const openvideo::Buffer& frame, stb::string *givenSinkName) = 0;


    /// Called when a new video frame is available.
    virtual void vu_update(const openvideo::Buffer& frame, stb::string *givenSinkName, bool forceUpdate = false) = 0;


	virtual UPDATE_MODE vu_getUpdateMode() const = 0;
};


typedef std::vector<VideoUser*> VideoUserVector;


END_NAMESPACE_STB


#endif//_STB_VIDEOUSER_H_
