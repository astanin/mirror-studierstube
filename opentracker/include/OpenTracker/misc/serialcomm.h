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
/** header file for serial communication.
 *
 * @author Thomas Peterseil
 *
 * $Id: serialcomm.h 2085 2007-11-07 21:45:56Z jfn $
 * @file                                                                   */
/* ======================================================================= */

/* error code : fehler rückgaben kleiner 0, codiern fehler */

#ifndef _SERIAL_H
#define _SERIAL_H

#ifdef WIN32
#include <Windows.h>
#endif

/** describes the parameter for a serial port. used to open a port with
 * specific parameters */
typedef struct
{
    /// port to open
    char pathname[256];
    /// bits per second
    int baudrate;                  
    /// none, even, odd = 0, 2, 1
    int parity;  
    /// bits of data 5-8   
    int bits;         
    /// stopbits 1-2    
    int sbit;   
    /// yes (!=0) or no (0) 
    int hwflow;           
    /// yes (!=0) or no (0)                  
    int swflow;  
    /// yes (!=0) or no (0)                  
    int blocking;
	// map CR and NL
	int mapCR;
	// canonical mode
	int canon;
} SerialParams;

/** describes a port, via its name and handle. This struct encapsulates 
 * the differences between UNIX and Windows handles etc. */
typedef struct
{
    char pathname[256];
#ifndef WIN32
    int fd;
#else
    HANDLE handle;
#endif
  SerialParams params;
} SerialPort;

#ifdef WIN32
  int setDCB(DCB *dcb, SerialParams *params);
#else
  int settermio(SerialParams *params, SerialPort* port);
#endif

void initSerialParams(SerialParams *params);

int openSerialPort(SerialPort *port, SerialParams *params);

int closeSerialPort(SerialPort *port);

int setRTSSerialPort(SerialPort *port, int level);

int waitforoneSerialPort(SerialPort *port, long time);

int waitforallSerialPorts(SerialPort **ports, int count, int *setofports, long time);

int readfromSerialPort(SerialPort *port, char *buf, int count);

int writetoSerialPort(SerialPort *port, const char *buf, int count);

int sendBreakSerialPort(SerialPort *port);

int setIOParams(SerialPort *port, int baud, int databits, char parity, int stopbits, bool handshake);

#endif
/* 
 * ------------------------------------------------------------
 *   End of serialcomm.h
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
