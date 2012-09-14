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
/** The Source file for the SoLineStringKit.
*
* @author Erick Mendez
*
* @file                                                                   */
/* ======================================================================= */


/*!
  \class SoMFVec2s SoMFVec2s.h SoMFVec2s.h
  \brief The SoMFVec2s class is a container for SbVec2s vectors.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store an array of vectors with two elements.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SoSFVec2s
*/

// *************************************************************************

#include <assert.h>
#include <stb/components/starlight/SoMFVec2s.h>
//#include <Inventor/fields/SoSubFieldP.h>
//#include <Inventor/SoInput.h>
//#include <Inventor/errors/SoDebugError.h>
//
//#include "shared.h"

// *************************************************************************

SO_MFIELD_SOURCE(SoMFVec2s, SbVec2s, const SbVec2s &);

//SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec2s, SbVec2s, SbVec2s);
//SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec2s, SbVec2s, short);

// *************************************************************************

// Override from parent class.
void
SoMFVec2s::initClass(void)
{
  SO_MFIELD_INIT_CLASS(SoMFVec2s, SoMField);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFVec2s::read1Value(SoInput * in, int idx)
{
  assert(idx < this->maxNum);
  return
    in->read(this->values[idx][0]) &&
    in->read(this->values[idx][1]);
}

void
SoMFVec2s::write1Value(SoOutput * out, int idx) const
{
  //sosfvec2f_write_value(out, (*this)[idx]);
  //void sosfvec2f_write_value(SoOutput * out, const SbVec2f & v)

    out->write((*this)[idx][0]);
    if (!out->isBinary()) out->write(' ');
    out->write((*this)[idx][1]);

}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set \a num vector array elements from \a xy, starting at index
  \a start.
*/
void
SoMFVec2s::setValues(const int start, const int numarg, const short xy[][2])
{
  if (start+numarg > this->maxNum) this->allocValues(start+numarg);
  else if (start+numarg > this->num) this->num = start+numarg;

  for(int i=0; i < numarg; i++) this->values[start+i] = SbVec2s(xy[i]);
  this->valueChanged();
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec2s::set1Value(const int idx, const short x, const short y)
{
  this->set1Value(idx, SbVec2s(x, y));
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec2s::set1Value(const int idx, const short xy[2])
{
  this->set1Value(idx, SbVec2s(xy));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec2s::setValue(const short x, const short y)
{
  this->setValue(SbVec2s(x, y));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec2s::setValue(const short xy[2])
{
  if (xy == NULL) this->setNum(0);
  else this->setValue(SbVec2s(xy[0], xy[1]));
}

// *************************************************************************
