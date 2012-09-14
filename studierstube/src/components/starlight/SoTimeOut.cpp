//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <iostream>

#include <stb/components/starlight/SoTimeOut.h>

using namespace std;

// #define TIMEOUTDEBUG

SO_NODEENGINE_SOURCE(SoTimeOut);

SoTimeOut::SoTimeOut()
{
    SO_NODEENGINE_CONSTRUCTOR(SoTimeOut);
    SO_NODE_ADD_FIELD(trigger, ());
    SO_NODE_ADD_FIELD(on, ("TRUE"));
    SO_NODE_ADD_FIELD(off, ("FALSE"));
    SO_NODE_ADD_FIELD(timeout, (1.0));

    SO_NODEENGINE_ADD_OUTPUT(out, SoSFString);

    timeOutSensor.setFunction(SoTimeOut::timeOutFired);
    timeOutSensor.setData(this);
    inputChanged(&trigger);
}

void SoTimeOut::initClass(void)
{
    if( SoType::fromName("SoTimeOut").isBad())
    {
        SO_NODEENGINE_INIT_CLASS(SoTimeOut, SoNodeEngine, "NodeEngine");
    }
}

void SoTimeOut::inputChanged( SoField * which )
{
    if( which == &trigger )
    {
        trigger.getValue();
        if(timeOutSensor.isScheduled())
            timeOutSensor.unschedule();
        else
            out.enable(TRUE);
        timeOutSensor.setTimeFromNow(timeout.getValue());
        timeOutSensor.schedule();
#ifdef TIMEOUTDEBUG
        cout << "trigger fired " << out.isEnabled() << endl;
#endif
    }
}

void SoTimeOut::evaluate(void)
{
#ifdef TIMEOUTDEBUG
    cout << "evaluate with " << timeOutSensor.isScheduled() << " and " << out.isEnabled() << endl;
#endif
    SbString temp;
    if( timeOutSensor.isScheduled())
        temp = on.getValue();
    else
        temp = off.getValue();
    SO_ENGINE_OUTPUT(out, SoSFString, setValue(temp));
    if( timeOutSensor.isScheduled())
        out.enable(FALSE);
}

void SoTimeOut::timeOutFired( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    SoTimeOut * self = (SoTimeOut *) data;

    self->out.enable(TRUE);
    self->touch();
#ifdef TIMEOUTDEBUG
    cout << "timer fired" << endl;
#endif
}
