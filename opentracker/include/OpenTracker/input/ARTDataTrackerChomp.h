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
/** header file for ARTDataTrackerChomp class.
 *
 * @author Christopher Schmidt
 *
 * $Id: ARTDataTrackerChomp.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _ARTDATATRACKERCHOMP_H
#define _ARTDATATRACKERCHOMP_H

#include <string>
#include <map>
#include "../OpenTracker.h"



/**
 * Class chomp the received ASCII String (frame) 
 * and brings the Data in two structured Arrays
 * One for the bodies and one for the markers
 * @author Christopher Schmidt
 * @ingroup input
 *
 * @note Documentation of the ART Data tracker format:
 @verbatim
 ASCII Format des A.R.T. Trackers

 Normal 3 Zeilen  mit aktivierter Zustatzfunktion 4 Zeilen

 1. Zeile:		Framenummer

 fr			21753

 2. Zeile:		6D-Werte, ANzahl der Bodies, Bodies

 6d			1 [0 1.000] [326.848 -187.216 109.503 -160.4704 -3.6963 -7.0913]
 [-0.940508 -0.332545 0.333588 -0.932566 0.137735 -0.064467 0.123194 0.990286]
			
 1
 gibt die Anzahl der Bodies die folgen

 [0 1.000]
 1. Zahl --> Index beginennd bei 0
 2. Zahl --> Qualitaet

 [326.848 -187.216 109.503 -160.4704 -3.6963 -7.0913]
 1. Zahl --> Ort sx
 2. Zahl --> Ort sy
 3. Zahl --> Ort sz
 4. Zahl --> Winkel n
 5. Zahl --> Winkel o
 6. Zahl --> Winkel p
			
 [-0.940508 -0.332545 0.333588 -0.932566 0.137735 -0.064467 0.123194 0.990286]
 1. Zahl --> Rotationsmatrix Element b0
 2. Zahl --> Rotationsmatrix Element b1
 3. Zahl --> Rotationsmatrix Element b2
 4. Zahl --> Rotationsmatrix Element b3
 5. Zahl --> Rotationsmatrix Element b4
 6. Zahl --> Rotationsmatrix Element b5
 7. Zahl --> Rotationsmatrix Element b6
 8. Zahl --> Rotationsmatrix Element b7
 9. Zahl --> Rotationsmatrix Element b8

 3. Zeile		3D-Werte, Anzahl der Marker, Marker

 3d			2 [79 1.000] [210.123 -90.123 -108.123] [80 1.000] [-90.123 -50.123 108.123]

 [0 1.000]
 1. Zahl --> Index beginennd bei 0
 2. Zahl --> Qualitaet

 [210.123 -90.123 -108.123]
 1. Zahl --> Ort sx
 2. Zahl --> Ort sy
 3. Zahl --> Ort sz

 Zusatzzeile am Ende des Pakets gibt Anzahl der einkalibrierten 6D Bosies an

 6dcal			3
 @endverbatim
*/


#ifndef OT_NO_ARTDATATRACKER_SUPPORT


namespace ot {

    class ARTDataTrackerChomp
    {	
	
    public:
	
	typedef struct BodyRecord		// Structur for the 6d Bodies
	{
	    unsigned long id;			// Body ID taken from the Datagramm
	    float quality;				// Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
	    float location[3];			// Array for the loaction of the Body (s0 s1 s2)
	    float eulerAngles[3];		// Array for the Eulerangles
	    float rotationMatrix[9];	        // Array for the Rotation Matrix
	    float orientation[4];		// Array for the Quaternion (calculated by ARTDataTrackerModule) pushed in the Nodetree
	    bool  valid;			// Flag is true if body is tracked by DTrack
	} BodyRecord;
	
	typedef struct MarkerRecord             // Structur for the 3d Markers
	{
	    unsigned long id;			// Marker ID taken from the Datagramm
	    float quality;			// Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
	    float location[3];			// Array for the loaction of the Body (s0 s1 s2)		
	    bool  valid;			// Flag is true if body is tracked by DTrack

	} MarkerRecord;

	typedef struct FlystickRecord
	{
	    unsigned long id;
	    float quality;
	    unsigned long buttons;
	    float location[3];
	    float eulerAngles[3];	         // Array for the Eulerangles
	    float rotationMatrix[9];	         // Array for the Rotation Matrix
	    float orientation[4];		 // Array for the Quaternion (calculated by ARTDataTrackerModule) pushed in the Nodetree
	    bool  valid;		         // Flag is true if body is tracked by DTrack
	} FlystickRecord;

	typedef struct MeasuretargetRecord
	{	    
	    unsigned long id;
	    float quality;
	    unsigned long buttons;
	    float location[3];
	    float rotationMatrix[9];	         // Array for the Rotation Matrix
	    float orientation[4];		// Array for the Quaternion (calculated by ARTDataTrackerModule) pushed in the Nodetree
	    bool  valid;			// Flag is true if body is tracked by DTrack
	} MeasuretargetRecord;

    protected:
	int frameNumber;
	int numberTrackedBodies;
	int numberTrackedMarkers;
	int numberTrackedFlysticks;
	int numberTrackedMeasuretargets;
	int numberTrackedCalBodies;

	std::map<int, BodyRecord > tempBodyRecord;
	std::map<int, MarkerRecord > tempMarkerRecord;
	std::map<int, MeasuretargetRecord > tempMeasuretargetRecord;
	std::map<int, FlystickRecord > tempFlystickRecord;

	/* Methods of ARTDataTrackerChomp
	 *
	 */


    public:
	ARTDataTrackerChomp();
	virtual ~ARTDataTrackerChomp();
	virtual void displayRecords();                                 // Display the contens of the RecordArray
	virtual void chomp(std::string Datagramm);                     // Main part that chomp the string
	virtual int getFrameNumber();                                  // return the framenumber
	virtual int getTrackedBodyNumber();                            // return the Number of tracked Bodies
	virtual int getCalibratedTrackedBodyNumber();		       // return the Number of calibrated & tracked Bodies	
	virtual std::map<int, BodyRecord > & getBodyRecord();	       // return the BodyRecord
	virtual int getTrackedMarkerNumber();			       // return the Number of tracked Markers	
	virtual std::map<int, MarkerRecord > & getMarkerRecord();      // return the MarkerRecord
	virtual int getTrackedFlystickNumber();			             // return the Number of tracked flysticks
	virtual std::map<int, FlystickRecord > & getFlystickRecord();        // return the FlystickRecord
	virtual int getTrackedMeasuretargetNumber();		              // return the Number of tracked measurement targets
	virtual std::map<int, MeasuretargetRecord > & getMeasuretargetRecord();  // return the MeasuretargetRecord
    };

} // namespace ot


#endif // OT_NO_ARTDATATRACKER_SUPPORT


#endif

/* ===========================================================================
   End of ARTDataTrackerChomp.h
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
