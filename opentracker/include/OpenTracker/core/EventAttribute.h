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
/** The header file for the EventAttribute class.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#ifndef EVENTATTRIBUTE_HEADER
#define EVENTATTRIBUTE_HEADER

#ifndef SWIG
#include <iostream>
#include <string>
#endif

#include "EventAttributeBase.h"
#include "iostream_ext.h"

namespace ot
{
    /**
     * The EventAttribute class, which realizes one single attribute belonging to exactly one
     * event. An EventAttribute encapsulates the attribute's value and provides an interface for
     * serialization, de-serialization, and accessing type information. All methods are private
     * and can just be accessed through the interface of friend class Event.
     * @author Jochen von Spiczak
     * @ ingroup core
     */
    template <class T>
    class EventAttribute : public EventAttributeBase
    {
        friend class Event;

    private:
        /**
         * Standard constructor. Sets the generic type name.
         */
        EventAttribute()
        {
            genericTypeName = translator.getGenericName(typeid(T).name());
        };
        /**
         * Constructor setting the initial value.
         * @param initValue the initial value of the attribute
         */
        EventAttribute(const T &initValue)
            : value(initValue)
        {
            genericTypeName = translator.getGenericName(typeid(T).name());
        };
        /**
         * Copy constructor. Copies the value of another attribute into the new one.
         * @param rv the right-value, which is the attribute to copy from
         */
        EventAttribute(const EventAttribute &rv)
        {
            *this = rv;
        };
        /**
         * Static creator function, which simply returns a newly constructed EventAttribute. For
         * every template instantiation of the EventAttribute class, this creator function is added
         * to the CreatorMap of the EventAttributeBase class, which maps the generic type name of the
         * template paramter to an according creator function pointer. Thus, an EventAttribute can
         * be created with the right template paramter by just knowing the generic type name.
         * @return pointer to the newly created EventAttribute
         */
        static EventAttributeBase* create()
        {
            return new EventAttribute<T>;
        };
        /**
         * Assignment operator. Copies the value of another attribute into this one. If the right
         * value is of a different template type than the left value, the left value will not be
         * changed.
         * @param rv the right-value, which is the attribute to copy from
         * @return the left-value
         */
        EventAttributeBase& operator=(const EventAttributeBase &rv)
        {
            const EventAttribute<T> *trueRv = dynamic_cast< const EventAttribute<T>* >(&rv);
            if (trueRv)
                value = trueRv->value;
            return *this;
        };
        /**
         * Serializes the attribute's value into an output stream.
         * @param out the output stream
         */
        void serialize(std::ostream &out)
        {
            out << value;
        };
        /**
         * De-serializes a value from an input stream. Naturally, the data coming from the input
         * stream must be a correctly serialized value of the same or a similar type.
         * @param in the input stream
         */
        void deserialize(std::istream &in)
        {
            in >> value;
        };
        /**
         * Returns the attribute's value.
         * @return a reference to the attribute's value
         */
        T& get()
        {
            return value;
        };
        /**
         * Returns the attribute's value.
         * @param a const reference to the attribute's value
         * @return
         */
        const T& get() const
        {
            return value;
        };
        /**
         * Sets the attribute's value.
         * @param newValue the new value
         */
        void set(const T &newValue)
        {
            value = newValue;
        };
        /**
         * Returns the RTTI type information of the attribute's value.
         * @return RTTI type information
         */
        const std::type_info& getType() const
        {
            return typeid(T);
        };
        /**
         * Returns the generic type name of the attribute's value.
         * @return the generic type name
         */
        const std::string& getGenericTypeName() const
        {
            return genericTypeName;
        };

    private:
        /// the value stored by the attribute
        T value;
        /// the generic type name of the attribute's value
        std::string genericTypeName;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of EventAttribute.h
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
