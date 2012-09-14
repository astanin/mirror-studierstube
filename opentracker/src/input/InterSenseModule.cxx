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
/** source file for InterSenseModule module.
 *
 * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr, Jan Prikryl
 *
 * $Id: InterSenseModule.cxx 2124 2008-02-15 16:30:09Z veas $
 *
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/tool/StringAligner.h>

#include <ace/Log_Msg.h>

#include <OpenTracker/input/InterSenseSource.h>
#include <OpenTracker/input/InterSenseModule.h>

#include <cstdio>
#include <iostream>
#include <isense.h>


#ifndef OT_NO_INTERSENSE_SUPPORT
#ifndef WIN32
#   warning "Compiling Intersense Support"
#endif  //win32

namespace ot {

	OT_MODULE_REGISTER_FUNC(InterSenseModule){
		OT_MODULE_REGISTRATION_DEFAULT(InterSenseModule, "InterSenseConfig");
	}

    struct ISTracker {
        ISD_TRACKER_HANDLE handle;
        ISD_TRACKER_INFO_TYPE info;
        ISD_TRACKER_DATA_TYPE data;
        std::string id;
        int comport;
        NodeVector sources;
    };

    // Destructor method
    InterSenseModule::~InterSenseModule()
    {
    }

    // initializes trackers
    void InterSenseModule::init(StringTable& attributes, ConfigNode * localTree)
    {
        Module::init( attributes, localTree );
        bool verbose = FALSE;
        resetheading = TRUE;
        headtracking = FALSE;

        StringTable & localAttrib= localTree->getAttributes();
        if( localAttrib.get("verbose").compare("true") == 0 )
            verbose=TRUE;
        if( localAttrib.get("resetheading").compare("false") == 0 )
            resetheading=FALSE;
        if( localAttrib.get("headtracking").compare("true") == 0 )
            headtracking=TRUE;

        for( unsigned i = 0; i < localTree->countChildren(); i++ )
        {
            ConfigNode * trConfig = (ConfigNode *)localTree->getChild(i);
            StringTable & trackerAttrib = trConfig->getAttributes();
            const std::string & id = trackerAttrib.get("id");
            int comport = 0;
            int num = sscanf(trackerAttrib.get("comport").c_str(), " %i", &comport );
            if( num == 0 )
            {
                logPrintE("Error in converting serial port number !\n");
                comport = 0;
            }
            ISTrackerVector::iterator it;
            for(it = trackers.begin(); it != trackers.end(); it ++ )
            {
                if( (*it)->id.compare( id ) == 0 && (*it)->comport == comport )
                    break;
            }
            if(it == trackers.end())  // we got a truly new tracker
            {
                ISD_TRACKER_HANDLE handle = ISD_OpenTracker( 0, comport, FALSE, verbose );
                if( handle <= 0 )
                {                
                    logPrintE("Failed to open tracker %s\n", id.c_str());
                } 
                else {
                    ISTracker * tracker = new ISTracker;
                    tracker->id = id;
                    tracker->comport = comport;
                    tracker->handle = handle;
                    Bool res;
                    res = ISD_GetTrackerConfig( tracker->handle, &tracker->info , FALSE);
                    if( res == FALSE )
                    {                    
                        logPrintE("Failed to get tracker config for %s\n", id.c_str());
                    }
                    /* InterTrax does not support quaternions */
                    if (tracker->info.TrackerType != ISD_INTERTRAX_SERIES ) 
                    {
                        /* Set up each station for Precision class of trackers.
                           Ensure the orientation measure of the tracker stations is
                           quaternions.  Ensure that button values are returned. */
                        ISD_STATION_INFO_TYPE station;
                        for( int j = 1; j <= ISD_MAX_STATIONS; j++ )
                        {   
                            if( ISD_GetStationConfig( tracker->handle, &station, j, FALSE ) )
                            {
                                if( station.State == TRUE )
                                {
                                    station.AngleFormat = ISD_QUATERNION;
                                    // Do Not Delete this commented code.
                                    // Necessary to test Euler (and Gimbal Lock).
                                    //
                                    //station.AngleFormat = ISD_EULER;
                                    station.GetInputs = TRUE;
                                    if( ISD_SetStationConfig( tracker->handle, 
                                                               &station, j, FALSE ) )
                                      logPrintI("InterSenseModule correctly set event for tracker %s station %d \n", id.c_str(), j);

                                }
                            }               
                        }
                    }   // setup not intertrax
                    trackers.push_back( tracker );                
                    logPrintI("Configured tracker %s of %d\n", id.c_str(), tracker->info.TrackerType);
                }       // open tracker ok
            }           // got a new tracker
            else {      // some conflict with another tracker            
                logPrintE("Intersense Tracker %s at port %d conflicts with with %s\n", id.c_str(), comport, (*it)->id.c_str());
            }
        }               // all ConfigNodes
    }

    // This method is called to construct a new Node.
    Node * InterSenseModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("InterSenseSource") == 0 )
        {       
            const std::string & id = attributes.get( "id" );
            ISTrackerVector::iterator it;
            for(it = trackers.begin(); it != trackers.end(); it ++ )
            {
                if( (*it)->id.compare( id ) == 0 )
                    break;
            }
            if( it != trackers.end())
            {
                int station;
                int num = sscanf(attributes.get("station").c_str(), " %i", &station );
                if( num == 0 ){                
                    logPrintE("Error in converting station number !\n");
                    return NULL;
                }
                if( station < 0 || station >= ISD_MAX_STATIONS )
                {
                    logPrintE("Station number out of range [0,%d] !\n", ISD_MAX_STATIONS-1);
                    return NULL;
                }
                InterSenseSource * source = new InterSenseSource( station );
                (*it)->sources.push_back( source );            
                logPrintI("Built InterSenseSource node\n");
                return source;
            }
            else {            
                logPrintW("No tracker %s configured !\n", id.c_str());
            }
        }
        return NULL;
    }

    void InterSenseModule::start()
    {
        if (resetheading)
        {
            logPrintW("Resetting heading of Intersense trackers\n");
            ISTrackerVector::iterator it;
            for(it = trackers.begin(); it != trackers.end(); it ++ )
            {
                if( (*it)->info.TrackerType == ISD_INTERTRAX_SERIES || 
                    (*it)->info.TrackerModel == ISD_IS300 ||
                    (*it)->info.TrackerModel == ISD_ICUBE2 ||
                    (*it)->info.TrackerModel == ISD_ICUBE2_PRO ||
                    (*it)->info.TrackerModel == ISD_ICUBE3)
                {
                    for( int j = 1; j <= ISD_MAX_STATIONS; j++ )
                        ISD_ResetHeading((*it)->handle, j);
                }
            }
        }
    }

    // closes InterSense library
    void InterSenseModule::close()
    {
        for(ISTrackerVector::iterator it = trackers.begin(); it != trackers.end(); it ++ )
        {
            ISD_CloseTracker( (*it)->handle );
        }    
    }

    // pushes events into the tracker tree.
    void InterSenseModule::pushEvent()
    {
        for(ISTrackerVector::iterator it = trackers.begin(); it != trackers.end(); it ++ )
        {
            if((*it)->sources.size() > 0 )
            {
                ISTracker * tracker = *it;
                ISD_GetData( tracker->handle, &tracker->data );
                for( NodeVector::iterator si = tracker->sources.begin(); si != tracker->sources.end(); si++ )
                {
                    InterSenseSource * source = ( InterSenseSource * ) ((Node*)*si);
                    ISD_STATION_STATE_TYPE * data = &tracker->data.Station[source->station];
                    if( tracker->info.TrackerType == ISD_INTERTRAX_SERIES )
                    {
                        float quat[4];
                        MathUtils::eulerToQuaternion(- data->Orientation[0] * MathUtils::GradToRad,
                                                     data->Orientation[1] * MathUtils::GradToRad,
                                                     - data->Orientation[2] * MathUtils::GradToRad,
                                                     MathUtils::YXZ,
                                                     quat);
                        if( quat[0] != source->event.getOrientation()[0] || 
                            quat[1] != source->event.getOrientation()[1] ||
                            quat[2] != source->event.getOrientation()[2] ||
                            quat[3] != source->event.getOrientation()[3] )
                        {
                            source->event.getOrientation()[0] = quat[0];
                            source->event.getOrientation()[1] = quat[1];
                            source->event.getOrientation()[2] = quat[2];
                            source->event.getOrientation()[3] = quat[3];
                            source->event.timeStamp();
                            source->updateObservers( source->event );
                        }
                    }
                    else {
                        int changed = 0;

                        if (headtracking)
                        {
                            // If headtracking is activated, we send the "Real" component of the
                            // quaternion to the end. Plus we align the axes of the InertiaCubes
                            // so that the cable points in the positive Z direction. And its base
                            // is the ZX plane. Also, we switch from Left handed to Right handed
                            // coordinate system.
                            if( data->Orientation[2] != source->event.getOrientation()[0] || 
                                -data->Orientation[3] != source->event.getOrientation()[1] ||
                                -data->Orientation[1] != source->event.getOrientation()[2] ||
                                data->Orientation[0] != source->event.getOrientation()[3] )
                            {
                                changed = 1;
                                source->event.getOrientation()[0] = data->Orientation[2];
                                source->event.getOrientation()[1] = -data->Orientation[3];
                                source->event.getOrientation()[2] = -data->Orientation[1];
                                source->event.getOrientation()[3] = data->Orientation[0];
                            }
                        }
                        else {
                            // The reason to switch the Quaterion[0] to the end is
                            // that the Intersense tracker provides the "Real" component of the
                            // quaternion as the first value, however, inventor assumes is the last value.
                            // If you find problems on your opentracker configuration files, is maybe
                            // because you found a hack to get around this problem. You should now
                            // be able to get rid of that hack. Mendez. 20061115.
                            if( data->Orientation[1] != source->event.getOrientation()[0] || 
                                data->Orientation[2] != source->event.getOrientation()[1] ||
                                data->Orientation[3] != source->event.getOrientation()[2] ||
                                data->Orientation[0] != source->event.getOrientation()[3] )
                            {
                                changed = 1;
                                source->event.getOrientation()[0] = data->Orientation[1];
                                source->event.getOrientation()[1] = data->Orientation[2];
                                source->event.getOrientation()[2] = data->Orientation[3];
                                source->event.getOrientation()[3] = data->Orientation[0];
                            }
                        }

                        if( data->Position[0] != source->event.getPosition()[0] || 
                            data->Position[1] != source->event.getPosition()[1] ||
                            data->Position[2] != source->event.getPosition()[2] )
                        {
                            changed = 1;
                            source->event.getPosition()[0] = data->Position[0];
                            source->event.getPosition()[1] = data->Position[1];
                            source->event.getPosition()[2] = data->Position[2];
                        }
                        unsigned short button = 0;
                        for( int i = 0; i < ISD_MAX_BUTTONS; i++ )
                        {
                            button |= data->ButtonState[i];
                        }
                        if( button != source->event.getButton() )
                        {
                            changed = 1;
                            source->event.getButton() = button;
                        }
                        if( changed == 1 )
                        {
                            source->event.timeStamp();
                            source->updateObservers( source->event );
                        }
                    }       
                } // for all sinks
            
            } 
        } // for all trackers       
    }

} // namespace ot


#endif // OT_NO_INTERSENSE_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of InterSenseModule.cxx
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
