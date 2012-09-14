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
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Various template definitions
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: tools.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <map>
#include <cstring>
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>

struct lessName {
public :
    bool operator()( const SbName & n1, const SbName & n2 ) const
    {
        return (n1.getString() < n2.getString());
    }
};


/// A Map containing a Name, a String, and a comparison between the two.
typedef std::map<SbName, SbString, lessName> NameStringMap;

inline bool operator<( const SbString & s1, const SbString & s2)
{
    return (strcmp( s1.getString(), s2.getString()) < 0);
}

#endif //__TOOLS_H__
