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
/** header file for CommonNodeFactory class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: CommonNodeFactory.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _COMMONNODEFACTORY_H
#define _COMMONNODEFACTORY_H

#include "../dllinclude.h"
#include "../OpenTracker.h"
#include "EventQueueNode.h"

/**
 * @defgroup common Common Classes
 * This group contains classes that implement general functionality that 
 * can be used by all OpenTracker applications. It includes nodes for
 * geometric transformations, event merging, the ConsoleModule for 
 * console based input / output among others.
 */

/**
 * This class provides the general node types found in the configuration files.
 * The following nodes are build by this factory :
 * \li Transformation, for all types of transformations
 * \li VirtualTransformation, again all types of transformations
 * \li InvertTransformation, all types
 * \li MatrixTransformation, all types
 * \li EventQueueNode
 * \li MergeNode
 * \li TriangulateOrientationNode
 * \li SelectionNode
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API CommonNodeFactory : public NodeFactory
    {

        // members 
    protected:
        /// stores names of known NodePorts, to make instantiation more generic
        std::vector<std::string> nodePorts;
		static std::vector<std::string> knownNodes;
    protected:
    
        /** parses a string for a three component vector, returns int value
         * indicating an error. If an error occurs, the val values are not changed.
         * @param line string to parse
         * @param val pointer to three floats
         * @return 0 if ok, other value if error occured
         */
        static int parseVector( const std::string & line, float * val );
        /** parses a string for a rotation, where the type is defined by
         * a second string, returns int value indicating an error. If an error occurs, 
         * the val values are not changed.
         * @param line string to parse
         * @param type string defining type of rotation (quaternion|matrix|axisangle|euler)
         * @param val pointer to four floats, rotation will be stored in quaternion format
         * @return 0 if ok, other value if error occured
         */
        static int parseRotation( const std::string & line, const std::string & type, float * val );

    public:
        /** constructor method. Initalizes the wrapperNodes array.*/
        CommonNodeFactory();
        /** destructor */
        virtual ~CommonNodeFactory();
        /** creates the described node types.
         * @param name reference to string containing element name
         * @param attributes reference to StringMap of elements attribute values
         * @return new Node or NULL, if element name was not recognized */
        virtual Node * createNode( const std::string& name,  const StringTable& attributes);
		static bool isKnownNode(std::string nodeName);
        friend class StaticTransformation;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of CommonNodeFactory.h
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
