/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** header file for SoEventAction node
 *
 * @author   Gerhard Reitmayr
 *
 * $Id: SoOpenTrackerSource.cpp 52 2006-02-02 20:14:26Z bara $
 * @file                                                                   */
/* ======================================================================= */

#include <algorithm>

#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <stb/kernel/StbLogger.h>

#ifndef USE_OT_2_0
#include <OpenTracker.h>
#else
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#endif //USE_OT_2_0

#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/components/event/EventModule.h>
#include <stb/components/event/EventSchema.h>


void SoOpenTrackerSource::createOTModule(ot::Context * context, void * data){
    stb::logPrintD("SoOpenTrackerSource::CreateOTModule\n");
    SoOpenTrackerSource * self = (SoOpenTrackerSource *) data;
    self->eventHandler = new EventModule(self);
    context->addModule( "EventConfig", *(self->eventHandler) );
    //   printf("calling context add fatori\n");
    context->addFactory( *(self->eventHandler) );

}

SO_NODE_SOURCE(SoOpenTrackerSource);

void SoOpenTrackerSource::initClass(void)
{
    SO_NODE_INIT_CLASS(SoOpenTrackerSource, SoEventSource, "SoEventSource");
}

SoOpenTrackerSource::SoOpenTrackerSource(void) :
    context(NULL),
    //artkpModule(NULL),
    eventHandler(NULL),
    runSensor(NULL)
{
    SO_NODE_CONSTRUCTOR(SoOpenTrackerSource);

    SO_NODE_DEFINE_ENUM_VALUE(PROCESSING, IDLE);
    SO_NODE_DEFINE_ENUM_VALUE(PROCESSING, TIME);
    SO_NODE_DEFINE_ENUM_VALUE(PROCESSING, POLL);

    SO_NODE_SET_SF_ENUM_TYPE( processing, PROCESSING );

    SO_NODE_ADD_FIELD( configuration, (""));
    SO_NODE_ADD_FIELD( processing, (TIME));
    SO_NODE_ADD_FIELD( interval, (0.01));
    SO_NODE_ADD_FIELD( shouldStop, (FALSE));

    configSensor.setFunction( SoOpenTrackerSource::configChanged );
    configSensor.setData( this );
    configSensor.attach( &configuration );

    processSensor.setFunction( SoOpenTrackerSource::processChanged );
    processSensor.setData( this );
    processSensor.attach( &processing );

    active.setValue(FALSE);

    // setup the timer sensor for running the show
    SoOpenTrackerSource::processChanged( this, NULL );

#ifdef USE_OT_2_0
    // add initialization procedure to the opentracker Configurator,
    // this allows opentracker to create the stb modules, whenever they are needed
    //    logPrintI("Adding eventmodule %p\n", createOTModule);
    ot::Configurator::addModuleInit("EventModule", createOTModule, (void *)this);

#endif //USE_OT_2_0
}

SoOpenTrackerSource::~SoOpenTrackerSource()
{
    if(context)
    {
        context->close();
#ifndef USE_OT_2_0
        delete context;
#endif //	
        active.setValue(FALSE);
        context = NULL;
        eventHandler = NULL;
    }
    if(runSensor)
    {
        runSensor->unschedule();
        delete runSensor;
        runSensor = NULL;
    }
}

void SoOpenTrackerSource::configChanged( void * data, SoSensor * )
{
    assert(data);
    SoOpenTrackerSource * self = (SoOpenTrackerSource *)data;

    if(self->context)
    {
        self->context->close();
        delete self->context;
        self->context = NULL;
        self->eventHandler = NULL;
        self->active.setValue(FALSE);
        self->shouldStop.setValue(FALSE);
    }
    SbString configFile = self->configuration.getValue();

    if(configFile.getLength() > 0)
    {
#ifdef USE_OT_2_0
	std::string configFileString = configFile.getString();
#ifndef USE_LIVE
	ot::Configurator * conf = ot::Configurator::instance();
#else
	ot::Configurator * conf = ot::Configurator::instance();
	conf->getContext()->setActivateLive(false);
	//ot::Configurator * conf = ot::Configurator::instance(ot::LIVE);
#endif
#  ifdef OT_ENABLE_RECONFIGURATION
	conf ->runConfigurationThread( configFileString.c_str() );
#  endif //OT_ENABLE_RECONFIGURATION

	conf->changeConfiguration(configFileString);

	self->context = conf->getContext();		


#else  //USE_OT_2_0
        self->context = new ot::Context(1);
	
	createOTModule((self->context),  self);

	std::string configFileString = configFile.getString();
        self->context->parseConfiguration( configFileString );


#endif // USE_OT_2_0


        self->context->start();

        self->active.setValue(TRUE);
	
        // try to find an ARToolKitPlusModule
        // FIXME: maybe OpenTracker should rather have a generic video input interface for all its nodes...
        //
        //self->artkpModule = reinterpret_cast<ot::ARToolKitPlusModule*>(self->context->getModule("ARToolKitPlusConfig"));
    }
}

void SoOpenTrackerSource::processChanged( void * data, SoSensor * )
{
    assert(data);
    SoOpenTrackerSource * self = (SoOpenTrackerSource *)data;

    if(self->runSensor)
    {
        self->runSensor->unschedule();
        delete self->runSensor;
        self->runSensor = NULL;
    }
    switch(self->processing.getValue()){
        case SoOpenTrackerSource::IDLE: 
            {
                SoIdleSensor * idle = new SoIdleSensor( SoOpenTrackerSource::runTrackerCB, self);
                idle->schedule();
                self->runSensor = idle;
            }
            break;
        case SoOpenTrackerSource::TIME:
            {
                SoTimerSensor * timer = new SoTimerSensor( SoOpenTrackerSource::runTrackerCB, self );
                timer->setInterval( self->interval.getValue());
                timer->schedule();
                self->runSensor = timer;
            }
            break;
        case SoOpenTrackerSource::POLL:
            // do nothing here, because the tracker source is polled by the application
            break;
    }
}

void SoOpenTrackerSource::runTrackerCB( void * data, SoSensor * )
{
    assert(data);
    ((SoOpenTrackerSource *)data)->runTracker();
}

void SoOpenTrackerSource::runTracker( void )
{
    //printf("SoOpenTrackerSource::runTracker\n");

    if (active.getValue())
    {
        //assert(context);
        if (context) {
			
#ifdef USE_OT_1_1
            context->pushStates();
            context->pullStates();
            int stopflag= context->stop();
#elif USE_OT_2_0


            int stopflag = context->loopOnce();
            context->consumedBroadcast();
#else
            context->pushEvents();
            context->pullEvents();
            int stopflag= context->stop();
#endif
            if(stopflag && !shouldStop.getValue()) shouldStop.setValue(TRUE);
        }
    }
}

void SoOpenTrackerSource::handleEvent(SoHandleEventAction * action)
{
    if(!active.getValue())
        return;

    assert(eventHandler);
    eventHandler->handleEvent(action);
}

void SoOpenTrackerSource::shutDown()
{
    context->close();
}

struct Setter {
public:
    SoInputEvent & event;
    Setter( SoInputEvent & ev_ ) : event( ev_ ) {};
    void operator()(const std::pair<const SbName, SbString> & pair)
    {
        event.set(pair.first, pair.second);
    };
};

#ifdef USE_OT_1_1
void SoOpenTrackerSource::processEvent( const ot::State * state, const NameStringMap * attributes )
#else
    void SoOpenTrackerSource::processEvent( const ot::Event * state, const NameStringMap * attributes )
#endif
{
    SoInputEvent event(this);

#ifdef USE_OT_1_1
    // copy the state to the event using the EventSchema
    EventSchema schema(event);
    schema.position( state->position );
    schema.orientation( state->orientation );
    schema.confidence( state->confidence );
    schema.time( state->time / 1000.0 );
    for( unsigned int i = 0, j = 1; i < 16; i++, j *= 2 )
        schema.button( i, !!(state->button & j) );
#else
    // copy the OpenTracker event to the Inventor event using the EventSchema
    EventSchema schema(event);
    int attrcount = state->getSize();
    std::string attrname;
    for (int i = 0; i< attrcount; i++){
        attrname = state->getAttributeName(i);
        if (attrname == "position" )
            schema.position( state->getPosition() );
        else if (attrname == "orientation"  )
            schema.orientation( state->getOrientation() );
        else if (attrname == "confidence" )
            schema.confidence( state->getConfidence() );
        else if (attrname == "button")
            for( unsigned int i = 0, j = 1; i < 16; i++, j *= 2 )
                schema.button( i, !!(state->getButton() & j) );	
        else {
            // some multimodal attribute, which has to be added somehow to stb event
            schema.multimodal(attrname, state);
			
        }
    }

    schema.time( state->time );
    
#endif
    // copy attributes, if present
    if( attributes )
    {
   
        std::for_each(attributes->begin(), attributes->end(), Setter(event));
    }
    // publish
    publish( &event );
}


/*void 
  SoOpenTrackerSource::doAction(SoAction*  action) 
  {
  //printf("SoOpenTrackerSource::GLRender\n");
  runTracker();
  }*/

/* 
 * ------------------------------------------------------------
 *   End of SoOpenTrackSource.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
