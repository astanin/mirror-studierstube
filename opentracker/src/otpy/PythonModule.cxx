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
/** source file for class PythonModule.
 *
 * @author Mathis Csisinko
 *
 * $Id: PythonModule.cxx 2111 2008-01-22 15:03:41Z mathis $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/otpy/PythonModule.h>

#ifdef USE_PYTHON

#include <OpenTracker/otpy/PythonNode.h>

#define STRINGIFY(identifier) #identifier

using namespace py;

namespace ot {

    OT_MODULE_REGISTER_FUNC(PythonModule){
        OT_MODULE_REGISTRATION_DEFAULT(PythonModule,"PythonConfig");
    }	

	PythonModule::PythonModule(): Module(),NodeFactory(),pPyModule(0),pPyOtModule(0)
	{
		Py_Initialize();
	}

	PythonModule::~PythonModule()
	{
		if (Py_IsInitialized())
		{
			Py_XDECREF(pPyOtModule);
			Py_XDECREF(pPyModule);
			Py_Finalize();
		}
	}

    Node* PythonModule::createNode(const std::string &name,const StringTable &attributes)
    {
        if (name.compare("Python") == 0 && pPyOtModule && pPyModule)
		{
			PyObject* pPyModuleDict = PyModule_GetDict(pPyOtModule);
			if (pPyModuleDict)
			{
				PyObject* pPyBaseClassObject = PyDict_GetItemString(pPyModuleDict,STRINGIFY(PythonNode));
				pPyModuleDict = PyModule_GetDict(pPyModule);
				if (pPyModuleDict)
				{
					PyObject* pPyClassObject = PyDict_GetItemString(pPyModuleDict,attributes.get("class").c_str());
					if (pPyClassObject && PyCallable_Check(pPyClassObject) && PyObject_IsSubclass(pPyClassObject,pPyBaseClassObject))
					{
						PythonNode* pNode = new PythonNode(pPyClassObject);

						logPrintI("Built Python node\n");
						return pNode;
					}
					else
						logPrintI("Python class %s is not a valid class derived from PythonNode.\n",attributes.get("class").c_str());
				}
			}
		}
		return NULL;
	}

    void PythonModule::init(StringTable &attributes,ConfigNode* pLocalTree)
	{
		Module::init(attributes,pLocalTree);
		if (Py_IsInitialized())
		{
			pPyOtModule = PyImport_ImportModule("ot");
			if (PyErr_Occurred())
			{
				PyErr_Print();
				PyErr_Clear();
			}
			if (pPyOtModule)
			{
				pPyModule = PyImport_ImportModule(attributes.get("module").c_str());
				if (PyErr_Occurred())
				{
					PyErr_Print();
					PyErr_Clear();
				}
			}
		}
	}


} // namespace ot

#else
#ifdef WIN32
#pragma message(">>> no Python support")
#endif
#endif //USE_PYTHON
