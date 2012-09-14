/* ========================================================================
 * Copyright (C) 2004-2006  Graz University of Technology
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
 * Graz University of Technology, Institut für Maschinelles Sehen und Darstellen,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

//
// Optimized Converter from YUV2 to RGBA and Luminance
// Written by Alexander Borni based on  
//  Daniel Wagner's Y12 to RGB565 converter
// For questions send a mail to: bornik@icg.tu-graz.ac.at
//
// Modified by Bernhard Reitinger in order to convert YV12 to RGBA32
//
// Highly optimized C++ version. Uses look-up
// tables for almost everything; thereby requires
// no multiplication or if operations for doing
// format conversion including saturation checks.
//
// Memory usage: allocates 3249 bytes for lookup tables.
//
// Should run pretty fast on any device. Intel IPP
// probably includes a much faster version using WirelessMXX
// (would only work on Intel XScale processors).
//
//

#ifndef _CONVERTERYUY2_H_
#define _CONVERTERYUY2_H_

#include <openvideo/Converter.h>

namespace avm {
    class IVideoDecoder;
    class BitmapInfo;
    class CImage;
}

namespace openvideo {
    
    class ConverterYUY2 : public Converter
    {
    public:
        ConverterYUY2();
	virtual void init();
        virtual void deinit();

	void convertToRGB32(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstRGB32, bool nSwizzle34, int nCropX=0, int nCropY=0);
	void convertToRGB24(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstRGB24, bool nSwizzle34, int nCropX=0, int nCropY=0);

	void convertToLum(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstLum, bool nSwizzle34, int nCropX=0, int nCropY=0);

    protected:
        avm::IVideoDecoder *ivdec;
        avm::BitmapInfo *bihin;
        avm::BitmapInfo *bihout;
        //avm::CImage *outimg;
    };


}  // namespace openvideo

#endif // _CONVERTERYUY2_H_

//========================================================================
// End of $FILENAME$
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
