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
/** The source file for the EventAttributeBase class.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#include <iostream>
#include <string>

#include <OpenTracker/core/EventAttributeBase.h>

namespace ot
{

    // instantiation of static member variables
    CreatorMap EventAttributeBase::creators;
    Translator EventAttributeBase::translator;

    // virtual destructor
    EventAttributeBase::~EventAttributeBase()
    {
    }

    // create method
    EventAttributeBase* EventAttributeBase::create(const std::string &genericTypeName) //throw (std::runtime_error)
    {
        CreatorMap::iterator it = creators.find(genericTypeName);
        if (it != creators.end())
            return creators[genericTypeName]();
        else
            throw std::runtime_error("Unregistered type '" + genericTypeName + "' - please register new types using Event::registerGenericTypeName<>() or Event::registerAllKnownTypes() to fix this problem.");
    }

    // create method
    EventAttributeBase* EventAttributeBase::create(const std::type_info &typeInfo) //throw (std::runtime_error)
    {
        const std::string genericTypeName = translator.getGenericName(typeInfo.name());
        return create(genericTypeName);
    }

    // register create function and generic name of new type
    void EventAttributeBase::registerType(const std::string &genericTypeName, const std::type_info &typeInfo, CreateFunction create)
    {
        creators[genericTypeName] = create;
        translator.registerTypeName(genericTypeName, typeInfo);
    };

    // stream the attribute (not a member function)
    std::istream& operator>>(std::istream &in, ot::EventAttributeBase &att)
    {
        att.deserialize(in);
        return in;
    }

    // stream the attribute (not a member function)
    std::ostream& operator<<(std::ostream &out, ot::EventAttributeBase &att)
    {
        att.serialize(out);
        return out;
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of EventAttributeBase.cxx
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
