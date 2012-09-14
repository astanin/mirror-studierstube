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


#ifndef _STB_KERNELEVENTSUBSCRIBER_H_
#define _STB_KERNELEVENTSUBSCRIBER_H_


#include <stb/base/macros.h>
#include <vector>


BEGIN_NAMESPACE_STB


/// Interface to 
class  KernelEventSubscriber
{
public:
	/// Virtual destructor to prevent warnings with GCC
    virtual ~KernelEventSubscriber()  {}

    /// Called by the kernel just before scene graph rendering starts
	virtual void kes_beforeRender()  {}

	/// Called by the kernel just after scene graph rendering ended
	virtual void kes_afterRender()  {}
};


typedef std::vector<KernelEventSubscriber*> KernelEventSubscriberVector;


END_NAMESPACE_STB


#endif//_STB_KERNELEVENTSUBSCRIBER_H_
