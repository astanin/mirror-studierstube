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
/** ubitrack message
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/ubitrack/UbitrackMessage.h>

#include <string>
#include <cassert>
#include <iostream>


namespace ot {

    UbitrackMessage::UbitrackMessage() :   
        length_(0),
        data_(NULL)
    {     
        logPrintI("UbitrackMessage::UbitrackMessage()\n");
    }


    UbitrackMessage::UbitrackMessage(UbitrackMessage &msg) 
    {
        logPrintI("UbitrackMessage::UbitrackMessage(UbitrackMessage &msg)\n");
        if (msg.data_ != NULL)
        {
            ACE_TCHAR *new_data = 0;
            ACE_NEW(new_data, ACE_TCHAR[msg.length_]);
            length_ = msg.length_;
            memcpy(data_, msg.data_, length_); 
        }
        else
        {
            length_ = 0;
            data_ = NULL;
        }
    }


    UbitrackMessage::~UbitrackMessage()
    {
        if (this->data_)
        {
            this->length_ = 0;
            delete [] this->data_;
        }
    }

    void UbitrackMessage::setXMLString(const std::string &instr)
    {
        using namespace std;

        size_t const newlen = instr.size() + 1;  // Will need room for '\0'

        if (instr.size() > this->length_)
        {
            ACE_TCHAR *new_data = 0;
            ACE_NEW(new_data, ACE_TCHAR[newlen]);
            new_data[0]=0;
            new_data[newlen-1]=0;
            delete [] this->data_;
            this->data_ = new_data;
            this->length_ = newlen;
        }
        memcpy(data_, instr.c_str(), newlen);
        //cout << "newlen: " << newlen 
        //     << " " << instr.c_str() << endl;
        //printf("%s\n", data_);
    }


} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of UbitrackMessage.cxx
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
