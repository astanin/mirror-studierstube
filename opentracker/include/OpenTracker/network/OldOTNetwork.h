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
/** header file for Opentracker 1.1 Network data structures
  *
  * @author Valerie Maquil
  *
  * $Id: OldOTNetwork.h 900 2006-01-19 16:47:43Z spiczak $
  * @file                                                                    */
 /* ======================================================================== */

#ifndef _OLDOTNETWORK_H
#define _OLDOTNETWORK_H

/**
 * @defgroup network Network Classes
 * This group contains classes related to network input / output. Sofar only
 * a single network protocol is implemented and the whole design is very
 * basic. It is expected to grow in the future to be more versatile.
 */

#ifdef OT_OLD_NETWORK_SUPPORT

/** the data structure for a record of the Flexible Network Protocol used by
 * the Studierstube. 
 * @ingroup network
 */
typedef struct
{
    short int headerId;         // Id (magic number)
    short int revNum;           // Revision Number
    short int maxStationNum;    // Number of stations dealt with by the server
    short int headerLength;     // Length of the header (from the start of the
                                // record; Offset to the first station data)
    short int numOfStations;    // Number of station packets to follow
    short int commentLength;    // Length of the comment
    char data[10000];           // Holds the following data:
                                // n bytes of comment

                                // Data per Station:
                                // short int number of the station
                                // short int format (Quaternion, Euler, Matrix)
                                // short int button states (binary coded)
                                // short int bytes per station (incl. this header)
                                // short int length of the name of the station
                                // n bytes name of the station
                                // position and orientation according to format
} OldFlexibleTrackerDataRecord;

#endif
#endif
