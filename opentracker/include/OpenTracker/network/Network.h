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
/** header file for Network data structures
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Network.h 1724 2007-01-16 22:14:18Z samset $
 * @file                                                                    */
/* ======================================================================== */

#ifndef _NETWORK_H
#define _NETWORK_H

#define MAXMSGLEN 0x7fff

/**
 * @defgroup network Network Classes
 * This group contains classes related to network input / output. So far only
 * a single network protocol is implemented and the whole design is very
 * basic. It is expected to grow in the future to be more versatile.
 */

/** the data structure for a record of the Flexible Network Protocol used by
 * the Studierstube.
 * @ingroup network
 */
typedef struct FlexibleTrackerDataRecord
{
    FlexibleTrackerDataRecord() : headerId(0), revNum(0), maxStationNum(0), numOfStations(0), bufferLength(0) {};
    short int headerId;         // Id (magic number)
    short int revNum;           // Revision Number
    short int maxStationNum;    // Number of stations dealt with by the server
    short int numOfStations;    // Number of station packets to follow
    ACE_INT32 bufferLength;

}FlexibleTrackerDataRecord;

#endif

/*
 * ------------------------------------------------------------
 *   End of Network.h
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
