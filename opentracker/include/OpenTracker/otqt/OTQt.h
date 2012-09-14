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
/**
 * @file   OTQt.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c OTQt
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */
/**
 * @defgroup group_otqt_classes OTQt Classes
 *
 * This group contains classes that implement the %OTQt module. %OTQt converts tracking
 * data originating from several dedicated input devices into mouse events which are
 * posted to a target Qt desktop application. %OTQt consists of two modes:
 *
 * @li Calibration mode (see main classes @c ot::QtMouseEventCalibModule and @c
 * ot::OTQtMEMCalibProc): Prepending configuration creating a convenient XML configuration
 * file for @e operation @e mode. Implemented as command line tool @c otqt-mem-calib,
 * which determines the spatial location and extent of the target 2D desktop screen.
 *
 * @li Operation mode (see main class @c ot::QtMouseEventModule): Actually generates mouse
 * events from tracking data. Triggered in target Qt application.
 */

#ifndef OTQT_OTQT_H
#define OTQT_OTQT_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventModule.h>
#include <OpenTracker/otqt/QtMouseEventCalibModule.h>
#include <OpenTracker/otqt/OTQtLog.h>
#include <OpenTracker/OpenTracker.h>
#ifdef USE_QT4
#include <Qt/qobject.h>
#include <Qt/qtimer.h>
#include <Qt/qmutex.h>
#else
#include <qobject.h>
#include <qtimer.h>
#include <qmutex.h>
#endif
#include <string>

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib,"QtCored4.lib")
#pragma comment(lib,"QtGuid4.lib")
#else
#pragma comment(lib,"QtCore4.lib")
#pragma comment(lib,"QtGui4.lib")
#endif
#endif


namespace ot {

    //--------------------------------------------------------------------------------
    /**
     * @class OTQt
     * @ingroup group_otqt_classes
     * @brief %OTQt main class
     *
     * Provides the public interface towards target Qt applications and offers some
     * protected members to deriving classes. Internally the class is responsible for
     * initializing the OpenTracker context and adding the MEM and MECM to a context factory.
     *
     * It provides the necessary interface to exectute both OTQt modes. Calibration and
     * operation mode differ in the way how the OpenTracker data flow is driven.  Basically
     * OTQt drives the OpenTracker data flow in cycles. Performing an OpenTracker cycle means
     * one execution of the data flow graph: source nodes import adjacent tracking events from
     * "outside", filter nodes queue events or perform their operations and forward the
     * modified events according to the OpenTracker push pattern until the events arrive at
     * the OTQt sinks nodes. The method driveOT() abstracts the procedure of such an
     * OpenTracker cycle.  The two modes use this method differently. In operation mode,
     * driveOT() method is called periodically by a timer thread, a tradeoff between in-time
     * delivery (accuracy) of mouse events and processing overhead of OTQt. For the
     * calibration procedure, executed in a dedicated command line tool as separate process,
     * the processing overhead restrictions do not apply. Therefore the driveOT() method is
     * called from within the calibration procedure code directly often nested in busy-waiting
     * <code>while</code>-loops.
     *
     * The OTQt class provides a singleton pattern to the target Qt application. With the
     * getInstance() method the singleton instance can be retrieved from any point in Qt code
     * allowing the access of the OTQt class public interface. The triggerMEM() method
     * abstracts the initialization and startup of OTQt reducing the effort of implementation
     * within a Qt application to one line of code.
     */
    class OPENTRACKER_API OTQt : public QObject {

        // mandatory macro for the Qt MOC (Meta Object Compiler)
        Q_OBJECT

    public:
        /// default operation mode timer thread timeout in milliseconds
        static int const DEFAULT_TIMER_TIMEOUT_MSEC = 20;

        /**
         * Returns the OTQt singleton instance. Creates the singleton instance if it currently
         * does not exist.
         *
         * @return singleton instance
         */
        static OTQt & getInstance();

        /**
         * Initializes the OTQt MEM module. Designed to be implemented in target Qt
         * applications. Initializes the OpenTracker context, adds MEM to the context factory,
         * parses the given configuration file, enables the mouse event generation and finally
         * starts the timer thread with the default timeout. Returns true if OTQt startup was
         * successful, otherwise returns false (eg in case of an exception).
         *
         * @param cfg_filename XML configuration filename
         * @return true if OTQt startup successful
         */
        static bool triggerMEM(std::string cfg_filename, bool dosync = false);

    private:
        /// the singleton instance
        static OTQt * the_instance_;

    public:
        /**
         * Destructor.
         */
        virtual ~OTQt();

    protected:
        /**
         * Initializes member variables. Does NOT initialize the OpenTracker context, thus
         * returns an object in @e not @e intialized state.
         */
        OTQt();

    private:
        OTQt(OTQt const & ref);
        OTQt operator = (OTQt const & ref);

    public:

        /**
         * Creates and initializes the OpenTracker context. Adds the MEM and MECM to a context
         * factory. Parses the given configuration file. During parsing the node graph is
         * created. Switches state of object to @e initialized.
         *
         * @param cfg_filename XML configuration filename
         */
        void init(std::string & cfg_filename, bool dosync = false);

        /**
         * Returns true if object state is @e initialized. Otherwise returns false.
         *
         * @return true if object state is @e initialized.
         */
        inline bool isInitialized() const { return is_initialized_; };

        /**
         * Starts the operation mode timer thread with given timeout. If the timer argument is
         * omitted, the default timeout is used. Does nothing if the timer thread is already
         * running.
         *
         * @param timeout timer thread timeout in milliseconds
         */
        void startTimer(unsigned int timeout = DEFAULT_TIMER_TIMEOUT_MSEC);

        /**
         * Returns true if the timer thread is currently running.
         *
         * @return true if timer thread currently running
         */
        inline bool isTimerActive() const { return timer_->isActive(); };

        /**
         * Stops the timer thread if it is currently running. Otherwise does nothing.
         */
        void stopTimer();

        /**
         * Enables or disables the distribution of mouse events to the target Qt application in
         * the MEM depending on the given boolean argument. Returns the current state of the
         * mouse event distribution, thus returns true if mouse event distribution is enabled
         * and false if disabled.
         *
         * @param enable true means enable, false means disable mouse event distribution
         *
         * @return current state of MEM mouse event distribution, true if enabled, false if
         * disabled
         */
        bool enableMouseEvents(bool enable) { return (me_mod_->enableQtEventDispatch(enable)); };

    protected:

        /**
         * Returns the MEM object added to the OpenTracker context.
         * @return MEM object
         */
        inline QtMouseEventModule & getMEM() { return *me_mod_; };

        /**
         * Returns the MECM object added to the OpenTracker context.
         * @return MECM object
         */
        inline QtMouseEventCalibModule & getMECM() { return *mec_mod_; };

protected slots:
/**
 * Performs one OpenTracker cycle. Performing an OpenTracker cycle means one execution
 * of the data flow graph: source nodes import adjacent tracking events from "outside",
 * filter nodes queue events or perform their operations and forward the modified events
 * according to the OpenTracker push pattern until the events arrive at the OTQt sinks
 * nodes.
 *
 * The method body is treated as critical section secured with a mutex lock. A thread
 * calling the method trying to acquire the locked mutex returns immediately, thus the
 * method is non-blocking. Otherwise the mutex is acquired at the beginning and released
 * at the end of the critical section implementing the OpenTracker cycle.
 *
 * Disables the pending event flags set in the previous OT cycle. All sinks in both
 * modules are reseted. The reset is done at the beginning of the new cycle BEFORE the
 * sinks and modules are processed. Thus, when this method returns, the pending event
 * flags set in the current cycle are still enabled and accessible.
 */
void driveOT();

    private:
        /// OpenTracker context initialzed state varible (true if initialized, false if not initialized)
        bool is_initialized_;
        /// OpenTracker context object
        Context context_;
        /// MEM object added to OpenTracker context
        QtMouseEventModule * me_mod_;
        /// MECM object added to OpenTracker context
        QtMouseEventCalibModule * mec_mod_;
        /// name of XML configuration file
        std::string cfg_filename_;
	/// whether or not to use synchroneous traversals
	bool sync_trav_;
        /// mutex lock employed in the driveOT() method
        QMutex * mutex_;
        /// operation mode timer thread
        QTimer * timer_;

#if OTQT_DEBUG_ON
        unsigned int cycle_counter_;
#endif

    };

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_OTQT_H


/* 
 * ------------------------------------------------------------
 *   End of OTQt.h
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
