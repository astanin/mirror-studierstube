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
/** header file for SoEventRoot node
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoEventRoot.cpp 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#include <algorithm>

//#include <stb/components/starlight/SoContext.h>
#include <stb/components/event/SoEventRoot.h>
#include <stb/components/event/SoEventAction.h>
#include <stb/components/event/EventBus.h>
#include <stb/components/event/EventContextFilter.h>
// #include "EventContext.h"

#include <Inventor/nodes/SoGroup.h>

SO_KIT_SOURCE(SoEventRoot);

void SoEventRoot::initClass(void)
{    
    SO_KIT_INIT_CLASS(SoEventRoot, SoBaseKit, "BaseKit");
}

SoEventRoot::SoEventRoot(void) :
localAction(NULL),
adapter(NULL),
lastEvent( NULL )
{
    SO_KIT_CONSTRUCTOR(SoEventRoot);

    //SO_KIT_ADD_CATALOG_ENTRY(context, SoContext, FALSE, this, \0x, TRUE); // TODO: add context
    SO_KIT_ADD_CATALOG_ENTRY(attributes, SoGroup, FALSE, this, \0x, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(scene,SoNode,SoGroup,FALSE, this,\0x,TRUE);

    SO_KIT_INIT_INSTANCE();

    SO_KIT_ADD_FIELD( key, (""));
    SO_KIT_ADD_FIELD( value, (""));

    keySensor.setData( this );
    keySensor.setFunction( SoEventRoot::dataChanged );
    valueSensor.setData( this );
    valueSensor.setFunction( SoEventRoot::dataChanged );

    sceneSensor.setData( this );
    sceneSensor.setFunction( SoEventRoot::sceneChanged );
    sceneSensor.setPriority( 100 );

    localAction=new SoEventAction;
    
    setUpConnections( TRUE, TRUE );
}

SoEventRoot::~SoEventRoot()
{
    keySensor.detach();
    valueSensor.detach();
    sceneSensor.detach();

    if( adapter != NULL )
        delete adapter;
}

SbBool SoEventRoot::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        keySensor.attach(&key);
        valueSensor.attach(&value);
        SoEventRoot::dataChanged( this, NULL );
        sceneSensor.attach( this );
    }
    else {
        keySensor.detach();
        valueSensor.detach();
        sceneSensor.detach();
    }
    return !(connectionsSetUp = onOff);
}

void SoEventRoot::dataChanged( void * data, SoSensor * /*sensor*/ )
{
    assert( data );
    SoEventRoot * self = (SoEventRoot *) data;

    if( self->adapter != NULL )
        delete self->adapter;
    self->adapter = new EventAdapter<SoEventRoot>( self );
}

//class ContextCopy {
//public:
//    SoContext * context;
//    SbString * values;
//    SbName * indices;
//    SbBool oldNotify;
//
//    ContextCopy(SoEventRoot * myRoot, int len)
//    {
//        context = SO_GET_PART(myRoot, "context", SoContext );
//        assert( context != NULL );
//        oldNotify = context->enableNotify( FALSE );
//        context->index.setNum( len );
//        context->value.setNum( len );
//        indices = context->index.startEditing();
//        values = context->value.startEditing();
//    }
//
//    void operator()(const std::pair<SbName, SbString> & item )
//    {
//        *indices = item.first;
//        *values = item.second;
//        indices++;
//        values++;
//    }
//
//    ~ContextCopy()
//    {
//        context->index.finishEditing();
//        context->value.finishEditing();
//        context->enableNotify( TRUE );
//    }
//};

void SoEventRoot::processEvent( SoInputEvent * event )
{
    // set context attributes from event context on context node
    // TODO make sure to only set attributes, if the context has changed !
    //const NameStringMap & data = event->getContext()->getData();
    //std::for_each( data.begin(), data.end(), ContextCopy(this, data.size())); 

    lastEvent = *event;
    localAction->setEvent( &lastEvent );
    applyAction();
}

void SoEventRoot::sceneChanged( void * data, SoSensor * /*sensor*/ )
{
    assert( data );
    SoEventRoot * self = (SoEventRoot *) data;
    self->applyAction();
 }

void SoEventRoot::applyAction(void)
{
    /*
    if grabber is set
        send event to grabber
        if grabber is unset
            send event to the whole scene graph
    else
      send event to the whole scene graph
    */
    if( localAction->getGrabber() != NULL )
    {
        localAction->apply(localAction->getGrabberPath());
        if( localAction->getGrabber() == NULL )
            localAction->apply(this);
    }
    else
    {
        localAction->apply(this);
    }
}
