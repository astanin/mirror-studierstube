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
/** header file for class PythonModule.
 *
 * @author Mathis Csisinko
 *
 * $Id: PythonModule.h 2111 2008-01-22 15:03:41Z mathis $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section pythonmodule PythonModule
 * PythonModule drives @ref python nodes featuring scripting capabilities.
 * It loads and executes a Python module containing class definitions used
 * in conjunction with @ref python nodes.
 * The @c PythonConfig configuration element has the following attribute :
 * @li @c module a string identifying the Python code module to load.
 * An example configuration element looks like this :
 *
 * @verbatim
 <PythonConfig module="otpy"/>@endverbatim
 *
 * @note For the Python interpreter it is necessary that the environment
 * variable @c PYTHONPATH points to the locations of all needed modules
 * including the actual Python code module as well as the modules @c ot and
 * @c _ot.
 */

#ifndef _PYTHONMODULE_H
#define _PYTHONMODULE_H

#include "../OpenTracker.h"

#ifdef USE_PYTHON

namespace py {
	#include <Python.h>
}

namespace ot {

	/**
	 * PythonModule is the factory for PythonNode nodes.
	 * 
	 * @ingroup common
	 */

    class OPENTRACKER_API PythonModule: public Module,public NodeFactory
    {
        // methods
    public:
        /// constructor method
        PythonModule();

        /// destructor method
        virtual ~PythonModule();

        /** This method is called to construct a new Node. It compares
         * name to the Python element name, and if it matches creates a
		 * new PythonNode node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL.
         */
        virtual Node* createNode(const std::string &name,const StringTable &attributes);

        /**
         * closes the module.
         */
        //virtual void close();

        /**
         * initializes the PythonModule. 
         * @param attributes StringMap of elements attribute values.
         * @param pLocalTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable &attributes,ConfigNode* pLocalTree);

        // members
    protected:
		py::PyObject* pPyOtModule;
		py::PyObject* pPyModule;
	};

	OT_MODULE(PythonModule);

} // namespace ot


#endif //USE_PYTHON


#endif
