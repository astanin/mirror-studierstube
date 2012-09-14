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
/** source file for StringTable class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: StringTable.cxx 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/core/StringTable.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <exception>

#ifdef USE_LIVE
#include <OpenTracker/skeletons/OTGraph.hh>
#endif

//using namespace std;

// emtpy string to be returned, if key is not in the map

const std::string empty("");

// initializes the map

namespace ot {

    StringTable::StringTable() : map()
    {}

    // copy constructor
    StringTable::StringTable(const StringTable& table ) : map(table.map)
    {}

#ifdef USE_LIVE
    // copy constructor
    StringTable::StringTable(const OTGraph::StringTable& st ) 
    {
        for (CORBA::ULong i = 0; i < st.length(); i++) { 
            OTGraph::KeyValuePair kvp = st[i];
            std::string key(kvp.Key);
            std::string value(kvp.Value);
            map[key] = value;
        }
    }
#endif //USE_LIVE

    // clears the map

    StringTable::~StringTable()
    {
        map.clear();
    }

#ifdef USE_LIVE
    OTGraph::StringTable* StringTable::getStringTable() {
        OTGraph::StringTable_var st = new OTGraph::StringTable;
        st->length((CORBA::ULong) map.size());
        int i=0;
        for (StringMap::iterator it=map.begin(); it!=map.end(); ++it) {
            OTGraph::KeyValuePair kvp;
            kvp.Key = (it->first).c_str();
            kvp.Value = (it->second).c_str();
            st[i++] = kvp;
        }
        return st._retn();
    }
#endif //USE_LIVE


    // returns a value to a given key

    const std::string & StringTable::get( const std::string & key ) const
    {
        StringMap::const_iterator it = map.find( key );
        if( it == map.end())
            return empty;
        return (*it).second;
    }

    // stores a key, value pair

    void StringTable::put( const std::string & key, const std::string & value )
    {
        map[key] = value;
    }

    // removes a key, value pair

    void StringTable::remove( const std::string & key )
    {
        StringMap::iterator it = map.find( key );
        if( it == map.end())
            return;
        map.erase( it );
    }

    // tests for a given key

    int StringTable::containsKey( const std::string & key ) const
    {
        StringMap::const_iterator it = map.find( key );
        if( it == map.end())
            return 0;
        return 1;
    }

    // returns the size of the table

    unsigned StringTable::size() const
    {
        return map.size();
    }

    // some put and get methods

    void StringTable::put(const std::string & key, const int value)
    {
        char buffer[20];

        sprintf( buffer, "%i", value );
        map[key] = buffer;
    }

    void StringTable::put(const std::string & key, const float value)
    {
        char buffer[20];

        sprintf( buffer, "%f", value );
        map[key] = buffer;
    }

    void StringTable::put(const std::string & key, const double value)
    {
        char buffer[30];

        sprintf( buffer, "%lf", value );
        map[key] = buffer;
    }

    void StringTable::put(const std::string & key, const int * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%i", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %i", value[i] );
            strvalue.append(buffer);
        }
        map[key] = strvalue;
    }

    void StringTable::put(const std::string & key, const float * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%f", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %f", value[i] );
            strvalue.append(buffer);
        }
        map[key] = strvalue;
    }

    void StringTable::put(const std::string & key, const double * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%lf", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %lf", value[i] );
            strvalue.append(buffer);
        }
        map[key] = strvalue;
    }

    int StringTable::get(const std::string & key, int * value, int len ) const
    {
        StringMap::const_iterator it = map.find( key );
        if( it == map.end())
            return 0;

        char * data = (char *)(*it).second.c_str();
        char * end = data;
        int count = 0;
        value[count++] = strtol( data, &end, 0 );
        while( end != data && count < len){
            data = end;
            value[count++] = strtol( data, &end, 0 );
        }
        return count;
    }

    int StringTable::get(const std::string & key, float * value, int len ) const
    {
        StringMap::const_iterator it = map.find( key );
        if( it == map.end())
            return 0;

        char * data = (char *)(*it).second.c_str();
        char * end = data;
        int count = 0;
        value[count++] = (float)strtod( data, &end );
        while( end != data && count < len){
            data = end;
            value[count++] = (float)strtod( data, &end );
        }
        return count;
    }

    int StringTable::get(const std::string & key, std::vector<float> & vector, int len ) const
    {
        using namespace std;
        StringMap::const_iterator it = map.find( key );
        if( it == map.end())
            return 0;

        std::istringstream is(it->second);
        
        int count = 0;
        try
        {
            double inval;
	    vector.resize(len);
            while (!is.eof() && count < len)
            {
                is >> inval;
                vector[count] = inval;
                count++;
            }
           
        }
        catch (exception &e)
        {
            printf("Exception %s\n", e.what());
        }

        return count;
    }

    int StringTable::get(const std::string & key, double * value, int len ) const
    {
        StringMap::const_iterator it = map.find( key );
        if( it == map.end())
            return 0;

        char * data = (char *)(*it).second.c_str();
        char * end = data;
        int count = 0;
        value[count++] = strtod( data, &end );
        while( end != data && count < len){
            data = end;
            value[count++] = strtod( data, &end );
        }

        return count;
    }
    int StringTable::get(const std::string & key, std::vector<double> & vector, int len ) const
    {
        using namespace std;
        StringMap::const_iterator it = map.find( key );
        if( it == map.end())
            return 0;

        std::istringstream is(it->second);
        
        int count = 0;
        try
        {
            double inval;
	    vector.resize(len);
            while (!is.eof() && count < len)
            {
                is >> inval;
                vector[count] = inval;
                count++;
            }
           
        }
        catch (exception &e)
        {
            printf("Exception %s\n", e.what());
        }

        return count;
    }

    int KeyIterator::hasMoreKeys() const
    {
        return((int)(it != map.end()));
    }

    const std::string & KeyIterator::nextElement()
    {
        if( hasMoreKeys()){
            const std::string & res = (*it).first;
            it++;
            return res;
        }
        return empty;
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of StringTable.cxx
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
