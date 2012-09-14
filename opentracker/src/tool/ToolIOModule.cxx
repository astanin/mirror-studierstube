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
/** source file for special module handling ConsoleSink / ConsoleSource nodes
 * instead of the usuall ConsoleModule.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ToolIOModule.cxx 1159 2006-06-13 13:40:36Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include "ToolIOModule.h"
#include "ToolSource.h"
#include "ToolSink.h"

// constructor method

ToolIOModule::ToolIOModule( QTable * out_ ) :
    Module(), NodeFactory(), output( out_ ), quit(0)
{
    output->setNumRows( 0 );    
}
	 
// Destructor method, clears nodes member.

ToolIOModule::~ToolIOModule()
{
    sinks.clear();
    sources.clear();
}

// initializes the tracker module.

void ToolIOModule::init(StringTable& attributes, ConfigNode * localTree)
{
    Module::init( attributes, localTree );
}

// creates new nodes

Node * ToolIOModule::createNode( const std::string& name, StringTable& attributes)
{
    if( name.compare("ConsoleSink") == 0 || name.compare("ToolSink") == 0 )
    {
        ToolSink * sink = new ToolSink( attributes.get("comment"));
        sinks.push_back( sink );
        //        cout << "Built ToolSink node for ConsoleSink" << endl;       
        return sink;
    } else if( name.compare("ConsoleSource") == 0 || name.compare("ToolSource") == 0)
    {
        int number;
        if( sscanf( attributes.get("number").c_str()," %i", &number ) == 1 )
        {
            if( number >= 0 && number < 10 )
            {
                ToolSource * source = new ToolSource( number );
                sources.push_back( source );
                //                cout << "Build ToolSource node." << endl;
                return source;
            } else
            {
                //                cout << "ToolSource station number not in [0,9] : " << number << endl;
            }
        }
    }
    return NULL;
}

// reads out node events and displays them

void ToolIOModule::pullEvent()
{
    QString s;
    int row;
    double diff;
    for(NodeVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
    {
        ToolSink * sink = (ToolSink *)(*it);
        if( sink->changed == 1 )
        {
            row = it - sinks.begin();
            Event & event = sink->event;
            diff = OSUtils::currentTime() - event.time;
            s.sprintf("%f %f %f", event.getPosition()[0],event.getPosition()[1],event.getPosition()[2]);
            output->setText( row, 0, s );
            s.sprintf("%f %f %f %f", event.getOrientation()[0], event.getOrientation()[1], 
                      event.getOrientation()[2], event.getOrientation()[3] );
            output->setText( row, 1, s );
            s.sprintf("%hx", event.getButton() );
            output->setText( row, 2, s );
            s.sprintf("%f", event.getConfidence() );
            output->setText( row, 3, s );
            s.sprintf("%lf", event.time );
            output->setText( row, 4, s );
            s.sprintf("%lf", diff );
            output->setText( row, 5, s );
            sink->changed = 0;
        }
    }
}

// sets the table properties

void ToolIOModule::start()
{
    output->setNumRows( sinks.size());
    QHeader * head = output->verticalHeader();
    for(NodeVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
    {
        ToolSink * sink = (ToolSink *)(*it);
        head->setLabel(it - sinks.begin(),sink->comment.c_str()); 
    }
    head->adjustSize();
    Module::start();
}

void ToolIOModule::close()
{
}

int ToolIOModule::stop()
{
    return quit;
}

/* 
 * ------------------------------------------------------------
 *   End of ToolIOModule.cxx
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
