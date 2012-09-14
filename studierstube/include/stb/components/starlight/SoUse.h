/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoUse property node header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoUse.h 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOUSE_H_
#define _SOUSE_H_

#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/nodes/SoGroup.h>

#include "starlight.h"

class SoFieldSensor;

/**  
 * SoUse is a group node that mimicks the use of USE in the file format. However, it is 
 * based on the use of SoNode::getByName() and can hold more than one reference. It
 * is configured with a set of names in the field name that are use to retrieve references to
 * nodes via SoNode::getByName(). It adds them as children to the node and also
 * exports the references via the field node. By using the global name hash it can be 
 * used across multiple files as well.
 * 
 * File format :
@code
SoUse {
  MFName name []
  MFNode node []
}
@endcode
 *
 * @author Gerhard Reitmayr
 * @ingroup util
 */
class STARLIGHT_API SoUse : public SoGroup
{
    SO_NODE_HEADER(SoUse);

public:

    /// the def names to find the nodes
    SoMFName name;
    /// the found nodes, NULL if a node was not found
    SoMFNode node;
    
    /// static class initialization method of OpenInventor
    static void initClass();
    
    /// Constructor
    SoUse();

protected:
    virtual void write(SoWriteAction * action);

    SoFieldSensor * nameSensor;

    static void nameChanged( void * data, SoSensor * sensor);

    /// destructor
    virtual ~SoUse();
};

#endif
