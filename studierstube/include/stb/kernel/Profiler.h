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
/** 
*
* @author Daniel Wagner
*
* $Id$
* @file                                                                   */
/* ======================================================================= */


#ifndef _STB_PROFILER_H_
#define _STB_PROFILER_H_


#include <stb/base/OS.h>
#include <stb/base/macros.h>
#include <stb/kernel/Studierstube.h>

#include <vector>


#define STB_PROFILING_ENABLED


#ifdef STB_PROFILING_ENABLED
#  define STB_PROFILER_MEASUREMENT_BEGIN(MES_NAME) stb::Profiler::getInstance()->MES_NAME->beginSection();
#  define STB_PROFILER_MEASUREMENT_END(MES_NAME) stb::Profiler::getInstance()->MES_NAME->endSection();
#  define STB_PROFILER_AUTOMEASURE(MES_NAME) AutoMeasurement AUTO_##MES_NAME(stb::Profiler::getInstance()->MES_NAME);
#else
#  define STB_PROFILER_MEASUREMENT_BEGIN(MES_NAME)
#  define STB_PROFILER_MEASUREMENT_END(MES_NAME)
#  define STB_PROFILER_AUTOMEASURE(MES_NAME)
#endif


BEGIN_NAMESPACE_STB


class Profiler;


class KERNEL_API Measurement
{
friend class Profiler;
public:
	void beginSection();
	void endSection();
	void reset();

	double getDuration() const;

protected:
#ifdef STB_IS_WINDOWS
	LARGE_INTEGER secBegin, secEnd, sum;
#endif

	Measurement(Profiler* prof);

	Profiler* profiler;
	bool      isMeasuring;
};


/// Starts measures as long as the instance exists
/**
 *  This class can be used to measure the duration of functions execution times.
 *  Make sure to not use the same Measurement instance within several functions
 *  at the same time!
 */
class KERNEL_API AutoMeasurement
{
public:
	AutoMeasurement(Measurement* m)
	{
		mesurement = m;
		mesurement->beginSection();
	}

	~AutoMeasurement()
	{
		mesurement->endSection();
		mesurement = NULL;
	}

private:
	Measurement* mesurement;
};


class KERNEL_API Profiler
{
public:
	Profiler();

	static Profiler* getInstance();

	void reset();

	void createReport(std::string& report, int results_divisor=1);


	Measurement	*OT_VIDEO_FEED,
				*COIN_ACTUAL_REDRAW,
				*VIDEO_BACKGROUND,
				*OPEN_VIDEO;

protected:

	static Profiler* instance;

	std::vector<Measurement*> measurements;
};


END_NAMESPACE_STB


#endif //_STB_PROFILER_H_
