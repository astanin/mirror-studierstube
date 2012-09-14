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

#include <stb/components/starlight/SoFaderVec3fEngine.h>


SO_ENGINE_SOURCE(SoFaderVec3fEngine);

void
SoFaderVec3fEngine::initClass()
{
    SO_ENGINE_INIT_CLASS(SoFaderVec3fEngine, SoEngine, "Engine");
}

SoFaderVec3fEngine::SoFaderVec3fEngine()
{
    SO_ENGINE_CONSTRUCTOR(SoFaderVec3fEngine);

    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, EASE );
    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, LOGARITHMIC );
    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, PULSE );

    // **********   Field Additions
    SO_ENGINE_ADD_INPUT(signal, (FALSE));
    SO_ENGINE_ADD_INPUT(fireOn, (TRUE));
    SO_ENGINE_ADD_INPUT(ease, (1));
    SO_ENGINE_ADD_INPUT(duration, (1));
    SO_ENGINE_ADD_INPUT(interpolate0, (0,0,0));
    SO_ENGINE_ADD_INPUT(interpolate1, (1,1,1));
    SO_ENGINE_ADD_INPUT(in, (0,0,0));
    SO_ENGINE_ADD_OUTPUT(out, SoSFVec3f);
    SO_ENGINE_SET_SF_ENUM_TYPE(style, Styles);
    SO_ENGINE_ADD_INPUT(style, (EASE));


    // **********   Private Engine Initializations
    conditional=new SoConditionalTrigger;
    oneshot=new SoOneShot;
    easein=new SoEaseIn;
    interpolatefloat=new SoInterpolateVec3f;

    conditional->boolIn.setValue(false);
    conditional->triggerBool=fireOn.getValue();
    conditional->comparison=SoConditionalTrigger::EQUAL;
    oneshot->trigger.connectFrom(&conditional->trigger);
    oneshot->duration=1;
    oneshot->flags=(SoOneShot::RETRIGGERABLE | SoOneShot::HOLD_FINAL);
    oneshot->disable=false;
    easein->ease=1;
    easein->in.connectFrom(&oneshot->ramp);
    interpolatefloat->alpha.connectFrom(&easein->out);
    interpolatefloat->input0.setValue(0,0,0);
    interpolatefloat->input1.setValue(0,0,0);
    in.connectFrom(&interpolatefloat->output);

    signalSensor=new SoFieldSensor(SoFaderVec3fEngine::refreshCB, this);
    signalSensor->attach(&this->signal);
    fireOnSensor=new SoFieldSensor(SoFaderVec3fEngine::fieldsCB, this);
    fireOnSensor->attach(&this->fireOn);
    interpolate0Sensor=new SoFieldSensor(SoFaderVec3fEngine::fieldsCB, this);
    interpolate0Sensor->attach(&this->interpolate0);
    interpolate1Sensor=new SoFieldSensor(SoFaderVec3fEngine::fieldsCB, this);
    interpolate1Sensor->attach(&this->interpolate1);
}

SoFaderVec3fEngine::~SoFaderVec3fEngine()
{
    delete [] signalSensor;
}

void SoFaderVec3fEngine::refreshCB(void *data, SoSensor * /*sensor*/)
{
    SoFaderVec3fEngine *self= (SoFaderVec3fEngine *)data;
    self->updateEngines();
}

void SoFaderVec3fEngine::fieldsCB(void *data, SoSensor * /*sensor*/)
{
    SoFaderVec3fEngine *self= (SoFaderVec3fEngine *)data;
    self->interpolatefloat->input0.setValue(self->interpolate0.getValue());
    self->interpolatefloat->input1.setValue(self->interpolate1.getValue());
    self->conditional->triggerBool=self->fireOn.getValue();
}

void SoFaderVec3fEngine::updateEngines()
{
    interpolatefloat->input0.setValue(interpolate0.getValue());
    interpolatefloat->input1.setValue(interpolate1.getValue());
    oneshot->duration.setValue(duration.getValue());
    easein->ease.setValue(ease.getValue());
    easein->style.setValue(style.getValue());
    conditional->boolIn.setValue(signal.getValue());
}

void SoFaderVec3fEngine::evaluate()
{
	SO_ENGINE_OUTPUT(out, SoSFVec3f, setValue(in.getValue()) );
}


