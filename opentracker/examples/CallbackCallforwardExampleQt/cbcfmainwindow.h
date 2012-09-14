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

 #ifndef CALCULATORFORM_H
 #define CALCULATORFORM_H

#include "ui_cbcfmainwindow.h"

#include "opentrackerthread.h"

#include <QtGui>
#include <QtCore>

class CbCfMainWindow : public QMainWindow, private Ui::CbCfMainWindow
{
Q_OBJECT
   
private:
    OpentrackerThread *otthread;
    QTextCursor textCursor;
    QQueue<QString> messagequeue;
    QMutex *qmutex;
    QTimer *timer;
                          
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

public:
    CbCfMainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    ~CbCfMainWindow();
protected:
    static void clientACB(ot::CallbackNode *, ot::Event &, void *);
    static void clientBCB(ot::CallbackNode *, ot::Event &, void *);
    static void clientTestCB(ot::CallbackNode *, ot::Event &, void *);
    static void globalClientCB(ot::CallbackNode *, ot::Event &, void *);
};

#endif

/* 
 * ------------------------------------------------------------
 *   End of cbcfmainwindow.h
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
