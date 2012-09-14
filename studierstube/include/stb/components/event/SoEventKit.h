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
/** Header file for SoEventKit Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoEventKit.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOEVENTKIT_H__
#define __SOEVENTKIT_H__

#include <Inventor/nodekits/SoBaseKit.h>

#include "event.h"

class SoEventAction;

/// An abstract class for creating Event aware nodekits
class EVENT_API SoEventKit : public SoBaseKit
{
    SO_KIT_HEADER(SoEventKit);

public:
    SoEventKit(void);
    static void initClass(void);

	/// Derived classes should implement this function
    virtual void preOrder( SoEventAction * )
    {
    };

	/// Derived classes should implement this function
    virtual void postOrder( SoEventAction *)
    {
    };
};

#endif //__SOEVENTKIT_H__
