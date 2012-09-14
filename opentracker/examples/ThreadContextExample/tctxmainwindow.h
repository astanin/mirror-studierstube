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
/** main window of the Callback/Callforward program based on threaded
 *  context.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

 #ifndef TCTXMAINWINDOW_H
 #define TCTXMAINWINDOW_H

#include "ui_tctxmainwindow.h"

#include <QtGui>
#include <QtCore>

#include <ace/Condition_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/core/ThreadContext.h>
#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallforwardModule.h>

#include "pongscene.h"
#include "pongview.h"

class TCtxMainWindow : public QMainWindow, private Ui::TCtxMainWindow
{
Q_OBJECT
   
protected:
    QTextCursor textCursor;
    QQueue<QString> messagequeue;
    QMutex *qmutex;
    QTimer *timer;
    ot::Configurator *configurator;
    ot::ThreadContext *tcontext;
    PongView *view;
    PongScene *scene;

                          
private slots:
    void readFileA();
    void readFileB();
    void readFileTest();
    void registerCallbacks();
    void updateConfigFileEdit(const QString &);
    void setupConfigFromEdit();
    void createEventFromEdit();
    void updateEventsEdit();

signals:
    void fileNameSignal(const QString &);
public slots:
    void setConfigurationFile(const QString &);
    void setConfigurationString(const QString &);
public:
    TCtxMainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    ~TCtxMainWindow();
protected:
    static void clientACB(ot::CallbackNode *, ot::Event &, void *);
    static void clientBCB(ot::CallbackNode *, ot::Event &, void *);
    static void clientTestCB(ot::CallbackNode *, ot::Event &, void *);
    static void globalClientCB(ot::CallbackNode *, ot::Event &, void *);
    ot::CallbackModule* getCallbackModule();
    ot::CallforwardModule* getCallforwardModule();
};

#endif

/* 
 * ------------------------------------------------------------
 *   End of tctxmainwindow.h
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
