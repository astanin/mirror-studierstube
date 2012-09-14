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
/** source file for ARTDataTrackerModule module.
 *
 * @author Christopher Schmidt
 *
 * $Id: ARTDataTrackerModule.cxx 2120 2008-02-07 15:19:59Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>
#include <string>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Time_Value.h>
#include <ace/Log_Msg.h>

#include <OpenTracker/input/ARTDataTrackerModule.h>
#include <OpenTracker/input/ARTDataTrackerSource.h>
#include <OpenTracker/input/ARTDataTrackerChomp.h>

#include <cmath>
#include <cstdio>
#include <iostream>


#ifndef OT_NO_ARTDATATRACKER_SUPPORT


namespace ot {
    
    OT_MODULE_REGISTER_FUNC(ARTDataTrackerModule){
        OT_MODULE_REGISTRATION_DEFAULT(ARTDataTrackerModule, "ARTDataTrackerConfig" );
    }
    

    static const float DEG_TO_RAD = (float)(3.14159/180.0);

    // -------------------------------------------------------------------------------------------------------
    // constructor initializing the thread manager
    ARTDataTrackerModule::ARTDataTrackerModule() : ThreadModule(), NodeFactory(), stop(0), DataTracker(NULL)
    {

    }
    // -------------------------------------------------------------------------------------------------------

    // Destructor method

    ARTDataTrackerModule::~ARTDataTrackerModule()
    {
	if (DataTracker)
	    delete (DataTracker);
	sources.clear();
    }


    // This method is called to construct a new Node.

    Node * ARTDataTrackerModule::createNode( const std::string& name, const StringTable& attributes)
    {
	if( name.compare("ARTDataTrackerSource") == 0 )
        {
            stop = 0;
            int number;
            int num = sscanf(attributes.get("number").c_str(), " %i", &number );

            if (attributes.get("type") == "3d" )
            {
                number += 20;
            }
            else if (attributes.get("type") == "fly" )
            {
                number += 40;
            }
            else if (attributes.get("type") == "pen" )
            {
                number += 60;
            }

            if( num == 0 )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting ARTDataTrackerSource number !\n")));
                return NULL;
            }

            NodeVector::iterator it;
            for( it = sources.begin(); it != sources.end(); it++ ){
                ARTDataTrackerSource * source = (ARTDataTrackerSource*)((Node*) *it);
                if( source->number == number )
                {
                    break;
                }
            }
            if( it != sources.end())
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Source with number %d already exists\n"), number));
                return NULL;
            }
            ARTDataTrackerSource * source = new ARTDataTrackerSource( number);
            sources.push_back( source );
            logPrintI("Built ARTDataTrackerSource node. Number: %d\n", number);
            return source;
        }
	return NULL;
    }

    // -------------------------------------------------------------------------------------------------------

    void ARTDataTrackerModule::start()
    {
	if( isInitialized() && !sources.empty())
	    ThreadModule::start();
    }


    // -------------------------------------------------------------------------------------------------------

    void ARTDataTrackerModule::close()
    {
        logPrintD("ARTDataTrackerModule::close() ... \n");
	lockLoop();
	stop = 1;
	unlockLoop();
        logPrintD("ARTDataTrackerModule::close() done. \n");
    }

    // -------------------------------------------------------------------------------------------------------

    void ARTDataTrackerModule::run()
    {
	ACE_Time_Value timeOut( 1, 0 );

	int retval;

	ACE_INET_Addr addr( port );
	ACE_Addr local( -1 , -1);
	socket = new ACE_SOCK_Dgram( addr );

	receiveBufferSize =  3 * sizeof(long) + 20 * sizeof(ARTDataTrackerChomp::BodyRecord) +
	    20 * sizeof(ARTDataTrackerChomp::MarkerRecord) +
            20 * sizeof(ARTDataTrackerChomp::FlystickRecord) +
            20 * sizeof(ARTDataTrackerChomp::MeasuretargetRecord);

	receiveBuffer = new char[receiveBufferSize];

	std::string receiveString;
	// mainloop for reading data from the port

	while (1)
        {
            do
            {
                if( (retval = socket->recv( receiveBuffer, receiveBufferSize , addr, 0, &timeOut )) == -1 )
                {
                    if(errno != ETIME && errno != 0)
                    {
                        ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:ERROR %d receiving data\n"), errno));
                        exit( -1 );
                    }
                }
            } while( retval < 0 && stop == 0);

            lockLoop();
            if( stop)
            {
                unlockLoop();
                break;
            }
            unlockLoop();

            // from here the String is in the Buffer!
            // converts c-String into String
            receiveString = std::string(receiveBuffer, retval);
            // call .chomp Method from DataTrackerInstance to bring the received  String into a Record
            //endwin();

            DataTracker->chomp(receiveString);
            //DataTracker->displayRecords();

            // brings the Record from the ARTDataTrackerChomp class to the BodyRecordTemp Record
            std::map<int, ARTDataTrackerChomp::BodyRecord> & BodyRecordTemp = DataTracker->getBodyRecord();
            std::map<int, ARTDataTrackerChomp::MarkerRecord> & MarkerRecordTemp = DataTracker->getMarkerRecord();
            std::map<int, ARTDataTrackerChomp::FlystickRecord> & FlystickRecordTemp = DataTracker->getFlystickRecord();
            std::map<int, ARTDataTrackerChomp::MeasuretargetRecord> & MeasuretargetRecordTemp = DataTracker->getMeasuretargetRecord();



            NodeVector::iterator it;

            for( it = sources.begin(); it != sources.end(); it++)
            {
                ARTDataTrackerSource * source = 
                    (ARTDataTrackerSource*)((Node*) *it);
                source->lock();

                bodyID = source->number;
                if (bodyID < 20)
                {
                    if( BodyRecordTemp[bodyID].valid == true )
                    {
                        // convert Data such as quaternion and euler-Angles
                        convert( BodyRecordTemp[bodyID] );
                        // Brings the locationdata from BodyRecordTemp to  source->event.position !
                        source->event.getPosition()[0] = BodyRecordTemp[bodyID].location[0];
                        source->event.getPosition()[1] = BodyRecordTemp[bodyID].location[1];
                        source->event.getPosition()[2] = BodyRecordTemp[bodyID].location[2];
                        // Brings the calculated Quaternion Data from BodyRecordTemp to source->event.orientation !
                        source->event.getOrientation()[0] = BodyRecordTemp[bodyID].orientation[0];
                        source->event.getOrientation()[1] = BodyRecordTemp[bodyID].orientation[1];
                        source->event.getOrientation()[2] = BodyRecordTemp[bodyID].orientation[2];
                        source->event.getOrientation()[3] = BodyRecordTemp[bodyID].orientation[3];
                        // Bring a timeStamp to source->event
                        source->event.timeStamp();
                        // Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
                        // fixed to 0.5 ??
                        source->event.getConfidence() = 0.5;
                        // Source was definitly changed !
                        source->changed = 1;
                    }
                    else
                    {
                        // only if marker was found in the last grab (event.getConfidence() > epsilon) set
                        // confidence to 0.0!
                        if (source->event.getConfidence() > 0.000001f)
                        {
                            source->changed = 1;
                            source->event.getConfidence() = 0.0f;
                        }
                    }
                }
                else if (bodyID < 40)
                {
                    if( MarkerRecordTemp[bodyID-20].valid == true )
                    {
                        // convert Data such as quaternion and euler-Angles
                        convert( BodyRecordTemp[bodyID-20] );
                        // Brings the locationdata from BodyRecordTemp to  source->event.position !
                        source->event.getPosition()[0] = MarkerRecordTemp[bodyID-20].location[0];
                        source->event.getPosition()[1] = MarkerRecordTemp[bodyID-20].location[1];
                        source->event.getPosition()[2] = MarkerRecordTemp[bodyID-20].location[2];
                        // Bring a timeStamp to source->event
                        source->event.timeStamp();
                        // Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
                        // fixed to 0.5 ??
                        source->event.getConfidence() = 0.5;
                        // Source was definitly changed !
                        source->changed = 1;
                    }
                    else
                    {
                        // only if marker was found in the last grab (event.getConfidence() > epsilon) set
                        // confidence to 0.0!
                        if (source->event.getConfidence() > 0.000001f)
                        {
                            source->changed = 1;
                            source->event.getConfidence() = 0.0f;
                        }
                    }
                }
                else if (bodyID < 60)
                {
                    if( FlystickRecordTemp[bodyID-40].valid == true )
                    {
                        // convert Data such as quaternion and euler-Angles
                        convert( FlystickRecordTemp[bodyID-40] );
                        // Brings the locationdata from BodyRecordTemp to  source->event.position !
                        source->event.getPosition()[0] = FlystickRecordTemp[bodyID-40].location[0];
                        source->event.getPosition()[1] = FlystickRecordTemp[bodyID-40].location[1];
                        source->event.getPosition()[2] = FlystickRecordTemp[bodyID-40].location[2];
                        // Brings the calculated Quaternion Data from BodyRecordTemp to source->event.orientation !
                        source->event.getOrientation()[0] = FlystickRecordTemp[bodyID-40].orientation[0];
                        source->event.getOrientation()[1] = FlystickRecordTemp[bodyID-40].orientation[1];
                        source->event.getOrientation()[2] = FlystickRecordTemp[bodyID-40].orientation[2];
                        source->event.getOrientation()[3] = FlystickRecordTemp[bodyID-40].orientation[3];
                        // buttons ...
                        source->event.getButton() = (unsigned short)FlystickRecordTemp[bodyID-40].buttons;
                        // Bring a timeStamp to source->event
                        source->event.timeStamp();
                        // Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
                        // fixed to 0.5 ??
                        source->event.getConfidence() = 0.5;
                        // Source was definitly changed !
                        source->changed = 1;
                    }
                    else
                    {
                        // only if marker was found in the last grab (event.getConfidence() > epsilon) set
                        // confidence to 0.0!
                        if (source->event.getConfidence() > 0.000001f)
                        {
                            source->changed = 1;
                            source->event.getConfidence() = 0.0f;
                        }
                    }
                }
                else if (bodyID < 80)
                {
                    if( MeasuretargetRecordTemp[bodyID-60].valid == true )
                    {
                        // convert Data such as quaternion and euler-Angles
                        convert( MeasuretargetRecordTemp[bodyID-60] );
                        // Brings the locationdata from BodyRecordTemp to  source->event.position !
                        source->event.getPosition()[0] = MeasuretargetRecordTemp[bodyID-60].location[0];
                        source->event.getPosition()[1] = MeasuretargetRecordTemp[bodyID-60].location[1];
                        source->event.getPosition()[2] = MeasuretargetRecordTemp[bodyID-60].location[2];
                        // Brings the calculated Quaternion Data from BodyRecordTemp to source->event.orientation !
                        source->event.getOrientation()[0] = MeasuretargetRecordTemp[bodyID-60].orientation[0];
                        source->event.getOrientation()[1] = MeasuretargetRecordTemp[bodyID-60].orientation[1];
                        source->event.getOrientation()[2] = MeasuretargetRecordTemp[bodyID-60].orientation[2];
                        source->event.getOrientation()[3] = MeasuretargetRecordTemp[bodyID-60].orientation[3];
                        // buttons ...
                        source->event.getButton() = (unsigned short)MeasuretargetRecordTemp[bodyID-60].buttons;
                        // Bring a timeStamp to source->event
                        source->event.timeStamp();
                        // Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
                        // fixed to 0.5 ??
                        source->event.getConfidence() = 0.5;
                        // Source was definitly changed !
                        source->changed = 1;
                    }
                    else
                    {
                        // only if marker was found in the last grab (event.getConfidence() > epsilon) set
                        // confidence to 0.0!
                        if (source->event.getConfidence() > 0.000001f)
                        {
                            source->changed = 1;
                            source->event.getConfidence() = 0.0f;
                        }
                    }
                }
				bool chgflag = source->changed;
				source->unlock();
                if ( chgflag && context != NULL)
                {
                    // notify main loop
		    if (context->doSynchronization())
		    {
                        context->dataSignal();
                        context->consumedWait();
		    }
                }
                
            } // for ...

        } // while(1)

	socket->close();
    }


    // -------------------------------------------------------------------------------------------------------

    void ARTDataTrackerModule::convert( ARTDataTrackerChomp::BodyRecord & BodyRecordTemp )
    {
	//	static const float DEG_TO_RAD = (3.14159/180.0);
	float m[3][3];
	//	set the matrix m with the values from our BodyRecordTemp.rotationMatrix
	//	as discribed in the DTrack Technical Appendix B the matrix is formed like this:
	//	in the ASCII String the rotation Data is like this: [b0 b1 b2 b3 b4 b5 b6 b7 b8]
	//	this formes the Rotation Matrix R:
	//		|b0 b3 b6|
	//	R = |b1 b4 b7|
	//		|b2 b5 b8|
	m[0][0] = BodyRecordTemp.rotationMatrix[0];
	m[0][1] = BodyRecordTemp.rotationMatrix[3];
	m[0][2] = BodyRecordTemp.rotationMatrix[6];
	m[1][0] = BodyRecordTemp.rotationMatrix[1];
	m[1][1] = BodyRecordTemp.rotationMatrix[4];
	m[1][2] = BodyRecordTemp.rotationMatrix[7];
	m[2][0] = BodyRecordTemp.rotationMatrix[2];
	m[2][1] = BodyRecordTemp.rotationMatrix[5];
	m[2][2] = BodyRecordTemp.rotationMatrix[8];

	// convert to Quaternion Format result is given back in .orientation Array
	MathUtils::matrixToQuaternion( m, BodyRecordTemp.orientation);

    }

    void ARTDataTrackerModule::convert( ARTDataTrackerChomp::FlystickRecord & FlystickRecordTemp )
    {
	//	static const float DEG_TO_RAD = (3.14159/180.0);
	float m[3][3];
	//	set the matrix m with the values from our BodyRecordTemp.rotationMatrix
	//	as discribed in the DTrack Technical Appendix B the matrix is formed like this:
	//	in the ASCII String the rotation Data is like this: [b0 b1 b2 b3 b4 b5 b6 b7 b8]
	//	this formes the Rotation Matrix R:
	//		|b0 b3 b6|
	//	R = |b1 b4 b7|
	//		|b2 b5 b8|
	m[0][0] = FlystickRecordTemp.rotationMatrix[0];
	m[0][1] = FlystickRecordTemp.rotationMatrix[3];
	m[0][2] = FlystickRecordTemp.rotationMatrix[6];
	m[1][0] = FlystickRecordTemp.rotationMatrix[1];
	m[1][1] = FlystickRecordTemp.rotationMatrix[4];
	m[1][2] = FlystickRecordTemp.rotationMatrix[7];
	m[2][0] = FlystickRecordTemp.rotationMatrix[2];
	m[2][1] = FlystickRecordTemp.rotationMatrix[5];
	m[2][2] = FlystickRecordTemp.rotationMatrix[8];

	// convert to Quaternion Format result is given back in .orientation Array
	MathUtils::matrixToQuaternion( m, FlystickRecordTemp.orientation);

    }
    void ARTDataTrackerModule::convert( ARTDataTrackerChomp::MeasuretargetRecord & MeasuretargetRecordTemp )
    {
	//	static const float DEG_TO_RAD = (3.14159/180.0);
	float m[3][3];
	//	set the matrix m with the values from our BodyRecordTemp.rotationMatrix
	//	as discribed in the DTrack Technical Appendix B the matrix is formed like this:
	//	in the ASCII String the rotation Data is like this: [b0 b1 b2 b3 b4 b5 b6 b7 b8]
	//	this formes the Rotation Matrix R:
	//		|b0 b3 b6|
	//	R = |b1 b4 b7|
	//		|b2 b5 b8|
	m[0][0] = MeasuretargetRecordTemp.rotationMatrix[0];
	m[0][1] = MeasuretargetRecordTemp.rotationMatrix[3];
	m[0][2] = MeasuretargetRecordTemp.rotationMatrix[6];
	m[1][0] = MeasuretargetRecordTemp.rotationMatrix[1];
	m[1][1] = MeasuretargetRecordTemp.rotationMatrix[4];
	m[1][2] = MeasuretargetRecordTemp.rotationMatrix[7];
	m[2][0] = MeasuretargetRecordTemp.rotationMatrix[2];
	m[2][1] = MeasuretargetRecordTemp.rotationMatrix[5];
	m[2][2] = MeasuretargetRecordTemp.rotationMatrix[8];

	// convert to Quaternion Format result is given back in .orientation Array
	MathUtils::matrixToQuaternion( m, MeasuretargetRecordTemp.orientation);

    }



    // -------------------------------------------------------------------------------------------------------

    // pushes events into the tracker tree
    void ARTDataTrackerModule::pushEvent()
    {
        //nothing to do here
    }

    // -------------------------------------------------------------------------------------------------------

    // initializes the ARTDataTracker module

    void ARTDataTrackerModule::init(StringTable& attributes, ConfigNode * localTree)
    {
	ThreadModule::init( attributes, localTree );
	int num;

	// Scannig port number from XML-File
	num = sscanf(attributes.get("port").c_str(), " %i", &port );
	if( num == 0 )
        {
            port = 12345;
        }

	DataTracker = new ARTDataTrackerChomp();
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_ARTDATATRACKER_SUPPORT")
#endif // OT_NO_ARTDATATRACKER_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of ARTDataTrackerModule.cxx
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
