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
/** header file for UltraTrakModule module.
 *
 * @author Rainer Splechtna
 *
 * $Id: UltraTrakModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                    */
/* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section UltraTrakModule UltraTrakModule
 * The UltraTrakModule listens for data from the UltraTrak sent via UDP
 * to a specified port of the host and pushes the event updates into the 
 * tracker tree via @ref UltraTrakSource nodes. 
 * It's configuration element has the following attributes :
 * @li @c port port to listen to
 * @li @c positionMapping defines mapping of position components (x,y,z) from ultratrak to opentracker
 * @li @c orientationMapping defines mapping of quaternion components (x,y,z) from ultratrak to opentracker
 * 0=x 1=y 2=z component of ultratrak
 * @li @c invertPosition defines, which components of the position should be inverted
 * @li @c invertOrientation defines, which components of the quaternion should be inverted
 * 0=don't invert 1=invert component
 *
 * An example configuration element looks like this:
 * @verbatim
 <UltraTrakConfig port="12345"
 positionMapping="1 0 2"	
 orientationMapping="2 0 1"
 invertPosition="0 1 1"
 invertOrientation="0 0 0"
 />@endverbatim
*/

#ifndef _ULTRATRAKMODULE_H
#define _ULTRATRAKMODULE_H

#include "../OpenTracker.h"
#include "UltraTrakSource.h"

#include <ace/SOCK_Dgram.h>

#include <vector>


#ifndef OT_NO_ULTRATRACK_SUPPORT


namespace ot {

    typedef std::vector<UltraTrakSource*> StationVector;

    /// maximum number of stations that can be processed
    const int ultraTrakMaxUnits = 16;

    /// structure of an UltraTrakStationData section
    struct UltraTrakStationData
    {
        char dummy1;
        char boardNumber;
        char updateFlag;
        char stationNumber;
#ifdef ERRORCODE
        char errorCode;
#endif
        char dummy2;
        char position[3][4];
        char orientation[4][4];
    };
	
    /// structure of a whole UltraTrak frame 
    struct UltraTrakFrameData
    {
        char header[56];
        UltraTrakStationData stationData[ultraTrakMaxUnits];
    };


     
    /**
     * The module and factory to drive the reception of UltraTrak event updates.
     * It builds UltraTrakSource nodes that insert data from the UltraTrak into
     * the tracker tree. It uses the UltraTrak Protocol and runs in a thread.
     * 
     * @author Rainer Splechtna
     * @ingroup Network
     */
    class OPENTRACKER_API UltraTrakModule : public ThreadModule, public NodeFactory
    {
        // members
    protected:    
    
	/// vector containing all stations, which have an associated sourceNode
	StationVector stations;
	/// buffer for incoming package
        UltraTrakFrameData buffer;
	/// port to listen for ultratrak packages
        int port;
        int stop;

	int positionMapping[3];
	int invertPosition[3];
	int orientationMapping[3];
	int invertOrientation[3];
    
        // methods
    protected:
        /** Converts num floats from Network byte order.
         * @param floats pointer to source data
         * @param result pointer to result array
         * @param num number of floats to convert
         */
        static void convertFloatsNToHl(float* floats, float* result, int num);
        /** The work method for this module's thread, i.e. waiting for and processing data
	    sent from the UltraTrak server */
        void run();
	int parseVector(const std::string & line, int * val);
	void initMappping(int *mapping);
	void initInversion(int *inversion);
	void calcInversion(int *inversion);
	void correctData(std::vector<float> &d, int *mapping, int *inversion);

    public:    
        /** basic constructor */
        UltraTrakModule();

        /** destructor */
        virtual ~UltraTrakModule();
        /**
         * initializes the UltraTrak module. 
         * @param attributes refenrence to StringTable containing attribute values
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes, ConfigNode * localTree);
        /** This method is called to construct a new Node. It compares
         * name to the UltraTrakSource element name, and if it matches
         * creates a new UltraTrakSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. 
	 */
        virtual Node * createNode( const std::string& name, const StringTable& attributes);        
        /**
         * starts the receive thread. It is called after initialisation is done.
	 */
        virtual void start();
        /**
         * closes the module and closes any communication sockets and stops thread.
	 */
        virtual void close();    
        /**
         * pushes event information into the tree. It checks whether there is new
         * data for any UltraTrakSource node, copies it into the nodes and calls
         * push on them.    
         */
        virtual void pushEvent();             
    };
	OT_MODULE(UltraTrakModule);

} // namespace ot


#endif // OT_NO_ULTRATRACK_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of UltraTrakModule.h
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
