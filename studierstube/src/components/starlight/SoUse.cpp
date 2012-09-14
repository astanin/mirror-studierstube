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
/** SoUse source file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoUse.cpp 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/sensors/SoFieldSensor.h>

#include <stb/components/starlight/SoUse.h>

SO_NODE_SOURCE(SoUse);

void SoUse::initClass()
{
    SO_NODE_INIT_CLASS(SoUse, SoGroup, "Group");
}

// constructor

SoUse::SoUse()
{
    SO_NODE_CONSTRUCTOR(SoUse);

    SO_NODE_ADD_FIELD(name, (""));
    SO_NODE_ADD_FIELD(node, (NULL));

    name.deleteValues(0);
    name.setDefault(TRUE);
    node.deleteValues(0);
    node.setDefault(TRUE);
    isBuiltIn = TRUE;   

    nameSensor = new SoFieldSensor( SoUse::nameChanged, this);    
    nameSensor->attach(&name);
}

SoUse::~SoUse()
{
    delete nameSensor;
}

void SoUse::write(SoWriteAction *action)
{
    SoNode::write(action);
}

void SoUse::nameChanged( void * data, SoSensor * /*sensor*/ )
{
    assert(data != NULL);
    assert(((SoNode *)data)->isOfType(SoUse::getClassTypeId()));
    SoUse * self = (SoUse *)data;
    self->removeAllChildren();
    self->node.setNum(self->name.getNum());
    for( int i = 0; i < self->name.getNum(); i++)
    {
        SoNode * node = SoNode::getByName(self->name[i]);
        self->node.set1Value(i, node);
        if(node != NULL)
        {
            self->addChild(node);
        }
    }
}
