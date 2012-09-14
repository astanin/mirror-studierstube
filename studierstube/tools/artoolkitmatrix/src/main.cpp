/* ========================================================================
* PROJECT: ARToolKitPlus
* ========================================================================
* This work is based on the original ARToolKit developed by
*   Hirokazu Kato
*   Mark Billinghurst
*   HITLab, University of Washington, Seattle
* http://www.hitl.washington.edu/artoolkit/
*
* Copyright of the derived and new portions of this work
*     (C) 2006 Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact 
*   Dieter Schmalstieg
*   <schmalstieg@icg.tu-graz.ac.at>
*   Graz University of Technology, 
*   Institut for Computer Graphics and Vision,
*   Inffeldgasse 16a, 8010 Graz, Austria.
* ========================================================================
** @author   Daniel Wagner
*
* $Id$
* @file
 * ======================================================================== */


#include <ARToolKitPlus/TrackerSingleMarkerImpl.h>


class MyLogger : public ARToolKitPlus::Logger
{
    void artLog(const char* nStr)
    {
        printf(nStr);
    }
};


int main(int argc, char** argv)
{
    MyLogger      logger;

	if(argc<2)
	{
		printf("Usage:\n\n artoolkitmatrix CAMERAFILE\n");
		return -1;
	}

    // create a dummy tracker
	//
    ARToolKitPlus::TrackerSingleMarker *tracker = new ARToolKitPlus::TrackerSingleMarkerImpl<6,6,6, 1, 8>(320,240);
    tracker->setLogger(&logger);

	// load the camera file we want to get information about
	//
    if(!tracker->init(argv[1], 1.0f, 1000.0f))            // load std. ARToolKit camera file
	{
		printf("ERROR: loading camera file '%s failed\n", argv[1]);
		delete tracker;
		return -1;
	}

	ARToolKitPlus::Camera* camera = tracker->getCamera();
	assert(camera);

	printf("MATRIX:\n [ %.3f %.3f %.3f ; %.3f %.3f %.3f ; %.3f %.3f %.3f ]\n",
		camera->mat[0][0], camera->mat[0][1], camera->mat[0][2],
		camera->mat[1][0], camera->mat[1][1], camera->mat[1][2],
		camera->mat[2][0], camera->mat[2][1], camera->mat[2][2]);

	delete tracker;
	return 0;
}
