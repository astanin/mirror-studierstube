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
/** header file for RouteDB engine
  *
  * @author Gerhard Reitmayr
  *
  * $Id: RouteDB.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoRouteDB_H_
#define _SoRouteDB_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "starlight.h"

/**
 * The RouteDB class is a small utility node to debug field values and routes.
 * Just connect it to a field and get output from the fields changes on the
 * command line. It generates a one line message from the name and in fields, whenever
 * one of these changes. You can also enable to print a timestamp with the message.
 * This class is a tribute to the amazing and powerful CoRouteDebugger in the
 * only VRML IDE for the only systems VRML was ever meant to be used with :).
 *
 * The field display turns actual output to the console on and off. However,
 * the node still evaluates the incoming connection for each notification. This
 * can be useful, if you need to evaluate parts of engine networks constantly,
 * even though they are not attached to anything else. This can happen using
 * more complex networks with SoGate engines etc. The field priority sets the priority
 * of the underlying sensor. If you set this value to 0 then it will print out
 * the value after every single notification ! The priority value will only change
 * after the next notification.
 *
 * File format syntax :
 @verbatim
SoRouteDB {
    SoSFBString name ""
    SoSFBool    timeStamp FALSE
    SoMFString  in    ""
    SoSFBool    display TRUE
    SoSFUInt32  priority 100
}
@endverbatim
 *
 * @ingroup starlight
 * @author Gerhard Reitmayr
 */
class STARLIGHT_API SoRouteDB : public SoNode {

   SO_NODE_HEADER(SoRouteDB);

public:
   // Fields:
    /// flag to print timeStamps or not
    SoSFBool timeStamp;

    /// the name of the RouteDB, will be used in the output
    SoSFString name;

    /// the values of this input will be written in a message, with ',' in between
    SoMFString in;

    /// enable or disable output to the console
    SoSFBool display;

    /// the priority of the underlying field sensor
    SoSFUInt32 priority;

   /** Initializes this class for use in scene graphs. This
    * should be called after database initialization and before
    * any instance of this node is constructed.
    */
   static void initClass();

   /// Constructor
   SoRouteDB();

 private:
   SoFieldSensor sensor;

   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoRouteDB();

   static void inputChanged(void *data, SoSensor *sensor);
};

#endif // _SoRouteDB_H_
