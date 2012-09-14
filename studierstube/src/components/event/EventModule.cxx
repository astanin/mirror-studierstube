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
/** Source file for EventModule class.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: EventModule.cpp 52 2006-02-02 20:14:26Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#include <cstdio>
#include <iostream>
#include <algorithm>

#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <stb/components/event/EventModule.h>
#include <stb/components/event/EventSink.h>
#include <stb/components/event/EventSource.h>
#include <stb/components/event/EventKeyboardSource.h>
#include <stb/components/event/EventMouseSource.h>
#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/kernel/StbLogger.h>


using namespace std;
using namespace ot;

// maps the function names in the config file to indices
vector<string> EventModule::functionMap;

/* maps the function key names in the config file to key codes. 
  This provides a platform independend keycode map (aaarg) */
map<string,int> EventModule::keyCodeMap;

// EventModule * staticModule;

// the constructor

EventModule::EventModule( SoOpenTrackerSource * parent_ ) :
    Module(), 
    NodeFactory(), 
    keyMap(32), 
    states(11), 
    keyEvents( false ), 
    mouseEvents( true ), 
    parent(parent_)
{
    // initialize some variables
    angularSpeed = 1.0f;
    posSpeed = 0.1f;
    station = 0;
    changed = false;
    moving = false;

    // initialize function map and keycode map,
    // if no one has done it yet 
    if( functionMap.size() == 0 )
    {
        functionMap.resize( 32 );
        functionMap[MOVE_X_PLUS] = "Move_X_plus";
        functionMap[MOVE_X_MINUS] = "Move_X_minus";
        functionMap[MOVE_Y_PLUS] = "Move_Y_plus";
        functionMap[MOVE_Y_MINUS] = "Move_Y_minus";
        functionMap[MOVE_Z_PLUS] = "Move_Z_plus";
        functionMap[MOVE_Z_MINUS] = "Move_Z_minus";
        functionMap[ROT_X_PLUS] = "Rot_X_plus";
        functionMap[ROT_X_MINUS] = "Rot_X_minus";
        functionMap[ROT_Y_PLUS] = "Rot_Y_plus";
        functionMap[ROT_Y_MINUS] = "Rot_Y_minus";
        functionMap[ROT_Z_PLUS] = "Rot_Z_plus";
        functionMap[ROT_Z_MINUS] = "Rot_Z_minus";
        functionMap[ACCELL] = "Accelerate";
        functionMap[BRAKE] = "Brake";
        functionMap[BUTTON_1] = "Button_1";
        functionMap[BUTTON_2] = "Button_2";
        functionMap[BUTTON_3] = "Button_3";
        functionMap[BUTTON_4] = "Button_4";
        functionMap[STATION_0] = "Station_0";
        functionMap[STATION_1] = "Station_1";
        functionMap[STATION_2] = "Station_2";
        functionMap[STATION_3] = "Station_3";
        functionMap[STATION_4] = "Station_4";
        functionMap[STATION_5] = "Station_5";
        functionMap[STATION_6] = "Station_6";
        functionMap[STATION_7] = "Station_7";
        functionMap[STATION_8] = "Station_8";
        functionMap[STATION_9] = "Station_9";
        functionMap[RESET] = "Reset";
        functionMap[QUIT] = "Quit";
        
        keyCodeMap["down"]  = SoKeyboardEvent::DOWN_ARROW;
        keyCodeMap["up"]    = SoKeyboardEvent::UP_ARROW;
        keyCodeMap["left"]  = SoKeyboardEvent::LEFT_ARROW;
        keyCodeMap["right"] = SoKeyboardEvent::RIGHT_ARROW;
        keyCodeMap["home"]  = SoKeyboardEvent::HOME;
        keyCodeMap["backspace"] = SoKeyboardEvent::BACKSPACE;
        keyCodeMap["F0"]    = SoKeyboardEvent::F1; 
        keyCodeMap["F1"]    = SoKeyboardEvent::F1;
        keyCodeMap["F2"]    = SoKeyboardEvent::F2;
        keyCodeMap["F3"]    = SoKeyboardEvent::F3;
        keyCodeMap["F4"]    = SoKeyboardEvent::F4;
        keyCodeMap["F5"]    = SoKeyboardEvent::F5;
        keyCodeMap["F6"]    = SoKeyboardEvent::F6;
        keyCodeMap["F7"]    = SoKeyboardEvent::F7;
        keyCodeMap["F8"]    = SoKeyboardEvent::F8;
        keyCodeMap["F9"]    = SoKeyboardEvent::F9;
        keyCodeMap["F10"]   = SoKeyboardEvent::F10;
        keyCodeMap["F11"]   = SoKeyboardEvent::F11;
        keyCodeMap["F12"]   = SoKeyboardEvent::F12;
        keyCodeMap["del"]   = SoKeyboardEvent::KEY_DELETE;
        keyCodeMap["page_down"] = SoKeyboardEvent::PAGE_DOWN;
        keyCodeMap["page_up"] = SoKeyboardEvent::PAGE_UP;
        keyCodeMap["end"] = SoKeyboardEvent::END;
        keyCodeMap["insert"] = SoKeyboardEvent::INSERT;
        keyCodeMap["enter"] = SoKeyboardEvent::ENTER;
        keyCodeMap["escape"] = SoKeyboardEvent::ESCAPE;
        keyCodeMap["pad0"] = SoKeyboardEvent::PAD_0;
        keyCodeMap["pad1"] = SoKeyboardEvent::PAD_1;
        keyCodeMap["pad2"] = SoKeyboardEvent::PAD_2;
        keyCodeMap["pad3"] = SoKeyboardEvent::PAD_3;
        keyCodeMap["pad4"] = SoKeyboardEvent::PAD_4;
        keyCodeMap["pad5"] = SoKeyboardEvent::PAD_5;
        keyCodeMap["pad6"] = SoKeyboardEvent::PAD_6;
        keyCodeMap["pad7"] = SoKeyboardEvent::PAD_7;
        keyCodeMap["pad8"] = SoKeyboardEvent::PAD_8;
        keyCodeMap["pad9"] = SoKeyboardEvent::PAD_9;
        keyCodeMap["pad+"] = SoKeyboardEvent::PAD_ADD;
        keyCodeMap["pad/"] = SoKeyboardEvent::PAD_DIVIDE;
        keyCodeMap["pad*"] = SoKeyboardEvent::PAD_MULTIPLY;
        keyCodeMap["pad-"] = SoKeyboardEvent::PAD_SUBTRACT;
        keyCodeMap["padenter"] = SoKeyboardEvent::PAD_ENTER;

    }

    // initialize key map
    keyMap[MOVE_X_PLUS] = keyCodeMap["left"];
    keyMap[MOVE_X_MINUS] = keyCodeMap["right"];
    keyMap[MOVE_Y_PLUS] = keyCodeMap["up"];
    keyMap[MOVE_Y_MINUS] = keyCodeMap["down"];
    keyMap[MOVE_Z_PLUS] = keyCodeMap["page_up"];
    keyMap[MOVE_Z_MINUS] = keyCodeMap["page_down"];
    keyMap[ROT_X_PLUS] = 'e';
    keyMap[ROT_X_MINUS] = 'd';
    keyMap[ROT_Y_PLUS] = 'r';
    keyMap[ROT_Y_MINUS] = 'f';
    keyMap[ROT_Z_PLUS] = 't';
    keyMap[ROT_Z_MINUS] = 'g';
    keyMap[ACCELL] = 'c';
    keyMap[BRAKE] = 'v';
    keyMap[BUTTON_1] = ' ';
    keyMap[BUTTON_2] = ',';
    keyMap[BUTTON_3] = '.';
    keyMap[BUTTON_4] = '-';
    keyMap[STATION_0] = '0';
    keyMap[STATION_1] = '1';
    keyMap[STATION_2] = '2';
    keyMap[STATION_3] = '3';
    keyMap[STATION_4] = '4';
    keyMap[STATION_5] = '5';
    keyMap[STATION_6] = '6';
    keyMap[STATION_7] = '7';
    keyMap[STATION_8] = '8';
    keyMap[STATION_9] = '9';
    keyMap[RESET] = 'w';
    keyMap[QUIT] = 'q';

//    SoStudierstubeViewer::setExternalHandler(processSoEvent);

//    staticModule = this;
    lastTime = OSUtils::currentTime();
}

// destructor deposes of any data structures

EventModule::~EventModule()
{
    sinks.clear();
    sources.clear();
    keySources.clear();
    mouseSources.clear();
}

// initializes the tracker module
    
void EventModule::init(StringTable& attributes, ConfigNode * localTree)
{
    if( localTree != NULL )
    {
        ConfigNode * base = localTree;
        for( unsigned int i = 0; i < base->countChildren(); i++ )
        {
            ConfigNode * config = (ConfigNode *)base->getChild( i );
            if( config->getType().compare("KeyDefinition") == 0 )
            {
                string function = config->getAttributes().get("function");
                string key = config->getAttributes().get("key");
                vector<string>::iterator funcIt = find( functionMap.begin(), functionMap.end(), function );                                
                if( funcIt != functionMap.end() )
                {
                    int index = (int) (funcIt - functionMap.begin());
                    map<string,int>::iterator codeIt = keyCodeMap.find( key );
                    unsigned int code;
                    if( codeIt == keyCodeMap.end())
                    {
                        int num = sscanf( key.c_str(), " %x", &code );
                        if( num == 0 )
                        {
                            code = key[0];
                        }
                    }
                    else {
                        code = (*codeIt).second;
                    }
                    keyMap[index] = code;
                }                 
            }
        }
    }
    if( attributes.get("keyevents").compare("on") == 0 )
    {
        keyEvents = true;
    }
    if( attributes.get("mouseevents").compare("off") == 0 )
    {
        mouseEvents = false;
    }
    Module::init( attributes, localTree );
}

// creates a new StbKeyboardSource node

Node * EventModule::createNode( const string& name,  const StringTable& attributes)
{
    if( name.compare("EventKeyboardSource") == 0 )
    {
        int number;
        if( attributes.get("number", &number ) == 1 )
        {
            if( number >= 0 && number < 10 )
            {
                if( keySources.find( number ) != keySources.end()) 
                {
					stb::logPrintW("Already an StbKeyboardSource for station %d defined.\n", number);
                    return NULL;
                }
                EventKeyboardSource * source = new EventKeyboardSource( number );
                keySources[number] = source;
                return source;
            } else
            {
				stb::logPrintW("StbKeyboardSource station number not in [0,9] : %d\n", number);
            }
        }
    } 
    else if( name.compare("EventMouseSource") == 0 )
    {
        int window = -1;
        attributes.get("window", &window );
        if( mouseSources.find( window ) != mouseSources.end())
        {
			stb::logPrintW("Already an StbMouseSource for window %d defined.\n", window);
            return NULL;
        }
        EventMouseSource * source;
        if( attributes.get("mode").compare("absolute") == 0 )
            source = new EventMouseSource( EventMouseSource::StbAbsolute, window );
        else
            source = new EventMouseSource( EventMouseSource::StbRelative, window );
        mouseSources[window] = source;
        return source;
    }
    else if( name.compare("EventSink") == 0 )
    {
        logPrintI("Creating EventSink\n");
        EventSink * sink = new EventSink( this );
        sinks.push_back( sink );
        KeyIterator keyIt(attributes);
        while( keyIt.hasMoreKeys())
        {
            const string & key = keyIt.nextElement();
            if( key.compare("DEF") != 0 )
                sink->attributes[key.c_str()] = attributes.get(key).c_str();
        }
	
        return sink;
    } 
    else if( name.compare("EventSource") == 0 )
    {       
        EventSource * source = new EventSource( attributes.get("node"),
            attributes.get("position"),
            attributes.get("orientation"),
            attributes.get("button"),
            attributes.get("confidence"));
        sources.push_back( source );
        source->checkForNodes();
        return source;
    }
    return NULL;
}

// pushes new events into the graph
#ifdef USE_OT_1_1
void EventModule::pushState()
#else
void EventModule::pushEvent()
#endif
{
    float data[4], help[4];

    if( keyEvents && (keySources.find(station) != keySources.end()) )   // there is a keyboard source configured for current selected 
    {
        int sum = abs( states[X] ) + abs( states[Y] ) + abs( states[Z] ) 
            + abs( states[RX] ) + abs( states[RY] ) + abs( states[RZ] );

        if( changed == true || sum != 0 )
        {    
            if( moving == false )
                lastTime = OSUtils::currentTime();
#ifdef USE_OT_1_1
            State & state = keySources[station]->state;
#else
            Event & state = keySources[station]->state;
#endif
            double currentTime = OSUtils::currentTime();
            double diff = (currentTime - lastTime) / 1000;    
            lastTime = currentTime;

#ifdef USE_OT_1_1
            state.position[0] += (float) (states[X] * posSpeed * diff);
            state.position[1] += (float) (states[Y] * posSpeed * diff);
            state.position[2] += (float) (states[Z] * posSpeed * diff);
#else
            state.getPosition()[0] += (float) (states[X] * posSpeed * diff);
            state.getPosition()[1] += (float) (states[Y] * posSpeed * diff);
            state.getPosition()[2] += (float) (states[Z] * posSpeed * diff);
#endif
            data[0] = (float) states[RX];
            data[1] = (float) states[RY];
            data[2] = (float) states[RZ];
            data[3] = (float) (angularSpeed * diff);
            MathUtils::axisAngleToQuaternion( data, data );

#ifdef USE_OT_1_1
            memcpy(help, state.orientation, sizeof( help ));
            MathUtils::multiplyQuaternion( data, help, state.orientation );
            MathUtils::normalizeQuaternion( state.orientation );
            state.button = states[B1] | (states[B2] << 1) | (states[B3] << 2) | (states[B4] << 3);
            state.timeStamp();
            state.confidence = 1;
#else
            memcpy(help, &state.getOrientation()[0], sizeof( help ));
            MathUtils::multiplyQuaternion( data, help, &state.getOrientation()[0] );
            MathUtils::normalizeQuaternion( &state.getOrientation()[0] );
            state.setButton(states[B1] | (states[B2] << 1) | (states[B3] << 2) | (states[B4] << 3));
            state.timeStamp();
            state.setConfidence(1);
#endif

            keySources[station]->updateObservers( state );    
            changed = false;
            moving = (sum>0);
        }
    }
    if (mouseEvents)
    {
        map<int,EventMouseSource *>::iterator it;
        for( it = mouseSources.begin(); it != mouseSources.end(); it++ )
        {
            EventMouseSource * source = it->second;
            if( source->changed )
            {
                source->changed = 0;
                source->updateObservers( source->state );       
            }
        }
    }
    
    {
        vector<EventSource *>::iterator it;
        for( it = sources.begin(); it != sources.end(); it++ )
        {
            (*it)->pushState();
        }
    }
}

#ifdef USE_OT_1_1
    void EventModule::processEvent( const ot::State * event, const EventSink * sink )
#else
    void EventModule::processEvent( const ot::Event * event, const EventSink * sink )
#endif
{
    parent->processEvent( event, &sink->attributes );
}

void EventModule::handleEvent(SoHandleEventAction * action)
{
    const SoEvent * event = action->getEvent();
    if (SO_KEY_PRESS_EVENT(event, ANY) && keySources.size() > 0) 
    {
        int keysym=((SoKeyboardEvent*)event)->getKey();
        int index = (int) (find(keyMap.begin(), keyMap.end(), keysym ) - keyMap.begin());
        if( index <= QUIT )
            handleKeyDown( index );
    }
    else if(SO_KEY_RELEASE_EVENT(event, ANY) && keySources.size() > 0)
    {
        int keysym=((SoKeyboardEvent*)event)->getKey();
        int index = (int) (find(keyMap.begin(), keyMap.end(), keysym ) - keyMap.begin());
        if( index <= QUIT )
            handleKeyUp( index );
    }
    else {
        // just use the mouse position for all other events :
        map<int,EventMouseSource *>::iterator it;
        for( it = mouseSources.begin(); it != mouseSources.end(); it++ )
        {
            it->second->processEvent( *event, action->getViewportRegion());
        }
    }
}

void EventModule::handleKeyDown( int key )
{
    switch( key )
    {
        case MOVE_X_PLUS:
            states[X] = 1;
            break;
        case MOVE_X_MINUS:
            states[X] = -1;
            break;
        case MOVE_Y_PLUS:
            states[Y] = 1;
            break;
        case MOVE_Y_MINUS:
            states[Y] = -1;
            break;
        case MOVE_Z_PLUS:
            states[Z] = 1;
            break;
        case MOVE_Z_MINUS:
            states[Z] = -1;
            break;
        case ROT_X_PLUS:
            states[RX] = 1;
            break;
        case ROT_X_MINUS:
            states[RX] = -1;
            break;
        case ROT_Y_PLUS:
            states[RY] = 1;
            break; 
        case ROT_Y_MINUS:
            states[RY] = -1;
            break;
        case ROT_Z_PLUS:
            states[RZ] = 1;
            break;
        case ROT_Z_MINUS:
            states[RZ] = -1;
            break;
        case ACCELL : 
            angularSpeed += 0.01f;
            posSpeed += 0.01f;
            break;
        case BRAKE :
            angularSpeed -= 0.01f;
            if( angularSpeed < 0 ) angularSpeed = 0;
            posSpeed -= 0.01f;
            if( posSpeed < 0 ) posSpeed = 0;
            break;                        
        case BUTTON_1:
        case BUTTON_2:
        case BUTTON_3:
        case BUTTON_4:
            states[key - BUTTON_1 + B1] = 1;
            break;
        case STATION_0:
        case STATION_1:
        case STATION_2:
        case STATION_3:
        case STATION_4:
        case STATION_5:
        case STATION_6:
        case STATION_7:
        case STATION_8:
        case STATION_9:
           //if( keySources.find( key - STATION_0 ) != keySources.end())
           //{
                station = key - STATION_0;
           //}
           break;
        case RESET:
            if( keySources.find(station) != keySources.end()){
#ifdef USE_OT_1_1
                keySources[station]->state.position[0] = 0;
                keySources[station]->state.position[1] = 0;
                keySources[station]->state.position[2] = 0;
                keySources[station]->state.orientation[0] = 0;
                keySources[station]->state.orientation[1] = 0;
                keySources[station]->state.orientation[2] = 0;
                keySources[station]->state.orientation[3] = 1;
#else
	            static const float position[] = { 0, 0, 0 };
		        static const float orientation[] = { 0, 0, 0, 1 };
                keySources[station]->state.setPosition(position);
                keySources[station]->state.setOrientation(orientation);
#endif
            }
            break;
    }
    changed = true;
}

void EventModule::handleKeyUp( int key )
{
    switch( key )
    {
        case MOVE_X_PLUS:
            states[X] = 0;
            break;
        case MOVE_X_MINUS:
            states[X] = 0;
            break;
        case MOVE_Y_PLUS:
            states[Y] = 0;
            break;
        case MOVE_Y_MINUS:
            states[Y] = 0;
            break;
        case MOVE_Z_PLUS:
            states[Z] = 0;
            break;
        case MOVE_Z_MINUS:
            states[Z] = 0;
            break;
        case ROT_X_PLUS:
            states[RX] = 0;
            break;
        case ROT_X_MINUS:
            states[RX] = 0;
            break;
        case ROT_Y_PLUS:
            states[RY] = 0;
            break; 
        case ROT_Y_MINUS:
            states[RY] = 0;
            break;
        case ROT_Z_PLUS:
            states[RZ] = 0;
            break;
        case ROT_Z_MINUS:
            states[RZ] = 0;
            break;
        case BUTTON_1:
        case BUTTON_2:
        case BUTTON_3:
        case BUTTON_4:
            states[key - BUTTON_1 + B1] = 0;
            break;   
    }
    changed = true;
}
