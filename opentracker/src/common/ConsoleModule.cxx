/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for ConsoleModule module.
 *
 * @author Gerhard Reitmayr, Jochen von Spiczak
 *
 * $Id: ConsoleModule.cxx 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/common/ConsoleModule.h>
#include <OpenTracker/common/ConsoleSink.h>
#include <OpenTracker/common/ConsoleSource.h>

#include <algorithm>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#ifdef WIN32
#include <Windows.h>
#endif


#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iomanip>


#ifndef OT_NO_CONSOLE_SUPPORT


#ifdef WIN32
#  include <conio.h>
#else
#  include <unistd.h>

#  if defined(USE_NCURSES) && !defined(RENAMED_NCURSES)
#     include <ncurses.h>
#  else
#     include <curses.h>
#  endif

#endif

#include <OpenTracker/core/Context.h>

namespace ot {
	
	//register the module with the context
	OT_MODULE_REGISTER_FUNC(ConsoleModule){
            OT_MODULE_REGISTRATION_DEFAULT(ConsoleModule, "ConsoleConfig");
	}

    // list of key symbols as ints to provide faster lookup

    const short ConsoleModule::MOVE_X_PLUS = 1;
    const short ConsoleModule::MOVE_X_MINUS = 2;
    const short ConsoleModule::MOVE_Y_PLUS = 3;
    const short ConsoleModule::MOVE_Y_MINUS = 4;
    const short ConsoleModule::MOVE_Z_PLUS = 5;
    const short ConsoleModule::MOVE_Z_MINUS = 6;
    const short ConsoleModule::ROT_X_PLUS = 7;
    const short ConsoleModule::ROT_X_MINUS = 8;
    const short ConsoleModule::ROT_Y_PLUS = 9;
    const short ConsoleModule::ROT_Y_MINUS = 10;
    const short ConsoleModule::ROT_Z_PLUS = 11;
    const short ConsoleModule::ROT_Z_MINUS = 12;
    const short ConsoleModule::ACCELL = 13;
    const short ConsoleModule::BRAKE = 14;
    const short ConsoleModule::BUTTON_1 = 15;
    const short ConsoleModule::BUTTON_2 = 16;
    const short ConsoleModule::BUTTON_3 = 17;
    const short ConsoleModule::BUTTON_4 = 18;
    const short ConsoleModule::STATION_0 = 20;
    const short ConsoleModule::STATION_1 = 21;
    const short ConsoleModule::STATION_2 = 22;
    const short ConsoleModule::STATION_3 = 23;
    const short ConsoleModule::STATION_4 = 24;
    const short ConsoleModule::STATION_5 = 25;
    const short ConsoleModule::STATION_6 = 26;
    const short ConsoleModule::STATION_7 = 27;
    const short ConsoleModule::STATION_8 = 28;
    const short ConsoleModule::STATION_9 = 29;
    const short ConsoleModule::RESET = 30;
    const short ConsoleModule::QUIT = 31;
    const short ConsoleModule::NEXT_ATTR = 32;
    const short ConsoleModule::ADD_ATTR = 33;
    const short ConsoleModule::DEL_ATTR = 34;
    const short ConsoleModule::CHANGE_ATTR_POS = 35;
    const short ConsoleModule::CHANGE_ATTR_NEG = 36;
    const short ConsoleModule::CHANGE_ATTR_VAL = 37;
    const short ConsoleModule::ACCEL_ANG = 38;
    const short ConsoleModule::BRAKE_ANG = 39;

    // Destructor method, this is here because curses seem to define some macro
    // which replaces clear with wclear !!!!!

    ConsoleModule::~ConsoleModule()
    {
        sinks.clear();
        sources.clear();
    }


    // constructor method.

    ConsoleModule::ConsoleModule() : Module(), NodeFactory(), sinks(), sources(), keyMap(40)
    {
        // initialize some variables
        cycle = 0;
        angularSpeed = 0.1f;
        posSpeed = 0.1f;
        station = 0;
        quit = 0;
        interval = 10;
        display = 1;
        curses = 1;

        delAttribute = false;
        addAttribute = false;
        changeAttrByVal = false;
        nameOfAttributeToBeChanged = "";
        currentChanged = false;
        stationChanged = false;

        // initialize function map and keycode map,
        // if no one has done it yet
        if( functionMap.size() == 0 )
        {
            functionMap.resize( keyMap.size() );
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
            functionMap[NEXT_ATTR] = "Next_Attribute";
            functionMap[ADD_ATTR] = "Add_Attribute";
            functionMap[DEL_ATTR] = "Del_Attribute";
            functionMap[CHANGE_ATTR_POS] = "Change_Attribute_Pos";
            functionMap[CHANGE_ATTR_NEG] = "Change_Attribute_Neg";
            functionMap[CHANGE_ATTR_VAL] = "Change_Attribute_Val";
            functionMap[ACCEL_ANG] = "Accelerate_Angular_Velocity";
            functionMap[BRAKE_ANG] = "Decelerate_Angular_Velocity";
            // set keymap, this is different for windows and unix ( curses )
            //        keyCodeMap.resize( 20 );
#ifdef WIN32
            keyCodeMap["down"]  = 0xe050;
            keyCodeMap["up"]    = 0xe048;
            keyCodeMap["left"]  = 0xe04b;
            keyCodeMap["right"] = 0xe04d;
            keyCodeMap["home"]  = 0xe047;
            keyCodeMap["backspace"] = 0x0008;
            keyCodeMap["F0"]    = 0x0000;
            keyCodeMap["F1"]    = 0xe03b;
            keyCodeMap["F2"]    = 0xe03c;
            keyCodeMap["F3"]    = 0xe03d;
            keyCodeMap["F4"]    = 0xe03e;
            keyCodeMap["F5"]    = 0xe03f;
            keyCodeMap["F6"]    = 0xe040;
            keyCodeMap["F7"]    = 0xe041;
            keyCodeMap["F8"]    = 0xe042;
            keyCodeMap["F9"]    = 0xe043;
            keyCodeMap["F10"]   = 0xe044;
            keyCodeMap["F11"]   = 0xe085;
            keyCodeMap["F12"]   = 0xe086;
            keyCodeMap["del"]   = 0xe053;
            keyCodeMap["page_down"] = 0xe051;
            keyCodeMap["page_up"] = 0xe049;
            keyCodeMap["end"] = 0xe04f;
            keyCodeMap["insert"] = 0xe052;
            keyCodeMap["enter"] = 0x000d;
            keyCodeMap["escape"] = 0x001b;

#else
            // This keycode map reflects my german sgi keyboard !!
            // Not everything makes sense, but it works :)
            keyCodeMap[std::string("down")] = 0x1;
            keyCodeMap["down"]  = KEY_DOWN;
            keyCodeMap["up"]    = KEY_UP;
            keyCodeMap["left"]  = KEY_LEFT;
            keyCodeMap["right"] = KEY_RIGHT;
            keyCodeMap["home"]  = KEY_HOME;
            keyCodeMap["backspace"] = KEY_BACKSPACE;
            keyCodeMap["F0"]    = KEY_F0;
            keyCodeMap["F1"]    = KEY_F(1);
            keyCodeMap["F2"]    = KEY_F(2);
            keyCodeMap["F3"]    = KEY_F(3);
            keyCodeMap["F4"]    = KEY_F(4);
            keyCodeMap["F5"]    = KEY_F(5);
            keyCodeMap["F6"]    = KEY_F(6);
            keyCodeMap["F7"]    = KEY_F(7);
            keyCodeMap["F8"]    = KEY_F(8);
            keyCodeMap["F9"]    = KEY_F(9);
            keyCodeMap["F10"]   = KEY_F(10);
            keyCodeMap["F11"]   = KEY_F(11);
            keyCodeMap["F12"]   = KEY_F(12);
            keyCodeMap["del"]   = KEY_DC;
            keyCodeMap["page_down"] = KEY_NPAGE;
            keyCodeMap["page_up"] = KEY_PPAGE;
            keyCodeMap["end"] = KEY_EIC;
            keyCodeMap["insert"] = KEY_IC;
            keyCodeMap["enter"] = KEY_ENTER;
            keyCodeMap["escape"] = 27;
#endif
        }

        // initialize key map
        keyMap[MOVE_X_PLUS] = keyCodeMap["up"];
        keyMap[MOVE_X_MINUS] = keyCodeMap["down"];
        keyMap[MOVE_Y_PLUS] = keyCodeMap["left"];
        keyMap[MOVE_Y_MINUS] = keyCodeMap["right"];
        keyMap[MOVE_Z_PLUS] = keyCodeMap["page_up"];
        keyMap[MOVE_Z_MINUS] = keyCodeMap["page_down"];
        keyMap[ROT_X_PLUS] = 'e';
        keyMap[ROT_X_MINUS] = 'd';
        keyMap[ROT_Y_PLUS] = 'r';
        keyMap[ROT_Y_MINUS] = 'f';
        keyMap[ROT_Z_PLUS] = 't';
        keyMap[ROT_Z_MINUS] = 'g';
        keyMap[ACCELL] = 'y';
        keyMap[BRAKE] = 'x';
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
        keyMap[NEXT_ATTR] = 'n';
        keyMap[ADD_ATTR] = 'a';
        keyMap[DEL_ATTR] = 'l';
        keyMap[CHANGE_ATTR_POS] = '*';
        keyMap[CHANGE_ATTR_NEG] = '/';
        keyMap[CHANGE_ATTR_VAL] = 'v';
        keyMap[ACCEL_ANG] = 'o';
        keyMap[BRAKE_ANG] = 'p';
    }

    // This method is called to construct a new Node.

    Node * ConsoleModule::createNode( const std::string& name, 
                                      const StringTable& attributes)
    {
        if( name.compare("ConsoleSink") == 0 )
        {
            int val = 1;
            if( attributes.get("active").compare("off") == 0)
                val = 0;
            ConsoleSink * sink = new ConsoleSink( attributes.get("comment"), val );
            if( attributes.get("toSysMouse").compare("on") == 0)
                sink->toSysMouse = 1;
            sinks.push_back( sink );
            logPrintS("Built ConsoleSink node.\n");
            return sink;
        } else if( name.compare("ConsoleSource") == 0 )
		{
            int number;
			int pen(0);
            if( attributes.get("number", &number ) == 1 )
			{
                if( number >= 0 && number < 10 )
				{
                    if ( !attributes.get("pen").empty() ) pen=1;
					ConsoleSource * source = new ConsoleSource( number, pen );
                    sources.push_back( source );
                    logPrintS("Built ConsoleSource node.\n");
                    return source;
				} else
                {
					logPrintW("ConsoleSource station number not in [0,9]: %d\n", number );
                }
			} else
                logPrintW("ConsoleSource station number not a number.\n");
		}
		return NULL;
    }

    // pushes new events into the tracker tree

    void ConsoleModule::pushEvent()
    {
        if(!isInitialized() || !curses)
        {
            return;
        }

        ConsoleSource * source;
        float data[4];

        // read all keyboard events and execute their functions
        // this may change various sources
        int key;
#ifdef WIN32
        while( _kbhit() )
        {
            key = _getch();
            if( key == 0 || key == 0xE0 )
            {
                key = 0xE0 << 8;
                key |= _getch();
            }
#else
        while( (key = getch()) != ERR )
        {
#endif
            int index = std::find(keyMap.begin(), keyMap.end(), key ) - keyMap.begin();
            switch( index )
            {
                case QUIT :
                    quit = 1;
                    break;
                case ACCELL :
                    posSpeed *= 2.0f;
                    stationChanged = true;
                    break;
                case BRAKE :
                    posSpeed /= 2.0f;
                    stationChanged = true;
                    break;
                case ACCEL_ANG :
                    angularSpeed *= 2.0f;
                    stationChanged = true;
                    break;
                case BRAKE_ANG :
                    angularSpeed /= 2.0f;
                    stationChanged = true;
                    break;
                case STATION_0 :
                    station = 0;
                    stationChanged = true;
                    break;
                case STATION_1 :
                    station = 1;
                    stationChanged = true;
                    break;
                case STATION_2 :
                    station = 2;
                    stationChanged = true;
                    break;
                case STATION_3 :
                    station = 3;
                    stationChanged = true;
                    break;
                case STATION_4 :
                    station = 4;
                    stationChanged = true;
                    break;
                case STATION_5 :
                    station = 5;
                    stationChanged = true;
                    break;
                case STATION_6 :
                    station = 6;
                    stationChanged = true;
                    break;
                case STATION_7 :
                    station = 7;
                    stationChanged = true;
                    break;
                case STATION_8 :
                    station = 8;
                    stationChanged = true;
                    break;
                case STATION_9 :
                    station = 9;
                    stationChanged = true;
                    break;
                case BUTTON_1 :
                    setButton( station , 1 );
                    break;
                case BUTTON_2 :
                    setButton( station , 2 );
                    break;
                case BUTTON_3 :
                    setButton( station , 3 );
                    break;
                case BUTTON_4 :
                    setButton( station , 4 );
                    break;
                case MOVE_X_MINUS :
                    data[0] = - posSpeed;
                    data[1] = 0;
                    data[2] = 0;
                    movePos( station, data );
                    break;
                case MOVE_X_PLUS :
                    data[0] = posSpeed;
                    data[1] = 0;
                    data[2] = 0;
                    movePos( station, data );
                    break;
                case MOVE_Y_MINUS :
                    data[0] = 0;
                    data[1] = - posSpeed;
                    data[2] = 0;
                    movePos( station, data );
                    break;
                case MOVE_Y_PLUS :
                    data[0] = 0;
                    data[1] = posSpeed;
                    data[2] = 0;
                    movePos( station, data );
                    break;
                case MOVE_Z_MINUS :
                    data[0] = 0;
                    data[1] = 0;
                    data[2] = - posSpeed;
                    movePos( station, data );
                    break;
                case MOVE_Z_PLUS :
                    data[0] = 0;
                    data[1] = 0;
                    data[2] = posSpeed;
                    movePos( station, data );
                    break;
                case ROT_X_PLUS :
                    data[0] = 1;
                    data[1] = 0;
                    data[2] = 0;
                    data[3] = angularSpeed;
                    MathUtils::axisAngleToQuaternion( data, data );
                    rotate( station, data );
                    break;
                case ROT_X_MINUS :
                    data[0] = 1;
                    data[1] = 0;
                    data[2] = 0;
                    data[3] = -angularSpeed;
                    MathUtils::axisAngleToQuaternion( data, data );
                    rotate( station, data );
                    break;
                case ROT_Y_PLUS :
                    data[0] = 0;
                    data[1] = 1;
                    data[2] = 0;
                    data[3] = angularSpeed;
                    MathUtils::axisAngleToQuaternion( data, data );
                    rotate( station, data );
                    break;
                case ROT_Y_MINUS :
                    data[0] = 0;
                    data[1] = 1;
                    data[2] = 0;
                    data[3] = -angularSpeed;
                    MathUtils::axisAngleToQuaternion( data, data );
                    rotate( station, data );
                    break;
                case ROT_Z_PLUS :
                    data[0] = 0;
                    data[1] = 0;
                    data[2] = 1;
                    data[3] = angularSpeed;
                    MathUtils::axisAngleToQuaternion( data, data );
                    rotate( station, data );
                    break;
                case ROT_Z_MINUS :
                    data[0] = 0;
                    data[1] = 0;
                    data[2] = 1;
                    data[3] = -angularSpeed;
                    MathUtils::axisAngleToQuaternion( data, data );
                    rotate( station, data );
                    break;
                case RESET :
                    reset( station );
                    break;
                case NEXT_ATTR :
                    nextAttr( station );
                    break;
                case ADD_ATTR :
                    addAttribute = true;
                    break;
                case DEL_ATTR :
                    delAttribute = true;
                    nameOfAttributeToBeChanged = ""; // because this attribute might be deleted
                    break;
                case CHANGE_ATTR_POS :
                    changeAttr( station, POS );
                    break;
                case CHANGE_ATTR_NEG :
                    changeAttr( station, NEG );
                    break;
                case CHANGE_ATTR_VAL :
                    if (nameOfAttributeToBeChanged != "")
                        changeAttrByVal = true;
                    break;
            }
            if (stationChanged)
                nameOfAttributeToBeChanged = "";
        }

        // check for changed sources and let them generate events
        for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
        {
            source = (ConsoleSource *)((Node*)*it);
            if( source->changed == 1 )
            {
                source->event.timeStamp();
                source->updateObservers( source->event );
                source->changed = 0;
            }
        }
    }

    // add attribute
    void ConsoleModule::addAttr(const int station, const std::string type, const std::string name, const std::string value) const
        {
            ConsoleSource *source;
            for( NodeVector::const_iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station && Event::knowsType(type))
                {
                    if (source->event.addAttribute(type, name, value))
                        source->changed = 1;
                    }
            }
        }
    
    // delete attribute
    void ConsoleModule::delAttr(const int station, const std::string name)
        {
            ConsoleSource *source;
            for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station)
                {
                    if (source->event.delAttribute(name))
                    {
                        source->changed = 1;
                        if (name == nameOfAttributeToBeChanged)
                            nameOfAttributeToBeChanged = "";
                    }
                }
            }
        }
    
    // switch to next attribute
    void ConsoleModule::nextAttr(const int station)
        {
            ConsoleSource *source;
            bool gotNextAttr = false;
            for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station && source->event.getSize() > 0)
                {
                    currentChanged = true;

                    // continue if next attribute was found already
                    if (gotNextAttr)
                        continue;

                    // get index
                    int index = 0;
                    if (nameOfAttributeToBeChanged != "")
                    {
                        index = source->event.getAttributeIndex(nameOfAttributeToBeChanged);
                        index += 1;
                    }

                    // get attribute name
                    if (index == source->event.getSize())
                        nameOfAttributeToBeChanged = "";
                    else
                        nameOfAttributeToBeChanged = source->event.getAttributeName(index);
                    gotNextAttr = true;
                }
            }
        }

    // change current attribute to typed in value
    void ConsoleModule::changeAttrToValue(const int station, const std::string value) const
        {
            if (nameOfAttributeToBeChanged == "")
                return;

            ConsoleSource *source;
            for( NodeVector::const_iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station)
                {
                    if (source->event.setAttribute(nameOfAttributeToBeChanged, value))
                        source->changed = 1;
                }
            }
        }

    // increase or decrease numerical attribute
    void ConsoleModule::changeAttr(const int station, const DIRECTION dir) const
        {
            if (nameOfAttributeToBeChanged == "")
                return;

            ConsoleSource * source;
            for( NodeVector::const_iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station )
                {
                    source->changed = 1;
                    double delta = (dir == POS) ? posSpeed : -posSpeed;

                    const std::type_info &type = source->event.getAttributeType(nameOfAttributeToBeChanged);
                    if (type == typeid(bool))
                    {
                        bool &value = source->event.getAttribute((bool*)NULL, nameOfAttributeToBeChanged);
                        value = !value;
                    }
#if !defined (_MSC_VER) || (_MSC_VER != 1200) // these lines will not work with MS VC6
                    else if (type == typeid(char))
                    {
                        char &c = source->event.getAttribute<char>((char*)NULL, nameOfAttributeToBeChanged);
                        c = c + 1 % 256;
                    }
#endif
                    else if (type == typeid(int))
                        source->event.getAttribute((int*)NULL, nameOfAttributeToBeChanged) += ((int)(delta * 10));
                    else if (type == typeid(short))
                        source->event.getAttribute((short*)NULL, nameOfAttributeToBeChanged) += ((int)(delta * 10));
                    else if (type == typeid(long))
                        source->event.getAttribute((long*)NULL, nameOfAttributeToBeChanged) += ((int)(delta * 10));
                    else if (type == typeid(unsigned char))
                        source->event.getAttribute((unsigned char*)NULL, nameOfAttributeToBeChanged) += ((int)(delta * 10));
                    else if (type == typeid(unsigned int))
                        source->event.getAttribute((unsigned int*)NULL, nameOfAttributeToBeChanged) += ((int)(delta * 10));
                    else if (type == typeid(unsigned short))
                        source->event.getAttribute((unsigned short*)NULL, nameOfAttributeToBeChanged) += ((int)(delta * 10));
                    else if (type == typeid(unsigned long))
                        source->event.getAttribute((unsigned long*)NULL, nameOfAttributeToBeChanged) += ((int)(delta * 10));
                    else if (type == typeid(double))
                        source->event.getAttribute((double*)NULL, nameOfAttributeToBeChanged) += delta;
                    else if (type == typeid(long double))
                        source->event.getAttribute((long double*)NULL, nameOfAttributeToBeChanged) += delta;
                    else if (type == typeid(float))
                        source->event.getAttribute((float*)NULL, nameOfAttributeToBeChanged) += (float)delta;
                    else
                        source->changed = 0;
                }
            }
        }
    
    // toggles button values on stations sources

    void ConsoleModule::setButton( int station , int button )
        {
            ConsoleSource * source;
            for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station )
                {
                    source->event.getButton() ^= ( 1 << ( button - 1 ));
                    source->changed = 1;

                }
            }
        }

    // moves stations sources by given amount

    void ConsoleModule::movePos( int station , float * data )
        {
            ConsoleSource * source;
            for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station )
                {
					if( source->penConsole )
					{
						if( data[2] > 0 )
						{ 
							setButton( station , 1 );
						}else
						{
							setButton( station , 2 );
						}
					}else
					{
						source->event.getPosition()[0] += data[0];
						source->event.getPosition()[1] += data[1];
						source->event.getPosition()[2] += data[2];
						source->changed = 1;
					}
                }
            }
        }

    // rotates stations sources by given amount

    void ConsoleModule::rotate( int station, float * data )
        {
            ConsoleSource * source;
            for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station )
                {
                    std::vector<float> help(source->event.getOrientation());
                    MathUtils::multiplyQuaternion( copyA2V(data, 4), help, source->event.getOrientation() );
                    source->changed = 1;
                }
            }
        }

    // resets a given stations sources to null position

    void ConsoleModule::reset( int station )
        {
            ConsoleSource * source;
            for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
            {
                source = (ConsoleSource *)((Node*)*it);
                if( source->number == station )
                {
                    source->event.clearAttributes();
                    source->changed = 1;
                }
            }
        }

    // pulls events out of the tracker tree

    void ConsoleModule::pullEvent()
        {
            if(isInitialized() == 0 || display == 0 || curses == 0 )
            {
                return;
            }
            cycle = (cycle+1) % interval;
            if( cycle == 0 ){
                int display = 0, count = 0;
                NodeVector::iterator it;
                for( it = sinks.begin(); it != sinks.end(); it++ )
                {
                    display |= ((ConsoleSink *)((Node*)*it))->changed;
                    ((ConsoleSink *)((Node*)*it))->changed = 0;
                    count += ((ConsoleSink *)((Node*)*it))->active;
                }

                if (display || stationChanged ||  currentChanged || delAttribute || addAttribute || changeAttrByVal)
                {
                    
                    
#if !defined (_WIN32_WCE) && !defined(USE_MSDEV_DEBUGOUTPUT)
#ifdef WIN32
					clearLastLines();
                    printf("%s\n\n", headerline.c_str());
                    if (sources.size() > 0)
                        printf( "Station: %d PosSpeed: %f RotSpeed: %f\n", station, posSpeed, angularSpeed );
#else
                    move(0, 0);
                    printw("%s\n\n", headerline.c_str());
                    if (sources.size() > 0)
                        printw("Station: %d PosSpeed: %f RotSpeed: %f\n", station, posSpeed, angularSpeed );
#endif
#endif //! _WIN32_WCE && !USE_MSDEV_DEBUGOUTPUT

                    for( it = sinks.begin(); it != sinks.end(); it++ )
                    {
                        ConsoleSink * sink = (ConsoleSink *) ((Node*)*it);
                        if( sink->active == 0 )
                            continue;
                        Event & event = sink->event;

#ifdef USE_MSDEV_DEBUGOUTPUT
                        char str[512];
                        sprintf(str, "\n%s :\n%s", sink->comment.c_str(), event.getPrintOut().c_str());
                        OutputDebugString(str);
#endif //USE_MSDEV_DEBUGOUTPUT

#if !defined (_WIN32_WCE) && !defined(USE_MSDEV_DEBUGOUTPUT)
#ifdef WIN32
						if(sink->toSysMouse)
						{
							// do sth
						}
							
                        printf("\n%s :\n", sink->comment.c_str());
                        printf(event.getPrintOut().c_str());
#else
                        printw("\n%s :\n",sink->comment.c_str());
                        printw(event.getPrintOut().c_str());
#endif
#endif //USE_MSDEV_DEBUGOUTPUT
                    }
                    stationChanged = false;
                }


                // print out attribute that is subject to change
                if (display || currentChanged || addAttribute || changeAttrByVal)
                {
                    if (nameOfAttributeToBeChanged != "")
                    {
#ifndef WIN32
                        printw("\nchange attribute: '%s' of station [%i]\n", nameOfAttributeToBeChanged.c_str(), station);
#else
                        printf("\nchange attribute: '%s' of station [%i]\n", nameOfAttributeToBeChanged.c_str(), station);
#endif
                    }
                    currentChanged = false;
                }

                //  add new attribute
                if (addAttribute)
                {
                    char type[256];
                    char name[256];
                    char value[256];

#ifndef WIN32
                    echo();
                    nodelay(stdscr, false);

                    printw("\nadd new attribute to station [%i]\n", station);
                    printw("enter type  >");
                    getnstr(type, 256);
                    printw("enter name  >");
                    getnstr(name, 256);
                    printw("enter value >");
                    getnstr(value, 256);

                    noecho();
                    nodelay(stdscr, true);
#else
                    printf("\nadd new attribute to station [%i]\n", station);
                    printf("enter type  >");
                    scanf("%s", type);
                    printf("enter name  >");
                    scanf("%s", name);
                    printf("enter value >");
                    scanf("%s", value);
#endif
                    clearLastLines(4);
                    addAttr(station,
                            std::string(type),
                            std::string(name),
                            std::string(value));
                    addAttribute = false;
                }

                // delete attribute
                else if (delAttribute)
                {
                    char name[256];

#ifndef WIN32
                    echo();
                    nodelay(stdscr, false);

                    printw("\ndelete attribute in sources of station [%i]\n", station);
                    printw("enter name >");
                    getnstr(name, 256);

                    noecho();
                    nodelay(stdscr, true);
#else
                    printf("\ndelete attribute in sources of station [%i]\n", station);
                    printf("enter name >");
                    scanf("%s", name);
#endif
                    clearLastLines(2);
                    delAttr(station, std::string(name));
                    delAttribute = false;
                }

                // change atribute by typing in new value
                else if (changeAttrByVal)
                {
                    char value[256];

#ifndef WIN32
                    echo();
                    nodelay(stdscr, false);

                    printw("enter value >");
                    getnstr(value, 256);

                    noecho();
                    nodelay(stdscr, true);
#else
                    printf("enter value >");
                    scanf("%s", value);
#endif
                    clearLastLines(1);
                    changeAttrToValue(station, std::string(value));
                    changeAttrByVal = false;
                }

                // clean up
                else
                {
#ifndef WIN32
                    printw("\n\n");
#endif
                }
            }
        }

    // initializes ConsoleModule

    void ConsoleModule::init(StringTable& attributes,  ConfigNode * localTree)
        {
            Module::init( attributes, localTree );

            if( attributes.get("interval", &interval ) == 0 ){
                interval = 10;
            }
            headerline = attributes.get("headerline");
            if( attributes.get("display").compare( "off" ) == 0 )
                display = 0;
            if( attributes.get("curses").compare( "off" ) == 0 )
                curses = 0;
            if( localTree != NULL )
            {
                ConfigNode * base = localTree;
                for( unsigned int i = 0; i < base->countChildren(); i++ )
                {
                    ConfigNode * config = (ConfigNode *)base->getChild( i );
                    if( config->getType().compare("KeyDefinition") == 0 )
                    {
                        std::string function = config->getAttributes().get("function");
                        std::string key = config->getAttributes().get("key");
                        std::vector<std::string>::iterator funcIt = 
                            std::find( functionMap.begin(), functionMap.end(), 
                                       function );
                        if( funcIt != functionMap.end() )
                        {
                            int index = funcIt - functionMap.begin();
                            std::map<std::string,int>::iterator codeIt = keyCodeMap.find( key );
                            int code;
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

#ifndef WIN32
            if( curses ) {
                initscr();
                cbreak();
                noecho();
                nonl();
                intrflush(stdscr,FALSE);
                keypad(stdscr,TRUE);
                nodelay(stdscr, TRUE);
                leaveok(stdscr, TRUE);
            }
#endif
        }

// start the module and init curses

void ConsoleModule::start()
    {}

// close ConsoleModule

 void ConsoleModule::close()
     {
#ifndef WIN32
         if( curses ) {
             endwin();
         }
#endif
     }
 
    // tests whether a key was pressed, if so it stops.
 
 int ConsoleModule::stop()
     {
         return quit;
     }
 
    // clear last lines
 void ConsoleModule::clearLastLines(int lines)
     {
#ifdef WIN32
         // clear screen as described in the Microsoft Knowledge Base:
         HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
         CONSOLE_SCREEN_BUFFER_INFO csbi;
         GetConsoleScreenBufferInfo(hndl, &csbi);
         DWORD written;
         DWORD N = csbi.dwSize.X * csbi.dwCursorPosition.Y + csbi.dwCursorPosition.X + 1;
         COORD curhome = {0, 0};
         if (lines >= 0)
             curhome.Y = csbi.dwCursorPosition.Y - lines;
         FillConsoleOutputCharacter(hndl, ' ', N, curhome, &written);
         csbi.srWindow.Bottom -= csbi.srWindow.Top;
         csbi.srWindow.Top = 0;
         SetConsoleWindowInfo(hndl, TRUE, &csbi.srWindow);
         SetConsoleCursorPosition(hndl, curhome);
#else
         if (lines < 0)
             clear();
         else
         {
             int x, y;
             getyx(stdscr, y, x);
             move(y - lines, 0);
             for (int i = 0; i < lines; i++)
                 printw("\n");
         }
#endif
     }

} // namespace ot


#endif // OT_NO_CONSOLE_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of ConsoleModule.cxx
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
