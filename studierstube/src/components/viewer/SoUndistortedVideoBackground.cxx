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
* $Id: SoVideoBackground.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/SoUndistortedVideoBackground.h>
#include <stb/components/viewer/SoVideoBackground.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/Profiler.h>
#include <stb/components/video/Video.h>
#include <stb/base/OS.h>

#include <stb/components/viewer/SoOffAxisCamera.h>
#include <Inventor/SbLinear.h>

#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/kernel/SceneManager.h>

BEGIN_NAMESPACE_STB

SO_NODE_SOURCE(SoUndistortedVideoBackground);

inline int round(float nVal)
{
	if(nVal<0.0f)
		return (int)(nVal-0.5f);
	else
		return (int)(nVal+0.5f);
}


void
SoUndistortedVideoBackground::initClass()
{ 
   SO_NODE_INIT_CLASS(SoUndistortedVideoBackground, SoVideoBackground, "VideoBackground");
}

struct CameraDistortion{
	float cc[2]; 
	float fc[2]; 
	float kc[6]; 
}; 

SoUndistortedVideoBackground::SoUndistortedVideoBackground(){
	SO_NODE_CONSTRUCTOR(SoUndistortedVideoBackground);

    SO_NODE_ADD_FIELD(sinkName, (""));
    SO_NODE_ADD_FIELD(calibFile, (""));
	SO_NODE_ADD_FIELD(xResolution, (10)); 
	SO_NODE_ADD_FIELD(yResolution, (10)); 
	SO_NODE_ADD_FIELD(cameraName, (""));

	camDist = NULL; 
	vertices = NULL; 
	texCoords = NULL; 
	this->init = true; 
}

void 
SoUndistortedVideoBackground::setOffAxisCamera(){
	//retrieve SoOffAxisCamera by name
	SoNode* node = dynamic_cast<SoNode*>(SoNode::getByName(this->cameraName.getValue()));
	if(node != NULL){
		if(SoOffAxisCamera::getClassTypeId() == node->getTypeId()){
			stb::logPrintD("SoUndistortedVideoBackground: SoOffAxisCamera '%s' found\n", 
				       cameraName.getValue().getString());
			SoOffAxisCamera* camera = dynamic_cast<SoOffAxisCamera*>(node); 
			SbVec3f position; 
			SbVec2f scale; 
			SbVec3f eyepointPosition; 

			//create intrinsics matrix (image plane --> pixel coordinates)
			SbMatrix k(camDist->fc[0], 0.0, 0.0, camDist->cc[0], 
				0.0, camDist->fc[1], 0.0, (this->ysize - camDist->cc[1]), 
				0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0); 
			//pixel --> image plane coordinates
			SbMatrix invK = k.inverse(); 
			//camera plane borders in pixel coordinates
			SbMatrix corner(0, (float)this->xsize, 0, (float)this->xsize, 
				0, 0, (float)this->ysize, (float)this->ysize, 
				0, 0, 0, 0, 1, 1, 1, 1); 
			//camera plane borders in image plane coordinates
			SbMatrix res = invK.multRight(corner); 
			//camera plane center (image plane coordinates)
			float centerX = 0.0f, centerY = 0.0f; 
			//camera plane size (image plane coordinates)
			float sizeX = res[0][1], sizeY = res[1][2]; 
			for(int i = 0; i < 4; i++){
				centerX += res[0][i]; 
				centerY += res[1][i]; 
			}
			centerX /= 4.0f; 
			centerY /= 4.0f; 
			sizeX -= centerX; 
			sizeY -= centerY; 

			//set eyepoint to origin
			eyepointPosition.setValue(0.0, 0.0, 0.0); 
			//set camera plane center to z=-1 and calculated image plane center
			position.setValue(centerX, centerY, -1.0f); 
			//set camera plane size
			scale.setValue(sizeX * 2.0f, sizeY * 2.0f); 

			//set given SoOffAxisCamera
			camera->eyepointPosition.setValue(eyepointPosition); 
			camera->position.setValue(position); 
			camera->size.setValue(scale); 

		}
	}
	else{
	    stb::logPrintD("SoUndistortedVideoBackground: SoOffAxisCamera '%s' not found\n", 
			   cameraName.getValue().getString());
	}
}

void 
SoUndistortedVideoBackground::observ2Ideal(float ox, float oy, float *ix, float *iy){
	//set outgoing pixels to incoming, if no iterations are required
	if(undist_iterations <= 0)
	{
		*ix = ox;
		*iy = oy;
	}
	else
	{
		//normalize pixel coordinates (consider principal point and focal length)
		//(this seems to be equal to xd = inv(K) * [ox, oy]', but not considering 
		//the skew factor) 
		const float xd[2] = {(ox - camDist->cc[0]) / camDist->fc[0], 
							(oy - camDist->cc[1]) / camDist->fc[1] };
		//save distortion parameters to other variables (??)
		const float k1 = camDist->kc[0];
		const float k2 = camDist->kc[1];
		const float k3 = camDist->kc[4];
		const float p1 = camDist->kc[2];
		const float p2 = camDist->kc[3];

		//save xd to other coordinates
		float x[2] = { xd[0], xd[1] };
		//for the number of iterations - which iterations for christ's sake?!
		for(int kk=0; kk<undist_iterations; kk++)
		{
			//square coordinates
			const float x0_sq = (x[0]*x[0]);
			const float x1_sq = (x[1]*x[1]);
			const float x0_x1 = (x[0]*x[1]);
			//calculate squared radius
			const float r_2 = x0_sq + x1_sq;
			//radius^4
			//const float r_2_sq = (r_2 * r_2);
			//radial distortion factor:  k_radial =  1 + k1 * r_2 + k2 * r_2.^2 + k3 * r_2.^3;
			//const float k_radial =  1 + k1 * r_2 + k2 * (r_2_sq) + k3 * (r_2*r_2_sq);
			const float k_radial =  1 + k1 * r_2 + k2 * (pow(r_2, 2)) + k3 * (pow(r_2, 3));
			//tangential distortion factor: 
			//delta_x = [	2*p1*x(1,:).*x(2,:) + p2*(r_2 + 2*x(1,:).^2)	;
			//				p1 * (r_2 + 2*x(2,:).^2)+2*p2*x(1,:).*x(2,:)	];
			const float delta_x[2] = {   2*p1*x0_x1 + p2*(r_2 + 2*x0_sq),
				p1 * (r_2 + 2*x1_sq) + 2*p2*x0_x1   };
			//undistort radial distortion
			x[0] = xd[0] - delta_x[0];
			x[1] = xd[1] - delta_x[1];
			//undistort tangential distortion
			x[0] /= k_radial;
			x[1] /= k_radial;
		}

		//back to (undistorted) pixel coordinates (without radial distortion)
		*ix = (x[0] * camDist->fc[0]) + camDist->cc[0];
		*iy = (x[1] * camDist->fc[1]) + camDist->cc[1];
	}
}

bool
SoUndistortedVideoBackground::calcDistortionParameters(){
#if defined(HAVE_OPENVIDEO)
	//create new camera distortion struct
	camDist = new CameraDistortion; 

	//get calibration file name
	SbString calibFileName = this->calibFile.getValue(); 
	stb::logPrintD("Reading calibration file '%s'\n", calibFileName.getString());

	//open calibration file
	FILE *fp = fopen(calibFileName.getString(), "r" );
	if( fp == NULL ){ 
		logPrintE("problem reading calibration file: '%s' \n", calibFileName.getString());
		delete this->camDist; 
		this->camDist = NULL; 
		return false;
	}

	//read header line
	int n;
	char _s[32];
	sprintf(_s,"%c%i%c%c%c",'%',(int)strlen(CAMERA_ADV_HEADER),'s','\\','n');
	char hdr[2048];
	n=fscanf(fp, _s, hdr);
	if(strstr(hdr,CAMERA_ADV_HEADER) == NULL){ 
		return false;
	}

	//read file
	n=fscanf(fp, "%d%d%f%f%f%f%f%f%f%f%f%f%d\n",
		&this->xsize, &this->ysize, &camDist->cc[0],&camDist->cc[1],
		&camDist->fc[0],&camDist->fc[1],
		&camDist->kc[0],&camDist->kc[1],
		&camDist->kc[2],&camDist->kc[3],
		&camDist->kc[4],&camDist->kc[5],
		&undist_iterations); 

	//error reading file
	if((n!=13) || ferror(fp))
	{
		logPrintE("incorrect calibration parameters in: %s \n", calibFileName.getString());
		delete this->camDist; 
		this->camDist = NULL; 
		return(false);
	}

	//close file
	fclose(fp);

	return true; 
#endif 
	return false; 
}

void
SoUndistortedVideoBackground::setPolygonPoint(int vertexIndex, int texIndex, int cornerNum, 
											  int vertexX, int vertexY, int vertexZ, 
											  float texX, float texY){
	//convert incoming pixel to ideal pixel coordinates
	float xp, yp; 
	this->observ2Ideal((float)vertexX, (float)(this->ysize - vertexY), &xp, &yp); 

	//set ideal vertex positions 
	int vi = vertexIndex + (3 * cornerNum); 
	vertices[vi + 0] = xp; 
	vertices[vi + 1] = (this->ysize - yp); 
	vertices[vi + 2] = (float)vertexZ; 

	//set texture coordinates
	int ti = texIndex + (2 * cornerNum); 
	texCoords[ti + 0] = texX; 
	texCoords[ti + 1] = texY; 
}

void 
SoUndistortedVideoBackground::createPolygon(){
	//this ends the initialization procedure
	this->init = false; 

	//set number of polygons per image width / height
	int xRes = this->xResolution.getValue(); 
	int yRes = this->yResolution.getValue(); 

	//if 1 polygon (or less) was given, undistortion is unnecessary
	//--> standard video background will be used instead
	if(xRes <= 1 && yRes <= 1){ 
		stb::logPrintD("SoUndistortedVideoBackground: Mesh (%d x %d) too small \
			-- using standard video background\n", xRes, yRes);
		return;
	}

	//clamp resolution parameter to reasonable size
	if(xRes > this->xsize / 10) xRes = this->xsize / 10; 
	if(yRes > this->ysize / 10) yRes = this->ysize / 10; 

	stb::logPrintD("Creating Video Background Polygon Mesh (%d x %d)\n", 
		xRes, yRes);

	//create vertex / tex coordinate arrays
	this->numPolygons = xRes * yRes; 
	int numCorners = this->numPolygons * 4;  
	this->vertices = new float[3 * numCorners]; 
	this->texCoords = new float[2 * numCorners]; 

	//set step size per vertex / tex coordinate
	int xPixelStep = this->xsize / xRes; 
	int yPixelStep = this->ysize / yRes; 

	float xTexCoordStep = (texInfo->u1 - texInfo->u0) / xRes; 
	float yTexCoordStep = (texInfo->v1 - texInfo->v0) / yRes; 

	int vi = 0, ti = 0; 
	float texXMin, texYMin, texXMax, texYMax; 
	int ix0, iy0, ix1, iy1; 


	int z = 0; 
	for(int y = 0; y < yRes; y++){
		for(int x = 0; x < xRes; x++){
			//pixel coordinates of quad
			ix0 = x * xPixelStep; 
			iy0 = y * yPixelStep; 
			ix1 = (x + 1) * xPixelStep; 
			iy1 = (y + 1) * yPixelStep; 

			//texture coordinates of quad
			texXMin = texInfo->u0 + (x * xTexCoordStep); 
			texYMin = texInfo->v0 + (y * yTexCoordStep); 
			texXMax = texInfo->u0 + ((x + 1) * xTexCoordStep); 
			texYMax = texInfo->v0 + ((y + 1) * yTexCoordStep); 

			//set the actual corner points
			this->setPolygonPoint(vi, ti, 0, ix0, iy0, z, texXMin, texYMin); 
			this->setPolygonPoint(vi, ti, 1, ix1, iy0, z, texXMax, texYMin); 
			this->setPolygonPoint(vi, ti, 2, ix1, iy1, z, texXMax, texYMax); 
			this->setPolygonPoint(vi, ti, 3, ix0, iy1, z, texXMin, texYMax); 

			//increment vertex and texture coordinate counter
			vi += 12;
			ti += 8;
		}
	}
}

void
SoUndistortedVideoBackground::drawTexture()
{
	
#if defined(HAVE_OPENVIDEO)
	if(!texInfo)
		return;

	/////////preGLCalls()
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, this->xsize, 0, this->ysize, 0, -2); 
	//debug: necessary to see the distortion degree on the edges
	//glOrtho(-(this->xsize / 4), this->xsize + (this->xsize / 4), 
	//	-(this->ysize / 4), this->ysize + (this->ysize / 4), 0, -2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#ifdef _IS_KLIMTES_
	// glPushAttrib() is not available in OpenGLES
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
#else
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
#endif
	///////////////////////    

	glColor3f(1.0f, 1.0f, 1.0f);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);

	// redraw texture
	glBindTexture(GL_TEXTURE_2D,  texInfo->texID);

	//call vertex and texture coordinate arrays
	if(this->vertices != NULL){
		glEnableClientState(GL_VERTEX_ARRAY); 
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glTexCoordPointer(2, GL_FLOAT, 0, this->texCoords);
		glVertexPointer(3, GL_FLOAT, 0, this->vertices); 
		glDrawArrays(GL_QUADS, 0, (4 * this->numPolygons)); 

		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
		glDisableClientState(GL_VERTEX_ARRAY); 
	}
	else if(this->texInfo != NULL){
		//if vertex array was not found but texture parameters are available, 
		//draw standard video background
		glBegin(GL_QUADS);
			glTexCoord2f(texInfo->u0,texInfo->v0); glVertex3f(0.0f, 0.0f,  0.0f);
			glTexCoord2f(texInfo->u1,texInfo->v0); glVertex3f((float)this->xsize, 0.0f,  0.0f);
			glTexCoord2f(texInfo->u1,texInfo->v1); glVertex3f((float)this->xsize, (float)this->ysize,  0.0f);
			glTexCoord2f(texInfo->u0,texInfo->v1); glVertex3f(0.0f, (float)this->ysize,  0.0f);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);

	///postGLCalls()
#ifdef _IS_KLIMTES_
	// glPopAttrib() is not available in OpenGLES
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
#else
	glPopAttrib();
#endif
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
#else
	glClear(GL_COLOR_BUFFER_BIT);
#endif // defined(HAVE_OPENVIDEO)
}

void
SoUndistortedVideoBackground::vu_init(const openvideo::Buffer& /*frame*/, stb::string *givenSinkName)
{
}

void
SoUndistortedVideoBackground::vu_update(const openvideo::Buffer& frame, stb::string *givenSinkName, bool forceUpdate)
{
#ifdef HAVE_OPENVIDEO
    // If this is not our sink then return
    if ((!sinkName.getValue().getLength()==0)&sinkName.getValue().compareSubString(givenSinkName->c_str())) return;

	//init camera distortion
	if(!camDist){
		if(!calcDistortionParameters())
			return; 
	}
	//if texture parameters are available (in second run...)
	if(texInfo != NULL && vertices == NULL && init){
		this->createPolygon(); 
		this->setOffAxisCamera(); 
	}

#ifdef _IS_KLIMTES_
	if (bufferSynchronizer == NULL)
		updateTexture(frame);
	else
		bufferSynchronizer->assign(const_cast<openvideo::Buffer*>(&frame));
#else
	updateTexture(frame);
#endif
#endif 
}

END_NAMESPACE_STB
