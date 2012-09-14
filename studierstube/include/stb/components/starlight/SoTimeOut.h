//
// C++ Interface: SoTimeOut
//
// Description:
//
//
// Author: Gerhard Reitmayr <gr281@tove>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _SOTIMEOUT_H
#define _SOTIMEOUT_H

#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#include "starlight.h"

/**
@author Gerhard Reitmayr
*/
class STARLIGHT_API SoTimeOut : public SoNodeEngine
{
    SO_NODEENGINE_HEADER(SoTimeOut);

public:
    SoTimeOut();
    static void initClass(void);

    SoSFTrigger trigger;
    SoSFString  on;
    SoSFString  off;
    SoSFTime    timeout;

    SoEngineOutput out;  // SoSFString

protected:
    virtual void inputChanged(SoField * which);
    virtual void evaluate(void);

    SoAlarmSensor timeOutSensor;
    static void timeOutFired( void * data, SoSensor * sensor );
};

#endif // _TIMEOUT_H
