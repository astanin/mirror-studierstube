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
/** header file for FastTrakModule module.
 *
 * @author Rainer Splechtna
 *
 * $Id: FastTrakModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section fasttrakmodule FastTrakModule (supports Polhemus FastTrak and IsoTrak II)
 *
 * This module provides and drives @ref FastTrakSource nodes that 
 * generate standard events.
 * 
 * The configuration element is called @c FastTrakConfig and has the following 
 * attributes :
 * @li @c type "fasttrak" specifies whether "fasttrak" or "isotrak" is used
 * @li @c device the serial port the tracker is connected to. 
 * @li @c stations number of stations connected to the tracker
 * @li @c hemisphere "1,0,0" the hemisphere the tracker should operate in 
 * @li @c reference-frame "0,0,0,200,0,0,0,200,0" the reference frame of the tracker 
 * @li @c init-string "" other init strings, which will be used to configure 
 *        the tracker at startup (e.g. "x0.9,0.6,1.0,0.9\rv0.3,0.6,0.9,0.9\r")
 *
 * An example configuration element looks like this :
 * @verbatim
 <FastTrakConfig type="isotrak" device="COM1" stations="2" hemisphere="-1,0,0"/>
 @endverbatim
 *
 * Note that to date this driver was only tested with IsoTrak II and WIN32.
 */

#ifndef _FASTTRAKMODULE_H
#define _FASTTRAKMODULE_H

#include "../OpenTracker.h"
#include "../misc/serialcomm.h"

#include <vector>


#ifndef OT_NO_FASTTRACK_SUPPORT


namespace ot {

    typedef std::vector<Node::Ptr> NodeVector;

    /**
     * developer level information and implementation specifics here
     *
     * @author Rainer Splechtna
     * @ingroup input
     */
    class OPENTRACKER_API FastTrakModule : public ThreadModule, public NodeFactory
    {
        // Members
    protected:

        /// flag to stop the thread
        int stop;

        /// vector of all created fasttrak source nodes 
        NodeVector nodes;

        /// stores which tracker 
        int trackerType;
        /// port structure for the serial port data
        SerialPort port;
        /// number of stations (sensors) connected to the tracker 
        int numberOfStations;
        /// the hemisphere the tracker should operate in ("1,0,0")
        std::string hemisphere;
        /// the reference frame of the tracker ("0,0,0,200,0,0,0,200,0")
        std::string referenceFrame;
        /// other global config strings sent to the tracker during init-phase
        std::string initString;

        // Methods
    protected:
        /** this method is the code executed in its own thread, it reads from the 
         *  specified serial port, parses the incoming data and updates the event
         *  of the utilized stations */
        virtual void run();

        /** inits the whole fasttrak according to the parameters stored
         * in the module. It can be used to reset the flock after
         * some communication failure.
         * @return 0 if everything is ok, otherwise < 0 */
        int initFastTrak();
        /** parses the input stream from the FastTrak tracker
         * @param c new charcter read from input stream
         * @param b pointer to char array, where the actual data 
         *          record of a station of the tracker is stored
         * @return -1 if reading of one data record is not completed 
         *         -2 if too much junk was read
         *          0,1 number of station for which a whole data record was read
         */
        int parseRecordFT(char c, char *b);
        /** parses the input stream from the IsoTrak II tracker
         * @param c new charcter read from input stream
         * @param b pointer to char array, where the actual data 
         *          record of a station of the tracker is stored
         * @return -1 if reading of one data record is not completed 
         *         -2 if too much junk was read
         *          0,1 number of station for which a whole data record was read
         */
        int parseRecordIT(char c, char *b);
        /** updates button event for specified station (only used for IsoTrak II)
         * @param stationNr number of station the button is associated with
         * @param button event of button
         */
        void setButtonIT(int stationNr, int button);
        /** converts data record to OpenTracker event values and updates 
         * the event of the specified station.
         * @param stationNr number of station the data record is associated with
         * @param b pointer to array of characters, representing a whole data record
         */
        void convert(int stationNr, char *b);

    public:
        /** constructor method. */
        FastTrakModule();

        /** Destructor method, clears nodes member. */
        virtual ~FastTrakModule();
        /**
         * initializes the tracker module. 
         * @param attributes StringTable of elements attribute values. 
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes,  ConfigNode * localTree);
        /** This method is called to construct a new Node. It compares
         * name to the FastTrakSource element name, and if it matches
         * creates a new FastTrakSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name, const StringTable& attributes);    
    
        virtual void start();
	/**
         * closes serial port */
        virtual void close();
        /**
         * pushes events into the tracker tree. Checks all stations for new data
         * and fires the FastTrakSources, if new data is present.
         */
        virtual void pushEvent();
    };
	OT_MODULE(FastTrakModule);

} // namespace ot


#endif // OT_NO_FASTTRACK_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of FastTrakModule.h
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
