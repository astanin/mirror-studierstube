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
/** header file for Lanc Node.
  *
  * @author Markus Sareika
  *
  * $Id: Lanc.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section PanTiltUnitSinkSource
 * The Lanc node encapsulates the lanc control interface
 *
 */

#ifndef _LANC_H
#define _LANC_H

#include "../OpenTracker.h"

#ifdef USE_PANTILTUNIT

#include <windows.h>
#include <iostream> 

#ifdef WIN32
#include  <Lanc32.h>
#pragma comment(lib,"lanc32")
#endif

namespace ot {

/**
 * This class encapsulates the lanc control interface
 * @author Markus Sareika
 * @ingroup input
 */
class Lanc
{
// Members
public: 

    /** constructor*/
	Lanc();

     /** destructor*/
	~Lanc();
	
	void Zoom( float speed );

	float getFieldOfView();

	void setTeleAngle(float);
	
	void setWideAngle(float);

	double timePos, zoomFactor;

	void updateTimePos();

	bool isZooming();
	
protected:

private:

    #ifdef WIN32
	HANDLE lancDevice;
	LANCUSBID usb_id;
	#endif

	bool initLanc, zoomout, zoomin;
	float wideAngle, teleAngle, offsetFactor;
	double startTime, maxTimePos;
	
};

}  // namespace ot

#endif //USE_PANTILTUNIT

#endif //_LANC_H
