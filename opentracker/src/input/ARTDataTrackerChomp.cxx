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
/** source file for ARTDataTrackerChomp class.
 *
 * @author Christopher Schmidt
 *
 * $Id: ARTDataTrackerChomp.cxx 1590 2006-10-31 17:31:17Z mendez $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <OpenTracker/input/ARTDataTrackerChomp.h>

#include <cstdlib>
#include <sys/types.h>
#include <cstdio>

#include <iostream>
#include <sstream>

#include <ace/Log_Msg.h>


#ifndef OT_NO_ARTDATATRACKER_SUPPORT


namespace ot {

    // Destructor method

    ARTDataTrackerChomp::~ARTDataTrackerChomp()
    {	
    }


    ARTDataTrackerChomp::ARTDataTrackerChomp()
    {
    }

    void ARTDataTrackerChomp::chomp(std::string datagramm)
    {
	using namespace std;

	istringstream iss(datagramm);
              
	int linenumber = 0;

	int station = -1;
	double timestamp = 0.0;

	std::map<int, BodyRecord>::iterator bdit;
	std::map<int, MarkerRecord>::iterator mkit;
	std::map<int, FlystickRecord>::iterator fsit;
	std::map<int, MeasuretargetRecord>::iterator mtit;
	for ( bdit = tempBodyRecord.begin(); bdit != tempBodyRecord.end(); bdit++) {
	    ((*bdit).second).valid = false;
	}
	for ( mkit = tempMarkerRecord.begin(); mkit != tempMarkerRecord.end(); mkit++) {
	    ((*mkit).second).valid = false;
	}
	for ( fsit = tempFlystickRecord.begin(); fsit != tempFlystickRecord.end(); fsit++) {
	    ((*fsit).second).valid = false;
	}
	for ( mtit = tempMeasuretargetRecord.begin(); mtit != tempMeasuretargetRecord.end(); mtit++) {
	    ((*mtit).second).valid = false;
	}
	numberTrackedBodies = 0;
	numberTrackedMarkers = 0;
	numberTrackedFlysticks = 0;
	numberTrackedMeasuretargets = 0;

	while (!iss.eof()) {
	    stringbuf linetype;
	    stringbuf linecontent;

	    iss.get(linetype,' ');
	    
	    if ((linenumber == 0) && (linetype.str() != "fr")) {
        logPrintD("Error receiving correct Data [#001]\n");
        logPrintD("Check if Format in ARTTracker Software is set to ASCII\n");
		//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error receiving correct Data!!! [#001]\n")));
		//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Check if Format in ARTTracker Software is set to ASCII !!!\n")));
		return;       
	    }

	    //ACE_DEBUG((LM_INFO, ACE_TEXT("%d linetype    |%s|\n"), linenumber, linetype.str().c_str() ));	    
	    if (linetype.str() == "fr") {
		iss >> frameNumber;
		
		iss.get(linecontent);

		if (static_cast<int>(iss.peek()) == 10) {		    
		    iss.ignore(1);
		}
	    }
	    else if (linetype.str() == "ts") {		
		iss >> timestamp;
		//ACE_DEBUG((LM_INFO, ACE_TEXT("%d timestamp  |%f|\n"), linenumber, timestamp ));
		iss.get(linecontent);
		if (static_cast<int>(iss.peek()) == 10) {		    
		    iss.ignore(1);
		}
	    }
	    else if (linetype.str() == "6d") {
		iss >> numberTrackedBodies;
		//ACE_DEBUG((LM_INFO, ACE_TEXT("%d 6dcount   |%d|\n"), linenumber, numberTrackedBodies ));

		if (numberTrackedBodies > 0) iss.ignore(2);
		
		int i;
		for (i=0; i<numberTrackedBodies; i++) {
		    iss >> station;
		    //ACE_DEBUG((LM_INFO, ACE_TEXT("%d station   |%d|\n"), linenumber, station ));
		    
		    tempBodyRecord[station].id = station;
		    iss >> tempBodyRecord[station].quality;
		    iss.ignore(2);
		    iss >> tempBodyRecord[station].location[0];
		    iss >> tempBodyRecord[station].location[1];
		    iss >> tempBodyRecord[station].location[2];
		    iss >> tempBodyRecord[station].eulerAngles[0];
		    iss >> tempBodyRecord[station].eulerAngles[1];
		    iss >> tempBodyRecord[station].eulerAngles[2];
		    iss.ignore(2);
		    iss >> tempBodyRecord[station].rotationMatrix[0];
		    iss >> tempBodyRecord[station].rotationMatrix[1];
		    iss >> tempBodyRecord[station].rotationMatrix[2];
		    iss >> tempBodyRecord[station].rotationMatrix[3];
		    iss >> tempBodyRecord[station].rotationMatrix[4];
		    iss >> tempBodyRecord[station].rotationMatrix[5];
		    iss >> tempBodyRecord[station].rotationMatrix[6];
		    iss >> tempBodyRecord[station].rotationMatrix[7];
		    iss >> tempBodyRecord[station].rotationMatrix[8];

		    tempBodyRecord[station].valid = true;

		    if (i < numberTrackedBodies-1) iss.ignore(3); else iss.ignore(1);		    
		}

		iss.get(linecontent);		
		if (static_cast<int>(iss.peek()) == 10) {		    
		    iss.ignore(1);
		}
	    }
	    else if (linetype.str() == "3d") {
		iss >> numberTrackedMarkers;
		//ACE_DEBUG((LM_INFO, ACE_TEXT("%d 3dcount   |%d|\n"), linenumber, numberTrackedMarkers ));

		if (numberTrackedMarkers > 0) iss.ignore(2);

		int i;		
		for (i=0; i<numberTrackedMarkers; i++) {
		    iss >> station;
		    //ACE_DEBUG((LM_INFO, ACE_TEXT("%d station   |%d|\n"), linenumber, station ));
		    
		    tempMarkerRecord[station].id = station;
		    iss >> tempMarkerRecord[station].quality;
		    
		    iss.ignore(2);
		    iss >> tempMarkerRecord[station].location[0];
		    iss >> tempMarkerRecord[station].location[1];
		    iss >> tempMarkerRecord[station].location[2];

		    tempMarkerRecord[station].valid = true;

		    if (i < numberTrackedMarkers-1) iss.ignore(3); else iss.ignore(1);
		}
    
		iss.get(linecontent);		
		if (static_cast<int>(iss.peek()) == 10) {		    
		    iss.ignore(1);
		}
	    }
	    else if (linetype.str() == "6df") {		
		iss >> numberTrackedFlysticks;		
		//ACE_DEBUG((LM_INFO, ACE_TEXT("%d 6dfcount   |%d|\n"), linenumber, numberTrackedFlysticks ));

		if (numberTrackedFlysticks > 0) iss.ignore(2);
		
		int i;
		for (i=0; i<numberTrackedFlysticks; i++) {
		    iss >> station;
		    //ACE_DEBUG((LM_INFO, ACE_TEXT("%d station   |%d|\n"), linenumber, station ));
		    
		    tempFlystickRecord[station].id = station;
		    iss >> tempFlystickRecord[station].quality;
		    iss >> tempFlystickRecord[station].buttons;
		    iss.ignore(2);
		    iss >> tempFlystickRecord[station].location[0];
		    iss >> tempFlystickRecord[station].location[1];
		    iss >> tempFlystickRecord[station].location[2];
		    iss >> tempFlystickRecord[station].eulerAngles[0];
		    iss >> tempFlystickRecord[station].eulerAngles[1];
		    iss >> tempFlystickRecord[station].eulerAngles[2];
		    iss.ignore(2);
		    iss >> tempFlystickRecord[station].rotationMatrix[0];
		    iss >> tempFlystickRecord[station].rotationMatrix[1];
		    iss >> tempFlystickRecord[station].rotationMatrix[2];
		    iss >> tempFlystickRecord[station].rotationMatrix[3];
		    iss >> tempFlystickRecord[station].rotationMatrix[4];
		    iss >> tempFlystickRecord[station].rotationMatrix[5];
		    iss >> tempFlystickRecord[station].rotationMatrix[6];
		    iss >> tempFlystickRecord[station].rotationMatrix[7];
		    iss >> tempFlystickRecord[station].rotationMatrix[8];

		    tempFlystickRecord[station].valid = true;

		    if (i < numberTrackedFlysticks-1) iss.ignore(3); else iss.ignore(1);		    
		}

		iss.get(linecontent);		
		if (static_cast<int>(iss.peek()) == 10) {		    
		    iss.ignore(1);
		}
	    }
	    else if (linetype.str() == "6dmt") {
		iss >> numberTrackedMeasuretargets;
		//ACE_DEBUG((LM_INFO, ACE_TEXT("%d 6dmtcount   |%d|\n"), linenumber, numberTrackedMeasuretargets ));

		if (numberTrackedMeasuretargets > 0) iss.ignore(2);
						
		int i;
		for (i=0; i<numberTrackedMeasuretargets; i++) {
		    iss >> station;
		    //ACE_DEBUG((LM_INFO, ACE_TEXT("%d station   |%d|\n"), linenumber, station ));
		    
		    tempMeasuretargetRecord[station].id = station;
		    iss >> tempMeasuretargetRecord[station].quality;
		    iss >> tempMeasuretargetRecord[station].buttons;
		    iss.ignore(2);
		    iss >> tempMeasuretargetRecord[station].location[0];
		    iss >> tempMeasuretargetRecord[station].location[1];
		    iss >> tempMeasuretargetRecord[station].location[2];
		    iss.ignore(2);
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[0];
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[1];
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[2];
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[3];
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[4];
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[5];
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[6];
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[7];
		    iss >> tempMeasuretargetRecord[station].rotationMatrix[8];

		    tempMeasuretargetRecord[station].valid = true;

		    if (i < numberTrackedMeasuretargets-1) iss.ignore(3); else iss.ignore(1);		    
		}
		
		iss.get(linecontent);		
		if (static_cast<int>(iss.peek()) == 10) {		    
		    iss.ignore(1);
		}
	    }
	    else if (linetype.str() == "6dcal") {
		iss >> numberTrackedCalBodies;
		//ACE_DEBUG((LM_INFO, ACE_TEXT("%d 6dcalcount   |%d|\n"), linenumber, numberTrackedCalBodies ));
		iss.get(linecontent);
		if (static_cast<int>(iss.peek()) == 10) {		    
		    iss.ignore(1);
		}		
	    }
	    else {
		iss.get(linecontent);
		if (static_cast<int>(iss.peek()) == 10) {		    
		    iss.ignore(1);
		}
	    }
	    
	    ++linenumber;
	}

    }


    void ARTDataTrackerChomp::displayRecords()
    {
	// Output
        logPrintD("Contents of tempBodyRecord, tempMarkerRecord, tempFlystickRecord and tempMeasuretargetRecord\n");
	//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Contents of tempBodyRecord, tempMarkerRecord, tempFlystickRecord and tempMeasuretargetRecord\n")));


        logPrintD("Number of  Tracked Bodies for 6d: %d\n", tempBodyRecord.size());
	//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Number of  Tracked Bodies for 6d: %d\n"), tempBodyRecord.size() ));
	std::map<int, BodyRecord>::iterator itb;
	for ( itb = tempBodyRecord.begin(); itb != tempBodyRecord.end(); itb++)
	    {
            logPrintD("Framenumber of Datagramm is: %d\n", frameNumber);
            logPrintD("Number of  Tracked Bodies for 6d: %d\n", numberTrackedBodies);
		//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Framenumber of Datagramm is: %d\n"), frameNumber));
		//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Number of  Tracked Bodies for 6d: %d\n"), numberTrackedBodies));
		if ( itb->second.valid == true )
		    {
                logPrintD("tempBodyRecord[%d].id  %d\n", itb->first, itb->second.id);
                logPrintD("tempBodyRecord[%d].quality %f\n", itb->first, itb->second.quality);
			//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].id  %d\n"), itb->first, itb->second.id));
			//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].quality %f\n"), itb->first, itb->second.quality));
			int j;
			for(j=0; j < 3; j++)
			    {
                    logPrintD("tempBodyRecord[%d].location[%d]: \n",itb->first, j, itb->second.location[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].location[%d]: \n"), 
					   //itb->first, j, itb->second.location[j]));
			    }
			for(j=0; j < 3; j++)
			    {
                    logPrintD("tempBodyRecord[%d].eulerAngles[%d]: \n", itb->first, j, itb->second.eulerAngles[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].eulerAngles[%d]: \n"), 
					   //itb->first, j, itb->second.eulerAngles[j]));
			    }
			for(j=0; j < 9; j++)
			    {
                    logPrintD("ot:tempBodyRecord[%d].rotationMatrix[%d]: \n", itb->first, j, itb->second.rotationMatrix[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].rotationMatrix[%d]: \n"), 
					   //itb->first, j, itb->second.rotationMatrix[j]));
			    }
		    }// END if
		else
		    {
                logPrintD("#### No Valid DATA for this Body ####\n");
	    		//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:#### No Valid DATA for this Body ####")));
		    }// END else
	    }

	std::map<int, MarkerRecord>::iterator itm;
	for(itm = tempMarkerRecord.begin(); itm != tempMarkerRecord.end(); itm++)
	    {
            logPrintD("Number of Markers for 3d: %d\n", numberTrackedMarkers);
		//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Number of Markers for 3d: %d\n"), numberTrackedMarkers));

		if ( itm->second.valid == true )
		    {

                logPrintD("tempMarkerRecord[%d].id  %d\n", itm->first, itm->second.id);
                logPrintD("tempMarkerRecord[%d].quality %f\n", itm->first, itm->second.quality);
			//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:tempMarkerRecord[%d].id %d\n"), itm->first, itm->second.id));
			//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:tempMarkerRecord[%d].quality %d\n"), itm->first, itm->second.quality));
			
			for(int j=0; j < 3; j++)
			    {
                    logPrintD("tempBodyRecord[%d].location[%d]: \n",itm->first, j, itm->second.location[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].location[%d]: \n"), itm->first, j, itm->second.location[j]));
			    }
		    }
		else
		    {
                logPrintD("#### No Valid DATA for this Marker ####\n");
			//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:#### No Valid DATA for this Marker ####")));
		    }
	    }

	std::map<int, FlystickRecord>::iterator itf;
	for ( itf = tempFlystickRecord.begin(); itf != tempFlystickRecord.end(); itf++)
	    {
            logPrintD("Framenumber of Datagramm is: %d\n", frameNumber);
            logPrintD("Number of Tracked Flysticks: %d\n", numberTrackedFlysticks);
		//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Framenumber of Datagramm is: %d\n"), frameNumber));
		//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Number of Tracked Flysticks: %d\n"), numberTrackedFlysticks));
		if ( itf->second.valid == true )
		    {
                logPrintD("tempFlystickRecord[%d].id  %d\n", itf->first, itf->second.id);
                logPrintD("tempFlystickRecord[%d].quality %f\n", itf->first, itf->second.quality);
                logPrintD("tempFlystickRecord[%d].buttons %d\n", itf->first, itf->second.buttons);
			//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempFlystickRecord[%d].id  %d\n"), itf->first, itf->second.id));
			//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempFlystickRecord[%d].quality %f\n"), itf->first, itf->second.quality));
			//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempFlystickRecord[%d].buttons %d\n"), itf->first, itf->second.buttons));
			int j;
			for(j=0; j < 3; j++)
			    {
                    logPrintD("tempFlystickRecord[%d].location[%d]: \n",itf->first, j, itf->second.location[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempFlystickRecord[%d].location[%d]: \n"), 
					   //itf->first, j, itf->second.location[j]));
			    }
			for(j=0; j < 3; j++)
			    {
                    logPrintD("tempFlystickRecord[%d].eulerAngles[%d]: \n",itf->first, j, itf->second.eulerAngles[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempFlystickRecord[%d].eulerAngles[%d]: \n"), 
					   //itf->first, j, itf->second.eulerAngles[j]));
			    }
			for(j=0; j < 9; j++)
			    {
                    logPrintD("tempFlystickRecord[%d].rotationMatrix[%d]: \n",itf->first, j, itf->second.rotationMatrix[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempFlystickRecord[%d].rotationMatrix[%d]: \n"), 
					   //itf->first, j, itf->second.rotationMatrix[j]));
			    }
		    }// END if
		else
		    {
                logPrintD("#### No Valid DATA for this Flystick ####\n");
			//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:#### No Valid DATA for this Flystick ####")));
		    }// END else
	    }

	std::map<int, MeasuretargetRecord>::iterator itmt;
	for ( itmt = tempMeasuretargetRecord.begin(); itmt != tempMeasuretargetRecord.end(); itmt++)
	    {
            logPrintD("Framenumber of Datagramm is: %d\n", frameNumber);
            logPrintD("Number of Tracked Measurement Targets: %d\n", numberTrackedMeasuretargets);
		//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Framenumber of Datagramm is: %d\n"), frameNumber));
		//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Number of Tracked Measurement Targets: %d\n"), numberTrackedMeasuretargets));
		if ( itmt->second.valid == true )
		    {
                logPrintD("tempMeasureTargetRecord[%d].id  %d\n", itmt->first, itmt->second.id);
                logPrintD("tempMeasureTargetRecord[%d].quality %f\n", itmt->first, itmt->second.quality);
                logPrintD("tempMeasureTargetRecord[%d].buttons %d\n", itmt->first, itmt->second.buttons);
			//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempMeasureTargetRecord[%d].id  %d\n"), itmt->first, itmt->second.id));
			//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempMeasureTargetRecord[%d].quality %f\n"), itmt->first, itmt->second.quality));
			//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempMeasureTargetRecord[%d].buttons %d\n"), itmt->first, itmt->second.buttons));
			int j;
			for(j=0; j < 3; j++)
			    {
                    logPrintD("tempMeasureTargetRecord[%d].location[%d]: \n",itmt->first, j, itmt->second.location[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempMeasureTargetRecord[%d].location[%d]: \n"), 
				//	   itmt->first, j, itmt->second.location[j]));
			    }
			for(j=0; j < 9; j++)
			    {
                    logPrintD("tempMeasureTargetRecord[%d].rotationMatrix[%d]: \n",itmt->first, j, itmt->second.rotationMatrix[j]);
				//ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempMeasureTargetRecord[%d].rotationMatrix[%d]: \n"), 
				//	   itmt->first, j, itmt->second.rotationMatrix[j]));
			    }
		    }// END if
		else
		    {
                logPrintD("#### No Valid DATA for this Measurement Target ####\n");
			//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:#### No Valid DATA for this Measurement Target ####")));
		    }// END else
	    }


	if(numberTrackedCalBodies != 0)
	    {
            logPrintD("Number of calibrated Bodies: %d\n", numberTrackedCalBodies);
		//ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Number of calibrated Bodies: %d\n"), numberTrackedCalBodies));
	    }

	//assert(0);
	//assert(tempBodyRecord.size() != 0);

    }

    int ARTDataTrackerChomp::getFrameNumber()
    {
	return frameNumber;
    }

    int ARTDataTrackerChomp::getTrackedBodyNumber()
    {
	return numberTrackedBodies;
    }

    std::map<int, ARTDataTrackerChomp::BodyRecord > & ARTDataTrackerChomp::getBodyRecord()
    {
	return tempBodyRecord;
    }

    int ARTDataTrackerChomp::getTrackedMarkerNumber()
    {
	return numberTrackedMarkers;
    }

    std::map<int, ARTDataTrackerChomp::MarkerRecord > &ARTDataTrackerChomp::getMarkerRecord()
    {
	return tempMarkerRecord;
    }

    int ARTDataTrackerChomp::getTrackedFlystickNumber()
    {
	return numberTrackedFlysticks;
    }

    std::map<int, ARTDataTrackerChomp::FlystickRecord > &ARTDataTrackerChomp::getFlystickRecord()
    {
	return tempFlystickRecord;
    }

    int ARTDataTrackerChomp::getTrackedMeasuretargetNumber()
    {
	return numberTrackedMeasuretargets;
    }

    std::map<int, ARTDataTrackerChomp::MeasuretargetRecord > &ARTDataTrackerChomp::getMeasuretargetRecord()
    {
	return tempMeasuretargetRecord;
    }

    int ARTDataTrackerChomp::getCalibratedTrackedBodyNumber()
    {
	return numberTrackedCalBodies;
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_ARTDATATRACKER_SUPPORT")
#endif // OT_NO_ARTDATATRACKER_SUPPORT


/* ===========================================================================
   End of ARTDataTrackerChomp.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'subeventment-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'eventment 'c-lineup-runin-eventments)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
