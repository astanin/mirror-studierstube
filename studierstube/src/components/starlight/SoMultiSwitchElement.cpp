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
/** SoMultiSwitchElement source file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoMultiSwitchElement.cpp 26 2005-02-10 19:57:28Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/components/starlight/SoMultiSwitchElement.h>
#include <stb/base/OS.h>


SO_ELEMENT_SOURCE(SoMultiSwitchElement);

// Initializes the element and sets a default value
void SoMultiSwitchElement::init(SoState *state)
{
    SoReplacedElement::init(state);
    indices.clear();
}

// Initializes the SoMultiSwitchElement class
void SoMultiSwitchElement::initClass()
{
    if( SoType::fromName("SoMultiSwitchElement").isBad())
    {
        SO_ELEMENT_INIT_CLASS(SoMultiSwitchElement, SoReplacedElement);
    }
}

void SoMultiSwitchElement::print(FILE *fp) const
{
#ifdef STB_IS_DEBUG
    SoReplacedElement::print(fp);
#endif
}

SoMultiSwitchElement::~SoMultiSwitchElement()
{
}
