/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
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
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube 
* ======================================================================== */
/** The header file for the SoVideoBackground class.
*
* @author Manuela Waldner
* @author Daniel Wagner
*
* $Id: SoVideoBackground.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef __SOUNDISTORTEDVIDEOBACKGROUND_H__
#define __SOUNDISTORTEDVIDEOBACKGROUND_H__

#include <stb/components/viewer/SoVideoBackground.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>

BEGIN_NAMESPACE_STB

/** Camera distortion parameters. */
struct CameraDistortion; 

#define CAMERA_ADV_HEADER "ARToolKitPlus_CamCal_Rev02"
#define CAMERA_ADV_MAX_UNDIST_ITERATIONS 20

/**
    This class is derived from SoVideoBackground and displays a full screen
    quad textured with a video image from OpenVideo. Unlike the usual video
    background, this implementation takes a calibration file, a name of an 
    SoOffAxisCamera to be modified and optional resolution parameters to 
    undistort the video image according to the given camera calibration file. 
    The same undistortion procedure as ArToolKitPlus is used so the video
    background and the displayed scene are (as far as possible) overlapping. 
    If a valid camera name was given, the parameters of the SoOffAxisCamera are 
    calculated as well, so the Matlab tool artoolkit2offaxis.m does not need
    to be run. The undistortion procedure only takes radial / tangential 
    distortion into account. 
*/
class VIEWER_API SoUndistortedVideoBackground : public SoVideoBackground
{
	SO_NODE_HEADER(SoUndistortedVideoBackground);

	public:
		/** Filename of camera calibration file. */
		SoSFString calibFile; 
		/** Number of polygons in x direction. */
		SoSFInt32 xResolution; 
		/** Number of polygons in y direction. */
		SoSFInt32 yResolution; 
		/** Name of OffAxisCamera in the scene to be used. */
		SoSFString cameraName; 

		static void initClass();
		SoUndistortedVideoBackground();
		virtual ~SoUndistortedVideoBackground(){}

		virtual void vu_init(const openvideo::Buffer& frame, stb::string *givenSinkName);
		virtual void vu_update(const openvideo::Buffer& frame, stb::string *givenSinkName, bool forceUpdate=false);

		/**
		 * Adds a vertex / texture coordinates to the vertex / texture coordinate
		 * array. Undistorts the incoming vertices before adding them to the array.
		 * @param vertexIndex Index of the first entry of this polygon in the vertex
		 * array. 
		 * @param texIndex Index of the first entry of this polygon in the texture
		 * coordinate array. 
		 * @param cornerNum The corner number of the current quad corner. 
		 * @param vertexX Vertex x coordinate (to be optimized). 
		 * @param vertexY Vertex y coordinate (to be optimized). 
		 * @param vertexZ Vertex z coordinate. 
		 * @param texX Texture x coordinate. 
		 * @param texY Texture y coordinate. 
		 */
		virtual void setPolygonPoint(int vertexIndex, int texIndex, int cornerNum, 
			int vertexX, int vertexY, int vertexZ, float texX, float texY); 
	protected:
		/** Camera distortion parameters. */
		CameraDistortion* camDist; 
		/** Number of pixels in x direction of camera image. */
		int xsize; 
		/** Number of pixels in y direction of camera image. */
		int ysize; 
		/** Number of iterations used for the undistortion. */
		int undist_iterations; 
		/** Vertex array of the background polygon. */
		float* vertices; 
		/** Texture coordinates array of background polygon. */
		float* texCoords;
		/** Number of polygons (quads) to be created. */
		int numPolygons; 
		/** Helper. */
		bool init; 

		/**
		 * Creates the background quad mesh and saves it to a vertex array. 
		 * The vertices will be modified according to the radial / tangential
		 * distortion of the used camera (according to the given calibration file. 
		 */
		virtual void createPolygon(); 
		/**
		 * Draws the background mesh full screen. 
		 * Calls the vertex / texture coordinate arrays created in 
		 * createPolygon(). 
		 * Overwritten from SoVideoBackground. 
		 */
		virtual void drawTexture();

		/**
		 * Loads the given calibration file and sets the distortion parameters. 
		 * If the calibration file was not found, nothing will be displayed
		 * and an error will be displayed. 
		 * This source code is taken from ArToolKitPlus. 
		 * @return Returns false if the calibration file was not found or could
		 * not be loaded, or if OpenVideo is not used. 
		 */
		virtual bool calcDistortionParameters(); 
		/**
		 * Converts given pixel coordinates to "ideal" pixel coordinates. 
		 * The radial distortion will be removed, other intrinsic parameters
		 * (skew factor, principal point offset) will not be considered in 
		 * order to be compatible with ArToolKitPlus. 
		 * This source code is taken from ArToolKitPlus. 
		 * @param ox Original x coordinate. 
		 * @param oy Original y coordinate. 
		 * @param ix [out] The ideal x coordinate. 
		 * @param iy [out] The ideal y coordinate. 
		 */
		virtual void observ2Ideal(float ox, float oy, float *ix, float *iy);
		/**
		 * Loads an OffAxisCamera given by its name and sets the parameters 
		 * according to the (real) camera calibration parameters. 
		 * Calculates the eye position, camera plane center position and the size
		 * of the camera plane of the virtual camera. 
		 * If no cameraName was given or the corresponding SoOffAxisCamera was
		 * not found, nothing is done. 
		 * The source is taken from matlab toolkit artoolkit2offaxis.m. 
		 */
		virtual void setOffAxisCamera(); 
};
END_NAMESPACE_STB

#endif // __SOUNDISTORTEDVIDEOBACKGROUND_H__
