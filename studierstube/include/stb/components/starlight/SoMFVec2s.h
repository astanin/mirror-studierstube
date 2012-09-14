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
/** The header file for the SoLineStringKit.
*
* @author Erick Mendez
* @ingroup starlight
*
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOMFVEC2S_H_
#define _SOMFVEC2S_H_

//#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec2s.h>
#include "starlight.h"

class STARLIGHT_API SoMFVec2s : public SoMField 
{
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFVec2s, SbVec2s, const SbVec2s &);

  //SO_MFIELD_SETVALUESPOINTER_HEADER(SbVec2s);
  //SO_MFIELD_SETVALUESPOINTER_HEADER(short);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const short xy[][2]);
  void set1Value(const int idx, const short x, const short y);
  void set1Value(const int idx, const short xy[2]);
  void setValue(const short x, const short y);
  void setValue(const short xy[2]);
};

#endif // _SOMFVEC2S_H_
