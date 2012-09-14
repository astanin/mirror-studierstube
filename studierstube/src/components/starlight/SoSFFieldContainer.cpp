
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoSFFieldContainer.cxx  - implementation of class SoSFFieldContainer.
  * SoSFFieldContainer is the implementation of a SFField that allows reference to a  
  * SoFieldContainers in a field.
  *
  * @author  Thomas Psik
  *
  * $Id: SoSFFieldContainer.cpp 8 2004-07-14 12:24:04Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SoDB.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/actions/SoWriteAction.h>

#include <stb/components/starlight/SoSFFieldContainer.h>


//----------------------------------------------------------------------------

SO_SFIELD_REQUIRED_SOURCE(SoSFFieldContainer);

//----------------------------------------------------------------------------

void
SoSFFieldContainer::initClass()
{
	SO_SFIELD_INIT_CLASS(SoSFFieldContainer, SoSField);
	// coin doesnt take this 
//    SO__FIELD_INIT_CLASS(SoSFFieldContainer, "SoSFFieldContainer", SoSField);
}

//----------------------------------------------------------------------------

SoSFFieldContainer::SoSFFieldContainer()
{
    // This will prevent treating random memory as an SoFieldContainer
    value = NULL;
}

//----------------------------------------------------------------------------

SoSFFieldContainer::~SoSFFieldContainer()
{
    if (value != NULL) {
	value->removeAuditor(this, SoNotRec::FIELD);
	value->unref();
    }
}

//----------------------------------------------------------------------------

void SoSFFieldContainer::setValue(SoFieldContainer *newValue)
{
    setVal(newValue);
    valueChanged();
}

//----------------------------------------------------------------------------

int SoSFFieldContainer::operator ==(const SoSFFieldContainer &f) const
{
    return getValue() == f.getValue();
}

//----------------------------------------------------------------------------

SbBool SoSFFieldContainer::readValue(SoInput *in)
{
    SbName	name;
    SoBase	*base;

    // See if it's a null pointer
    if (in->read(name)) {
	if (name == "NULL") {
	    setVal(NULL);
	    return TRUE;
	}
	else
	    in->putBack(name.getString());
    }
    
    // Read node
    if (! SoBase::read(in, base, SoFieldContainer::getClassTypeId())) {
	setVal(NULL);
	return FALSE;
    }

    setVal((SoFieldContainer *) base);

    return TRUE;
}

//----------------------------------------------------------------------------

void SoSFFieldContainer::countWriteRefs(SoOutput *out) const
{
    // Do standard counting for field
    SoField::countWriteRefs(out);

    // Count node
    if (value != NULL)
	value->writeInstance(out);
}

//----------------------------------------------------------------------------

void SoSFFieldContainer::writeValue(SoOutput *out) const
{
    if (value != NULL)
	value->writeInstance(out);
    else
	out->write("NULL");
}

//----------------------------------------------------------------------------

void SoSFFieldContainer::setVal(SoFieldContainer *newValue)
{
    // Play it safe if we are replacing one pointer with the same pointer
    if (newValue != NULL)
	newValue->ref();

    // Get rid of old node, if any
    if (value != NULL) {
	value->removeAuditor(this, SoNotRec::FIELD);
	value->unref();
    }

    value = newValue;

    if (value != NULL) {
	value->ref();
	value->addAuditor(this, SoNotRec::FIELD);
    }

    if (newValue != NULL)
	newValue->unref();
}

//----------------------------------------------------------------------------

void SoSFFieldContainer::fixCopy(SbBool copyConnections)
{
    if (value != NULL) {
	SoFieldContainer *nodeCopy = (SoFieldContainer *)
	    SoFieldContainer::findCopy(value, copyConnections);
	if (nodeCopy != NULL)
	    setVal(nodeCopy);
    }
}

//----------------------------------------------------------------------------

SbBool SoSFFieldContainer::referencesCopy() const
{
    // Do the normal test, and also see if the stored node is a copy
    return (SoSField::referencesCopy() ||
	    (value != NULL && SoFieldContainer::checkCopy(value) != NULL));
}
