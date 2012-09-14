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
/** thread running the opentracker context
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */


#include "opentrackerthread.h"

#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/core/OSUtils.h>

OpentrackerThread::OpentrackerThread(QObject *parent)
    : QThread(parent)
{
    using namespace ot;

    // parse the configuration file, builds the tracker tree
    Configurator::instance()->changeConfigurationFile("clientB.xml");
}
    
OpentrackerThread::~OpentrackerThread()
{
    using namespace std;

    if (isRunning())
    {
        cout << "stopping OpenTracker ... ";
        
        ot::Configurator::instance()->getContext()->stopLoop();       

        wait();
    }
    cout << "done." << endl;
}

ot::CallbackModule* OpentrackerThread::getCallbackModule()
{
    using namespace std;
    using namespace ot;


    return dynamic_cast<CallbackModule*>(ot::Configurator::instance()->getContext()->getModule("CallbackConfig"));
}

ot::CallforwardModule* OpentrackerThread::getCallforwardModule()
{
    using namespace ot;

    return dynamic_cast<CallforwardModule*>
        (ot::Configurator::instance()->getContext()->getModule("CallforwardConfig"));
}

void OpentrackerThread::setConfigurationFile(const QString& fname)
{
    using namespace std;
    using namespace ot;

    cout << " parsing new configuration file " 
         << fname.toAscii().constData() << " ... ";

    Configurator::instance()->
        changeConfigurationFile(fname.toAscii().constData());
   
    cout << "done." << endl;
}

void OpentrackerThread::setConfigurationString(const QString& fname)
{
    using namespace ot;

    Configurator::instance()->
        changeConfigurationString(fname.toAscii().constData());
}


void OpentrackerThread::run()
{
    using namespace std;

    cout << "starting OpenTracker loop ..." << endl; 
    ot::Configurator::instance()->getContext()->runOnDemand();
    cout << "loop finished." << endl;
}

/* 
 * ------------------------------------------------------------
 *   End of cbcfmainwindow.cpp
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
