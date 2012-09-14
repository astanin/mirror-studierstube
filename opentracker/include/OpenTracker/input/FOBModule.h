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
/** header file for FOBModule module.
 *
 * @author Thomas Peterseil, Gerhard Reitmayr
 *
 * $Id: FOBModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section fobmodule FOBModule (Flock of Birds)
 * 
 * The FOBModule is a device driver for the Ascension Flock of Birds tracker. It
 * supports single as well as multi port configurations and the extended range
 * transmitter. It provides the FOBSource source nodes in the data flow graph. 
 * It uses a configuration element called @c FOBConfig which in turn contains one
 * element called @c Bird for each bird in the setup. Only configured birds will
 * be used. 
 * The @c FOBConfig element has the following attributes :
 *
 * @li @c mode ( multi | single ) defines whether the flock is used in single mode
 *         (one serial comm port to the master bird), or multi mode (a serial port
 *         to each bird).
 * @li @c master the number of the master bird
 * @li @c transmitter if the transmitter is attached to another bird than the master,
 *         set the bird number here (i.e. for an ERT)
 * @li @c hemisphere ( forward | rear | upper | lower | left | right ) defines which
 *         hemisphere to set on the flock.
 * @li @c referenceframe a transformation of the position data
 * @li @c xyzframe ( true | false ) a flag defining whether position data should be 
 *        transformed by the referenceframe set. default is @c true.
 * @li @c scale ( 36 | 72 ) set the extend of the receivers
 * 
 * The @c Bird element is used to set the birds used in the setup. If the single
 * mode is used, only the master bird needs a device configured. Otherwise all
 * birds need their serial devices set.
 * The @c Bird element has the following attributes :
 * 
 * @li @c number the number of the bird
 * @li @c dev the device name of the bird (i.e. COM1, /dev/ttyS0)
 * @li @c anglealign  a transformation of the rotatin data to compensate for any 
 *        offset of the sensor itself. 
 *
 * An example configuration element looks like this :
 * @verbatim
 <FOBConfig mode="multi" master="1" scale="72">
 <Bird number="1" dev="COM1"/>
 <Bird number="2" dev="COM2"/>
 </FOBConfig>@endverbatim
 *
 */

#ifndef _FOBMODULE_H
#define _FOBMODULE_H

#include "../OpenTracker.h"
#ifndef OT_NO_FOB_SUPPORT
#include <OpenTracker/core/MathUtils.h>
#include <OpenTracker/misc/serialcomm.h>

#include <map>




namespace ot {
    
    class FOBSource;
    /// the hemisphere the tracker should operate in
    enum Hemisphere { FORWARD, REAR, UPPER, LOWER, LEFT, RIGHT };
    
    /** This class is a datatype helper class for the FOBModule class. 
     *  It stores 
     *  the relevant data for a single bird station and provides buffer 
     * storage and conversion routines.
     * @author Thomas Peterseil */

    class FOBModule;

    class Bird {
    public:

        /// constant to convert inches to meters
        static const float inchesToMeters ; //= 0.0254;

        /// the bird number
        int number;
    
        /// port structure for the serial port data
        SerialPort port;

        /// data buffer for incoming data
        char buffer[20];

        /// number of the current position in the buffer
        int count;

        /// associated source node
        FOBSource * source;

        /// the local event buffer event
        Event event;

        /// flag indicating a new value in event
        bool newVal;

        /// total range of positions
        float scale;

        /// anglealign data
        float angleAlign[3];

        /// constructor 
        Bird( int number_, const std::string & device_, float scale_, float * anglealign_ = NULL ) :
            number( number_ )
        {
            source = NULL;
            count = 0;
            newVal = false;
            scale = scale_ * inchesToMeters / 32767;
            if( anglealign_ != NULL )
            {
                angleAlign[0] = anglealign_[0];
                angleAlign[1] = anglealign_[1];
                angleAlign[2] = anglealign_[2];            
            }
            else 
            {
                angleAlign[0] = 0;
                angleAlign[1] = 0;
                angleAlign[2] = 0;
            }
            event.getConfidence() = 1;
            strncpy( port.pathname, device_.c_str(), 255 );
        }
   /** converts the buffer data and writes it to
         * the local event member */
        inline void convert();

        /** converts data from another buffer and writes it
         * to the local event member. It assumes that data is
         * in the POSITION/QUATERNION format.
         * @param buffer pointer to the char buffer storing
         * the input data. */
        inline void convert( const char * buffer );

        /** reads data from an input buffer into the internal buffer.
         * this method looks for the phasing bit and then starts to read 
         * in data until the input buffer ends, or it has read a whole 
         * frame. If the frame is completed it sets the flag newVal to true.
         * @param buffer input buffer
         * @param len length of the input buffer    
         * @param framesize size of the frame for the current mode
         * @returns the number of chars read from the buffer
         */
        inline int parse( const char * buffer, int len, int framesize );
    
        /** opens the serial port associated with the bird.
         * @return the error value from the serial library. */
        inline int open();

        /** writes data to the serial port associated with the 
         * bird.
         * @param data pointer to the char data to write
         * @param count number of chars to write
         * @return the error value from the serial library. */
        inline int write( const char * data, int count );

        /** reads data from the serial port associated with the 
         * bird.
         * @param data pointer to the char data buffer to read into
         * @param count maximal number of chars to read
         * @return the error value from the serial library. */
        inline int read( char * data, int count );

        /** closes the serial port associated with the bird.
         * @return the error value from the serial library. */
        inline int close();

        /** toggles some lines on the serial interface to 
         * reset the bird. 
         * @return the error value from the serial library. */
        inline int reset();

        // some bird protocol stuff
        inline int setGroupMode( const bool value );

        inline int getErrorCode();

        inline int autoConfig( const int number );

        inline void sleep();

        inline void setReportMode( const int toBird = -1 );

        inline void setScale( const int scale, const int toBird = -1 );

        inline void setXYZFrame( const bool useFrame, const int toBird = -1 );

        inline void setHemisphere( const Hemisphere hemisphere, const int toBird = -1 );

        inline void setAngleAlign( const float * angles, const int toBird = -1 );

        inline void setReferenceFrame( const float * angles, const int toBird = -1 );

        inline int sendReset();
    };


    /**
     * developer level information and implementation specifics here
     *
     * @author 
     * @ingroup input
     */
    class OPENTRACKER_API FOBModule : public ThreadModule, public NodeFactory
    {
        // Members
    protected:
        Hemisphere hemisphere;
        /** the serial line mode used, either multi (each bird connected via a serial) or single
         * (all birds via a single serial port). */ 
        enum Mode { MULTI, SINGLE } mode;

        /// number of the master bird
        int master;
    
        /// number of the erc transmitter, if present
        int transmitter;
    
        /// scale factor 
        float scale;

        /// reference frame values
        float referenceframe[3];

        /// xyzframe flag
        bool useXYZFrame;
    
        /// associative array of Bird data structures
        std::map<int, Bird *> birds;

        /// flag to stop the thread
        bool stop;

        // Methods
    protected:
        /** this method is executed in its own thread and reads data from the flock. */
        virtual void run(); 

    public:
        /** constructor method. */
        FOBModule();

        /** Destructor method, clears nodes member. */
        virtual ~FOBModule();
        /**
         * initializes the tracker module. For each configured tracker it
         * allocates a Bird structure and stores the configuration information.
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes,  ConfigNode * localTree);
        /** This method is called to construct a new Node. It compares
         * name to the FOBSource element name, and if it matches
         * creates a new FOBSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name, const StringTable& attributes);    
    
        /** this method initializes the flock and starts the processing thread. */
        virtual void start();
	/**
         * closes the flock again */
        virtual void close();
        /**
         * pushes events into the tracker tree. Checks all trackers for new data
         * and fires the FOBSources, if new data is present.
         */
        virtual void pushEvent();

    private :
        /** inits the whole flock according to the parameters stored
         * in the module. It can be used to reset the flock after
         * some communication failure.
         * @return 0 if everything is, otherwise the FOB error code of the operation
         *  that went wrong. */
        int initFoB();

        /** resets all birds by toggling the modem lines. Does the right thing 
         * depending on the mode. 
         * @return the FOB error code */
        int resetBirds();

        /** sets the report mode of all birds to POSITION/QUATERNION.
         * @return the FOB error code */
        int setReportMode();

        /** issues the Next Transmitter command to the master bird, if another
         * transmitter is set. This is used to activate an ERC transmitter as well.
         * @return the FOB error code */
        int setNextTransmitter();

        /** starts the stream mode to receive data from the birds.
         * @return the FOB error code */
        int startStreamMode();

        void setHemisphere();

        void setScale();

        void setAngleAlign();

        void setReferenceFrame();
    
        void setXYZFrame();

        friend class Bird;
    };

	OT_MODULE(FOBModule);

} // namespace ot


#endif // OT_NO_FOB_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of Transformation.cxx
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
