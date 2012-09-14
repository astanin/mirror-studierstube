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

#include <openvideo/Converter.h>
#include <memory.h>
#include <iostream>


namespace openvideo {


#define RGB888_to_RGB32(r, g, b)		( (unsigned int)( (((r&0xff))<<16) | (((g&0xff))<<8) | (((b&0xff))<<0) ) )


    void
    Converter::init()
    {
        using namespace std;
        cerr << "Converter::init()" << endl;
	int i;

	// initialize lookup table for capping values to 0..255
	// works for values [LUTCAP_MIN..LUTCAP_MAX]
	//
	lutCap0 = new unsigned char[LUTCAP_MAX-LUTCAP_MIN+1];
	lutCap = lutCap0 - LUTCAP_MIN;

	for(i=LUTCAP_MIN; i<=LUTCAP_MAX; i++)
            lutCap[i] = i>0 ? (i<256 ? i : 255) : 0;

	// initialize lookup table for multiplications
	//
	lutV_for_Red = new short[CHANNEL_RANGE];
	for(i=0; i<CHANNEL_RANGE; i++)
            lutV_for_Red[i] = static_cast<short>(1.596f*(i-128));

	lutU_for_Blue = new short[CHANNEL_RANGE];
	for(i=0; i<CHANNEL_RANGE; i++)
            lutU_for_Blue[i] = static_cast<short>(2.018f*(i-128));

	lutV_for_Green = new short[CHANNEL_RANGE];
	for(i=0; i<CHANNEL_RANGE; i++)
            lutV_for_Green[i] = static_cast<short>(-0.813f*(i-128));

	lutU_for_Green = new short[CHANNEL_RANGE];
	for(i=0; i<CHANNEL_RANGE; i++)
            lutU_for_Green[i] = static_cast<short>(-0.391f*(i-128));

	lutY = new short[CHANNEL_RANGE];
	for(i=0; i<CHANNEL_RANGE; i++)
            lutY[i] = static_cast<short>(1.164f*(i-16));
    }	


    void
    Converter::deinit()
    {
	delete lutCap0;
	delete lutV_for_Red;
	delete lutU_for_Blue;
	delete lutV_for_Green;
	delete lutU_for_Green;
	delete lutY;
    }

}  // namespace openvideo

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
