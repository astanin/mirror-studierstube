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
/** header file for StringTable class and helper classes.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: StringTable.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _STRINGTABLE_H
#define _STRINGTABLE_H

#include "../dllinclude.h"
#include "Event.h"

#ifndef SWIG
#include<string>
#include<vector>
#include<map>
#endif

#ifdef USE_LIVE
#ifndef SWIG
#include <OpenTracker/skeletons/OTGraph.hh>
#endif
#endif

typedef std::map<std::string,std::string> StringMap;

/**
 * This class implements a string to string mapping similar to a Hashtable in
 * Java. It is used to work around a problem in VC++ with passing STL maps 
 * between methods in different dlls. This way it cleans up the API, because
 * now all dependencies on the STL are removed from the OpenTracker API.
 * @author Gerhard Reitmayr
 * @ingroup core
 */ 

namespace ot {

    class OPENTRACKER_API StringTable {
    protected:
        /// the underlying STL map implementing the functionality
        StringMap map;
    public:
        /** constructor */
        StringTable();
        /** copy constructor, builds a new table by copying another
         * @param table StringTable to copy from
         */
        StringTable( const StringTable & table );
#ifdef USE_LIVE
        StringTable( const OTGraph::StringTable& st ) ;
#endif
        /** destructor */
        ~StringTable();
        /** returns a value stored in the map
         * @param key the key the value is for 
         * @returns the string value or an empty string, if the key is not found */
#ifdef USE_LIVE
        OTGraph::StringTable* getStringTable();
#endif
        const std::string & get( const std::string & key ) const;
        /** stores a key value pair in the table, overwritting a possible prior 
         * value
         * @param key the key to store it under
         * @param value the value to store */
        void put( const std::string &key, const std::string &value );
        /**
         * removes a key value pair from the table
         * @param key the key to the pair */
        void remove( const std::string &key );
        /**
         * tests, whether the table contains a certain key
         * @param the key to test for */
        int containsKey( const std::string & ) const;
        /** returns the number of pairs stored 
         * @returns unsigned with the number of pairs */
        unsigned size() const;
 
        /** 
         * sets an int value, the value is converted to a string and stored
         * under the given key.
         * @param key key string of the entry
         * @param value the integer value to store
         */
        void put(const std::string & key, const int value);
        /** 
         * sets a float value, the value is converted to a string and stored
         * under the given key.
         * @param key key string of the entry
         * @param value the floating point value to store
         */
        void put(const std::string & key, const float value);
        /** 
         * sets a double value, the value is converted to a string and stored
         * under the given key.
         * @param key key string of the entry
         * @param value the double precision value to store
         */
        void put(const std::string & key, const double value);

        /** 
         * sets an array of int values. It is converted to a string where the
         * values are separated by spaces and stored
         * under the given key.
         * @param key key string of the entry
         * @param value pointer to the array of integer values to store
         * @param len the length of the array
         */    
        void put(const std::string & key, const int * value, int len);
        /** 
         * sets an array of float values. It is converted to a string where the
         * values are separated by spaces and stored
         * under the given key.
         * @param key key string of the entry
         * @param value pointer to the array of floating point values to store
         * @param len the length of the array
         */
        void put(const std::string & key, const float * value, int len);
        /** 
         * sets an array of double values. It is converted to a string where the
         * values are separated by spaces and stored
         * under the given key.
         * @param key key string of the entry
         * @param value pointer to the array of double values to store
         * @param len the length of the array
         */
        void put(const std::string & key, const double * value, int len);

        /**
         * parses a stored entry into an array of integers. It assumes that the
         * integers are separated by spaces. It returns the number of actually
         * converted integers.
         * @param key the key of the entry to parse
         * @param value pointer to the array of integers to store the parsed values 
         * @param len the length of the array, default 1 to use it for a single int only
         * @return number of actually parsed values
         */
        int get(const std::string & key, int * value, int len = 1 ) const;
        /**
         * parses a stored entry into an array of floats. It assumes that the
         * floats are separated by spaces. It returns the number of actually
         * converted floats.
         * @param key the key of the entry to parse
         * @param value pointer to the array of integers to store the parsed values 
         * @param len the length of the array, default 1 to use it for a single float only
         * @return number of actually parsed values
         */
        int get(const std::string & key, float * value, int len = 1 ) const;
        /**
         * parses a stored entry into a vector of floats. It assumes that the
         * floats are separated by spaces. It returns the number of actually
         * converted floats.
         * @param key the key of the entry to parse
         * @param reference to the vector of floats to store the parsed values 
         * @param len the length of the array, default 1 to use it for a single float only
         * @return number of actually parsed values
         */
        int get(const std::string & key, std::vector<float> & vector, int len = 1 ) const;
        /**
         * parses a stored entry into an array of doubles. It assumes that the
         * doubles are separated by spaces. It returns the number of actually
         * converted doubles.
         * @param key the key of the entry to parse
         * @param value pointer to the array of doubles to store the parsed values 
         * @param len the length of the array, default 1 to use it for a single double only
         * @return number of actually parsed values
         */
        int get(const std::string & key, double * value, int len = 1 ) const;
        /**
         * parses a stored entry into a vector of doubles. It assumes that the
         * doubles are separated by spaces. It returns the number of actually
         * converted doubles.
         * @param key the key of the entry to parse
         * @param reference to the vector of double to store the parsed values 
         * @param len the length of the array, default 1 to use it for a single double only
         * @return number of actually parsed values
         */
        int get(const std::string & key, std::vector<double> & vector, int len = 1 ) const;

        friend class KeyIterator;
    };

    /**
     * This is a little helper class that iterates through the keys in a StringTable.
     * You cannot reset the Iterator. 
     * @author Gerhard Reitmayr
     * @ingroup core
     */

    class OPENTRACKER_API KeyIterator {
    protected:
        /// iterator to keep track of the KeyIterator
        StringMap::const_iterator it;
        /// the actual StringMap we are working with
        const StringMap & map;
    public:
        /**
         * constructor to create a new KeyIterator for a given StringTable. It 
         * initializes the iterator and sets it to the first key.
         * @param table the StringTable to iterate through
         */
        KeyIterator( const StringTable & table ) :
            it( table.map.begin()), map( table.map )
        {
        }
        /**
         * tests if there are more keys to see.
         * @return 0 if no more keys are left, 1 otherwise
         */
        int hasMoreKeys() const; 
        /**
         * returns the key at the current location and advances the iterator by one.
         * @return the key string at the current location.
         */
        const std::string & nextElement();
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of StringTable.h
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
