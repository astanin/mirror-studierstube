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
/** source file for class PythonNode.
 *
 * @author Mathis Csisinko
 *
 * $Id: PythonNode.cxx 1956 2007-08-13 13:23:03Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/otpy/PythonNode.h>

#ifdef USE_PYTHON

namespace py
{
#include "../otpy/ot_wrap.h"
}

#define STRINGIFY(identifier) #identifier
#define SWIG_TYPE(type) STRINGIFY(type)

using namespace py;

namespace ot {

	PythonNode::PythonNode(py::PyObject* pPyClassObject): Node(),pPyObject(0),pPyOnEventGeneratedObject(0),pPyPushEventObject(0)
    {
		if (pPyClassObject && PyCallable_Check(pPyClassObject))
		{
			PyObject* pPyTuple = Py_BuildValue("(O)",SWIG_NewPointerObj(this,SWIG_TypeQuery(SWIG_TYPE(ot::PythonNode*)),0));
			pPyObject = PyObject_CallObject(pPyClassObject,pPyTuple);
			Py_XDECREF(pPyTuple);
			if (PyErr_Occurred())
			{
				PyErr_Print();
				PyErr_Clear();
			}
			if (pPyObject && PyObject_IsInstance(pPyObject,pPyClassObject))
			{
				pPyOnEventGeneratedObject = PyObject_GetAttrString(pPyObject,STRINGIFY(onEventGenerated));
				pPyPushEventObject = PyObject_GetAttrString(pPyObject,STRINGIFY(pushEvent));
				PyErr_Clear();
			}
		}
	}

	PythonNode::~PythonNode()
    {
		Py_XDECREF(pPyObject);
		Py_XDECREF(pPyOnEventGeneratedObject);
		Py_XDECREF(pPyPushEventObject);
	}

    void PythonNode::onEventGenerated(Event &event,Node &generator)
	{
		if (pPyOnEventGeneratedObject && PyCallable_Check(pPyOnEventGeneratedObject))
		{
			PyObject* pPyTuple = Py_BuildValue("(OO)",SWIG_NewPointerObj(&event,SWIG_TypeQuery(SWIG_TYPE(ot::Event*)),0),SWIG_NewPointerObj(&generator,SWIG_TypeQuery(SWIG_TYPE(ot::Node*)),0));
			Py_XDECREF(PyObject_CallObject(pPyOnEventGeneratedObject,pPyTuple));
			Py_XDECREF(pPyTuple);
			if (PyErr_Occurred())
			{
				PyErr_Print();
				PyErr_Clear();
			}
		}
	}

    void PythonNode::pushEvent()
	{
		if (pPyPushEventObject && PyCallable_Check(pPyPushEventObject))
		{
			Py_XDECREF(PyObject_CallObject(pPyPushEventObject,NULL));
			if (PyErr_Occurred())
			{
				PyErr_Print();
				PyErr_Clear();
			}
		}
	}
} // namespace ot

#else
#ifdef WIN32
#pragma message(">>> no Python support")
#endif
#endif //USE_PYTHON
