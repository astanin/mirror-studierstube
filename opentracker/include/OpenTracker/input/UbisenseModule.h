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
/** header file for the Ubisense interface module.
 *
 * @author Mathis Csisinko
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section ubisensemodule UbisenseModule
 * The UbisenseModule interfaces to the Ubisense API to track the position and
 * button event of Ubisense tags. It uses the information provided by @ref
 * ubisensesource nodes and inserts the eventevents through @ref
 * ubisensesource nodes into the tree. The configuration element is @c
 * UbisenseConfig.
 * Without the @c UbisenseConfig configuration element data from any cell is
 * tracked. But as soon as a @c UbisenseConfig configuration element is present,
 * data retrieval is restricted: A single @c UbisenseCell configuration element
 * identifies a Ubisense location cell to retrieve data from. Use more than one
 * @c UbisenseCell configuration element to retrieve data from any number of
 * cells. The @c UbisenseCell configuration element has the following attribute:
 * @li @c name a string identifying the Ubisense location cell.
 *
 * An example configuration element looks like this :
 * @verbatim
 <UbisenseConfig>
 <UbisenseCell name="Location Cell 00001" />
 </UbisenseConfig>@endverbatim
*/

#ifndef _UBISENSEMODULE_H
#define _UBISENSEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_UBISENSE

#include "UClientAPI/location_client.h"
#include "UClientAPI/data_client.h"

using namespace UClientAPI;

namespace ot {

    /**
     * A Ubisense tracking source module using the Ubisense API to track Ubisense
     * tags. It acts as a NodeFactory for UbisenseSource nodes. It also keeps a
     * vector of the created nodes to update them.
     *
     * @ingroup input
     */

    class OPENTRACKER_API UbisenseModule: public Module,public NodeFactory
    {
        // methods
    public:
        /// constructor method
        UbisenseModule();

        /// destructor method
        virtual ~UbisenseModule();

        /** This method is called to construct a new Node. It compares
         * name to the UbisenseSource element name, and if it matches
         * creates a new UbisenseSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL.
	 */
        virtual Node* createNode(const std::string &name,StringTable &attributes);

        /**
         * This method is called after initialisation is finished and before the
         * main loop is started.
	 */
        virtual void start();

        /**
         * closes the module.
	 */
        virtual void close();

        /**
         * pushes events into the tracker tree. Checks all source nodes for
         * new events and pushes them into the tracker tree.
         */
        virtual void pushEvent();

        /**
         * initializes the Ubisense module. 
         * @param attributes StringMap of elements attribute values.
         * @param pLocalTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable &attributes,ConfigNode* pLocalTree);

        // members
    protected:
        /// stores the sources
        NodeVector pSources;

    private:
	class WrappedDataClient: public DataClient
        {
        public:
            WrappedDataClient(NodeVector &);

            virtual void on_button(const Object &,UbitagButton,double);

        private:
            NodeVector &pSources;
        };

        LocationClient locationClient;
        WrappedDataClient dataClient;
        UClientAPI::Set<String> cells; 
        //std::set<std::string> cells;
	OT_MODULE(UbisenseModule);

} // namespace ot

#endif  // USE_UBISENSE

#endif

/* 
 * ------------------------------------------------------------
 *   End of UbisenseModule.h
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
