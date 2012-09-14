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
/** header file for XMLWriter class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: XMLWriter.h 1162 2006-06-14 05:23:30Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _XMLWRITER_H
#define _XMLWRITER_H

#include "../../OpenTracker.h"

#ifndef OT_NO_XMLWRITER_SUPPORT

#include <iostream>
#include <map>
// need that to make it work with standard conformind stdc++ libs and older ones


namespace ot {

    /**
     * This class writes out the contents of the Context as a valid XML file. 
     * It can be used to store a changed tracker graph in a file or some other
     * device accessible by an output stream. It can be reused on a context as
     * often as desired and will allways record the current status of the context,
     * as stored in the XML elements.
     * @author Gerhard Reitmayr
     * @ingroup xml
     */ 
    class OPENTRACKER_API XMLWriter {
    public:
        typedef std::map<std::string, Node *> DefDict;
        DefDict defnodes;
    protected:

        /// the context to write out
        Context & context;
        /// amount of indentation per level
        unsigned int indent;


    public:
        /** constructor, takes a context to work with.
         * @param context_ the context to write out
         * @param indent_ the indentation step to use, default value is 2
         */
        XMLWriter( Context & context_ , unsigned int indent_ = 2);      
    
        virtual ~XMLWriter();
    
        /** writes the tracker graph and configuration to a file given as
         * the parameter. It creates the file if necessary. If the file
         * exists, it will be overwritten by the new configuration data.
         * @param file char array containing the file name. */
        void write( const char * file );
    
        /** writes the tracker graph and configuration to an output stream.
         * @param stream the stream to write to. */
        //void write( ostream & stream );            

        /** returns the current indentation step
         * @return indentation step */
        unsigned int getIndent()
        {
            return indent;
        }

        /** sets the indentation step
         * @param ind the new value */
        void setIndent( unsigned int ind )
        {
            indent = ind;
        }
    };

} // namespace ot


#endif // OT_NO_XMLWRITER_SUPPORT

#endif

/* 
 * ------------------------------------------------------------
 *   End of XMLWriter.h
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
