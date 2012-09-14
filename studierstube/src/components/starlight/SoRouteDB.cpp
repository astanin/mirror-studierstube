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
/** source file for SoRouteDB engine
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoRouteDB.cxx 3596 2004-08-10 13:34:54Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SbTime.h>

#include <stb/components/starlight/SoRouteDB.h>

SO_NODE_SOURCE(SoRouteDB);

void SoRouteDB::initClass()
{
    SO_NODE_INIT_CLASS(SoRouteDB, SoNode, "Node");
}

SoRouteDB::SoRouteDB()
{
    SO_NODE_CONSTRUCTOR(SoRouteDB);

    SO_NODE_ADD_FIELD(name, (""));
    SO_NODE_ADD_FIELD(timeStamp, (FALSE));
    SO_NODE_ADD_FIELD(in, (""));
    SO_NODE_ADD_FIELD(priority, (100));
    SO_NODE_ADD_FIELD(display, (TRUE));

    sensor.setData(this);
    sensor.setFunction(SoRouteDB::inputChanged);
    sensor.attach(&in);
}

SoRouteDB::~SoRouteDB()
{}

void SoRouteDB::inputChanged(void *data, SoSensor *sensor)
{
    SoRouteDB * routeDB = (SoRouteDB *)data;

    if( routeDB->display.getValue())
    {
        printf("%s:", routeDB->name.getValue().getString());

        if( routeDB->timeStamp.getValue())
            printf("%lf:", SbTime::getTimeOfDay().getValue());

        if( routeDB->in.getNum() > 0 )
        {
            int i;
            for( i = 0; i < routeDB->in.getNum()-1; i++)
                printf("%s,", routeDB->in[i].getString());
            printf("%s\n", routeDB->in[i].getString());
        }
        else
            printf("-\n");
    }
    else
    {
        routeDB->in.evaluate();
    }
    if((uint32_t)((SoDelayQueueSensor *)sensor)->getPriority() != routeDB->priority.getValue())
    {
        ((SoDelayQueueSensor *)sensor)->setPriority( routeDB->priority.getValue());
    }
}
