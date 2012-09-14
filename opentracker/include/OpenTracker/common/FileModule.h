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
/** header file for FileModule module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: FileModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section filemodule FileModule
 * The FileModule allows to read or write events from or to files. Every file can
 * contain several input or output streams identified by station numbers. However,
 * a single file can only serve as input or output at any given time. The configuration
 * element @c FileConfig has the following attributes:
 * @li @c append (true|false) default is @c false. denotes whether data should be appended
 * to existing output files or they should be overwritten.
 * @li @c loop (true|false) default is @c false. denotes whether input files should
 * start from the begining (loop is true) or stop after they end.
 * @li @c interval a minimal time interval specified in seconds that should pass between
 * event updates for individual stations. If not specified, events are played as fast
 * as possible.
 * @li @c ot11Format (true|false) default is @c false. Defines whether obsolete OpenTracker
 * 1.1 file format should be used (for testing purposes only!). For a more detailed explanation
 * of this subject, please confer to the documentation of the File class.
 * @li @c realtime (true|false) default is @c false. Defines whether data is played back in real
 * time. This means that succeeding events are played back in the same speed as recorded. Otherwise,
 * events are played back as fast as possible.
 *
 * An example configuration element looks like this :
 * @verbatim
 <FileConfig append="true" loop="true" realtime="true"/>@endverbatim
*/

#ifndef _FILEMODULE_H
#define _FILEMODULE_H

#include "../OpenTracker.h"


#ifndef OT_NO_FILEMODULE_SUPPORT


namespace ot {

    class File;
    class FileSource;

    /**
     * This class manages the files and FileSink and FileSource objects. FileSources are
     * driven by the main loop, whereas FileSinks write directly to the associated file,
     * whenever they receive an event.
     * @ingroup common
     * @author Gerhard Reitmayr
     */
    class OPENTRACKER_API FileModule: public Module, public NodeFactory
    {
        // Members
    protected:
        /// map of name to list of either FileSink or FileSource nodes
        std::map<std::string, NodeVector> nodes;
        /// map of name to File objects
        std::map<std::string, File *> files;
        /// list of file sources
        std::list<FileSource*> sources;
        /// flag whether output files should be appended to or not
        bool append;
        /// flag whether to loop input files or not
        bool loop;
        /// interval time between new events
        double interval;
        /// flag whether to use OT v1.1 compatible file format (for testing purposes only)
        bool ot11Format;
        /// flag whether to play back events in realtime
        bool realtime;
        /// last timestamp
        double lastTime;
        /// local event
        Event event;
        /// the time at which the first event was played back
        double firstPlaybackTime;
        /// the time at which the first event was recorded
        double firstSavedEventTime;

    public:
        /** constructor method. initializes internal and static data
         * such as the functionMap and keyMap tables. */
        FileModule() :
            interval(1000.0),
            realtime(false),
            lastTime(0.0)       
	{}
        /** Destructor method, clears nodes member. */
        virtual ~FileModule();

        inline bool getRealtime() const { return realtime; };
        inline double &getFirstPlaybackTime() { return firstPlaybackTime; };
        inline double getFirstPlaybackTime() const { return firstPlaybackTime; };
        inline double &getFirstSavedEventTime() { return firstSavedEventTime; };
        inline unsigned getSourcesSize() const { return sources.size(); };
        inline bool getLoop() const { return loop; };

        /**
         * initializes the tracker module. This class provides an implementation
         * that sets the initialization flag to true. Subclasses should call this
         * method if they override this method. It takes the attributes of the
         * element configuring this module and a local tree consisting of the
         * children of the element. This tree must be build of Nodes.
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes,  ConfigNode * localTree);

        /** This method is called to construct a new Node. It compares
         * name to the FileSink or FileSource element name, and if it matches
         * creates the necessary File and Node objects.
         * @param name reference to string containing element name
         * @attributes reference to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new !
         */
        virtual Node * createNode( const std::string& name, 
                                   const StringTable& attributes);
        /**
         * reads from the input files and fires new events, if necessary.
         */
        virtual void pushEvent();
        /**
         * Closes the files and cleans up data structures.
         */
        virtual void close();
    };

	OT_MODULE(FileModule);
}  //namespace ot


#endif //OT_NO_FILEMODULE_SUPPORT

#endif


/*
 * ------------------------------------------------------------
 *   End of FileModule.h
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
