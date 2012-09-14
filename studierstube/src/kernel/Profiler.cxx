/* ========================================================================
 * Copyright (C) 2004-2005  Graz University of Technology
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
 * PROJECT: 
 * ======================================================================== */
/** @author   Daniel Wagner
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */


#include <stb/kernel/Profiler.h>

BEGIN_NAMESPACE_STB


Profiler* Profiler::instance = NULL;


Measurement::Measurement(Profiler* prof) : profiler(prof)
{
	isMeasuring = false;
#ifdef STB_IS_WINDOWS
	secBegin.QuadPart = secEnd.QuadPart = sum.QuadPart = 0;
#endif
}


void
Measurement::reset()
{
	isMeasuring = false;
#ifdef STB_IS_WINDOWS
	sum.QuadPart = 0;
#endif
}


void
Measurement::beginSection()
{
	isMeasuring = true;
#ifdef STB_IS_WINDOWS
	QueryPerformanceCounter(&secBegin);
#endif
}


void
Measurement::endSection()
{
	isMeasuring = false;
#ifdef STB_IS_WINDOWS
	QueryPerformanceCounter(&secEnd);
	sum.QuadPart += secEnd.QuadPart - secBegin.QuadPart;
#endif
}


double
Measurement::getDuration() const
{
#ifdef STB_IS_WINDOWS
	LARGE_INTEGER freq;

	QueryPerformanceFrequency(&freq);

	long double ld_sum = (long double)sum.QuadPart,
				ld_freq = (long double)freq.QuadPart;

	long double dt = ld_sum/ld_freq;

	return (double)dt;
#endif

	return 0.0;
}



Profiler::Profiler()
{
	measurements.push_back(OT_VIDEO_FEED = new Measurement(this));
	measurements.push_back(COIN_ACTUAL_REDRAW = new Measurement(this));
	measurements.push_back(VIDEO_BACKGROUND = new Measurement(this));
	measurements.push_back(OPEN_VIDEO = new Measurement(this));
}


Profiler*
Profiler::getInstance()
{
	if(instance == NULL)
		instance = new Profiler();

	return instance;
}


void
Profiler::reset()
{
	for(size_t i=0; i<measurements.size(); i++)
		measurements[i]->reset();
}


void
Profiler::createReport(std::string& report, int results_divisor)
{
	char str[256];

	sprintf(str, "OV: %.2fms  OTVF: %.2fms  VB: %.2fms  CAR: %.2fms",
			1000.0f*OPEN_VIDEO->getDuration()/results_divisor,
			1000.0f*OT_VIDEO_FEED->getDuration()/results_divisor,
			1000.0f*VIDEO_BACKGROUND->getDuration()/results_divisor,
			1000.0f*COIN_ACTUAL_REDRAW->getDuration()/results_divisor);

	report = str;
}



END_NAMESPACE_STB
