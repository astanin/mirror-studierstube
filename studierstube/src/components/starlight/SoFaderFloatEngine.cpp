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

#include <stb/components/starlight/SoFaderFloatEngine.h>


SO_ENGINE_SOURCE(SoFaderFloatEngine);

void
SoFaderFloatEngine::initClass()
{
    SO_ENGINE_INIT_CLASS(SoFaderFloatEngine, SoEngine, "Engine");
}

SoFaderFloatEngine::SoFaderFloatEngine()
{
    SO_ENGINE_CONSTRUCTOR(SoFaderFloatEngine);

    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, EASE );
    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, LOGARITHMIC );
    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, PULSE );

    // **********   Field Additions
    SO_ENGINE_ADD_INPUT(enable, (FALSE));
    SO_ENGINE_ADD_INPUT(signalForward, (FALSE));
    SO_ENGINE_ADD_INPUT(signalBackward, (FALSE));
    SO_ENGINE_ADD_INPUT(signalReset, (FALSE));
    SO_ENGINE_ADD_INPUT(triggerForward, (FALSE));
    SO_ENGINE_ADD_INPUT(triggerBackward, (FALSE));
    SO_ENGINE_ADD_INPUT(triggerReset, (FALSE));
    SO_ENGINE_ADD_INPUT(fireOn, (TRUE));
    SO_ENGINE_ADD_INPUT(ease, (1));
    SO_ENGINE_ADD_INPUT(duration, (1));
    SO_ENGINE_ADD_INPUT(interpolate0, (0));
    SO_ENGINE_ADD_INPUT(interpolate1, (1));
    SO_ENGINE_ADD_INPUT(in, (0));
    SO_ENGINE_ADD_OUTPUT(out, SoSFFloat);
    SO_ENGINE_SET_SF_ENUM_TYPE(style, Styles);
    SO_ENGINE_ADD_INPUT(style, (EASE));


    // **********   Private Engine Initializations
    conditional=new SoConditionalTrigger;
    oneshot=new SoOneShot;
    easein=new SoEaseIn;
    interpolatefloat=new SoInterpolateFloat;

    conditional->boolIn.setValue(false);
    conditional->triggerBool=fireOn.getValue();
    conditional->comparison=SoConditionalTrigger::EQUAL;
    oneshot->trigger.connectFrom(&conditional->trigger);
    oneshot->flags=(SoOneShot::RETRIGGERABLE | SoOneShot::HOLD_FINAL);
    easein->in.connectFrom(&oneshot->ramp);
    interpolatefloat->alpha.connectFrom(&easein->out);
    in.connectFrom(&interpolatefloat->output);

    signalForwardSensor=new SoFieldSensor(SoFaderFloatEngine::refreshForwardCB, this);
    signalForwardSensor->attach(&this->signalForward);

    signalBackwardSensor=new SoFieldSensor(SoFaderFloatEngine::refreshBackwardCB, this);
    signalBackwardSensor->attach(&this->signalBackward);

    signalResetSensor=new SoFieldSensor(SoFaderFloatEngine::refreshResetCB, this);
    signalResetSensor->attach(&this->signalReset);

    triggerForwardSensor=new SoFieldSensor(SoFaderFloatEngine::refreshForwardCB, this);
    triggerForwardSensor->attach(&this->triggerForward);

    triggerBackwardSensor=new SoFieldSensor(SoFaderFloatEngine::refreshBackwardCB, this);
    triggerBackwardSensor->attach(&this->triggerBackward);

    triggerResetSensor=new SoFieldSensor(SoFaderFloatEngine::refreshResetCB, this);
    triggerResetSensor->attach(&this->triggerReset);

    fireOnSensor=new SoFieldSensor(SoFaderFloatEngine::fireOnCB, this);
    fireOnSensor->attach(&this->fireOn);
}

SoFaderFloatEngine::~SoFaderFloatEngine()
{
    delete signalForwardSensor;
    delete signalBackwardSensor;
    delete signalResetSensor;
    delete triggerForwardSensor;
    delete triggerBackwardSensor;
    delete triggerResetSensor;
}

void SoFaderFloatEngine::refreshForwardCB(void *data, SoSensor * sensor)
{
    SoFaderFloatEngine *self= (SoFaderFloatEngine *)data;

    if (!self->enable.getValue()) return;

    if (self->signalForward.getValue()!=self->fireOn.getValue())
        return;

    self->interpolatefloat->input0.setValue(self->interpolate0.getValue());
    self->interpolatefloat->input1.setValue(self->interpolate1.getValue());

    self->conditional->boolIn.setValue(self->signalForward.getValue());
    self->conditional->boolIn.setValue(true);

    self->in.setValue(self->interpolate0.getValue());
    self->updateEngines();
}

void SoFaderFloatEngine::refreshBackwardCB(void *data, SoSensor * sensor)
{
    SoFaderFloatEngine *self= (SoFaderFloatEngine *)data;

    if (!self->enable.getValue()) return;

    if (self->signalBackward.getValue()!=self->fireOn.getValue())
        return;

    self->interpolatefloat->input0.setValue(self->interpolate1.getValue());
    self->interpolatefloat->input1.setValue(self->interpolate0.getValue());
    self->in.setValue(self->interpolate1.getValue());
    self->conditional->boolIn.setValue(self->signalBackward.getValue());
    self->updateEngines();
}

void SoFaderFloatEngine::refreshResetCB(void *data, SoSensor * sensor)
{
    SoFaderFloatEngine *self= (SoFaderFloatEngine *)data;

    if (!self->enable.getValue()) return;

    if (self->signalReset.getValue()!=self->fireOn.getValue())
        return;
    self->reset();
}

void SoFaderFloatEngine::fireOnCB(void *data, SoSensor * /*sensor*/)
{
    SoFaderFloatEngine *self= (SoFaderFloatEngine *)data;
    self->conditional->triggerBool=self->fireOn.getValue();
}

void SoFaderFloatEngine::reset()
{
    signalForward.setValue(FALSE);
    signalBackward.setValue(FALSE);
    signalReset.setValue(FALSE);
    conditional->boolIn.setValue(false);
    conditional->triggerBool=fireOn.getValue();
    conditional->comparison=SoConditionalTrigger::EQUAL;
    oneshot->trigger.connectFrom(&conditional->trigger);
    oneshot->flags=(SoOneShot::RETRIGGERABLE | SoOneShot::HOLD_FINAL);
    easein->in.connectFrom(&oneshot->ramp);
    interpolatefloat->alpha.connectFrom(&easein->out);
    in.setValue(interpolate0.getValue());
    touch();
    in.connectFrom(&interpolatefloat->output);
}

void SoFaderFloatEngine::updateEngines()
{
    oneshot->duration.setValue(duration.getValue());
    easein->ease.setValue(ease.getValue());
    easein->style.setValue(style.getValue());
}

void SoFaderFloatEngine::evaluate()
{
    if (!enable.getValue()) return;

    SO_ENGINE_OUTPUT(out, SoSFFloat, setValue(in.getValue()));
}
