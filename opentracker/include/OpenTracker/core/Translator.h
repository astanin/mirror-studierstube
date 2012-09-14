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
/** The header file for the Translator class.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#ifndef TRANSLATOR_HEADER
#define TRANSLATOR_HEADER

#include "../dllinclude.h"

#ifndef SWIG
#include <stdexcept>
#include <string>
#include <map>
#endif

namespace ot
{
    /**
     * A map matching compiler dependent type names provided by the RunTime Type Information
     * (RTTI) system to generic type names.
     */
    typedef std::map<std::string, std::string> TranslationMap;

    /**
     * The Translator class translates compiler dependent type names provided by the RunTime Type
     * Information (RTTI) system to generic type names. This class provides an interface to register
     * combinations of generic type names and RTTI type information and for later translation.
     * Generic type names are arbitrary but must be unique.
     * @author Jochen von Spiczak
     * @ingroup core
     */
    class OPENTRACKER_API Translator
    {
    public:
        /**
         * Registers a combination of a generic type name and the type name provided by the compiler
         * for the RunTime Type Information (RTTI) system. The RTTI name is automatically generated
         * according to the @p typeInfo parameter.
         * @param genericName the generic type name to be registered (must be unique!)
         * @param typeInfo RTTI type information of the type to be registered
         */
        void registerTypeName(const std::string &genericName, const std::type_info &typeInfo);
        /**
         * Translates @p rttiName to the according generic type name. Throws an exception if the
         * type is not known by the system.
         * @param rttiName the RTTI name to be translated
         * @return the according generic type name
         */
        const std::string getGenericName(const std::string &rttiName) const throw (std::runtime_error);
        /**
         * Checks whether the generic type name @p genericName is known by the system.
         * @param genericName the generic type name to be checked
         * @return whether the generic type name is known
         */
        bool knowsType(const std::string genericName) const;

    protected:
        /**
         * A map matching compiler dependent type names provided by the RunTime Type Information
         * (RTTI) system to generic type names.
         */
        TranslationMap translationTable;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of Translator.h
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
