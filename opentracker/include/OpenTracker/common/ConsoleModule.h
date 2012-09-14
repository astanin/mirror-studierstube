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
/** header file for ConsoleModule module.
 *
 * @author Gerhard Reitmayr, Jochen von Spiczak
 *
 * $Id: ConsoleModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section consolemodule ConsoleModule
 * The ConsoleModule is a basic console based input / output module. It generates
 * input events via @ref consolesource nodes based on keyboard strokes and
 * displays event data collected by @ref consolesink nodes.
 * It displays the data every interval'th cycle. On a windows system it just writes
 * to the console. On a unix system it uses curses to format the display.
 * It cannot yet read all keys, as I'm still fighting with the Windows /
 * courses API :).
 * The configuration element is called @c ConsoleConfig and has the following attributes :
 * @li @c interval 10 number of cycles to wait between displaying data.
 * @li @c headerline "" a single line used as headerline in the display.
 * @li @c display on (on|off) either on or off, flag defining whether data
 *        should be displayed at all.
 *
 * Moreover, the keys for simulating tracking events can be configured freely.
 * The module supports 10 stations, numbered from 0 to 9. The input allows
 * to move the stations position in all 3 directions and to rotate around
 * all 3 axes. The velocities are displayed on top of the screen and can
 * be changed. The button bits 0 to 3 can be set, the keys strokes toggle
 * the event of the buttons, so you don't have to keep them pressed
 * all the time. Finally a station can
 * be reset to identity. To command several stations, the active station
 * has to be switched.
 *
 * Also, events of all ConsoleSources can be changed using certain commands. Attributes
 * can be added, their value can be increased or decreased, changed by typing in a new
 * value, or attributes can be deleted again.
 *
 * The @c KeyDefinition configuration element is used to
 * set the keys assigned to each function. It has the following attributes :
 * @li @c function name of the function ( see the following table )
 * @li @c key to assign the function to
 *
 * There are three ways to specifiy a key :
 * @li use one of the predefined key names for arrow keys, function keys etc.
 * @li use a hexadecimal expression giving the keycode as returned by Windows
 *     or curses. This looks like @c 0xHHHH for some key.
 * @li directly use the character printed on the key. This does only work for
 simple characters.
 *
 * The following table lists the supported key names :
 * @verbatim
 Name            Key

 down            cursor down
 up              cursor up
 left            cursor left
 right           cursor right
 home            Pos1 ( aka Home )
 end             End
 page_down       page down
 page_up         page up
 backspace       backspace ( <- )
 F0              F0 ( if present )
 F1 .. F12       F1 througth F12
 insert          Insert
 enter           Enter or Return
 escape          Escape
 @endverbatim
 * Note that the curses definition where taken from an SGI keyboard and
 * the keycodes returned by the curses library. Only the keypad ( numpad )
 * keys work correctly, the ones in the middle are send to the shell ?!
 *
 * The following table lists all possible functions and their default keys :
 * @verbatim
 Function        		Default Key     Description
 Move_X_plus     		up              moves in direction X +
 Move_X_minus    		down            moves in X -
 Move_Y_plus     		left            moves in Y +
 Move_Y_minus    		right           moves in Y -
 Move_Z_plus     		page_up         moves in Z +
 Move_Z_minus   		page_down       moves in Z -
 Rot_X_plus     		e               rotates positive around X
 Rot_X_minus    		d               rotates negative around X
 Rot_Y_plus     		r               rotates positive around Y
 Rot_Y_minus     		f               rotates negative around Y
 Rot_Z_plus      		t               rotates positive around Z
 Rot_Z_minus     		g               rotates negative around Z
 Accelerate      		y               increases position velocity
 Brake           		x               reduces position velocity
 Accelerate_Angular_Velocity	o		increases angular velocity
 Decelerate_Angular_Velocity	p		reduces angular velocity
 Button_1        		' '             sets button bit 0
 Button_2        		,               sets button bit 1
 Button_3        		.               sets button bit 2
 Button_4        		-               sets button bit 3
 Station_0       		0               activates Station 0
 Station_1       		1               activates Station 1
 Station_2       		2               activates Station 2
 Station_3       		3               activates Station 3
 Station_4       		4               activates Station 4
 Station_5       		5               activates Station 5
 Station_6       		6               activates Station 6
 Station_7       		7               activates Station 7
 Station_8       		8               activates Station 8
 Station_9       		9               activates Station 9
 Reset           		w               resets current station
 Quit            		q               signals to quit OpenTracker
 Next_Attribute  		n               switches to next attribute to be subject to change
 Add_Attribute   		a               allows user to add a new attribute
 Del_Attribute   		l               allows user to delete an attribute
 Change_Attribute_Pos 		*		increases value of current attribute
 Change_Attribute_Neg	  	/		decreases value of current attribute
 Change_Attribute_Val  	 	v		allows user to type in new value for current attribute
 @endverbatim
 *
 *
 * An example configuration element looks like this :
 * @verbatim
 <ConsoleConfig interval="10" headerline="Tracker Test 1">
 <KeyDefinition function="Move_X_plus" key="o"/>
 </ConsoleConfig>@endverbatim
*/

#ifndef _CONSOLEMODULE_H
#define _CONSOLEMODULE_H

#include "../OpenTracker.h"



/**
 * The module and factory to drive the console output sink nodes.
 * On a windows system it uses WIN32 API calls to clear the console and reset
 * the cursor. On a unix it uses the curses library to achieve the same
 * effect.
 *
 * @ingroup common
 * @author Gerhard Reitmayr
 */


#ifndef OT_NO_CONSOLE_SUPPORT


namespace ot {

    class OPENTRACKER_API ConsoleModule: public Module, public NodeFactory
    {
        enum DIRECTION { POS = 0, NEG };
        // Members
    protected:
        /// list of ConsoleSink nodes in the tree
        NodeVector sinks;
        /// list of ConsoleSource nodes  in the tree
        NodeVector sources;
        /// current cycle count, for computing when to print out the event again
        int cycle;
        /// cycle interval to use for printing out events
        int interval;
        /// headerline in display
        std::string headerline;
        /// angular velocity and positional velocity
        float angularSpeed, posSpeed;
        /// currently active station, must be in [0-9]
        int station;
        /// should the module quit ?
        int quit;
        /// is the console module displaying values ?
        int display;
        /// should curses be initialized ?
        int curses;
        /// maps key chars to indices
        std::vector<int> keyMap;
        /// whether the user chose to delete an attribute
        bool delAttribute;
        /// whether the user chose to add a new attribute
        bool addAttribute;
        /// whether the user chose to change an attribute by typing in the new value
        bool changeAttrByVal;
        /// the name of the attribute that is shown to be subject to change currently
        std::string nameOfAttributeToBeChanged;
        /// whether the user switched attribute that is currently subject to change
        bool currentChanged;
        /// whether station number or station information changed
        bool stationChanged;

        // Methods
    protected:
        /** adds the attribute to the events of all sources belonging to @p station
         * @param station the number of the station to add attribute to
         * @param type the generic type name of the new attribute
         * @param name the name of the new attribute
         * @param value the value of the new attribute
         */
        void addAttr(const int station, const std::string type, const std::string name, const std::string value) const;
        /** deletes the attribute from all events of all sources belonging to @p station
         * @param station the number of the station to add attribute to
         * @param name the name of the attribute to be deleted
         */
        void delAttr(const int station, const std::string name);
        /** chooses the next attribute to be subject to change
         * @param station the number of the station
         */
        void nextAttr(const int station);
        /** changes the attribute that is currently subject to change to @p value
         * @param station the number of the station
         * @param value the new value
         */
        void changeAttrToValue(const int station, const std::string value) const;
        /** increases or decreases the attribute currently subject to change according to @p dir
         * @param station the number of the station
         * @param dir whether too increase or decrease the value
         */
        void changeAttr(const int station, const DIRECTION dir) const;
        /** clear the @p lines last lines of the terminal. If @lines is negative, the entire
         * screen is cleared.
         * @param lines the number of lines to be cleared above the current one
         */
        void clearLastLines(int lines = -1);
        /** sets the button bit of given button on all sources that
         * are associated with station. Changes the changed flag on
         * the sources.
         * @param station the number of the station to change
         * @param button the number of the button to set ( 1 - 4, where 1 corresponds to LSB )
         */
        void setButton( int station , int button );
        /** moves the position by the given data on all sources that
         * are associated with station. Changes the changed flag on
         * the sources.
         * @param station the number of the station to change
         * @param data array of 3 floats giving the movement vector
         */
        void movePos( int station, float * data );
        /** rotates the event by the given data on all sources that
         * are associated with station. Changes the changed flag on
         * the sources.
         * @param station the number of the station to change
         * @param data array of 4 floats giving rotational quaternion
         */
        void rotate( int station, float * data );
        /** resets the data on all sources that are associated with
         * the given station. Changes the changed flag on
         * the sources.
         * @param station the number of the station to change
         */
        void reset( int station );
    public:
        /** constructor method. initializes internal and static data
         * such as the functionMap and keyMap tables. */
        ConsoleModule();
        /** Destructor method, clears nodes member. */
        virtual ~ConsoleModule();
        /**
         * initializes the tracker module.
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes, ConfigNode * localTree);
        /** This method is called to ruct a new Node. It compares
         * name to the ConsoleSink element name, and if it matches
         * creates a new ConsoleSink node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name, const StringTable& attributes);
        /**
         * checks the console for keyboard input and updates any
         * ConsoleSource nodes accordingly. This happens every cycle
         * and all key presses recorded since are used.
         */
        virtual void pushEvent();
        /**
         * reads out the ConsoleSink nodes current event an prints it
         * to the console. This is done only each length cylce.
         */
        virtual void pullEvent();
        /**
         * On Unix platforms initializes curses. This method is called after
         * initialisation is finished and before the main loop is started.*/
        virtual void start();
        /**
         * On Unix platforms closes curses.*/
        virtual void close();
        /**
         * tests whether a key was pressed, if so it stops.
         * @return 1 if main loop should stop, 0 otherwise. */
        virtual int stop();

        // maps the function names in the config file to indices
        std::vector<std::string> functionMap;
        // maps the function key names in the config file to key codes
        std::map<std::string,int> keyCodeMap;

        static const short MOVE_X_PLUS;
        static const short MOVE_X_MINUS;
        static const short MOVE_Y_PLUS;
        static const short MOVE_Y_MINUS;
        static const short MOVE_Z_PLUS;
        static const short MOVE_Z_MINUS;
        static const short ROT_X_PLUS;
        static const short ROT_X_MINUS;
        static const short ROT_Y_PLUS;
        static const short ROT_Y_MINUS;
        static const short ROT_Z_PLUS;
        static const short ROT_Z_MINUS;
        static const short ACCELL;
        static const short BRAKE;
        static const short BUTTON_1;
        static const short BUTTON_2;
        static const short BUTTON_3;
        static const short BUTTON_4;
        static const short STATION_0;
        static const short STATION_1;
        static const short STATION_2;
        static const short STATION_3;
        static const short STATION_4;
        static const short STATION_5;
        static const short STATION_6;
        static const short STATION_7;
        static const short STATION_8;
        static const short STATION_9;
        static const short RESET;
        static const short QUIT;
        static const short NEXT_ATTR;
        static const short ADD_ATTR;
        static const short DEL_ATTR;
        static const short CHANGE_ATTR_POS;
        static const short CHANGE_ATTR_NEG;
        static const short CHANGE_ATTR_VAL;
        static const short ACCEL_ANG;
        static const short BRAKE_ANG;
		
    };

	OT_MODULE(ConsoleModule);
} // namespace ot


#endif // OT_NO_CONSOLE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of ConsoleModule.h
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
