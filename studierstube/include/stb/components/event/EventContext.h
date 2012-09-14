/* ======================================================================== 
* Copyright (C) 2006  Graz University of Technology
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
* Dieter Schmalstieg
* <schmalstieg@icg.tu-graz.ac.at>
* Graz University of Technology, 
* Institut for Computer Graphics and Vision,
* Inffeldgasse 16a, 8010 Graz, Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** Header file for the EventContext
* 
* @author Gerhard Reitmayr
*
* @file                                                                   */
/* ======================================================================= */

#ifndef _EVENTCONTEXT_H_
#define _EVENTCONTEXT_H_

#include "event.h"
#include "tools.h"

class EVENT_API EventContext
{
protected:

	NameStringMap data;

    static SbString emptyString;

public:
	EventContext(void);
	virtual ~EventContext();

	const SbString & get(const SbName & name)
    {
        NameStringMap::iterator it = data.find( name );
        if(  it != data.end())
            return (*it).second;
        return EventContext::emptyString;
    }

    void set( const SbName & name, const SbString & string )
    {
        data[name] = string;
    }

    bool containsKey( const SbName & name )
    {
        return( data.find(name) != data.end());
    }

    const NameStringMap & getData(void) const
    {
        return data;
    }
};

#endif //_EVENTCONTEXT_H_
