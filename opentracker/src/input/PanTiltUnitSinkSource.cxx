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
/** header file for PanTiltUnitSinkSource Node.
 *
 * @author Markus Sareika
 *
 * $Id: PanTiltUnitSinkSource.h
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/PanTiltUnitSinkSource.h>
#ifdef USE_PANTILTUNIT

#include <cmath>

namespace ot {

    bool PanTiltUnitSinkSource::initComPort(int comPort)
    {
        // aquire com port name
        char COMportName[256];
#ifdef WIN32
        char COMportPrefix[10] = "COM";
#else
        char COMportPrefix[10] = "/dev/cua";	
#endif
        sprintf(COMportName, "%s%d", COMportPrefix, comPort);
        // initialize the serial port
        portstream_fd COMstream;
        COMstream = open_host_port(COMportName);
        if ( COMstream == PORT_NOT_OPENED ) return false;
        isInit = true;
        // actually move with max. speed both axis to give haptic feedback
        signed short int val=1000;
        set_desired(PAN,  SPEED, (PTU_PARM_PTR *) &val, ABSOLUTE);
        set_desired(TILT,  SPEED, (PTU_PARM_PTR *) &val, ABSOLUTE);
        val=100;
        set_desired(PAN,  POSITION, (PTU_PARM_PTR *) &val, ABSOLUTE);
        set_desired(TILT, POSITION, (PTU_PARM_PTR *) &val, ABSOLUTE);
        await_completion();
        val=0;
        set_desired(PAN,  POSITION, (PTU_PARM_PTR *) &val, ABSOLUTE);
        set_desired(TILT, POSITION, (PTU_PARM_PTR *) &val, ABSOLUTE);
        await_completion();

        if (set_mode(SPEED_CONTROL_MODE, PTU_PURE_VELOCITY_SPEED_CONTROL_MODE) != PTU_OK)
        { 
            printf("Speed control modes not supported in this PTU firmware version\n");
            return(PTU_OK); 
        }
        printf("\nSPEED_CONTROL_MODE set to PTU_PURE_VELOCITY_SPEED_CONTROL_MODE\n");
		
        // get resolution of the axis
        panResolution =  (double)get_current(PAN,RESOLUTION)*MathUtils::Pi/(216000*180);	//rad/PTUeinheit
        tiltResolution = (double)get_current(TILT,RESOLUTION)*MathUtils::Pi/(216000*180);	//rad/PTUeinheit
		
        //// test getting pos while moving
        //val=1000;
        //set_desired(PAN,  SPEED, (PTU_PARM_PTR *) &val, ABSOLUTE);
        //long res = get_current(PAN,POSITION);
        //float tip = tiltResolution*res;
        //printf("tiltPos: %f", tip);
        //val=0;
        //set_desired(PAN,  SPEED, (PTU_PARM_PTR *) &val, ABSOLUTE);

        return true;
    }


    void PanTiltUnitSinkSource::closeComPort()
    {
        close_host_port(COMstream);
    }
	
    // Process Data received from incomming Events
    void PanTiltUnitSinkSource::onEventGenerated( Event& event, Node& generator) 
    {
        static int i=0;
        // does nothing yet
        if (generator.getName().compare("SetPtuOrientation") == 0) {
            absoluteInput=event;
            if(1)
            {
                signed short int ptuPanAngle = (signed short int)(absoluteInput.getPosition()[1]/panResolution);
                signed short int ptuTiltAngle = (signed short int)(absoluteInput.getPosition()[0]/tiltResolution);

                //if (set_mode(SPEED_CONTROL_MODE, SET_INDEPENDENT_CONTROL_MODE) != PTU_OK)
                //	printf("Speed control mode change failed\n");
                //printf("\nSPEED_CONTROL_MODE set to SET_INDEPENDENT_CONTROL_MODE\n");
                float speed;
                signed short int val;
                if(absoluteInput.getAttribute("absoluteSpeed", speed)>0) val =(int)speed; else val=500;
                //signed short int val=1000;
                set_desired(PAN,  SPEED, (PTU_PARM_PTR *) &val, ABSOLUTE);
                set_desired(TILT,  SPEED, (PTU_PARM_PTR *) &val, ABSOLUTE);
                set_desired(PAN,  POSITION, (PTU_PARM_PTR *) &ptuPanAngle, ABSOLUTE);
                set_desired(TILT, POSITION, (PTU_PARM_PTR *) &ptuTiltAngle, ABSOLUTE);
				
                await_completion();
                process=true;
            }

            return;
        }
        // picking calculation
        if (generator.getName().compare("PickMouse") == 0) {
            // store input event from sink		
            pickMouse=event;		
            return;
        }
        // the top offset
        if (generator.getName().compare("TopOffset") == 0) {
            // store input event from sink		
            topOffset=event;
            process=true;			
            return;
        }
        // the pysical location of the ptu
        if (generator.getName().compare("PtuLocation") == 0) {
            // store input event from sink
            ptuLocation=event;
            process=true;			
            return;
        }
        // relative input which defines the moving speed of the ptu axis and the zoom
        if (generator.getName().compare("RelativeInput") == 0) {

            //if (set_mode(SPEED_CONTROL_MODE, PTU_PURE_VELOCITY_SPEED_CONTROL_MODE) != PTU_OK)
            //	printf("Speed control mode change failed\n");
            //printf("\nSPEED_CONTROL_MODE set to PTU_PURE_VELOCITY_SPEED_CONTROL_MODE\n");

            // store input event from sink
            relativeInput=event;

            if(isInit)
            {
                // rotate ptu according to relative input
                long val;
                // pan speed rotate
                if ( (relativeInput.getPosition()[0] < 0.2)&&(relativeInput.getPosition()[0] > -0.2) ) 
                {
                    halt(PAN);
                    movingPan = false;
                }else{
                    val=(long)(relativeInput.getPosition()[0]*-285);	// min 285 - max 1000
                    set_desired(PAN,  SPEED, (PTU_PARM_PTR *) &val, ABSOLUTE);
                    movingPan = true;
                }
                // tilt speed rotate
                if ( (relativeInput.getPosition()[1] < 0.2)&&(relativeInput.getPosition()[1] > -0.2) )
                {
                    halt(TILT);
                    movingTilt = false;
                }else{
                    val=(long)(relativeInput.getPosition()[1]*-285);	// min 285 - max 1000
                    set_desired(TILT, SPEED, (PTU_PARM_PTR *) &val, ABSOLUTE);
                    movingTilt = true;
                }
                ////////////////////////////////////////////
                // handle button events
                // button 1-4 pressed simultainiously
                if (relativeInput.getButton()== 15) // reset and recalibrate ptu
                    SerialOut(UNIT_RESET);
            }
            // handle lanc control
            lanc->Zoom(relativeInput.getPosition()[2]);
            process=true;	
        }
    }

    double PanTiltUnitSinkSource::getTiltAngle()
    {
        long pos = get_current(TILT,POSITION);
        return (tiltResolution*pos);
    }

    double PanTiltUnitSinkSource::getPanAngle()
    {
        long pos = get_current(PAN,POSITION);
        printf("panPos: %f \n", pos);
        return (panResolution*pos);
    }

    void PanTiltUnitSinkSource::pushEvent()
    {
        lock();
        if (process||movingPan||movingTilt)
        {
            //source->delay = source->delayEvent;

            //if((cycle + source->offset) % source->frequency == 0 )
            //{
            //	source->push();
            //}
				
            //source->publishEvent = false;

            process = false;
            //OSUtils::sleep(source->delayEvent);
					
            push();
            unlock();
            updateObservers(event);
        }
        else
        {
            unlock();
        }
		
    }
    
    void PanTiltUnitSinkSource::pullEvent()
    {
    }

    void PanTiltUnitSinkSource::push()
    {
        // calculate transformation with respect to ptu location and camera offset
        if(isInit)
        {
            tiltAngle= tiltResolution*(double)get_current(TILT,POSITION);
            panAngle= panResolution*(double)get_current(PAN,POSITION);
        }
        // initialize arrays and put ptu location and offset into matrices
        MathUtils::Matrix4x4 mPos = {{1, 0, 0, ptuLocation.getPosition()[0]},
                                     {0, 1, 0, ptuLocation.getPosition()[1]},
                                     {0, 0, 1, ptuLocation.getPosition()[2]},
                                     {0, 0, 0, 1}};
        MathUtils::Matrix4x4 mPosOffset = {{1, 0, 0, topOffset.getPosition()[0]},
                                           {0, 1, 0, topOffset.getPosition()[1]},
                                           {0, 0, 1, topOffset.getPosition()[2]},
                                           {0, 0, 0, 1}};
        MathUtils::Matrix4x4 mOri = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};		
        MathUtils::Quaternion qOri={ptuLocation.getOrientation()[0], ptuLocation.getOrientation()[1], ptuLocation.getOrientation()[2], ptuLocation.getOrientation()[3]};
        MathUtils::quaternionToMatrix(qOri, mOri);
        MathUtils::Matrix4x4 mOriOffset = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}};
        MathUtils::Quaternion qOriOffset={topOffset.getOrientation()[0], topOffset.getOrientation()[1], topOffset.getOrientation()[2], topOffset.getOrientation()[3]};
        MathUtils::quaternionToMatrix(qOriOffset, mOriOffset);
        MathUtils::Matrix4x4 mOriTop, mPtu, mTemp, mPosRes, mOriRes;
		
        // calc ptu angles
        MathUtils::Matrix4x4 mTilt = {{1, 0, 0, 0},{0, cos(tiltAngle), -sin(tiltAngle), 0},
                                      {0, sin(tiltAngle), cos(tiltAngle), 0},{0, 0, 0, 1}};               
        MathUtils::Matrix4x4 mPan = {{cos(panAngle), 0, sin(panAngle), 0}, {0, 1, 0, 0},
                                     {-sin(panAngle), 0, cos(panAngle), 0},{0, 0, 0, 1}};
        MathUtils::matrixMultiply(mTilt, mPan, mPtu);
        // convert the current ptu orientation for output
        double qPtuOri[4];
        MathUtils::matrixToQuaternion(mPtu, qPtuOri);
        // calc orientation top
        MathUtils::matrixMultiply(mOri, mPtu, mOriTop);
		
        //// calculate top offset
        ////MathUtils::matrixMultiply(mPosTop, mOriTop, mTemp);
        ////MathUtils::matrixMultiply(mPos, mOri, mPosTop);
        MathUtils::matrixMultiply(mPos, mOriTop, mTemp);
        MathUtils::matrixMultiply(mTemp, mPosOffset, mPosRes);
        MathUtils::matrixMultiply(mOriTop, mOriOffset, mOriRes);
        // calculate top offset RT
        //MathUtils::matrixMultiply(mOriTop, mPos, mTemp);
        //MathUtils::matrixMultiply(mTemp, mOriOffset, mTemp2);
        //MathUtils::matrixMultiply(mTemp2, mPosOffset, mPosRes);
        //MathUtils::matrixMultiply(mPosRes, mOriOffset, mOriRes);

        // put result into event
        for (int i(0);i<3;i++) event.getPosition()[i]=(float)mPosRes[i][3];
        double qOriRes[4];
        //MathUtils::matrixToQuaternion(mPosRes, qOriRes);
        MathUtils::matrixToQuaternion(mOriRes, qOriRes);
        for (int i(0);i<4;i++) event.getOrientation()[i]=(float)qOriRes[i];
		
        // put ptu angles and quaternion into event
        event.setAttribute<float>("ptuPan", (float)panAngle);
        event.setAttribute<float>("ptuTilt", (float)tiltAngle);
        for (int i(0);i<4;i++)
        {
            std::vector<float> defVec = std::vector<float>(3, .0f);
            defVec.push_back(1.f);
            event.getAttribute("ptuOri", defVec)[i]=(float)qPtuOri[i];
        }
		
        // put extra attributes into event
        float fov = lanc->getFieldOfView();
        event.setAttribute<float>("fieldOfView", fov);
        float tp = (float)lanc->timePos;
        event.setAttribute<float>("timePos", tp );
        float zf = (float)lanc->zoomFactor;
        event.setAttribute<float>("zoomFactor", zf );
        event.timeStamp();
		
        // this queue is meant as delay mechanism
		
        // push result event into queue
        //delayQueue.push(event);

        //// delayEvent = length of queue

        //// pop event from queue

        //if (delayQueue.size()>delayEvent)
        //{
        //	event = delayQueue.front();
        //	delayQueue.pop();
        //	event.timeStamp();
        //	updateObservers( event );
        //}
    }

	
    PanTiltUnitSinkSource::~PanTiltUnitSinkSource()
    {
        // Destructor method
    }
	
}

#endif //USE_PANTILTUNIT

/* 
 * ------------------------------------------------------------
 *   End of PanTiltUnitSinkSource.cxx
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
