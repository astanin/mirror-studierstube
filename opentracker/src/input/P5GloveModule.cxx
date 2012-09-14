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
/** source file for P5GloveModule module.
 *
 * @author Hannes Kaufmann, Istvan Barakonyi
 *
 * $Id: P5GloveModule.cxx 1606 2006-11-06 13:24:17Z mendez $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/P5GloveModule.h>

#ifdef USE_P5GLOVE

#include <cstdio>
#include <string>
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream>
#endif

//using namespace std;

#include <cmath>
#include <OpenTracker/input/P5GloveSource.h>

namespace ot {

	
	OT_MODULE_REGISTER_FUNC(P5GloveModule){
	      OT_MODULE_REGISTRATION_DEFAULT(P5GloveModule , "P5GloveConfig");
	}

    //These variables contain the actual x, Y, Z position of the cursor
    int nXPos = 0, nYPos = 0, nZPos = 0;

    //These variables contain the frame to frame deltas of the cursor
    float fXMickey = 0.0f, fYMickey = 0.0f, fZMickey = 0.0f;

    //These variables contain the filtered oreintation information
    float fAbsYawPos, fAbsPitchPos, fAbsRollPos;
    float fRelYawPos, fRelPitchPos, fRelRollPos;


    P5GloveModule::P5GloveModule() : Module(), NodeFactory()
    {
        P5device=new CP5DLL();
    }

    // Destructor method
    P5GloveModule::~P5GloveModule()
    {
	nodes.clear();
    }

    // initializes trackers
    void P5GloveModule::init(StringTable& attributes, ConfigNode * localTree)
    {
        if( attributes.get("P5Id", &P5Id ) == 0 )
            P5Id = 0;
        printf("P5 Glove is configured with id %d\n",P5Id);

        Module::init( attributes, localTree );
    }

    // This method is called to construct a new Node.
    Node * P5GloveModule::createNode( const string& name, StringTable& attributes)
    {
        if( name.compare("P5GloveSource") == 0 )
        {       
            int num;
            int finger;
            num = attributes.get("finger", &finger);
            if (num == 0) finger=0;
            else if( finger < 0 || finger > 4)
            {
                /// finger: 0 - 4, 0 = thumb, 1 = index, 2 = middle, 3 = ring, 4 = pinky
                printf("Finger index must be between 0 and 4 \n");
                return NULL;
            }
            P5GloveSource *source = new P5GloveSource(finger);
            source->event.getConfidence() = 1.0f;
            nodes.push_back( source );
            printf("Built P5GloveSource node for finger %d \n",finger);
            initialized = 1;
            return source;
        }
        return NULL;
    }

    // opens P5Glove library
    void P5GloveModule::start()
    {
        if( isInitialized() == 1 )
	{
            // The P5 can always be initialized, no matter if it is turned on or off
            // The P5_Init() method always returns 1
            // P5_Init() probably only checks if the device is registered in Windows.
            // printf("P5 status %d \n",P5device->P5_Init());
            if (P5device->P5_Init() != (P5BOOL)true)
	    {
                printf("No P5 detected...\n");
            }
	    else
	    {
                printf("P5 Found...\n");
    		P5device->P5_SetMouseEvent(P5Id, false);
	    }
        }
	
	printf("P5Glove started\n");
    }

    // closes P5Glove library
    void P5GloveModule::close()
    {
	// if P5 glove is turned ON and actually used by stb, than mouseEvent must be set back, 
	// otherwise calling SetMouseEvent would cause a crash on exit
	if( isInitialized() == 1 )
            P5device->P5_SetMouseEvent(0, true);
	P5device->P5_Close();
	printf("Closing P5Glove \n");
    }

    // pushes events into the tracker tree.
    void P5GloveModule::pushEvent()
    {
        if( isInitialized() == 1 )
        {
            for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
            {
                P5GloveSource *source = (P5GloveSource *)(*it);
   	        if( P5device->m_P5Devices != NULL )
	        {
                    //              P5Motion_InvertMouse(P5MOTION_INVERTAXIS, P5MOTION_NORMALAXIS, P5MOTION_NORMALAXIS);
                    //	    	    P5Motion_SetClipRegion((-dwHeight), dwHeight, (-dwWidth), dwWidth, 0, 1000);

                    P5Motion_Process();

                    // Rotation (Roll, Pitch, Yaw) is internally presented in Degrees
                    // Must convert it to radiant to pass it to the eulerToQuaternion conversion routine
                    // pure data without filtering would be 
                    // P5device->m_P5Devices[P5Id].m_froll, P5device->m_P5Devices[P5Id].m_fpitch
                    // P5device->m_P5Devices[P5Id].m_fyaw
                    MathUtils::eulerToQuaternion(fAbsRollPos*MathUtils::Pi/180, 
                                                 fAbsPitchPos*MathUtils::Pi/180, 
                                                 fAbsYawPos*MathUtils::Pi/180,
                                                 source->event.orientation);
                }

                source->event.getPosition()[0] = fFilterX;
                source->event.getPosition()[1] = fFilterY;
                source->event.getPosition()[2] = fFilterZ;

                /// button not implemented yet
                /// if the button is pushed (changes event) in any event --> button clicked
                /// there is only one button, so event.getButton() = 0 OR 1
			
            
                //            if (it == nodes.begin() && buttonEvent!=glove->buttonEvent()){
                if (it == nodes.begin() && 
                    P5device->m_P5Devices[P5Id].m_byBendSensor_Data[P5_INDEX]>BEND_THRESHOLD)
                {
                    source->event.getButton() = 1;
                }
                else 
                    source->event.getButton() = 0;

                //            source->event.getButton() = 0;

                source->event.timeStamp();
                source->updateObservers( source->event );			
            }
	}
    }


    /***********************************************
Function: P5Motion_SetClipRegion()
Use: Call this function to initialize a clipping region
Parameters: int xstart	- low side xclip point
			int xend	- high side xclip point
			int ystart	- low side yclip point
			int yend	- high side yclip point
			int zstart	- low side zclip point
			int zend	- high side zclip point
    ***********************************************/
    int nxclipstart = 0, nxclipend = 1024, nyclipstart = 0, nyclipend = 768, nzclipstart = 0, nzclipend = 1024;

    void P5GloveModule::P5Motion_SetClipRegion(int xstart, int xend, int ystart, int yend, int zstart, int zend)
    {
	nxclipstart = xstart;
	nxclipend = xend;
	nyclipstart = ystart;
	nyclipend = yend;
	nzclipstart = zstart;
	nzclipend = zend;
    }

    /***********************************************
Function: P5Motion_InvertMouse()
Use: Call this function to flip an axis if necessary
Parameters: int xaxis	- P5MOTION_NORMALAXIS or P5MOTION_INVERTAXIS
			int yaxis	- P5MOTION_NORMALAXIS or P5MOTION_INVERTAXIS
			int zaxis	- P5MOTION_NORMALAXIS or P5MOTION_INVERTAXIS
    ***********************************************/
    int nxinvert = 0, nyinvert = 0, nzinvert = 0;

    void P5Motion_InvertMouse(int xaxis, int yaxis, int zaxis)
    {
	nxinvert = xaxis;
	nyinvert = yaxis;
	nzinvert = zaxis;
    }

    /***********************************************
Function: P5Motion_FilterXYZ()
Use: Internal Function.  Used to filter XYZ Data
Parameter: None
    ***********************************************/
    float fXPos[P5MOTION_XYZFILTERSIZE], fYPos[P5MOTION_XYZFILTERSIZE], fZPos[P5MOTION_XYZFILTERSIZE];
    //float fFilterX, fFilterY, fFilterZ;

    void P5GloveModule::P5Motion_FilterXYZ()
    {
	static int firsttime = 1;
	if (P5device != NULL)
	{
            if (firsttime==1)
            {
                //dont process anything on the first call, just init our filter arrays
                for (int i=0; i<P5MOTION_XYZFILTERSIZE; i++)
                {
                    fXPos[i] = P5device->m_P5Devices[P5Id].m_fx;
                    fYPos[i] = P5device->m_P5Devices[P5Id].m_fy;
                    fZPos[i] = P5device->m_P5Devices[P5Id].m_fz;
                }

                firsttime = 0;
            }
            else
            {
                //We are going to implement a dynamic filter, which will flush the filter array values at different rates
                //based on the rate of change of the users hand.  This will allow for greater latency of motion.
                //The setpoint determines the number of pixel motion that will flush the entire filter.
                //The idea is when the user doensnt move much, we average alot of frames, but during fast motion, we
                //average fewer and fewer frames in order to reduce latency

#define FLUSH_SETPOINT	30.0f

                float xflushsize, yflushsize, zflushsize;
                int i, j;
			
                //Lets determine the number of frames we intend to average together

                xflushsize = fabs(P5device->m_P5Devices[P5Id].m_fx - fXPos[P5MOTION_XYZFILTERSIZE-1])/2.0f;
                xflushsize *= P5MOTION_XYZFILTERSIZE/FLUSH_SETPOINT;
                xflushsize = floor(xflushsize+1.0f);
                if (xflushsize>(P5MOTION_XYZFILTERSIZE-1))
                    xflushsize = P5MOTION_XYZFILTERSIZE-1;

                yflushsize = fabs(P5device->m_P5Devices[P5Id].m_fy - fYPos[P5MOTION_XYZFILTERSIZE-1])/2.0f;
                yflushsize *= P5MOTION_XYZFILTERSIZE/FLUSH_SETPOINT;
                yflushsize = floor(yflushsize+1.0f);
                if (yflushsize>(P5MOTION_XYZFILTERSIZE-1))
                    yflushsize = P5MOTION_XYZFILTERSIZE-1;

                zflushsize = fabs(P5device->m_P5Devices[P5Id].m_fz - fZPos[P5MOTION_XYZFILTERSIZE-1])/2.0f;
                zflushsize *= P5MOTION_XYZFILTERSIZE/FLUSH_SETPOINT;
                zflushsize = floor(zflushsize+1.0f);
                if (zflushsize>(P5MOTION_XYZFILTERSIZE-1))
                    zflushsize = P5MOTION_XYZFILTERSIZE-1;

                //flush the array based on the number of values determined before.
                for (j=0; j<(int)(xflushsize); j++)
                {
                    for (i=0; i<(P5MOTION_XYZFILTERSIZE-1); i++)
                    {
                        fXPos[i] = fXPos[i+1];
                    }
                    fXPos[P5MOTION_XYZFILTERSIZE-1] = P5device->m_P5Devices[P5Id].m_fx;
                }

                for (j=0; j<(int)(yflushsize); j++)
                {
                    for (i=0; i<(P5MOTION_XYZFILTERSIZE-1); i++)
                    {
                        fYPos[i] = fYPos[i+1];
                    }
                    fYPos[P5MOTION_XYZFILTERSIZE-1] = P5device->m_P5Devices[P5Id].m_fy;
                }

                for (j=0; j<(int)(zflushsize); j++)
                {
                    for (i=0; i<(P5MOTION_XYZFILTERSIZE-1); i++)
                    {
                        fZPos[i] = fZPos[i+1];
                    }
                    fZPos[P5MOTION_XYZFILTERSIZE-1] = P5device->m_P5Devices[P5Id].m_fz;
                }

            }

            //Average all the values in the filter to smoothen the data
            fFilterX = 0.0f;
            fFilterY = 0.0f;
            fFilterZ = 0.0f;
            for (int i=0; i<P5MOTION_XYZFILTERSIZE; i++)
            {
                //we are going to divide the values to get rid of some jitter
                fFilterX += fXPos[i]/2.0f;
                fFilterY += fYPos[i]/2.0f;
                fFilterZ += fZPos[i]/2.0f;
            }
            fFilterX /= P5MOTION_XYZFILTERSIZE;
            fFilterY /= P5MOTION_XYZFILTERSIZE;
            fFilterZ /= P5MOTION_XYZFILTERSIZE;
	}
    }

    /***********************************************
Function: P5Motion_FilterYPR()
Use: Internal Function.  Used to filter Orientation Data
Parameter: None
    ***********************************************/
    float fYaw[P5MOTION_YPRFILTERSIZE], fPitch[P5MOTION_YPRFILTERSIZE], fRoll[P5MOTION_YPRFILTERSIZE];
    float fFilterYaw, fFilterPitch, fFilterRoll;

    void P5GloveModule::P5Motion_FilterYPR()
    {
	static int firsttime = 1;
	if (P5device != NULL)
	{
            if (firsttime==1)
            {
                //dont process anything on the first call, just init our filter arrays
                for (int i=0; i<P5MOTION_YPRFILTERSIZE; i++)
                {
                    fYaw[i] = P5device->m_P5Devices[P5Id].m_fyaw;
                    fPitch[i] = P5device->m_P5Devices[P5Id].m_fpitch;
                    fRoll[i] = P5device->m_P5Devices[P5Id].m_froll;
                }

                firsttime = 0;
            }
            else
            {
                for (int i=0; i<(P5MOTION_YPRFILTERSIZE-1); i++)
                {
                    fYaw[i] = fYaw[i+1];
                    fPitch[i] = fPitch[i+1];
                    fRoll[i] = fRoll[i+1];
                }
                fYaw[P5MOTION_YPRFILTERSIZE-1] = P5device->m_P5Devices[P5Id].m_fyaw;
                fPitch[P5MOTION_YPRFILTERSIZE-1] = P5device->m_P5Devices[P5Id].m_fpitch;
                fRoll[P5MOTION_YPRFILTERSIZE-1] = P5device->m_P5Devices[P5Id].m_froll;

            }

            //Average all the values in the filter to smoothen the data
            fFilterYaw = 0.0f;
            fFilterPitch = 0.0f;
            fFilterRoll = 0.0f;
            for (int i=0; i<P5MOTION_YPRFILTERSIZE; i++)
            {
                fFilterYaw += fYaw[i];
                fFilterPitch += fPitch[i];
                fFilterRoll += fRoll[i];
            }
            fFilterYaw /= P5MOTION_YPRFILTERSIZE;
            fFilterPitch /= P5MOTION_YPRFILTERSIZE;
            fFilterRoll /= P5MOTION_YPRFILTERSIZE;

	}
    }

    /***********************************************
Function: P5Motion_Process()
Use: Processes the XYZ motion information every frame.  Call this function
	 to calculate a pointer's position and oreintation with filtering and acceleration
Parameter: None
    ***********************************************/
    void P5GloveModule::P5Motion_Process()
    {
	//XYZ information first
	static float fLastXpos = 0.0f, fLastYpos = 0.0f, fLastZpos = 0.0f;

	P5Motion_FilterXYZ();
	//apply axis invertion if required and calculate the delta from last frame
	fXMickey = (fLastXpos - fFilterX) * nxinvert;
	fYMickey = (fLastYpos - fFilterY) * nyinvert;
	fZMickey = (fLastZpos - fFilterZ) * nzinvert;

#define COEFF1	0.0042f
#define COEFF2	1.2403f

	//this is a third degree polynomial acceleration function.. seems to give the best results
	//by trial and error
	fXMickey = COEFF1*fXMickey*fXMickey*fXMickey + COEFF2*fXMickey;
	fYMickey = COEFF1*fYMickey*fYMickey*fYMickey + COEFF2*fYMickey;
	fZMickey = COEFF1*fZMickey*fZMickey*fZMickey + COEFF2*fZMickey;

	//A step function gain to speed up the high end motion for faster travel across the screen
	if ( fabs(fXMickey) > 3.0f)
            fXMickey *= 4.0f;
	else
            fXMickey *= 2.0f;

        if ( fabs(fYMickey) > 3.0f)
            fYMickey *= 4.0f;
	else
            fYMickey *= 2.0f;

	if ( fabs(fZMickey) > 3.0f)
            fZMickey *= 4.0f;
	else
            fZMickey *= 2.0f;

        nXPos += (int)(fXMickey);
	nYPos += (int)(fYMickey);
	nZPos += (int)(fZMickey);

	//Clip the data to fit the clipping region
	if (nXPos > nxclipend)
            nXPos = nxclipend;
	else if (nXPos < nxclipstart)
            nXPos = nxclipstart;

        if (nYPos > nyclipend)
            nYPos = nyclipend;
	else if (nYPos < nyclipstart)
            nYPos = nyclipstart;

	if (nZPos > nzclipend)
            nZPos = nzclipend;
	else if (nZPos < nzclipstart)
            nZPos = nzclipstart;

	fLastXpos = fFilterX;
	fLastYpos = fFilterY;
	fLastZpos = fFilterZ;

	//Lets process Yaw, Pitch, Roll information now
	P5Motion_FilterYPR();

	//Only prcess YPR information if a fist is made.. like grabbing the object
	//This is just an example gesture, and can be changed to be anything at all

	static float fZeroYawPos, fZeroPitchPos, fZeroRollPos;

        fZeroYawPos = fFilterYaw;
	fZeroPitchPos = fFilterPitch;
	fZeroRollPos = fFilterRoll;

	fAbsYawPos = fFilterYaw;
	fAbsPitchPos = fFilterPitch;
	fAbsRollPos = fFilterRoll;

	//In relative mode, YPR acts as a rate controller, providing continuous motion when a certain
	//limit is hit

#define YPR_ROTSPEED	0.5f

	if ( fFilterRoll > (fZeroRollPos+30.0f) )
            fRelRollPos += YPR_ROTSPEED;
	else if ( fFilterRoll<(fZeroRollPos-30.0f) )
            fRelRollPos -= YPR_ROTSPEED;
	if (fRelRollPos > 180.0f)
            fRelRollPos = -180.0f;
	else if (fRelRollPos < -180.0f)
            fRelRollPos = 180.0f;

	if (fFilterYaw > (fZeroYawPos+25.0f))
            fRelYawPos += YPR_ROTSPEED;
	else if (fFilterYaw < (fZeroYawPos-25.0f))
            fRelYawPos -= YPR_ROTSPEED;
	if (fRelYawPos > 180.0f)
            fRelYawPos = -180.0f;
	else if (fRelYawPos < -180.0f)
            fRelYawPos = 180.0f;

        if (fFilterPitch > (fZeroPitchPos+25.0f))
            fRelPitchPos += YPR_ROTSPEED;
	else if (fFilterPitch < (fZeroPitchPos-25.0f))
            fRelPitchPos -= YPR_ROTSPEED;
	if (fRelPitchPos > 180.0f)
            fRelPitchPos = -180.0f;
	else if (fRelPitchPos < -180.0f)
            fRelPitchPos = 180.0f;
    }

} // namespace ot

#else
#ifdef WIN32
#pragma message(">>> no P5 glove support")
#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of P5GloveModule.cxx
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
