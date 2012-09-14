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
/** source file for class DifferenceNode.
 *
 * @author Mathis Csisinko
 *
 * $Id: DifferenceNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/common/DifferenceNode.h>


#ifndef OT_NO_DIFFERENCE_SUPPORT


namespace ot {

    void DifferenceNode::onEventGenerated(Event &event,Node &generator)
	{
		insertAtTime(event);
		Event &previousEvent = EventQueueImplementation::getEventNearTime(event.time - timeFrame);
		while (queue.front().time < previousEvent.time)
			queue.pop_front();
		try
		{
			Event localEvent;
			const std::type_info &type = event.getAttributeType(attributeName);
			if (previousEvent.getAttributeType(attributeName) == type)
			{
			  if (type == typeid(bool))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<bool*>(0),attributeName) - previousEvent.getAttribute(static_cast<bool*>(0),attributeName));
			  else if (type == typeid(char))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<char*>(0),attributeName) - previousEvent.getAttribute(static_cast<char*>(0),attributeName));
			  else if (type == typeid(signed char))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<signed char*>(0),attributeName) - previousEvent.getAttribute(static_cast<signed char*>(0),attributeName));
			  else if (type == typeid(unsigned char))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<unsigned char*>(0),attributeName) - previousEvent.getAttribute(static_cast<unsigned char*>(0),attributeName));
			  else if (type == typeid(int))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<int*>(0),attributeName) - previousEvent.getAttribute(static_cast<int*>(0),attributeName));
			  else if (type == typeid(long))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<long*>(0),attributeName) - previousEvent.getAttribute(static_cast<long*>(0),attributeName));
			  else if (type == typeid(short))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<short*>(0),attributeName) - previousEvent.getAttribute(static_cast<short*>(0),attributeName));
			  else if (type == typeid(unsigned int))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<unsigned int*>(0),attributeName) - previousEvent.getAttribute(static_cast<unsigned int*>(0),attributeName));
			  else if (type == typeid(unsigned long))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<unsigned long*>(0),attributeName) - previousEvent.getAttribute(static_cast<unsigned long*>(0),attributeName));
			  else if (type == typeid(unsigned short))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<unsigned short*>(0),attributeName) - previousEvent.getAttribute(static_cast<unsigned short*>(0),attributeName));
			  else if (type == typeid(double))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<double*>(0),attributeName) - previousEvent.getAttribute(static_cast<double*>(0),attributeName));
			  else if (type == typeid(long double))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<long double*>(0),attributeName) - previousEvent.getAttribute(static_cast<long double*>(0),attributeName));
			  else if (type == typeid(float))
			    localEvent.setAttribute(attributeName,event.getAttribute(static_cast<float*>(0),attributeName) - previousEvent.getAttribute(static_cast<float*>(0),attributeName));
			  else if (type == typeid(std::vector<float>))
			    {
			      std::vector<float> &values = event.getAttribute(static_cast<std::vector<float>*>(0),attributeName);
			      std::vector<float> &previousValues = previousEvent.getAttribute(static_cast<std::vector<float>*>(0),attributeName);
			      if (values.size() == previousValues.size())
				{
				  std::vector<float> localValues(values.size());
				  for (std::vector<float>::size_type i = 0;i < localValues.size();i ++)
												  localValues[i] = values[i] - previousValues[i];
				  localEvent.setAttribute(attributeName,localValues);
				}
			      else
				return;
			    }
			  else
			    return;
			  localEvent.timeStamp();
			  updateObservers(localEvent);
			}
			else
			  return;
		}
		catch (std::invalid_argument)
		  {
		    return;
		  }
	}

  void DifferenceNode::pushEvent()
  {
    // nothing to do
  }

  void DifferenceNode::pullEvent()
  {
    // nothing to do
  }
} // namespace ot


#else
#pragma message(">>> OT_NO_DIFFERENCE_SUPPORT")
#endif //OT_NO_DIFFERENCE_SUPPORT

