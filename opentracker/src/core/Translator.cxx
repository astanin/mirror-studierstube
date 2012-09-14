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
/** The source file for the Translator class.
 *
 * @author Jochen von Spiczak
 *
 * $Id: Node.h 1011 2006-04-20 21:47:53Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#include <iostream>

#include <OpenTracker/core/Translator.h>

namespace ot
{

    // get according generic name
    const std::string Translator::getGenericName(const std::string &rttiName) const throw (std::runtime_error)
    {
        TranslationMap::const_iterator it = translationTable.find(rttiName);
		
		if (it == translationTable.end()){	      
			throw std::runtime_error("unregistered type '" + rttiName + "' - please register new types using Event::registerGenericTypeName<>() to fix this problem");
		}else
            return (*it).second;
    }

    // register new typename to translation table
    void Translator::registerTypeName(const std::string &genericName, const std::type_info &typeInfo)
    {
        translationTable[std::string(typeInfo.name())] = genericName;
    }

    // check whether generic type name was registered
    bool Translator::knowsType(const std::string genericName) const
    {
        TranslationMap::const_iterator it;
        for (it = translationTable.begin(); it != translationTable.end(); it++)
            if ((*it).second == genericName)
                return true;
        return false;
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of Translator.cxx
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
