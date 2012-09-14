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
/** The header file for the EventAttributeBase class.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#ifndef EVENTATTRIBUTEBASE_HEADER
#define EVENTATTRIBUTEBASE_HEADER

#ifndef SWIG
#include <map>
#include <string>
#include <iostream>
#include <stdexcept>
#endif

#include "../dllinclude.h"
#include "Translator.h"

namespace ot
{

    class OPENTRACKER_API EventAttributeBase;

    /**
     * A function pointer matching the declaration of creator functions.
     */
    typedef EventAttributeBase * (*CreateFunction)(void);
    /**
     * A map matching generic type names to according creator functions.
     */
    typedef std::map<std::string, CreateFunction> CreatorMap;

    /**
     * The base class of the EventAttribute class. This class must be provided as a common base
     * class of all template instantiations of the EventAttribute class, so that an Event can
     * store pointers of this class in its attributes map (::AttributeMap).
     * This class is purely abstract and provides an interface for serialization, de-serialization,
     * streaming, and accessing type information. The interface is implemented by the templated
     * EventAttribute class.
     *
     * Moreover, static create functions, a static function for registering types, and some static
     * members realize the concept of creating new EventAttribute objects without knowing anything
     * else but the attribute's generic type name. This functionality is crucial for the
     * de-serialization of events encoded in strings.
     *
     * @author Jochen von Spiczak
     * @ingroup core
     */
    class OPENTRACKER_API EventAttributeBase
    {
        friend class Event;
        friend OPENTRACKER_API std::istream& operator>>(std::istream &in, ot::EventAttributeBase &att);
        friend OPENTRACKER_API std::ostream& operator<<(std::ostream &out, ot::EventAttributeBase &att);

    protected:
        /**
         * Virtual destructor. Must provide empty implementation for creation of RTTI type
         * information.
         */
        virtual ~EventAttributeBase();
        /**
         * Abstract assignment operator.
         * @param rv the right-value, which is the attribute to copy from
         * @return the left value
         */
        virtual EventAttributeBase& operator=(const EventAttributeBase &rv) = 0;
        /**
         * Abstract function to serialize the attribute.
         * @param out the output stream
         */
        virtual void serialize(std::ostream &out) = 0;
        /**
         * Abstract function to de-serialize the attribute.
         * @param in the input stream
         */
        virtual void deserialize(std::istream &in) = 0;
        /**
         * Abstract function to get RTTI type information of the attribute's value.
         * @return RTTI type information
         */
        virtual const std::type_info& getType() const = 0;
        /**
         * Abstract function to get generic type name of the attribute's value.
         * @return generic type name
         */
        virtual const std::string& getGenericTypeName() const = 0;
        /**
         * Creates an EventAttribute according to @p genericTypeName. Throws an exception if this
         * type name is not known by the system.
         * @param genericTypeName generic type name
         * @return pointer to the newly created attribute
         */
        static EventAttributeBase* create(const std::string &genericTypeName); //throw (std::runtime_error);
        /**
         * Creates an EventAttribute according to @p typeInfo. Throws an exception if this type
         * is not known by the system.
         * @param typeInfo RTTI type info
         * @return pointer to the newly created attribute
         */
        static EventAttributeBase* create(const std::type_info &typeInfo); //throw (std::runtime_error);
        /**
         * Registers a new generic type name. Type names are arbitrary but must be unique. In this
         * function, the generic type name is matched to the according creator function and the
         * type name is registered in the translator.
         * @param genericTypeName the generic type name to be registered (must be unique!)
         * @param typeInfo RTTI type information of the new type
         * @param create creator function for attributes of the new type
         * @return
         */
        static void registerType(const std::string &genericTypeName, const std::type_info &typeInfo, CreateFunction create);
        /**
         * Static map matching generic type names to according creator functions.
         * @param
         * @return
         */
        static CreatorMap creators;
        /**
         * Static Translator to translate compiler dependent RTTI names to generic type names. Such
         * translations are used to create attributes by RTTI type information.
         */
        static Translator translator;
    };

    /**
     * Input streaming operator. Streams the data provided by the input stream into the attribute.
     * @param in the input stream
     * @param att the attribute
     * @return the input stream
     */
    OPENTRACKER_API std::istream& operator>>(std::istream &in, ot::EventAttributeBase &att);
    /**
     * Output streaming operator. Streams the attribute into the output stream.
     * @param out the output stream
     * @param att the attribute
     * @return the output stream
     */
    OPENTRACKER_API std::ostream& operator<<(std::ostream &out, ot::EventAttributeBase &att);

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of EventAttributeBase.h
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
