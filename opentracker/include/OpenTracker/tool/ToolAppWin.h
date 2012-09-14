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
/** header file for the main application window class of the OpenTracker tool
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ToolAppWin.h 1159 2006-06-13 13:40:36Z bornik $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _TOOLAPIWIN_H
#define _TOOLAPIWIN_H

#include <qmainwindow.h>

class ToolForm;
class QToolBar;
class QPopupMenu;
class QTimer;
class QLabel;
class Context;

/**
 * The main application window of the OpenTracker tool. This class currently
 * handles almost everything, besides the OpenTracker details. 
 * @author Gerhard Reitmayr
 * @ingroup tool
 */
class ToolAppWin: public QMainWindow
{
Q_OBJECT
public:
    ToolAppWin();
    ~ToolAppWin();
    
protected:
    void closeEvent( QCloseEvent* );

private slots:
    void load();
    void load( const char *fileName );
    void reload();

    void start();
    void pause();
    void stop();
    void loop();

    void about();

private:
    QToolBar *fileTools, *controlTools;
    ToolForm *win;
    QTimer *timer;
    QString filename;
    QLabel * framerate;
    double startTime;
    long int frames;

protected:
    Context * context;
};

#endif

/* 
 * ------------------------------------------------------------
 *   End of ToolAppWin.h
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
