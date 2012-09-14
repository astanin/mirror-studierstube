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
 * @file   OTQt.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c OTQt
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT
#include <OpenTracker/otqt/OTQt.h>
#include <newmat10/include.h> // activates the RBD_COMMON namespace in myexcept.h
#include <newmat10/myexcept.h>
#ifdef USE_QT4
#include <Qt/qdatetime.h>
#endif
#include <stdexcept>

#include <ace/Signal.h>

namespace ot {

    //-------------------------------------------------------------------------
    OTQt::OTQt()
        : QObject(),
          is_initialized_(false),
          me_mod_(new QtMouseEventModule()),
          mec_mod_(new QtMouseEventCalibModule()),
          mutex_(new QMutex()),
          timer_(new QTimer())
#if OTQT_DEBUG_ON
        , cycle_counter_(0)
#endif
    {
    }

    //-------------------------------------------------------------------------
    OTQt::~OTQt()
    {
        delete mec_mod_;
        delete me_mod_;
        delete timer_;
        delete mutex_;
    }

    OTQt * OTQt::the_instance_ = NULL;
    //-------------------------------------------------------------------------
    OTQt &
    OTQt::getInstance()
    {
        if (the_instance_ == NULL) {
            the_instance_ = new OTQt();
        }
        return *the_instance_;
    }

    //-------------------------------------------------------------------------
    void OTQt::init(std::string & cfg_filename, bool synctrav) {

        // store OT config filename
        cfg_filename_ = cfg_filename;
	sync_trav_ = synctrav;

        ACE_Sig_Action asa;
        
        
        int oflags = asa.flags();
        oflags |= SA_RESTART;
        asa.flags(oflags);
        cout << asa.flags() << endl;

        // INIT OpenTracker
        initializeContext(&context_, NULL);

        ///// add modules to context

        context_.addFactory(*mec_mod_);
        context_.addModule("QtMouseEventCalibConfig", *mec_mod_);
        context_.addFactory(*me_mod_);
        context_.addModule("QtMouseEventConfig", *me_mod_);

        // PARSE OpenTracker configuration file
        context_.parseConfiguration(cfg_filename_);

	// Pass over synchronization settings
	context_.setSynchronization(sync_trav_);
        
	// START OpenTracker
	
        context_.start();
        // connect timer to callback function
        QObject::connect(timer_, SIGNAL(timeout()), this, SLOT(driveOT()));
        // set state
        is_initialized_ = true;
    }

    //-------------------------------------------------------------------------
    void OTQt::startTimer(unsigned int timeout) {
        // stop running timer
        if (timer_->isActive())
            timer_->stop();
        // start non-singleshot timer with given timeout
        timer_->start(timeout);
    }
    //-------------------------------------------------------------------------
    void OTQt::stopTimer() {
        if (timer_->isActive())
            timer_->stop();
    }

    //-------------------------------------------------------------------------
    void OTQt::driveOT() {
        // acquire the mutex
        if (!mutex_->tryLock()) {
#if OTQT_DEBUG_ON
            OTQT_DEBUG("OTQt::driveOT(): *** START/FINISHED with cycle_counter_ = %d\n: Mutex is locked!", ++cycle_counter_);
#endif
            return;
        }
#if OTQT_DEBUG_ON
        OTQT_DEBUG("OTQt::driveOT(): *** START with cycle_counter_ = %d\n", ++cycle_counter_);
        QTime start = QTime::currentTime();
#endif

        try {

            // some OT module wants to stop
            if (context_.stop() != 0) {
                // stop timer
                stopTimer();
                // stop all OT modules
                context_.close();
                // release mutex
                mutex_->unlock();
                return;
            }

            // reset pending event flags of ALL sinks
            me_mod_->resetPendingEventBitAllSinks();
            mec_mod_->resetPendingEventBitAllSinks();

            /// todo improve this strategy for optimal cpu usage
            //context_.waitDataSignal();
	    
	    //logPrintI("OTQTloop\n");
	   
            // receive states from sources
            //context_.pushEvents();
            // process new states in modules
            //context_.pullEvents();
            if (sync_trav_) 
	    {  
	        context_.syncLoopOnce();
	    }
	    else
	    {
                context_.loopOnce();
            }
        }
#ifndef WIN32
        // exception from newmat lib
        catch (RBD_COMMON::Exception & ex_nummat) {
            OTQT_ERROR("%s\n", ex_nummat.what());
        }
#endif
// all other exceptions
 catch (std::exception & ex) {
     OTQT_ERROR("%s\n", ex.what());
 }

#if OTQT_DEBUG_ON
 QTime end = QTime::currentTime();
 int duration_msec = end.msec() - start.msec();
 OTQT_DEBUG("OTQt::driveOT(): *** FINISHED with cycle_counter_ = %d, duration_msec = %d\n",
            cycle_counter_, duration_msec);
#endif

 // release mutex
 mutex_->unlock();
    }

    //-------------------------------------------------------------------------
    bool OTQt::triggerMEM(std::string cfg_filename, bool dosync)
    {
        try {
            OTQt::getInstance().init(cfg_filename, dosync);
            OTQt::getInstance().enableMouseEvents(true);
            OTQt::getInstance().startTimer();
        } catch(std::exception & ex) {
            OTQT_ERROR("ERROR: %s\n", ex.what());
            return false;
        }
        return true;
    }

} // namespace ot

#endif // USE_OTQT


/*
 * ------------------------------------------------------------
 *   End of OTQt.cxx
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
