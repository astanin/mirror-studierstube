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
// Base class for convertes
//

#ifndef _CONVERTER_H_
#define _CONVERTER_H_

namespace openvideo {
    
    class Converter
    {
    public:
	
	enum {
	    LUTCAP_MIN = -153,
	    LUTCAP_MAX = 535,
	    CHANNEL_RANGE = 256
	};

	Converter()  {  init();  }

	virtual ~Converter()  {  deinit();  }

	virtual void convertToRGB32(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstRGB32, bool nSwizzle34, int nCropX=0, int nCropY=0) = 0;
	virtual void convertToRGB24(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstRGB24, bool nSwizzle34, int nCropX=0, int nCropY=0) = 0;

	virtual void convertToLum(const unsigned char* nSrcYUV, int nWidth, int nHeight, unsigned char* nDstLum, bool nSwizzle34, int nCropX=0, int nCropY=0) = 0;

    protected:
	virtual void init();
	virtual void deinit();

	int cap(int nV)					{  /*assert(nV>=LUTCAP_MIN && nV<=LUTCAP_MAX);*/  return lutCap[nV];  }

	int getV_for_Red(int nV)		{  /*assert(nV>=0 && nV<CHANNEL_RANGE);*/  return lutV_for_Red[nV];  }
	int getU_for_Blue(int nU)		{  /*assert(nU>=0 && nU<CHANNEL_RANGE);*/  return lutU_for_Blue[nU];  }
	int getV_for_Green(int nV)		{  /*assert(nV>=0 && nV<CHANNEL_RANGE);*/  return lutV_for_Green[nV];  }
	int getU_for_Green(int nU)		{  /*assert(nU>=0 && nU<CHANNEL_RANGE);*/  return lutU_for_Green[nU];  }
	int getY(int nY)				{  /*assert(nY>=0 && nY<CHANNEL_RANGE);*/  return lutY[nY];  }

	unsigned char	*lutCap0,
	    *lutCap;

	short			*lutV_for_Red,
	    *lutU_for_Blue,
	    *lutV_for_Green,
	    *lutU_for_Green,
	    *lutY;
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
