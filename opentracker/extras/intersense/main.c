/**********************************************************************************
*
*    File Name:      main.c
*    Description:    Illustrates use of the InterSense driver API
*    Created:        12/7/98
*    Author:         Yury Altshuler
*    Contact:        yurya@isense.com, 781-270-0090 ext. 124
*
*    Copyright:      InterSense 2003 - All rights Reserved.
*
*    Comments:       This program illustrates the use of the functions
*                    defined in isense.h. 
*                    
**********************************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <memory.h>
#include <string.h>

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
#include <conio.h>
#endif

#include "isense.h"

#define ESC 0X1B

/*****************************************************************************/
const char *systemType( int Type ) 
{
    switch( Type ) 
    {
    case ISD_NONE:
        return "Unknown";
    case ISD_PRECISION_SERIES:
        return "IS Precision Series";
    case ISD_INTERTRAX_SERIES:
        return "InterTrax Series";
    }
    return "Unknown";
}


/*****************************************************************************/
const char *systemName( int Model ) 
{
    switch( Model ) 
    {
    case ISD_IS300:
        return "IS-300 Series";
    case ISD_IS600:
        return "IS-600 Series";
    case ISD_IS900:
        return "IS-900 Series";
    case ISD_INTERTRAX:
        return "InterTrax 30";
    case ISD_INTERTRAX_2:
        return "InterTrax2";
    case ISD_INTERTRAX_LS:
        return "InterTraxLS";
    case ISD_INTERTRAX_LC:
        return "InterTraxLC";
    case ISD_ICUBE2:
        return "InertiaCube2";
    case ISD_ICUBE2_PRO:
        return "InertiaCube2 Pro";
    case ISD_ICUBE3:
        return "InertiaCube3";
    case ISD_IS1200:
        return "IS-1200 Series";
    }
    return "Unknown";
}


/*****************************************************************************
*
*   functionName:   showTrackerStats
*   Description:    
*   Created:        12/7/98
*   Author:         Yury Altshuler
*
*   Comments:       Get and display tracker information
*
******************************************************************************/
void showTrackerStats( ISD_TRACKER_HANDLE handle )
{
    ISD_TRACKER_INFO_TYPE Tracker;
    ISD_STATION_INFO_TYPE Station;
    WORD i, numStations = 4;
    char buf[20];

    if(ISD_GetTrackerConfig( handle, &Tracker, TRUE ))
    {
        printf("\n\n********** InterSense Tracker Information ***********\n\n");

        printf("Type:     %s device on port %d\n", 
            systemType(Tracker.TrackerType), Tracker.Port);

        printf("Model:    %s\n", 
            systemName(Tracker.TrackerModel));

        switch( Tracker.TrackerModel ) 
        {
        case ISD_IS300:
        case ISD_IS1200:
            numStations = 4;
            break;
        case ISD_IS600:
        case ISD_IS900:
            numStations = ISD_MAX_STATIONS;
            break;
        default:
            numStations = 1;
            break;
        }
        
        printf("\nStation\tTime\tState\tCube  Enhancement  Sensitivity  Compass  Prediction\n");
        
        for(i = 1; i <= numStations; i++)
        {
            printf("%d\t", i);
            
            if(ISD_GetStationConfig( handle, &Station, i, FALSE ))
            {
                sprintf(buf, "%d", Station.InertiaCube);
                
                printf("%s\t%s\t%s\t   %u\t\t%u\t  %u\t  %u\n", 
                    Station.TimeStamped ? "ON" : "OFF", 
                    Station.State ? "ON" : "OFF", 
                    Station.InertiaCube == -1 ? "None" : buf, 
                    Station.Enhancement, 
                    Station.Sensitivity, 
                    Station.Compass, 
                    Station.Prediction);
            }
            else
            {
                printf("ISD_GetStationConfig failed\n");
                break;
            }
        }
        printf("\n");
    }
    else
    {
        printf("ISD_GetTrackerConfig failed\n");
    }
}


/*****************************************************************************
*
*   functionName:   showTrackerData
*   Description:    
*   Created:        12/7/98
*   Author:         Yury Altshuler
*
*   Comments:       Get and display tracker information
*
******************************************************************************/
void showStationData( ISD_TRACKER_HANDLE             handle, 
                      ISD_TRACKER_INFO_TYPE         *Tracker,
                      ISD_STATION_INFO_TYPE         *Station,
                      ISD_STATION_STATE_TYPE        *data, 
                      ISD_CAMERA_ENCODER_DATA_TYPE  *cameraData )
{
    // Get comm port statistics for display with tracker data 
    if(ISD_GetCommInfo( handle, Tracker ))
    {
        printf("%3.0fKbps %d R/s ", Tracker->KBitsPerSec, Tracker->RecordsPerSec);

        // display position only if system supports it 
        if( Tracker->TrackerModel == ISD_IS600 || 
            Tracker->TrackerModel == ISD_IS900 ||
            Tracker->TrackerModel == ISD_IS1200 )
        {
            printf("%6.2f %6.2f %6.2f ",
                data->Position[0], data->Position[1], data->Position[2]);
        }

        // all products can return orientation 
        if( Station->AngleFormat == ISD_QUATERNION )
        {
            printf("%5.2f %5.2f %5.2f %5.2f ",
                data->Orientation[0], data->Orientation[1], data->Orientation[2], data->Orientation[3]);
        }
        else // Euler angles
        {
            printf("%7.2f %7.2f %7.2f ",
                data->Orientation[0], data->Orientation[1], data->Orientation[2]);
        }

        if( Station->GetAuxInputs ) 
        {
            printf("%d %d %d %d ", 
                (int) data->AuxInputs[0], 
                (int) data->AuxInputs[1], 
                (int) data->AuxInputs[2], 
                (int) data->AuxInputs[3]);
        }
        
        // if system is configured to read stylus or wand buttons 
        if( Station->GetInputs ) 
        {
            // Currently available products have at most 5 buttons,
            // stylus has 2, wand has 5
            printf("%d%d%d%d%d ", 
                (int) data->ButtonState[0], 
                (int) data->ButtonState[1], 
                (int) data->ButtonState[2], 
                (int) data->ButtonState[3], 
                (int) data->ButtonState[4]);

            printf("%d %d ", data->AnalogData[0], data->AnalogData[1]); 
        }

        // if system is configured to read encoder data 
        if( Station->GetCameraData ) 
        {
            printf("\n\tTime: %.3f  Camera data: %-ld %-ld %-ld \n", 
                data->TimeStamp,
                cameraData->ApertureEncoder, 
                cameraData->FocusEncoder,
                cameraData->ZoomEncoder);
        }
        else
        {
            printf("\r");
        }
        fflush(0);
    }
}


#define CAMERA_TRACKER  0
#define GET_AUX_INPUTS  0

/*****************************************************************************
*
*   functionName:   main
*   Description:    a sample main
*   Created:        12/7/98
*   Author:         Yury Altshuler
*
*   Comments:       This simple main shows how to initialize and get data from
*                   two InterSense trackers. Most of the code is only relevant 
*                   when used with an IS-x series device and can be removed if
*                   using with InterTrax. 
*
******************************************************************************/
int main()
{
    ISD_TRACKER_HANDLE              handle;
    ISD_TRACKER_DATA_TYPE           data;
    ISD_STATION_INFO_TYPE           Station[ISD_MAX_STATIONS];
    ISD_CAMERA_DATA_TYPE            cameraData;
    ISD_TRACKER_INFO_TYPE           Tracker;
    ISD_HARDWARE_INFO_TYPE          hwInfo;
    ISD_STATION_HARDWARE_INFO_TYPE  stationHwInfo[ISD_MAX_STATIONS];

   
    WORD done = FALSE, station = 1;
    DWORD maxStations = 4;
    Bool verbose = TRUE;
    float lastTime;

    // Detect first tracker. If you have more than one InterSense device and
    // would like to have a specific tracker, connected to a known port, 
    // initialized first, then enter the port number instead of 0. Otherwise, 
    // tracker connected to the rs232 port with lower number is found first 

    handle = ISD_OpenTracker( (Hwnd) NULL, 0, FALSE, verbose );
    
    // Check value of handle to see if tracker was located 
    if( handle < 1 )
    {
        printf("Failed to detect InterSense tracking device\n");
    }
    else
    {
        // Get tracker configuration info 
        ISD_GetTrackerConfig( handle, &Tracker, verbose );
                
        memset((void *) &hwInfo, 0, sizeof(hwInfo));

        if( ISD_GetSystemHardwareInfo( handle, &hwInfo ) )
        {
            if( hwInfo.Valid )
            {
                maxStations = hwInfo.Capability.MaxStations;
            }
        }

        // Clear station configuration info to make sure GetAnalogData and other flags are FALSE 
        memset((void *) Station, 0, sizeof(Station));

        // General procedure for changing any setting is to first retrieve current 
        // configuration, make the change, and then apply it. Calling 
        // ISD_GetStationConfig is important because you only want to change 
        // some of the settings, leaving the rest unchanged. 
        
        if( Tracker.TrackerType == ISD_PRECISION_SERIES )
        {
            for( station = 1; station <= maxStations; station++ )
            {         
                // fill ISD_STATION_INFO_TYPE structure with current station configuration 
                if( !ISD_GetStationConfig( handle, 
                    &Station[station-1], station, verbose )) break;
                
                if( !ISD_GetStationHardwareInfo( handle, 
                    &stationHwInfo[station-1], station )) break;

                if( GET_AUX_INPUTS && Tracker.TrackerModel == ISD_IS900 )
                {
                    Station[station-1].GetAuxInputs = TRUE;
                    
                    // apply new configuration 
                    if( !ISD_SetStationConfig( handle,
                        &Station[station-1], station, verbose )) break;
                }

                if( CAMERA_TRACKER && Tracker.TrackerModel == ISD_IS900 )
                {
                    Station[station-1].GetCameraData = TRUE;
                    
                    // apply new configuration 
                    if( !ISD_SetStationConfig( handle,
                        &Station[station-1], station, verbose )) break;
                }
            }
        }
    }

    station = 1;
    lastTime = ISD_GetTime();

    while(!done)
    {
#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)

        if(kbhit())
        {
            switch(getch())
            {
            case 'A':
                {
                    BYTE AuxOutput[4];

                    AuxOutput[0] = 255;
                    AuxOutput[1] = 255;
                    ISD_AuxOutput( handle, 1, AuxOutput, 4 );
                }
                break;

            case 'a':
                {
                    BYTE AuxOutput[4];

                    AuxOutput[0] = 0;
                    AuxOutput[1] = 0;
                    ISD_AuxOutput( handle, 1, AuxOutput, 4 );
                }
                break;

            case 'b':
                ISD_Boresight( handle, 1, FALSE );
                break;

            case 'B':
                ISD_Boresight( handle, 1, TRUE );
                break;

            case 'e': // IS-x products only, not for InterTrax 
            case 'E':
                
                // First get current station configuration 
                if( ISD_GetStationConfig( handle,
                    &Station[station-1], station, verbose ) )
                {
                    if( Station[station-1].Enhancement == 1 )
                        Station[station-1].Enhancement = 3;
                    else
                        Station[station-1].Enhancement = 1;

                    // Send the new configuration to the tracker 
                    if( ISD_SetStationConfig( handle, 
                        &Station[station-1], station, verbose ) )
                    {
                        // display the results 
                        showTrackerStats( handle );
                    }
                }
                break;
                
            case 'p': 
            case 'P': // IS-x products only, not for InterTrax 
                
                if( ISD_GetStationConfig( handle, 
                    &Station[station-1], station, verbose ))
                {
                    Station[station-1].Prediction += 10;
                    if( Station[station-1].Prediction > 50 ) 
                    {
                        Station[station-1].Prediction = 0;
                    }
                    
                    if( ISD_SetStationConfig( handle,
                        &Station[station-1], station, verbose ) )
                    {
                        showTrackerStats( handle );
                    }
                }
                break;
                
            case 'R':
                ISD_ResetHeading( handle, 1 );
                break;
                
            case 't': 
            case 'T': // IS-x products only, not for InterTrax 
                
                if( ISD_GetStationConfig( handle, &Station[station-1], 
                    station, verbose ) )
                {
                    Station[station-1].TimeStamped = !Station[station-1].TimeStamped;
                    
                    if( ISD_SetStationConfig( handle, 
                        &Station[station-1], station, verbose ) )
                    {
                        showTrackerStats( handle );
                    }
                }
                break;
                
            case '1':
                
                station = 1;
                printf("Current Station is set to %d \n", station);
                break;
                
            case '2': // IS-x products only, not for InterTrax 
                
                if( maxStations > 1 ) 
                {
                    station = 2;
                    printf("Current Station is set to %d \n", station);
                }
                break;
                
            case '3': // IS-x products only, not for InterTrax 
                
                if( maxStations > 2 ) 
                {
                    station = 3;
                    printf("Current Station is set to %d \n", station);
                }
                break;
                
            case '4': // IS-x products only, not for InterTrax 
                
                if( maxStations > 3 ) 
                {
                    station = 4;
                    printf("Current Station is set to %d \n", station);
                }
                break;
                
            case 'O':
                if( ISD_GetStationConfig( handle, &Station[station-1], 
                    station, verbose ) )
                {
                    Station[station-1].TipOffset[0] += 0.1f;
                    Station[station-1].TipOffset[0] += 0.2f;
                    Station[station-1].TipOffset[0] += 0.3f;
                    
                    if( ISD_SetStationConfig( handle, &Station[station-1],
                        station, verbose ) )
                    {
                        showTrackerStats( handle );
                    }
                }
                break;

            case ESC:
            case 'q':
            case 'Q':
                done = TRUE;
                break;
                
            case 'C':
            case 'c':
                if( ISD_GetStationConfig( handle, 
                    &Station[station-1], station, verbose ) )
                {
                    if( Station[station-1].Compass == 2 ) 
                        Station[station-1].Compass = 0;
                    else
                        Station[station-1].Compass = 2;
                    
                    if( ISD_SetStationConfig( handle,
                        &Station[station-1], station, verbose ) )
                    {
                        showTrackerStats( handle );
                    }
                    else
                    {
                        printf( "ISD_SetStationConfig failed\n" );
                    }
                }
                else
                {
                    printf("ISD_GetStationConfig failed\n" );
                }
                break;
                
            default:
                printf( "q -- quit\n");
                printf( "1 -- make station 1 current\n" );
                printf( "2 -- make station 2 current\n" );
                printf( "3 -- make station 3 current\n" );
                printf( "4 -- make station 4 current\n" );
                printf( "E -- change enhancement mode\n" );
                printf( "P -- change predition\n" );
                printf( "T -- Turn time stamps ON/OFF\n" );
                break;
            }
        }
#endif
        // must be called at a reasonable rate
        if( handle > 0 )
        {
            ISD_GetData( handle, &data );    
            ISD_GetCameraData( handle, &cameraData );
            
            if( ISD_GetTime() - lastTime > 0.01f )
            {
                lastTime = ISD_GetTime();
                   
                if( handle > 0 )
                {
                    showStationData( handle, &Tracker,
                        &Station[station-1], &data.Station[station-1], &cameraData.Camera[station-1] );
                }
            }
        }
#ifdef _WIN32
        Sleep( 10 );
#endif
    }
    
    ISD_CloseTracker( handle );
    return 1;
}





