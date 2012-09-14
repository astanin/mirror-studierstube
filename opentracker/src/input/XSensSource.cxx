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
/** source file for XSensSource
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: XSensSource.cxx 717 2004-07-27 11:54:49Z reitmayr $
 *
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <ace/Log_Msg.h>

#ifdef WIN32

#include <objbase.h>
#include <xsens/IMTObj.h>
#endif

// return values for MT_GetOrientation function
#define MT_NEWDATA			1
#define MT_NODATA			2
#define MT_NOSENSORID		3
#define MT_INCOMPLETE		4
#define MT_CHECKSUMERROR	5
#define MT_NOPORT			6
#define MT_NOCALIBVALUES	7
#define MT_POWERLOSS		8

// output possiblities for MT object
#define MT_LOGQUATERNION	0
#define MT_LOGEULER		    1
#define MT_LOGROTMATRIX	    2

#include <OpenTracker/input/XSensSource.h>


#ifndef OT_NO_XSENS_SUPPORT


namespace ot {


    XSensSource::XSensSource( int _comport, bool _amd ):
        comport( _comport ),
        amd( _amd )
    {
#ifdef WIN32
        HRESULT hRes = CoCreateInstance(CLSID_MotionTracker, NULL, CLSCTX_SERVER, IID_IMotionTracker, (void**) &pMT);
	if (FAILED(hRes))
	{
            ACE_DEBUG((LM_ERROR, "XSensSource : Error %x in CoCreateInstance for MT object!",hRes));
            exit(1);
	}

        // pMT->MT_SetCalibratedOutput( TRUE );

        pMT->MT_SetOutputMode(MT_LOGQUATERNION);

        pMT->MT_SetCOMPort( comport );
#endif
    }

    XSensSource::~XSensSource()
    {
#ifdef WIN32
        if( pMT != NULL )
        {
            pMT->Release();
            pMT = NULL;
        }
#endif
    }

    void XSensSource::pushEvent()
    {
#ifdef WIN32
        // do something with xsens
        float orientationData[9] = {0};
	VARIANT orientationBuffer;
        void * pDest;
        short nNew = 0;

        pMT->MT_GetOrientationData( &nNew, &orientationBuffer );
        if( nNew == MT_NEWDATA )
        {
            if( orientationBuffer.vt != VT_EMPTY )
            {
                HRESULT hr = SafeArrayAccessData(orientationBuffer.parray, &pDest);
                // One dimensional array. Get the bounds for the array.
            
                if (SUCCEEDED(hr))
                {				
                    __try{
                        // Copy data from the VARIANT array to the local fData array
                        memcpy(orientationData,pDest,(orientationBuffer.parray->rgsabound->cElements * sizeof(float)));
                    }
                    __except(GetExceptionCode() == STATUS_ACCESS_VIOLATION){
                        return;                
                    }	
                
                    SafeArrayUnaccessData(orientationBuffer.parray);	// Invalidate pointer
                
                    // Variant must be cleared. This also destroys the SafeArray
                    VariantClear(&orientationBuffer);
                
                    // orientationData now contains orientation data
                
                    // skalar component
                    event.timeStamp();
                    event.getOrientation()[Q_W] = orientationData[0];
                    event.getOrientation()[Q_X] = orientationData[1];
                    event.getOrientation()[Q_Y] = orientationData[2];
                    event.getOrientation()[Q_Z] = orientationData[3];

                    updateObservers( event );

                }
            }
        }
#if 0
        else
        {
            ACE_DEBUG((LM_DEBUG,"XSensSource : got result %i\n", nNew));
        }
#endif
#endif
    }

    void XSensSource::pullEvent()
    {
        // nothing to do
    }

}  // namespace ot


#endif // OT_NO_XSENS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of XSensSource.cxx
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
