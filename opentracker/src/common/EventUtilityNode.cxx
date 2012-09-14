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
/** The source file for the EventUtility node.
 *
 * @author Jochen von Spiczak
 *
 * $Id: EventUtilityNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/common/EventUtilityNode.h>

#ifndef OT_NO_EVENTUTITLY_SUPPORT


namespace ot
{
    // set old and new name for attribute renaming
    void EventUtilityNode::setRename(const std::string &oldName, const std::string &newName)
    {
        this->oldName = oldName;
        this->newName = newName;
    }

    // set name of attribute to be discarded
    void EventUtilityNode::setDiscard(const std::string &discard)
    {
        this->discard = discard;
    }

    // set type, name, and value of new attribute to be created
    void EventUtilityNode::setCreate(const std::string &type, const std::string &name, const std::string &value)
    {
        //normalize the type naming
        this->type = type;
        // it is impossible to write < or > in xml, so the parser requires
        // writing &lt instead, but c++ does not understand this. In order to 
        // use an independent mapping, we need to specify the type of templated
        // datastructures in another way. The [] is for such purpose.
        //        logPrintI("Event utilitynode using type %s\n", type.c_str());
        std::string::size_type left_bracket = this->type.find_first_of("[");
        std::string::size_type right_bracket = this->type.find_last_of("]");
        if ((left_bracket != std::string::npos) && (right_bracket != std::string::npos)){
            this->type[(int)left_bracket] = '<';
            this->type[(int)right_bracket] = '>';
        }
        //        logPrintI("Event utilitynode created type %s\n", (this->type.c_str()));
        this->name = name;
        this->value = value;
    }

    // rename, discard, or create attributes and update observers
    void EventUtilityNode::onEventGenerated(Event &event, Node &generator)
    {
		
		
        bool success = false;
		
        if (oldName.length() > 0)
            success |= event.renAttribute(oldName, newName);
        if (discard.length() > 0)
            success |= event.delAttribute(discard);
		if (type.length() > 0){
			// this is necessary in order to updateObservers 
                        // even when reusing an event
			// to which the attribute has already been added
			success |=event.hasAttribute(name);
			if (!success)
				success |= event.addAttribute(type, name, value);
		
		}
        if (success)
            updateObservers(event);


    }

    void EventUtilityNode::pushEvent()
    {
        // nothing to do
    }

    void EventUtilityNode::pullEvent()
    {
        // nothing to do
    }
} // namespace ot


#else
#pragma message(">>> OT_NO_EVENTUTITLY_SUPPORT")
#endif //OT_NO_EVENTUTITLY_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of EventUtilityNode.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
