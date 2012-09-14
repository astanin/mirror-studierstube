/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** The header file for the EventUtilityNode.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section EventUtilityNode EventUtility
 * The EventUtilityNode provides the possibility to rename, discard, or add attributes
 * to every event passed to this node.
 *
 * The element has the following attributes:
 * @li @c rename an attribute's name and a new name for this attribute separated by a space (' ')
 * @li @c discard the name of an attribute to be discarded
 * @li @c create generic type name, name, and value (separated by spaces) of an attribute that should be added to the event
 *
 * An example element looks like this:
 * @verbatim
 <EventUtilityNode rename="position pos" discard="confidence" create="double doubleAttribute 7.4">
 <Any EventGenerator element type>
 </EventUtilityNode>@endverbatim
*/

#ifndef _EVENTUTILITYNODE_H
#define _EVENTUTILITYNODE_H

#ifndef SWIG
#include <string>
#endif

#include "../OpenTracker.h"


#ifndef OT_NO_EVENTUTITLY_SUPPORT


namespace ot {
  /**
   * The EventUtilityNode provides the possibility to rename, discard, or add attributes
   * to every event passed to this node.
   * @author Jochen von Spiczak
   * @ingroup common
   */
  class OPENTRACKER_API EventUtilityNode : public Node
    {
    protected:
      /// the old name of the attribute to be renamed
      std::string oldName;
      /// the new name of the attribute to be renamed
      std::string newName;
      /// the name of the attribute to be discarded
      std::string discard;
      /// the type of the attribute to be created
      std::string type;
      /// the name of the attribute to be created
      std::string name;
      /// the value of the attribute to be created
      std::string value;

    public:
      /**
       * Returns 1 to show that this node is an event generator.
       * @return 1 because node implements EventGenerator
       */
      virtual int isEventGenerator() { return 1; };
      /**
       * This method changes the incoming event and updates the node's observers.
       * @param event the incoming event
       * @param generator the node that generated the incoming event
       */
      virtual void onEventGenerated(Event &event, Node &generator);
      /**
       * Sets the old and new name of the attribute, which is to be renamed.
       * @param oldName the old name
       * @param newName the new name
       */
      void setRename(const std::string &oldName, const std::string &newName);
      /**
       * Sets the name of the attribute, which is to be discarded.
       * @param discard name of the attribute to be discarded
       */
      void setDiscard(const std::string &discard);
      /**
       * Sets the type, name, and value (encoded in strings) of the attribute, which is to be
       * created.
       * @param type generic type name of the new attribute
       * @param name the name of the new attribute
       * @param value the initial value of the new attribute
       * @return
       */
      void setCreate(const std::string &type, const std::string &name, const std::string &value);

      void pushEvent();
      void pullEvent();

      friend class CommonNodeFactory;
    };

} // namespace ot


#endif //OT_NO_EVENTUTITLY_SUPPORT


#endif
