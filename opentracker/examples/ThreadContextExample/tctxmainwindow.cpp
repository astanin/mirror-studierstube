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
/** main window of the Callback/Callforward program
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <QtGui>
#include "tctxmainwindow.h"

#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/core/OSUtils.h>
#include <OpenTracker/common/CallbackNode.h>


TCtxMainWindow::TCtxMainWindow( QWidget * parent, 
                                Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      configurator(NULL),
      tcontext(NULL)
{
    using namespace std;
 
    setupUi(this);

    scene = new PongScene();
    view = new PongView(scene);
  
   
    QVBoxLayout *pongla = new QVBoxLayout(tabWidget->widget(1));
    pongla->addWidget(view);

    configurator = ot::Configurator::instance(ot::THREAD);
    tcontext = dynamic_cast<ot::ThreadContext*>(configurator->getContext());

    connect(actionClient_A, SIGNAL (activated()), this, SLOT(readFileA()));
    connect(actionClient_B, SIGNAL (activated()), this, SLOT(readFileB()));
    connect(actionLocal_TestSource, SIGNAL (activated()), 
            this, SLOT(readFileTest()));        

    qmutex = new QMutex();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateEventsEdit()));

    /// setup the opentracker context
    ot::initializeContext( tcontext , NULL);

    // parse the configuration file, builds the tracker tree
    configurator->changeConfigurationFile("clientLocal.xml");

    /// register all callbacks
    registerCallbacks();

    /// load a configuration file into the upper right textedit
    updateConfigFileEdit("clientLocal.xml");
     
    /// connect signal, for configuration file update
    connect(this, SIGNAL(fileNameSignal(const QString&)),
            this, SLOT(setConfigurationFile(const QString&)));

    /// connect signal for upper right textedit content update
    connect(this, SIGNAL(fileNameSignal(const QString&)),
            this, SLOT(updateConfigFileEdit(const QString&)));

    /// connect signal for configuration update from upper right textedit
    connect(configPushButton, SIGNAL(pressed()),
            this, SLOT(setupConfigFromEdit()));

    /// connect signal for calling the CallForward node
    connect(eventPushButton, SIGNAL(pressed()),
            this, SLOT(createEventFromEdit()));

    /// start the timer necessary to do updates of the left textedit
    /// since this must be done in the UI thread.
    /// the callbacks actually run in the OpenTracker thread

    timer->start();
    
    /// finally start the OpenTracker thread.
    tcontext->run();
    cerr << "TCtxMainWindow::TCtxMainWindow done." << endl;
}

TCtxMainWindow::~TCtxMainWindow()
{
    using namespace std;
    cerr << "TCtxMainWindow::~TCtxMainWindow  ..." << endl;

    /// stop left textedit update timer
    timer->stop();

    delete timer;
    delete qmutex;

    delete configurator;
    cerr << "TCtxMainWindow::~TCtxMainWindow done." << endl;
    
}

void TCtxMainWindow::readFileA()
{
    emit fileNameSignal("clientA.xml");
}

void TCtxMainWindow::readFileB()
{
    emit fileNameSignal("clientB.xml");
}

void TCtxMainWindow::readFileTest()
{
    emit fileNameSignal("clientLocal.xml");
}

/// This is registers the callbacks
void TCtxMainWindow::registerCallbacks()
{
    /// some per-node callbacks
    /// note, that the nodes don't need to exist!
    getCallbackModule()->
        setCallback( "clientAInput", TCtxMainWindow::clientACB, this );
    getCallbackModule()->
        setCallback( "clientBInput", TCtxMainWindow::clientBCB, this );
    getCallbackModule()->
        setCallback( "clientTestInput", TCtxMainWindow::clientTestCB, this );

    /// register a global callback
    getCallbackModule()->
        setGlobalCallback(TCtxMainWindow::globalClientCB, this);
}

/// reads a configuration file into the upper right textedit
void TCtxMainWindow::updateConfigFileEdit(const QString &fileName)
{
    QFile file(fileName);

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        configEdit->setPlainText(file.readAll());   
    }
}

/// setup of an OpenTracker configuration form the upper right textedit
void TCtxMainWindow::setupConfigFromEdit()
{
    setConfigurationString(configEdit->toPlainText());
}

/// creates an event from the content of the lower right textedit
void TCtxMainWindow::createEventFromEdit()
{
    using namespace ot;
    Event event;

    /// FIXXXME: this should acually be parsing the textedit!

    event.timeStamp();
    event.getPosition()[0] = 1.0;
    if (!getCallforwardModule()->callForward("clientAInput", event))
    {
        outEventsEdit->append("callForward to 'clientAInput' failed");
    }
    else
    {
        outEventsEdit->append("callForward to 'clientAInput' succeeded");
    }

    if (!getCallforwardModule()->callForward("clientBInput", event))
    {
        outEventsEdit->append("callForward to 'clientBInput' failed");
    }
    else
    {
        outEventsEdit->append("callForward to 'clientBInput' succeeded");
    }


    if (!getCallforwardModule()->callForward("clientLocalInput", event))
    {
        outEventsEdit->append("callForward to 'clientLocalInput' failed");
    }
    else
    {
        outEventsEdit->append("callForward to 'clientLocalInput' succeeded");
    }
                                                  
}

/// update of the left textedit from a queue storing strings
void TCtxMainWindow::updateEventsEdit()
{
    qmutex->lock();
    while (not messagequeue.empty())
    {
        eventsEdit->append(messagequeue.dequeue());
    }
    qmutex->unlock();
}

/// set OpenTracker XML configuration file
void TCtxMainWindow::setConfigurationFile(const QString& fname)
{
    using namespace std;
    using namespace ot;

    cout << " parsing new configuration file " 
         << fname.toAscii().constData() << " ... ";

    configurator->changeConfigurationFile(fname.toAscii().constData());
   
    cout << "done." << endl;
}

/// set OpenTracker XML configuration string
void TCtxMainWindow::setConfigurationString(const QString& fname)
{
    using namespace ot;

    configurator->changeConfigurationString(fname.toAscii().constData());
}

/// below are the callback functions running in the OpenTracker thread
void TCtxMainWindow::clientACB( ot::CallbackNode * node,  ot::Event & event, void * data )
{
    using namespace ot;

    TCtxMainWindow *self = (TCtxMainWindow*)(data);

    //double diff = (OSUtils::currentTime() - event.time ) / 1000;

    self->qmutex->lock();

    self->messagequeue.append("TCtxMainWindow::clientACB");
    /// get a readable form of the event
    self->messagequeue.append(event.getPrintOut().c_str());

    self->qmutex->unlock();
}

void TCtxMainWindow::clientBCB( ot::CallbackNode * node, 
                                ot::Event & event, void * data )
{
    using namespace ot;

    TCtxMainWindow *self = (TCtxMainWindow*)(data);

    //double diff = (OSUtils::currentTime() - event.time ) / 1000;

    self->qmutex->lock();

    self->messagequeue.append("TCtxMainWindow::clientBCB");
    self->messagequeue.append(event.getPrintOut().c_str());

    self->qmutex->unlock();
}

void TCtxMainWindow::clientTestCB( ot::CallbackNode * node, 
                                ot::Event & event, void * data )
{
    using namespace ot;

    TCtxMainWindow *self = (TCtxMainWindow*)(data);

    //double diff = (OSUtils::currentTime() - event.time ) / 1000;

    self->qmutex->lock();
   
    self->messagequeue.append("TCtxMainWindow::clientTestCB");
    self->messagequeue.append(event.getPrintOut().c_str());
  
    self->qmutex->unlock();
}

void TCtxMainWindow::globalClientCB( ot::CallbackNode * node,  
                                     ot::Event & event, void * data )
{
    using namespace std;

    TCtxMainWindow *self = (TCtxMainWindow*)(data);

    self->qmutex->lock();

    self->messagequeue.append("TCtxMainWindow::globalClientCB");
    self->messagequeue.append("  emitter:");
    self->messagequeue.append(node->getName().c_str());

    self->qmutex->unlock();
   
    // code for dispatching events from different would
    // go here


}

ot::CallbackModule* TCtxMainWindow::getCallbackModule()
{
    using namespace ot;

    return dynamic_cast<CallbackModule*>(tcontext->getModule("CallbackConfig"));
}

ot::CallforwardModule* TCtxMainWindow::getCallforwardModule()
{
    using namespace ot;

    return dynamic_cast<CallforwardModule*>
        (tcontext->getModule("CallforwardConfig"));
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
