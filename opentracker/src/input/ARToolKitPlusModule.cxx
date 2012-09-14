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
/** source file for ARToolKitPlusModule module.
 *
 * @author Daniel Wagner
 * @author Erick Mendez
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */


// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <cstdlib>
#include <algorithm>
#include <cctype>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/OtLogger.h>
#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <OpenTracker/input/ARToolKitPlusModule.h>
#include <OpenTracker/input/ARToolKitSource.h>
#include <OpenTracker/input/ARToolKitSingleMarkerSource.h>
#include <OpenTracker/input/ARToolKitMultiMarkerSource.h>


#ifdef USE_ARTOOLKITPLUS

#include <OpenTracker/core/Context.h>
namespace ot{
OT_MODULE_REGISTER_FUNC(ARToolKitPlusModule){
	        // Create an ARToolKitPlusModule instance
			ARToolKitPlusModule *artoolplus = new ARToolKitPlusModule;
			context->addFactory( * artoolplus );
			context->addModule( "ARToolKitPlusConfig", *artoolplus );
			context->registerVideoUser(artoolplus);
	}
}

#ifdef WIN32
// in SAM we use another mechanism to link against ARToolKitPlus
// in order to use the release DLL even in debug mode.
// for that case we can define OT_DO_NOT_LINK_ARTOOLKITPLUS
// to deactivate the automatic linking process
//
#ifdef ARTOOLKITPLUS_DLL
#  include <ARToolKitPlus/TrackerSingleMarker.h>
#  pragma message("compiling against ARToolKitPlus DLL")
#  ifndef OT_DO_NOT_LINK_ARTOOLKITPLUS
#    if (defined(DEBUG) || defined(_DEBUG)) && !defined(ARTOOLKITPLUS_ALWAYS_RELEASE)
#      pragma comment(lib, "ARToolKitPlusDllD.lib")
#      pragma message("ARToolKitPlusModule: linking against ARToolKitPlusDllD.lib")
#    else
#      pragma comment(lib, "ARToolKitPlusDll.lib")
#      pragma message("ARToolKitPlusModule: linking against ARToolKitPlusDll.lib")
#    endif
#  endif
#else
#  pragma message("compiling against ARToolKitPlus Lib")
#  include <ARToolKitPlus/TrackerSingleMarkerImpl.h>
#  ifndef OT_DO_NOT_LINK_ARTOOLKITPLUS
#    if (defined(DEBUG) || defined(_DEBUG)) &&  !defined(ARTOOLKITPLUS_ALWAYS_RELEASE)
#      pragma comment(lib, "ARToolKitPlusD.lib")
#      pragma message("ARToolKitPlusModule: linking against ARToolKitPlusD.lib")
#    else
#      pragma comment(lib, "ARToolKitPlus.lib")
#      pragma message("ARToolKitPlusModule: linking against ARToolKitPlus.lib")
#    endif
#  endif
#endif
#else // WIN32
#  include <ARToolKitPlus/TrackerSingleMarker.h>
#  include <ARToolKitPlus/TrackerSingleMarkerImpl.h>
#  include <cctype>
#  include <iostream>
#endif
#include <ARToolKitPlus/Logger.h>


class ARToolKitPlusModuleLogger : public ARToolKitPlus::Logger
{
    // implement ARToolKitPlus::Logger
    void artLog(const char* nStr)
    {
        ot::logPrintI("%s", nStr);
    }

    void artLogEx(const char* nStr, ...)
    {
        char tmpString[512];
        va_list marker;

        va_start(marker, nStr);
        vsprintf(tmpString, nStr, marker);

        artLog(tmpString);
    }
};

#include <cstdio>
#include <cstdlib>
#include <iostream>


namespace ot {


    bool convertPixelFormat_OpenTracker_to_ARToolKitPlus(ot::PIXEL_FORMAT nSrcFormat, ARToolKitPlus::PIXEL_FORMAT &nDstFormat)
    {
	switch(nSrcFormat)
	{
            case ot::FORMAT_X8B8G8R8:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_ABGR;
		return true;
            case ot::FORMAT_B8G8R8X8:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_BGRA;
		return true;
            case ot::FORMAT_B8G8R8:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_BGR;
		return true;
            case ot::FORMAT_R8G8B8X8:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_RGBA;
		return true;
            case ot::FORMAT_R8G8B8:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_RGB;
		return true;
            case ot::FORMAT_R5G6B5:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_RGB565;
		return true;
            case ot::FORMAT_L8:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_LUM;
		return true;
            default:
		return false;
	}
    }

    bool convertPixelFormat_ARToolKitPlus_to_OpenTracker(ARToolKitPlus::PIXEL_FORMAT nSrcFormat, ot::PIXEL_FORMAT &nDstFormat)
    {
	switch(nSrcFormat)
	{
            case ARToolKitPlus::PIXEL_FORMAT_ABGR:
		nDstFormat = ot::FORMAT_X8B8G8R8;
		return true;
            case ARToolKitPlus::PIXEL_FORMAT_BGRA:
		nDstFormat = ot::FORMAT_B8G8R8X8;
		return true;
            case ARToolKitPlus::PIXEL_FORMAT_BGR:
		nDstFormat = ot::FORMAT_B8G8R8;
		return true;
            case ARToolKitPlus::PIXEL_FORMAT_RGBA:
		nDstFormat = ot::FORMAT_R8G8B8X8;
		return true;
            case ARToolKitPlus::PIXEL_FORMAT_RGB:
		nDstFormat = ot::FORMAT_R8G8B8;
		return true;
            case ARToolKitPlus::PIXEL_FORMAT_RGB565:
		nDstFormat = ot::FORMAT_R5G6B5;
		return true;
            case ARToolKitPlus::PIXEL_FORMAT_LUM:
		nDstFormat = ot::FORMAT_L8;
		return true;
            default:
		return false;
	}
    }


    ARToolKitPlusModule::ARToolKitPlusModule() : ThreadModule(), NodeFactory()
    {
	doBench = false;
	sizeX = sizeY = -1;

	trackerNear = 1.0f;
	trackerFar = 1000.0f;

	logger = new ARToolKitPlusModuleLogger;


#ifdef ARTOOLKITPLUS_DLL

	tracker = ARToolKitPlus::createTrackerSingleMarker(320,240, 6,6,6);

#else

	// NOTE: if you get an error here complaining about 6 template parameters
	//       then please update to the latest version of ARToolKitPlus
	tracker = new ARToolKitPlus::TrackerSingleMarkerImpl<6,6,6, 32,32>(320,240);

#endif //ARTOOLKITPLUS_DLL


	tracker->init(NULL, trackerNear, trackerFar, logger);
	tracker->setThreshold(100);

	bestCFs = NULL;
	maxMarkerId = MAX_MARKERID;
	useMarkerDetectLite = false;

	stop = 0;
    }

    ARToolKitPlusModule::~ARToolKitPlusModule()
    {
        sources.clear();
	sourcesMap.clear();

	delete tracker;
	delete logger;
    }

    // This method is called to construct a new Node.

    Node* ARToolKitPlusModule::createNode( const std::string& name, const StringTable& attributes)
    {
        // ARToolKitPlusSource Deprecated. Mendez 20070716
        bool tmpFlag=false;
        if(name.compare("ARToolKitPlusSingleMarkerSource") == 0 )
            tmpFlag=true;
        if(name.compare("ARToolKitPlusSource") == 0 ) // ARToolKitPlusSource Deprecated. Mendez 20070716
        {
            logPrintW("Name 'ARToolKitPlusSource' deprecated. Please use 'ARToolKitPlusSingleMarkerSource'.\n");
            tmpFlag=true;
        }
        if(tmpFlag) // ARToolKitPlusSource Deprecated. Mendez 20070716
        {
            double center[2], size;
            int num;

            if( (num = attributes.get("center", center, 2)) != 2 )
            {
                //cout << "ARToolKit Wrong number of center coordinates : " << num << endl;
                ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:ARToolKit Wrong number of center coordinates : %d\n"), num));
                return NULL;
            }
            if( attributes.get("size", &size) != 1 )
            {
                //cout << "ARToolKit Not a size" << endl;
                ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:ARToolKit Not a size\n")));
                return NULL;
            }

            int id;
            std::string filename = attributes.get("tag-file");
            std::string markerid = attributes.get("tag-id");
            std::string fullname;

            // see if we have a marker-id field
            if(markerid.length())
            {
                id = atoi(markerid.c_str());
            }
            else
		// otherwise look for a marker-filename
		if(filename.length())
		{
                    if( patternDirectory.compare("") != 0)
                        context->addDirectoryFirst( patternDirectory );

                    if( context->findFile(filename, fullname))
                    {
                        filename = fullname;
                    }
                    else
                    {
                        logPrintE("ARToolkit could not find tag file %s\n", filename.c_str());
                        return NULL;
                    }

                    if( patternDirectory.compare("") != 0)
                        context->removeDirectory( patternDirectory );
        
                    if((id = tracker->addPattern((char *)filename.c_str() )) < 0 )
                    {
                        logPrintE("ARToolKit Error reading tag-file %s or %s\n", attributes.get("tag-file").c_str(), filename.c_str());
                        return NULL;
                    }
		}
		else
		{
                    // not good, this means we have a problem...
		}

            // we need to store the maximum marker id for later...
            if(id>maxMarkerId)
            {
                maxMarkerId = id;
                if(bestCFs)
                {
                    delete [] bestCFs;
                    bestCFs = NULL;
                }
            }

            ARToolKitSingleMarkerSource* source = new ARToolKitSingleMarkerSource( id, center, size );
            //source->type = "ARToolKitPlusSingleMarkerSource";
            sources.push_back( source );
            sourcesMap.insert(std::make_pair(id, source));

            //cout << "Built ARToolKitSource " << filename << " id " << id << endl;
            logPrintI("Built ARToolKitPlusSingleMarkerSource %s id %d\n", filename.c_str(), id);
            return source;
        }

        if( name.compare("ARToolKitPlusMultiMarkerSource") == 0 )
        {
            std::string filename = attributes.get("cfg-file");
            std::string fullname;

            if(patternDirectory.compare("") != 0)
                context->addDirectoryFirst(patternDirectory);

            if(context->findFile(filename, fullname))
                filename = fullname;
            else
            {
                logPrintE("ARToolkit could not find multi-cfg file %s\n", filename.c_str());
                return NULL;
            }


            ARToolKitPlus::ARMultiMarkerInfoT* mmConfig = tracker->arMultiReadConfigFile(filename.c_str());

            if(mmConfig)
            {

                ARToolKitMultiMarkerSource * source = new ARToolKitMultiMarkerSource( filename, mmConfig );

                //source->type = "ARToolKitMultiMarkerSource";
                sources.push_back( source );

                // we store the ids of all markers in this config...
                for(int i=0; i<mmConfig->marker_num; i++)
                {
                    int mId = mmConfig->marker[i].patt_id;

                    sourcesMap.insert(std::make_pair(mId, source));

                    if(mId>maxMarkerId)
                        maxMarkerId = mId;
                }

                if(bestCFs)
                {
                    delete [] bestCFs;
                    bestCFs = NULL;
                }

                return source;
            }
            else
            {
                logPrintE("ARToolKit Error reading multi-cfg %s\n");
                return NULL;
            }
	}

        return NULL;
    }


    // initializes the ARToolKit module

#ifdef WIN32
#define MAKE_STRING_LOWER(STR)  std::transform(STR.begin(), STR.end(), STR.begin(), std::tolower);

#else
#define MAKE_STRING_LOWER(STR)  std::transform(STR.begin(), STR.end(), STR.begin(), static_cast<int(*)(int)>(std::tolower));
#endif
void ARToolKitPlusModule::init(StringTable& attributes, ConfigNode * localTree)
{
	cameradata = attributes.get("camera-parameter");
        patternDirectory = attributes.get("pattern-dir");

	std::string undistmode = attributes.get("undist-mode");
	std::string detectmode = attributes.get("detect-mode");
	std::string posemode = attributes.get("pose-estimator");
	std::string threshold = attributes.get("treshold");
	std::string markermode = attributes.get("marker-mode");


	MAKE_STRING_LOWER(undistmode);
	MAKE_STRING_LOWER(detectmode);
	MAKE_STRING_LOWER(posemode);
	MAKE_STRING_LOWER(threshold);
	MAKE_STRING_LOWER(markermode);

#ifndef WIN32
        std::cerr << undistmode << std::endl;
        std::cerr << detectmode << std::endl;
        std::cerr << posemode << std::endl;
        std::cerr << threshold << std::endl;
        std::cerr << markermode << std::endl;
        //exit(1);
#endif



	// marker detection mode: lite vs. full
	//
	if(useMarkerDetectLite = (detectmode=="lite"))
	{
            logPrintI("ARToolkitModule using marker detect mode 'lite'\n");
	}
	else
	{
            logPrintI("ARToolkitModule using marker detect mode 'normal'\n");
	}


	if(undistmode=="none")
	{
            tracker->setUndistortionMode(ARToolKitPlus::UNDIST_NONE);
            logPrintI("ARToolkitModule lens undistortion disabled\n");
	}
	else if(undistmode=="lut")
	{
            tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
            logPrintI("ARToolkitModule lens undistortion set to lookup-table\n");
	}
	else
	{
            logPrintI("ARToolkitModule using default lens undistortion (this will be slow!)\n");
	}


	// pose estimator settings
	//
	if(posemode=="cont")
	{
            tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_ORIGINAL_CONT);
            logPrintI("ARToolkitModule using CONT pose estimator\n");
	}
	else if(posemode=="rpp")
	{
            tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);
            logPrintI("ARToolkitModule using robust pose estimator\n");
	}
	else
	{
            logPrintI("ARToolkitModule using default pose estimator\n");
	}


	// setting of threshold value
	//
	if(threshold == "auto")
	{
            tracker->activateAutoThreshold(true);
            logPrintI("ARToolkitModule auto-thresholding activated\n");
	}
	else
	{
            int tmpThreshold = atoi(threshold.c_str());

            if( tmpThreshold < 0 )
                tmpThreshold = 0;
            else if( tmpThreshold > 255 )
                tmpThreshold = 255;

            tracker->setThreshold(tmpThreshold);
            logPrintI("ARToolkitModule manual thresholding with '%s'\n", tmpThreshold);
	}



	// setting for id-based vs. template-based markers
	//
	if(markermode == "idbased")
	{
            tracker->setMarkerMode(ARToolKitPlus::MARKER_ID_SIMPLE);
            logPrintI("ARToolkitModule using id-based markers\n");
	}
	else
            if(markermode == "bch")
            {
		tracker->setMarkerMode(ARToolKitPlus::MARKER_ID_BCH);
		logPrintI("ARToolkitModule using BCH markers\n");
            }
            else
            {
		logPrintI("ARToolkitModule using template markers\n");
            }


	// setting for the border width - default value is 0.250
	//
	if(attributes.get("border-width").length()>0)
	{
            float w = (float)atof(attributes.get("border-width").c_str());
            logPrintI("ARToolkitModule using border-width of %.3f\n", w);
            tracker->setBorderWidth(w);
	}
	else
	{
            logPrintI("ARToolkitModule using default border-width of 0.250\n");
	}


        // parsing camera config hints
	//
	if(cameraDeviceHint.length()>0)
            for(unsigned int i = 0; i <localTree->countChildren(); i++)
            {
                ConfigNode * child = (ConfigNode *)localTree->getChild(i);
                StringTable & childAttr = child->getAttributes();

                std::string devName = childAttr.get("device-name"),
                    devFile = childAttr.get("camera-parameter");

                if(devName.length()>0 && devFile.length()>0 && devName==cameraDeviceHint)
                {
                    logPrintI("ARToolkitModule uses camera-hint for '%s'\n", devName.c_str());
                    cameradata = devFile;
                    break;
                }
            }

	logPrintI("ARToolkitModule loads camera file %s\n", cameradata.c_str());


	// setting for template pattern folder
	//
        if( patternDirectory.compare("") != 0)
            context->addDirectoryFirst( patternDirectory );

        std::string fullname;
        if( context->findFile(cameradata, fullname))
        {
            cameradata = fullname;
        }
        else
        {
            //cout << "ARToolkitModule could not find camera parameter file " << cameradata << endl;
            logPrintE("ARToolkitModule could not find camera parameter file %s\n", cameradata.c_str());
            initialized = 0;
            return;
        }

        if( patternDirectory.compare("") != 0)
            context->removeDirectory( patternDirectory );


	// finally load the camera file
	//
	if(!tracker->loadCameraFile(cameradata.c_str(), trackerNear, trackerFar))
	{
            logPrintE("ARToolkitModule error loading camera parameters from %s\n", cameradata.c_str());
            initialized = 0;
            return;
	}


	initialized = 1;
	logPrintI("ARToolkitModule initialization finished\n");
    }


    void
    ARToolKitPlusModule::newVideoFrame(const unsigned char* frameData, int newSizeX, int newSizeY, PIXEL_FORMAT imgFormat)
    {
	    if(!initialized || maxMarkerId<0)
                return;


        ARToolKitPlus::ARMarkerInfo * markerInfo;
        int markerNum;
        int j;
        ARFloat matrix[3][4];
        PIXEL_FORMAT artkpImgFormat;

	    if(!convertPixelFormat_ARToolKitPlus_to_OpenTracker(tracker->getPixelFormat(), artkpImgFormat))
        {
            logPrintW("ARToolKitPlusModule::newVideoFrame: could not convert format\n");
            return;
        }

	    // did we get another pixel format than ARToolKitPlus currently expects?
	    if(imgFormat!=artkpImgFormat)
	    {
            // ARToolKitPlus 2.1 and later can change pixel format at runtime!
            //
            ARToolKitPlus::PIXEL_FORMAT artkpFormat;
            if(!convertPixelFormat_OpenTracker_to_ARToolKitPlus(imgFormat, artkpFormat))
            {
                logPrintW("ARToolKitPlusModule::newVideoFrame: could not convert format\n");
                return;
            }
            tracker->setPixelFormat(artkpFormat);
	    }


	    // if the image size changed we have to reinitialize some ARToolKitPlus internal stuff...
	    // 
	    if(newSizeX!=sizeX || newSizeY!=sizeY)
	    {
            sizeX = newSizeX;
            sizeY = newSizeY;
            tracker->changeCameraSize(newSizeX, newSizeY);
	    }


	    // reset all markers from last grab, then clear list
	    //
        for(NodeVector::iterator it=visibleMarkers.begin(); it!=visibleMarkers.end(); it++)
	    {
            Node* source = ((Node *)*it);

            //if(source->getType()=="ARToolKitPlusSource")
            //{
               ARToolKitSource *smSource = (ARToolKitSource *)source;

               Event & event = smSource->buffer;
               if (event.getConfidence() > 0.00000001f) 
               {
                  event.getConfidence() = 0.0f;
                  event.timeStamp();
                  smSource->modified = 1;
               }
  	    }
	    visibleMarkers.clear();

	    // try to find markers in the camera image
	    //
	    if(useMarkerDetectLite)
	    {
            if(tracker->arDetectMarkerLite((ARToolKitPlus::ARUint8*)frameData, tracker->getThreshold(), &markerInfo, &markerNum ) < 0 )
            {
                //logPrintW("ARToolKitPlusModule::newVideoFrame: no marker found (lite)\n");
                return;
            }
	    }
	    else
	    {
            if(tracker->arDetectMarker((ARToolKitPlus::ARUint8*)frameData, tracker->getThreshold(), &markerInfo, &markerNum ) < 0 )
            {
                //logPrintW("ARToolKitPlusModule::newVideoFrame: no marker found\n");
                return;
            }
	    }


        if( markerNum < 1 )
	    {
            return;
	    }

	    // we use an array of best confidences to quickly find
	    // the best markers for each id. a simple check against all
	    // other visible markers would result in O(n²), now this method 
	    // results roughly in 4*O(n) which is better than O(n²) for large n.
	    //
	    if(!bestCFs)
	    {
            bestCFs = new float[maxMarkerId];
            for(j=0; j<maxMarkerId; j++)
                bestCFs[j] = 0.0f;
	    }

	    // store best confidences
	    //
        int id=0;
	    for(j=0; j<markerNum; j++)
	    {
            //logPrintI("marker %d found (%f)\n", 
            //          markerInfo[j].id, markerInfo[j].cf);

            id = markerInfo[j].id;
            if(id==-1) continue;

            if(bestCFs[id]<markerInfo[j].cf)
                bestCFs[id] = markerInfo[j].cf;
	    }
        id=0;

	    // stores all multi.marker sources that have already been processed
	    // in order to prevent multiple calculation of the same source due to multiple markers
	    NodeVector processedSources;

	    // walk through all markers in the image...
	    //
        for(j=0; j<markerNum; j++)
	    {
            id = markerInfo[j].id;
            if(id==-1) continue;

            // only use a marker if it has the best confidence for its id
            //
            if(markerInfo[j].cf<bestCFs[id]) continue;

            MarkerIdMap::iterator it = sourcesMap.find(id);

            if(it==sourcesMap.end()) continue;

            Node* source = NULL;

            source = it->second;
            if(source==0) continue;


            // we found a "best" marker and its source.
            // everything is fine to process it...

            // store that source for later usage
            //
            visibleMarkers.push_back(source);

            if((source->getType()=="ARToolKitPlusSingleMarkerSource")||(source->getType()=="ARToolKitPlusSource"))
            {
                ARToolKitSingleMarkerSource *sourceA = (ARToolKitSingleMarkerSource*)source;
                ARFloat source_center[2], source_size;

                source_center[0] = (ARFloat)sourceA->center[0];
                source_center[1] = (ARFloat)sourceA->center[1];
                source_size = (ARFloat)sourceA->size;

                //if(tracker->arGetTransMat(&markerInfo[j], source_center, source_size, matrix)>=0)
                if(tracker->executeSingleMarkerPoseEstimator(&markerInfo[j], source_center, source_size, matrix)>=0)
                    updateSingleMarkerSource(sourceA, markerInfo[j].cf, matrix);
            }
            else if(source->getType()=="ARToolKitPlusMultiMarkerSource")
            {
                bool alreadyProcessed = false;

                for(NodeVector::iterator it=processedSources.begin(); it!=processedSources.end(); it++)
                {
                    if(*it==source)
                    {
                        alreadyProcessed = true;
                        break;
                    }
                }

                if(!alreadyProcessed)
                {
                    ARToolKitMultiMarkerSource *sourceM = (ARToolKitMultiMarkerSource*)source;
                    ARToolKitPlus::ARMultiMarkerInfoT* mmConfig = (ARToolKitPlus::ARMultiMarkerInfoT*)sourceM->mmConfig;

                    //if((tracker->arMultiGetTransMat(markerInfo, markerNum, mmConfig))>=0)
                    if((tracker->executeMultiMarkerPoseEstimator(markerInfo, markerNum, mmConfig))>=0)
                        updateMultiMarkerSource(sourceM, 1.0f, mmConfig->trans);

                    processedSources.push_back(source);
                }
            }
	    }
        id=0;

	    // reset array of best confidences
	    //
	    for(j=0; j<markerNum; j++)
        {
            id = markerInfo[j].id;
            if(id==-1) continue;

            bestCFs[id] = 0.0f;
        }

	    return;
    }

    // pushes events into the tracker tree.

    void ARToolKitPlusModule::pushEvent()
    {
	      for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
	      {
                  ARToolKitSource * source = (ARToolKitSource *)((Node *)*it);
                  //if( TRUE ) 
                  if( source->modified == 1 )
                  {
                      source->event = source->buffer;
                      source->modified = 0;
                      source->updateObservers( source->event );
                  }
	      }
    }

    void ARToolKitPlusModule::updateEvent(Event &event, float matrix[3][4])
    {
#ifdef ARTOOLKIT_UNFLIP_V
#undef ARTOOLKIT_UNFLIP_V
#endif

#ifdef ARTOOLKIT_UNFLIP_V

	//  --- correct ARToolkit's vertical image mirroring ---

	MathUtils::Matrix4x4 matrix_4x4;
	for(int r = 0; r < 3; r ++ )
            for(int c = 0; c < 4; c ++ )
                matrix_4x4[r][c] = (float)matrix[r][c];
    
	matrix_4x4[3][0] = 0; matrix_4x4[3][1] = 0;
	matrix_4x4[3][2] = 0; matrix_4x4[3][3] = 1;
    
	MathUtils::Matrix4x4 matrix_4x4_corrected;
 
	// fix translation
	MathUtils::matrixMultiply(MathUtils::matrix4x4_flipY,matrix_4x4,matrix_4x4_corrected);
    
	MathUtils::Vector3 euler_angles;
	MathUtils::MatrixToEuler(euler_angles,matrix_4x4);

	MathUtils::eulerToQuaternion(-euler_angles[Q_Z],euler_angles[Q_Y],-euler_angles[Q_X], event.getOrientation);

	event.getPosition()[0] = (float)matrix_4x4_corrected[0][3];
	event.getPosition()[1] = (float)matrix_4x4_corrected[1][3];
	event.getPosition()[2] = (float)matrix_4x4_corrected[2][3];
	//  -----------------------------------------------------------
#else
	//  --- DO NOT correct ARToolkit's vertical image mirroring ---

	float m[3][3];
	for( int r = 0; r < 3; r ++ )
	{
            for( int s = 0; s < 3; s ++ )
            {
                m[r][s] = (float)matrix[r][s];
            }
	}
	MathUtils::matrixToQuaternion( m, event.getOrientation() );
	event.getPosition()[0] = (float)matrix[0][3];
	event.getPosition()[1] = (float)matrix[1][3];
	event.getPosition()[2] = (float)matrix[2][3];
	//  -----------------------------------------------------------
#endif
	event.timeStamp();
    }

    void ARToolKitPlusModule::updateSingleMarkerSource(Node *node, float cf, ARFloat matrix[3][4])
    {
	ARToolKitSource* source = (ARToolKitSource*)node;

	Event & event = source->buffer;
	event.setConfidence(cf);
	updateEvent(event,matrix);
	source->modified = 1;
    }

    void ARToolKitPlusModule::updateMultiMarkerSource(Node *node, float cf, ARFloat matrix[3][4])
    {
	ARToolKitMultiMarkerSource* source = (ARToolKitMultiMarkerSource*)node;

	Event & event = source->buffer;
	event.setConfidence(cf);
	updateEvent(event,matrix);
	source->modified = 1;
    }


    const char*
    ARToolKitPlusModule::getARToolKitPlusDescription() const
    {
	const char* descr = tracker->getDescription();
	return descr;
    }


    bool newVideoFrame(const unsigned char* image, int width, int height, PIXEL_FORMAT format);


} //namespace ot


#else
#ifdef WIN32
#pragma message(">>> no ARToolKitPlus support")
#endif
#endif //USE_ARTOOLKITPLUS

/* 
 * ------------------------------------------------------------
 *   End of ARToolKitPlusModule.cxx
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
