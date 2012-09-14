 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  include <windows.h>
#  include <math.h>
#endif

#include <stb/components/starlight/SoEaseIn.h>


SO_ENGINE_SOURCE(SoEaseIn);

void
SoEaseIn::initClass()
{
    SO_ENGINE_INIT_CLASS(SoEaseIn, SoEngine, "Engine");
}

SoEaseIn::SoEaseIn()
{
    SO_ENGINE_CONSTRUCTOR(SoEaseIn);

    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, EASE );
    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, LOGARITHMIC );
    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, PULSE );
    SO_ENGINE_SET_SF_ENUM_TYPE(style, Styles);

    // declare fields and their default-values
    SO_ENGINE_ADD_INPUT(style, (EASE));

    SO_ENGINE_ADD_INPUT(in,             (0.0));
    SO_ENGINE_ADD_INPUT(ease,           (1.0));
    SO_ENGINE_ADD_INPUT(reverse,        (FALSE));

    SO_ENGINE_ADD_OUTPUT(out, SoSFFloat);
}

SoEaseIn::~SoEaseIn()
{
}

void SoEaseIn::evaluate()
{
    float val = in.getValue();

    if( style.getValue() & EASE )
    {
        if (ease.getValue() > 0.0) {
            val = val * (1.0f - ease.getValue()) + ease.getValue() * (float)pow((float)(sin(val/2.0f * M_PI)), 0.3f);
        }
    }
    else if( style.getValue() & LOGARITHMIC )
    {
        if (ease.getValue() > 0.0) {
            val = val * (1.0f - ease.getValue()) + ease.getValue() * (float)pow((float)(sin(val/2.0f * M_PI)), 3.0f);
        }
    }
    else if( style.getValue() & PULSE )
    {
        val = sin((1-val)* M_PI);
    }
   if (reverse.getValue()) val = 1-val;

	SO_ENGINE_OUTPUT(out, SoSFFloat, setValue(val) );
}


