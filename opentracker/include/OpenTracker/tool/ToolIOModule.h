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
/** header file for special module handling ConsoleSink / ConsoleSource nodes
 * instead of the usuall ConsoleModule.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ToolIOModule.h 1159 2006-06-13 13:40:36Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _TOOLIOMODULE_H
#define _TOOLIOMODULE_H

#include <OpenTracker.h>
#include <QTable.h>

/** This module substitutes the usuall ConsoleModule in an OpenTracker Context
 * and takes over its nodes and configuration. It does not implement ConsoleSources
 * with keyboard events, but will at some event provide a special user interface
 * to do so. Until then it will only output stuff.
 * @author Gerhard Reitmayr
 * @ingroup tool
 */
class ToolIOModule : public Module, public NodeFactory 
{
    // Members
protected:
    /// list of ConsoleSink nodes in the tree
    NodeVector sinks;
    /// list of ConsoleSource nodes  in the tree
    NodeVector sources;
    /// current cycle count, for computing when to print out the event again
    int cycle;
    /// cycle interval to use for printing out events
    int interval;
    /// should the module quit ?
    int quit;
    /// the table displaying the output information
    QTable * output;

public:
    /** constructor method. initializes internal and static data
     * such as the functionMap and keyMap tables. */
    ToolIOModule( QTable * out_ ); 
    /** Destructor method, clears nodes member. */
    virtual ~ToolIOModule();
    /**
     * initializes the tracker module. 
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode * localTree);
    /** This method is called to ruct a new Node. It compares
     * name to the ConsoleSink element name, and if it matches
     * creates a new ConsoleSink node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name, StringTable& attributes);
    /**
     * checks the console for keyboard input and updates any
     * ConsoleSource nodes accordingly. This happens every cycle 
     * and all key presses recorded since are used.
     */
    // virtual void pushEvent();
    /**
     * reads out the ConsoleSink nodes current event an prints it
     * to the console. This is done only each length cylce.
     */
    virtual void pullEvent();
    /**
     * On Unix platforms initializes curses. This method is called after 
     * initialisation is finished and before the main loop is started.*/
    virtual void start();
    /**
     * On Unix platforms closes curses.*/
    virtual void close();
    /**
     * tests whether a key was pressed, if so it stops.
     * @return 1 if main loop should stop, 0 otherwise. */
    virtual int stop();
};

#endif

/* 
 * ------------------------------------------------------------
 *   End of ToolIOModule.h
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
