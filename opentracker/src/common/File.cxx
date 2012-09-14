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
/** source file for File support class for the FileModule.
 *
 * @author Gerhard Reitmayr, Jochen von Spiczak
 *
 * $Id: File.h 1192 2006-06-19 21:52:27Z spiczak $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/common/File.h>


#include <cstdio>
#include <string>

#include <iomanip>
#include <fstream>


namespace ot {

    File::File(const std::string filename_ , modeFlags mode_, bool append , bool loop_ , bool ot11Format_ ) :
        loop( loop_ ), ot11Format( ot11Format_ ), filename( filename_), mode( mode_ )
    {
#ifdef WIN32
#pragma message(">>> daniel2everybody: please do not use cout!")
#endif
        //std::cout << "FILE constructed" << std::endl;
        if( mode == FILE_OUT ) // output mode
        {
            //outputbuf = new char[1024*1024*4];
            output = new std::ofstream();
            //output->rdbuf()-setbuf(outputbuf, 1024*1024*4);
                
            if( append )
            {
                output->open( filename.c_str(), std::ios_base::out | std::ios_base::app);
            }
            else
            {
                output->open( filename.c_str(), std::ios_base::out | std::ios_base::trunc);
            }
            input = NULL;
        }
        else {          // otherwise input mode
            input = new std::ifstream( filename.c_str());
            input->setf( std::ios::skipws );
            output = NULL;
        }

    }

    File::~File()
    {
        if( input != NULL )
        {
            input->close();
            delete input;
            //delete [] outputbuf;
        }
        if( output != NULL )
        {
            output->close();
            delete output;
            //delete [] outputbuf;
        }
    }

    void File::write( Event & event, int station )
    {
        if( output != NULL)
        {
            if (!ot11Format)
                *output << station << " " << event << std::endl;
            else
            {
                char str[220];
                sprintf(str, "%d %.15f %.15f %.15f %.15f %.15f %.15f %.15f %.15f %d %.15f\n",
                        station,
                        event.time,
                        event.getPosition()[0],
                        event.getPosition()[1],
                        event.getPosition()[2],
                        event.getOrientation()[0],
                        event.getOrientation()[1],
                        event.getOrientation()[2],
                        event.getOrientation()[3],
                        event.getButton(),
                        event.getConfidence());
                *output << str << std::flush;
            }
        }
    }

    bool File::read( Event & event, int * station )
    {

        if( !input->is_open())
        {
            return false;
        }

        if (!ot11Format)
        {
            input->clear();
            *input >> *station;
            *input >> event;
        }
        else
        {
            input->clear();
            *input >> *station;
            *input >> event.time;
            *input >> event.getPosition()[0] >> event.getPosition()[1] >> event.getPosition()[2];
            *input >> event.getOrientation()[0] >> event.getOrientation()[1] >> event.getOrientation()[2] >> event.getOrientation()[3];
            *input >> event.getButton();
            *input >> event.getConfidence();
        }

        bool failed = input->fail();
        if(failed && loop)
        {
            input->clear();
            input->seekg(0, std::ios::beg);
            failed = false;
        }
        return !failed;
    }

    bool File::reset()
    {
        if (!input->is_open())
            return false;
        input->clear();
        input->seekg(0, std::ios::beg);
        return true;
    }

} // namespace ot


/*
 * ------------------------------------------------------------
 *   End of File.cxx
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
