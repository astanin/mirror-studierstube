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
/** header file for Merge Tracker Node.
*
* @author Gratl/Werlberger VRVU
*
* $Id:$
* @file                                                                   */
/* ======================================================================= */

/**
* @page Nodes Node Reference
* @section TriangulateOrientation TriangulateOrientation
* TODO description
*
* inputs :
* 
* TODO
* 
* An example element using all wrapper elements looks like this :
* @verbatim
TODO example
*/

#include <OpenTracker/common/TriangulateOrientationNode.h>
#include <ace/Log_Msg.h>
#include <iostream>

namespace ot
{
	TriangulateOrientationNode::TriangulateOrientationNode() : Node()
	{
	}

	void TriangulateOrientationNode::onEventGenerated(Event &event, Node &generator)
	{	
		//std::cout << "TriangulateOrientationNode:onEventGenerated" << std::endl;
		//std::cout << " getType: " << generator.getType() << std::endl;
		//std::cout << " getName: " << generator.getName() << std::endl;

		// final location updates:
		// FIXMEE: for a first test the positions are hard-coded
		// TODO - change if-then-else to a switch statement with the help of enum definitions
		// top tracking device
		if (generator.getName().compare("top") == 0)
		{
			topState.setPosition(event.getPosition());

		}
		else if (generator.getName().compare("bLeft") == 0)
		{
			bLeftState.setPosition(event.getPosition());

		}
		else if (generator.getName().compare("bRight") == 0)
		{
			bRightState.setPosition(event.getPosition());
		}
		
		// afterwards combine the location and update the merged state
		if ((generator.getName().compare("top") == 0)| (generator.getName().compare("bLeft") == 0) |
			(generator.getName().compare("bRight") == 0))
		{
			//std::cout << " merging positions and rotations" << std::endl;
			// merge Position
			/*mergedState.position[0] = (topState.getPosition()[0] + bLeftState.getPosition()[0] + 
				bRightState.position[0]) / 3;
			mergedState.position[1] = (topState.getPosition()[1] + bLeftState.getPosition()[1] + 
				bRightState.position[1]) / 3;
			mergedState.position[2] = (topState.getPosition()[2] + bLeftState.getPosition()[2] + 
				bRightState.position[2]) / 3;
			*/
            

			
			float orientation_vector[4];
			float pos_vector[3];

			pos_vector[0] = (topState.getPosition()[0] + bRightState.getPosition()[0] + bLeftState.getPosition()[0])/3;
			pos_vector[1] = (topState.getPosition()[1] + bRightState.getPosition()[1] + bLeftState.getPosition()[1])/3;
			pos_vector[2] = (topState.getPosition()[2] + bRightState.getPosition()[2] + bLeftState.getPosition()[2])/3;

			mergedState.setPosition(pos_vector);

			//std::cout << "Position = " << mergedState.getPosition()[0] << mergedState.getPosition()[1] << mergedState.getPosition()[2] << std::endl;


			//calculate the orientation as vector (vector from the middle of the two base tags to the top)
			orientation_vector[0] = 0.0;
			orientation_vector[1] = topState.getPosition()[0] - (bRightState.getPosition()[0] + bLeftState.getPosition()[0])/2;
			orientation_vector[2] = topState.getPosition()[1] - (bRightState.getPosition()[1] + bLeftState.getPosition()[1])/2;
			orientation_vector[3] = topState.getPosition()[2] - (bRightState.getPosition()[2] + bLeftState.getPosition()[2])/2;
			
			/*
			mergedState.orientation[0] = 0.0;
			mergedState.orientation[1] = orientation_vector[0];
			mergedState.orientation[2] = orientation_vector[1];
			mergedState.orientation[3] = orientation_vector[2];
			*/

			mergedState.setOrientation(orientation_vector);

			// do update
			updateObservers( mergedState );

//
//			float orientation_vector[3];
//			float tmp[3];
//			float x_axis[3] = {1.0, 0.0, 0.0};
//			float y_axis[3] = {0.0, 1.0, 0.0};
//			float z_axis[3] = {0.0, 0.0, 1.0};
//			double angleX, angleY, angleZ;
//
//			//calculate the orientation as vector (vector from the middle of the two base tags to the top)
//			orientation_vector[0] = topState.position[0] - (bRightState.position[0] + bLeftState.position[0])/2;
//			orientation_vector[1] = topState.position[1] - (bRightState.position[1] + bLeftState.position[1])/2;
//			orientation_vector[2] = topState.position[2] - (bRightState.position[2] + bLeftState.position[2])/2;
//			
//			//calculate the three euler angles of the vector
//			tmp[0] = orientation_vector[0];
//			tmp[1] = orientation_vector[1];
//			tmp[2] = 0.0;
//
//
//			angleX = MathUtils::angle(x_axis,tmp, 3);
//			angleY = MathUtils::angle(tmp, orientation_vector, 3);
////			angleZ = MathUtils::angle(orientation_vector, z_axis, 3);
//
//			std::cout << "   x-angle: " << angleX << std::endl;
//			std::cout << "   y-angle: " << angleY << std::endl;
////			std::cout << "   z-angle: " << angleZ << std::endl;
//			
//			//convert them to a quaternion (type of orientation in OT)
//			MathUtils::eulerToQuaternion(angleX, angleY, 0.0,MathUtils::EULER::XYZ, mergedState.orientation );
//		    
//			// do update
//			updateObservers( mergedState );




		}
	}

  void TriangulateOrientationNode::pushEvent()
  {
    // nothing to do
  }

  void TriangulateOrientationNode::pullEvent()
  {
    // nothing to do
  }

} // namespace ot
