/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
//  PROJECT:    Studierstube
//  CONTENT:    various math convenience functions
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 16:59:18  gh      last modification
// ===========================================================================


#ifndef _MATH2_H_
#define _MATH2_H_

#include <math.h>

//--integer functions

inline int m2abs(const int a)
{
    return (a>0) ? a : -a;
}

inline int m2mod(const int a, const int b)
{
    return (a>=0) ? (a%b) : (b-((-a)%b));
}

inline int m2sgn(const int a)
{
  return (a>0) ? 1 : ((a<0) ? -1 : 0);
}

inline float m2fsgn(const float a)
{
  return float((a>0.0) ? 1.0 : ((a<0.0) ? -1.0 : 0.0));
}

inline int m2max(const int a, const int b)
{
    return (a>b) ? a : b;
}


inline int m2min(const int a, const int b)
{
    return (a<b) ? a : b;
}

//--float functions

inline float m2fmax(const float a, const float b)
{
    return (a>b) ? a : b;
}


inline float m2fmin(const float a, const float b)
{
    return (a<b) ? a : b;
}

inline float m2fmod(const float a, const float b)
{
    return float((a>=0) ? fmod(a,b) : b-fmod(-a,b));
}

inline float clamp(float x,const float lower,const float upper) 
{
    return (x>upper) ? upper : ((x<lower) ? lower : x);
}

#endif//_MATH2_H_
