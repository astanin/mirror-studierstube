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
 * PROJECT: UbitrackServer1
 * ======================================================================== */
/** ubitrack message
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifndef UBITRACKMESSAGE_H
#define UBITRACKMESSAGE_H

#include "../dllinclude.h"

#include <string>

#include <ace/ACE.h>
#include <ace/Default_Constants.h>
#include <ace/Basic_Types.h>

namespace ot {
    class OPENTRACKER_API UbitrackMessage
    {
    public:
        enum {
            DEFAULTUMSGLEN = 1024*128, // 128k is a reasonable value ...
            ALIGN_WORDB = 8
        };

        UbitrackMessage();
        UbitrackMessage(UbitrackMessage &msg);
        virtual ~UbitrackMessage();

        void setXMLString(const std::string &instr);
        void dump() const;

        const ACE_TCHAR *data () const;
        ACE_UINT64 length() const;
        const ACE_UINT64 *lengthR() const;
        std::string XMLString() const;

    private:
        /// Total length of the ubitrack record in bytes
        ACE_UINT64 length_;

        /// storage for sending the XML string
        ACE_TCHAR *data_;   // Heap-allocated text message area

        /// normal version of the XML string
    };

    ACE_INLINE ACE_UINT64 UbitrackMessage::length (void) const
    {
        ACE_TRACE ("UbitrackMessage::length");
        return this->length_;
    }

    ACE_INLINE const ACE_UINT64 *UbitrackMessage::lengthR (void) const
    {
        ACE_TRACE ("UbitrackMessage::lengthR");
        return &(this->length_);
    }

    ACE_INLINE const ACE_TCHAR *
    UbitrackMessage::data (void) const
    {
        ACE_TRACE ("UbitrackMessage::msg_data");
        return this->data_;
    }

    ACE_INLINE std::string UbitrackMessage::XMLString() const
    {
        return std::string(this->data_);
    }
}  // namespace ot

#endif //UBITRACKMESSAGE_H

/* 
 * ------------------------------------------------------------
 *   End of UbitrackMessage.h
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
