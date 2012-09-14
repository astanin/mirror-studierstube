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
/** cxx file for GoGoSinkSource Node.
 *
 * @author Markus Sareika
 *
 * $Id: GoGoSinkSource.h  sareika $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/GoGoSinkSource.h>

#ifdef USE_GOGO

#include <cmath>

#define DIST(x, y) sqrt(((x[2]-y[2])*(x[2]-y[2])+(x[1]-y[1])*(x[1]-y[1])+(x[0]-y[0])*(x[0]-y[0])))


namespace ot {
    void GoGoSinkSource::onEventGenerated( Event& event, Node& generator) 
    {
        // the RelativeInput
        if (generator.getName().compare("RelativeInput") == 0) {
            // store input event from sink		
            relativeInputPos[0] = event.getPosition()[0];
            relativeInputPos[1] = event.getPosition()[1];
            relativeInputPos[2] = event.getPosition()[2];
            relativeInputOri[0] = event.getOrientation()[0];
            relativeInputOri[1] = event.getOrientation()[1];
            relativeInputOri[2] = event.getOrientation()[2];
            relativeInputOri[3] = event.getOrientation()[3];
            relativeInputBut = event.getButton();

            newEvent = true;			
            return;
        }
		// the SetAbsolutePosition
		if (generator.getName().compare("SetAbsoluteLocation") == 0) {
			// store input event from sink	
			setAbsolutePos[0] = event.getPosition()[0];
			setAbsolutePos[1] = event.getPosition()[1];
			setAbsolutePos[2] = event.getPosition()[2];
			setAbsoluteOri[0] = event.getOrientation()[0];
			setAbsoluteOri[1] = event.getOrientation()[1];
			setAbsoluteOri[2] = event.getOrientation()[2];
			setAbsoluteOri[3] = event.getOrientation()[3];
			setAbsoluteBut = event.getButton();
			setAbsPos = true;		
			return;
		}

        // the ViewerLocation
        if (generator.getName().compare("ViewerLocation") == 0) {
            // store input event from sink		
            viewerLocationPos[0] = event.getPosition()[0];
            viewerLocationPos[1] = event.getPosition()[1];
            viewerLocationPos[2] = event.getPosition()[2];
            viewerLocationOri[0] = event.getOrientation()[0];
            viewerLocationOri[1] = event.getOrientation()[1];
            viewerLocationOri[2] = event.getOrientation()[2];
            viewerLocationOri[3] = event.getOrientation()[3];
            newEvent = true;		
            return;
        }

        // the GoGoDeviceKit
        if (generator.getName().compare("GoGoDeviceKit") == 0) {
            // store input event from sink		
            gogoDeviceKitBut = event.getButton();
            newKitEvent = true;		
            return;
        }
    }
	
    void GoGoSinkSource::computeNewLocation()
    {
        float tmp[3], rq[4], q[4];
		
        tmp[0] = relativeInputPos[0] * tWeight;
        tmp[1] = relativeInputPos[1] * tWeight;
        tmp[2] = relativeInputPos[2] * tWeight;
		
        // debug output
        //#define prvec3(n) printf("%.2f %.2f %.2f", n[0], n[1], n[2]);
        //printf ("\nCursor Pos:"); prvec3(tmpEvent.getPosition());
        //printf ("\nViewer Pos:"); prvec3 (viewerLocation.getPosition());
        //printf ("\n");

        // calculate distance between cursor and viewer
        tmpEventCursorDistance = DIST(tmpEventPos, viewerLocationPos);
		
		// reset if cursor is to near the viewer
		if (tmpEventCursorDistance<0.3f)
		{
			gogoDeviceKitBut = OTCOM_RESETPOSITION;
			newKitEvent = 1;
			return;
		}

        // GoGo factor
        float gogofact = 1.0f;
        if (tmpEventCursorDistance > nlDistance) {
            gogofact = 1.0f+(gogok*(1.0f - nlDistance/tmpEventCursorDistance)*(1.0f - nlDistance/tmpEventCursorDistance));
        }

        // Scaling factor
        if (tmpEventCursorDistance > 25.0f) {
            tmpEventScalingFactor = (float)(1.0f+(tmpEventCursorDistance-25.0f)/25.0f);
        } else {
            tmpEventScalingFactor = 1.f;
        }

        // fix me i was deactivated because i dont work correct in some situationes
        ///MathUtils::rotateVector(viewerLocationOri, tmp, corr);
        ///tmpEventPos[0] += corr[0]*gogofact;
        ///tmpEventPos[1] += corr[1]*gogofact;
        ///tmpEventPos[2] += corr[2]*gogofact;
        tmpEventPos[0] += tmp[0]*gogofact;
        tmpEventPos[1] += tmp[1]*gogofact;
        tmpEventPos[2] += tmp[2]*gogofact;


        MathUtils::Matrix4x4 mTmp = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};
        MathUtils::Quaternion qTmp={(double)relativeInputOri[0], (double)relativeInputOri[1], 
                                    (double)relativeInputOri[2], (double)relativeInputOri[3]};
        MathUtils::quaternionToMatrix(qTmp, mTmp);
        double newOri[3];
        MathUtils::MatrixToEuler(newOri, mTmp);
        MathUtils::eulerToQuaternion ((float)(newOri[0]*rWeight), (float)(newOri[1]*rWeight), (float)(newOri[2]*rWeight), q);

        if (!useAbsRotation) {		
            // Rotate in Cursor Space
            MathUtils::multiplyQuaternion(tmpEventOri, q, rq);
        } else {
            // Use Absolute Rotation (Viewer Space)
            // Transfer identity quaternion from Object into Camera Space, rotate, and transform back
            float irotC[4],irotO[4];
            MathUtils::invertQuaternion(viewerLocationOri, irotC);
            MathUtils::invertQuaternion(tmpEventOri, irotO);
            float init[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            float st[4],q1[4],q2[4],q3[4],q4[4],q5[4];
            MathUtils::axisAngleToQuaternion(init, st);
            MathUtils::multiplyQuaternion(init, irotO, q1);
            MathUtils::multiplyQuaternion(q1, viewerLocationOri, q2);
            MathUtils::multiplyQuaternion(q2,q,q3);
            MathUtils::multiplyQuaternion(q3, irotC, q4);
            MathUtils::multiplyQuaternion(q4, tmpEventOri, q5);
            MathUtils::multiplyQuaternion(tmpEventOri, q5, rq);
        }
        tmpEventOri[0] = rq[0];
        tmpEventOri[1] = rq[1];
        tmpEventOri[2] = rq[2];
        tmpEventOri[3] = rq[3];
        MathUtils::normalizeQuaternion(tmpEventOri);
        // forward button input
        tmpEventBut = relativeInputBut;
    }

    void GoGoSinkSource::computeGoGoNodeKitEvent()
    {
        // Process Data received from the Nodekit
        switch(gogoDeviceKitBut) {
            case OTCOM_RESETPOSITION:
                { 
                    // init
                    MathUtils::Matrix4x4 vPos = {{1, 0, 0, viewerLocationPos[0]},
                                                 {0, 1, 0, viewerLocationPos[1]},
                                                 {0, 0, 1, viewerLocationPos[2]},
                                                 {0, 0, 0, 1}};
                    MathUtils::Matrix4x4 vOri = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};
                    MathUtils::Matrix4x4 cTmp = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};
                    MathUtils::Matrix4x4 cPos = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};
                    MathUtils::Matrix4x4 cOff = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, -5},{0, 0, 0, 1}};
                    MathUtils::Quaternion vqOri={viewerLocationOri[0], viewerLocationOri[1], viewerLocationOri[2], viewerLocationOri[3]};
                    MathUtils::quaternionToMatrix(vqOri, vOri);
                    // calc
                    MathUtils::matrixMultiply(vPos, vOri, cTmp);
                    MathUtils::matrixMultiply(cTmp, cOff, cPos);
				
                    for (int i(0);i<3;i++) tmpEventPos[i]=(float)cPos[i][3];
                    // update distance
                    tmpEventCursorDistance = DIST(tmpEventPos, viewerLocationPos);		
                }
                break;
            case OTCOM_RESETROTATION:
                { 
                    float np[4], cq[4], rq[4];
                    np[0]=0.0f; 
                    np[1]=1.0f; 
                    np[2]=0.0f; 
                    np[3]=0.0f;
                    MathUtils::axisAngleToQuaternion(np, cq);

                    MathUtils::multiplyQuaternion(cq, viewerLocationOri ,rq);
                    tmpEventOri[0] = rq[0];
                    tmpEventOri[1] = rq[1];
                    tmpEventOri[2] = rq[2];
                    tmpEventOri[3] = rq[3];
                }
                break;
            case OTCOM_TOGGLE_ROTATECAMERAAXIS:
                {
                    useAbsRotation?useAbsRotation=false:useAbsRotation=true;
                }
                break;
            default:
                break;
        }
    }

	void GoGoSinkSource::setAbsoluteLocation()
	{
		for (int i(0);i<3;i++) 
		{
			tmpEventPos[i] = setAbsolutePos[i];
			tmpEventOri[i] = setAbsoluteOri[i];
		}
		tmpEventOri[3] = setAbsoluteOri[3];
		tmpEventBut = setAbsoluteBut;
	}

    void GoGoSinkSource::push()
    {
        event.timeStamp();
        updateObservers( event );
    }

	
} // namespace ot

#endif //USE_GOGO

/* 
 * ------------------------------------------------------------
 *   End of GoGoSinkSource.cxx
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
