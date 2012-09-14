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
/** Source file for the EventProducer, EventSubscriber, EventFilter
  * and EventBus classes
  * 
  * @author Gerhard Reitmayr
  *
  * $Id: EventBus.cpp 52 2006-02-02 20:14:26Z bara $
  * @file                                                                   */
 /* ======================================================================= */

// #include "../tool/disable4786.h"
#include <algorithm>

#include <stb/components/event/EventBus.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/SceneManager.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/components/event/EventContextFilter.h>
#include <stb/components/event/event.h>
#include <Inventor/fields/SoMFName.h>

#include <iostream>

EventBus *EventBus::singleton = NULL;

EventBus & EventBus::getSingleton(void)
{
  using namespace std;
  if(singleton==NULL)
    {
		stb::logPrintD("creating new Singleton\n");
		singleton = new EventBus();
    }    
  return *singleton;
}

EventBus::EventBus(void)
{
}

EventBus::~EventBus(void)
{
}

void EventBus::addSubscriber( EventSubscriber * subscriber , EventFilter * filter)
{
    std::map<EventSubscriber *, EventFilter *>::value_type temp(subscriber, filter);
    subscriptions.insert( temp );
}

void EventBus::removeSubscriber( EventSubscriber * subscriber )
{
    subscriptions.erase( subscriber );
}


class Sender
{
public:
    SoInputEvent * event;
    stb::Event * eventComponent;
    BasicEventFilter wildcardFilter;
    Sender(SoInputEvent * myEvent, stb::Event * myEventComponent) : event(myEvent), eventComponent(myEventComponent)
    {
        for( int i = 0; i < eventComponent->getKey()->getNum(); i++ )
            wildcardFilter.setPredicate( (*eventComponent->getKey())[i], (*eventComponent->getValue())[i]);
    };

    void operator()(const std::map<EventSubscriber *, EventFilter *>::value_type & pair)
    {

		/// Verifies that there is at least one filter
        if( pair.second )
        {    
            // Check if it is paused
            if (!eventComponent->isPaused())
            {
                // If is not continue normally
                /// Queries if the event matches the filter
                if( pair.second->accept( event )){
                    /// Notifies the subscriber about this new event
                    pair.first->newEvent( event );
                }
            }
            else
            {
                // If it is paused the check if the wildcard is the one being passed around
                if (wildcardFilter.accept(event))
                {
                    /// Queries if the event matches the filter
                    if( pair.second->accept( event )){
                        /// Notifies the subscriber about this new event
                        pair.first->newEvent( event );
                    }
                }
            }
        }
    }
};

void EventBus::publish( SoInputEvent * event )
{
	stb::Event* eventInstance=stb::Event::instance;
    Sender sender( event, eventInstance );
    /// Try to notify all the subscribers
    
    std::for_each( subscriptions.begin(), subscriptions.end(), sender );
}

