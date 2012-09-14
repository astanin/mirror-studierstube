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
/** Extensions of std::iostream.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#ifndef IOSTREAM_EXT_HEADER
#define IOSTREAM_EXT_HEADER

#ifndef SWIG
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#endif

#ifdef WIN32
#pragma warning(disable:4267)
#endif
namespace ot
{
    /**
     * Output streaming operator. Streams the vector into the output stream.
     * @param os the output stream
     * @param object the vector
     * @return the output stream
     */
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& object)
    {
        typename std::vector<T>::const_iterator it;

        os << "[" << object.size() << ":";
        for (it = object.begin(); it != object.end(); it++)
        {
            if (it != object.begin())
                os << ";";
            os << *it;
        }
        os << "]";
        return os;
    };
    /**
     * Output streaming operator. Streams the list into the output stream.
     * @param os the output stream
     * @param object the list
     * @return the output stream
     */
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::list<T>& object)
    {
        typename std::list<T>::const_iterator it;

        os << object.size() << ":";
        for (it = object.begin(); it != object.end(); it++)
        {
            if (it != object.begin())
                os << ";";
            os << *it;
        }
        os << ".";
        return os;
    };
    /**
     * Input streaming operator. Streams the data provided by the input stream into the vector.
     * @param is the input stream
     * @param object the vector
     * @return the input stream
     */
    template <typename T>
    std::istream&  operator>>(std::istream& is, std::vector<T>& object)
    {
        typename std::vector<T>::size_type n;
        typename std::vector<T>::size_type i;
        char c;

        object.clear();

        // read "["
        if (!(is >> c))
            return is;
        if (c != '[')
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        // read size
        if (!(is >> n))
            return is;
        object.reserve(n);

        // read ":"
        if (!(is >> c))
            return is;
        if (c != ':')
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        // read data
        for (i = 0; i < n; i++)
        {
            T t;
            is >> t;
            object.push_back(t);
            // read ";" or "]"
            if (!(is >> c))
                return is;
            if (c != ';' && c != ']')
            {
                is.setstate(std::ios_base::failbit);
                return is;
            }
        }

        return is;
    };
    /**
     * Input streaming operator. Streams the data provided by the input stream into the list.
     * @param is the input stream
     * @param object the list
     * @return the input stream
     */
    template <typename T>
    std::istream& operator>>(std::istream& is, std::list<T>& object)
    {
        typename std::list<T>::size_type n;
        typename std::list<T>::size_type i;
        char c;

        object.clear();

        // read size
        if (!(is >> n))
            return is;

        // read ":"
        if (!(is >> c))
            return is;
        if (c != ':')
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        // read data
        for (i = 0; i < n; i++)
        {
            T t;
            is >> t;
            object.push_back(t);
            // read ";"
            if (!(is >> c))
                return is;
            if (c != ';' && c!= '.')
            {
                is.setstate(std::ios_base::failbit);
                return is;
            }
        }

        return is;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of iostream_ext.h
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
