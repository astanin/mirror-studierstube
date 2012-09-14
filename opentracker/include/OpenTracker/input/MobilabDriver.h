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
/** header file for MobilabDriver
 *
 * @author Alexander Bornik
 * 
 * $Id $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef _MOBILABDRIVER_H
#define _MOBILABDRIVER_H

#include <map>
#include <string>
#include <vector>

class ACE_Reactor;
class ACE_TTY_IO;


#ifndef EXCLUDE_MOBILAB_SUPPORT

namespace ot {

    class Mobilab_Handler;
    class MobilabModule;

    /**
     * This is an abstract interface for the clients of the MobilabDriver class.
     * They have to register with the MobilabDriver class and will be
     * called back, if new data arrives.
     * @ingroup input
     * @author Alexander Bornik
     */
    class MobilabListener {
    public:
        /** 
         * This is the callback method called by the MobilabDriver class, 
         * whenever new data arrives. Any subclasses of MobilabListener must 
         * override it to receive the data.
         * @param sampleValue the value received from the Mobilab device.
         */
	virtual void newData( short sampleValue, double tv) = 0;
	virtual ~MobilabListener() {};
    };

    /**
     * The MobilabDriver class encapsulates the whole Mobilab and ACE framework 
     * code for reading data from the g.Mobilab device. The data provided to 
     * listeners is the original Mobilab data without any transformations.
     * All such operations are left to the client code.     
     *
     * @ingroup input
     * @author Alexander Bornik
     */
    class MobilabDriver  
    {
    public:
	MobilabDriver( ACE_Reactor * reactor_ = NULL, MobilabModule * module_ = NULL);
	virtual ~MobilabDriver();

	int open(std::string device );
	void close();

	void setDebug( bool debug );
	bool getDebug()
	{
            return debugOn;
	};

	ACE_Reactor * getReactor()
	{
            return reactor;
	};

	void addListener( MobilabListener * listener, 
                          void * userData = NULL );
	void removeListener( MobilabListener * listener, 
                             void * userData = NULL );

    protected:

        void newSample( const short * sample );
        int sendStopTransferCommand() const;

	ACE_Reactor * reactor;
	Mobilab_Handler * receiver;
        MobilabModule *module;

	bool debugOn;
	double lastsampletime;
	int samplecounter;

	std::map<MobilabListener *, void *> listeners;

	friend class Mobilab_Handler;
    };

} // namespace ot

#endif //#ifdef EXCLUDE_MOBILAB_SUPPORT
#endif // !defined(_MOBILABDRIVER_H)

/* 
 * ------------------------------------------------------------
 *   End of MobilabDriver.h
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
