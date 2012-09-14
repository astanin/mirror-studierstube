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
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The header file for the SpectecSrc class.
 *
 * @author Daniel Wagner
 * 
 * $Id:
 * @file                                                                   
 * ======================================================================= */

#ifndef _SPECTECSRC_H
#define _SPECTECSRC_H

#include <openvideo/openVideo.h>


#ifdef ENABLE_SPECTECSRC

#include <openvideo/Node.h>


namespace openvideo {

/**
*@ingroup nodes
*	SpectecSrc implements a video source using a Spectec SD Camera. 
*   Only works under Windows Mobile 2003 and 2005.
*/
class OPENVIDEO_API SpectecSrc : 
    public openvideo::Node
{
public:
    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
	SpectecSrc();

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
	virtual ~SpectecSrc();

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
	virtual void init();

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
	virtual void process( );

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
	virtual void initPixelFormats();

 protected:
	void prepareImageReading();
	bool readConfigValues(void* nHKey);
	bool readConfigFromRegistry();
	int VC_SetControlValue(int CtrlID, int Value, int ValueLen);
	int VC_GetControlValue(int CtrlID, void *Value, int ValueLen, int *RetLen);


	unsigned short *img1, *img2, *imgActive;
	int width,height;

	bool doublebuffer;
	bool didInit;

	void*	hVSD;        // the device

	// camera settings
	int     VcBacklight;
	int     VcBrightness;
	int     VcInvert;
	int     VcMirror;
	int     VcSaturation;
	int     VcSharpness;
	int     VcContrast;
	int		VcGamma;
	int		dataAvailable;
};


} //namespace openvideo


#endif  // ENABLE_SPECTECSRC

#endif  // _SPECTECSRC_H
