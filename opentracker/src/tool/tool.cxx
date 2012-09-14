#include <qapplication.h>
#include "ToolAppWin.h"

/**
 * @defgroup tool Tool Classes
 * This group contains classes that implement the OpenTracker Tool, a
 * GUI program to execute and test configurations. 
 */

/** main function for the tool application. Just instantiates the main window
 * and starts the QT application loop.
 * @author Gerhard Reitmayr
 * @ingroup tool
 */
int main( int argc, char **argv )
{
    QApplication a( argc, argv );
    ToolAppWin * mw = new ToolAppWin();
    mw->setCaption( "OpenTracker Tool" );
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}

/* 
 * ------------------------------------------------------------
 *   End of tool.cxx
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
