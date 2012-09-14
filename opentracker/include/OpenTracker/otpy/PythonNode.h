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
/** header file for class PythonNode.
 *
 * @author Mathis Csisinko
 *
 * $Id: PythonNode.h 1890 2007-06-12 14:44:57Z mathis $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section python Python
 * PythonNode is the interface to Python. In order to script a custom Python
 * node a Python class has to be derived from @c PythonNode. This class resides
 * in a module which has to be configured properly by @ref pythonmodule.
 * Apart from the instantiation method (@c __init__), @c pushEvent and
 * @c onEventGenerated are relevant in this context. On presence of
 * @c onEventGenerated, this method is called every time an event is generated
 * from any child. If there's a @c pushEvent method, it is executed in each
 * pass of the main loop.
 * The PythonNode element has the following attributes :
 * @li @c class name of the associated Python class
 *
 * An example element looks like this :
 * @verbatim
 <Python class="Python">
 <Any EventGenerator element type>
 </Python>@endverbatim
*/

#ifndef _PYTHONNODE_H
#define _PYTHONNODE_H

#include "../OpenTracker.h"

#ifdef USE_PYTHON

namespace py {
	#include <Python.h>
}

namespace ot {

	/**
	 * With PythonNode, custom node functionality can be expressed in the
	 * Python language.
	 * 
	 * @ingroup common
	 */

    class OPENTRACKER_API PythonNode: public Node
    {
        friend class PythonModule;

        // Members
    protected:
		py::PyObject* pPyObject;
		py::PyObject* pPyOnEventGeneratedObject;
		py::PyObject* pPyPushEventObject;

        // Methods
    protected:
        /** simple constructor, sets members to initial values
         * @param pPyObject associated Python class object */
        PythonNode(py::PyObject* pPyObject);

		virtual ~PythonNode();
	public:    
        /** tests for EventGenerator interface being present. Overriden in this
         * node to return this.
         * @return always this */
        virtual int isEventGenerator()
		{
            return 1;
		}
        /** this method is called by the EventGenerator to update it's observers
         * @param event new event
         * @param generator the calling EventGenerator */
        virtual void onEventGenerated(Event &event,Node &generator);

        /**
         * pushes events into the tracker tree.
         */
		virtual void pushEvent();
	};

} // namespace ot


#endif //USE_PYTHON


#endif
