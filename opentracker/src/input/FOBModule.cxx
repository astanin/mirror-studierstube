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
/** source file for FOBModule module.
 *
 * @author Thomas Peterseil, Gerhard Reitmayr
 *
 * $Id: FOBModule.cxx 2119 2008-02-07 09:39:14Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <ace/Log_Msg.h>
#ifndef OT_NO_FOB_SUPPORT
#include <OpenTracker/input/FOBSource.h>
#include <OpenTracker/input/FOBModule.h>
#include <OpenTracker/core/MathUtils.h>
#include <OpenTracker/misc/serialcomm.h>

#include <cstdio>
#include <cstring>
#include <iostream>




namespace ot {

    OT_MODULE_REGISTER_FUNC(FOBModule){
        OT_MODULE_REGISTRATION_DEFAULT(FOBModule, "FOBConfig" );
    }    
        
    // constructor method.
    FOBModule::FOBModule() : ThreadModule(), NodeFactory()
    {
    }

    // Destructor method, clears nodes member.
    FOBModule::~FOBModule()
    {
        std::map<int, Bird *>::iterator it;
        for( it = birds.begin(); it != birds.end(); it++)
        {
            delete it->second;
        }
        birds.clear();
    }

    // initializes the tracker module. 
    void FOBModule::init(StringTable& attributes,  ConfigNode * localTree)
    {
        // checking mode parameter
        if( attributes.get("mode").compare("multi") == 0 )
            mode = MULTI;
        else
            mode = SINGLE;
    
        // getting master parameter
        if( attributes.get("master", &master ) != 1 )
        {        
            logPrintE("error in master parameter %s\n", attributes.get("master").c_str());
            return;
        }
    
        // getting scale
        if( attributes.get("scale", &scale ) != 1 )
            scale = 36;
    
        // getting erc transmitter
        if( attributes.get("transmitter", &transmitter ) != 1 )
            transmitter = -1;
        else        
            scale = 144;

        // getting hemisphere
        if( attributes.containsKey("hemisphere"))
        {    
            if( attributes.get("hemisphere").compare("forward") == 0 )
                hemisphere = FORWARD;
            else if ( attributes.get("hemisphere").compare("rear") == 0 )
                hemisphere = REAR;
            else if ( attributes.get("hemisphere").compare("upper") == 0 )
                hemisphere = UPPER;
            else if ( attributes.get("hemisphere").compare("lower") == 0 )
                hemisphere = LOWER;
            else if ( attributes.get("hemisphere").compare("left") == 0 )
                hemisphere = LEFT;
            else if ( attributes.get("hemisphere").compare("right") == 0 )
                hemisphere = RIGHT;
        }       

        // getting reference frame    
        if( attributes.get("referenceframe", referenceframe, 3 ) != 3 )
        {
            referenceframe[0] = 0;
            referenceframe[1] = 0;
            referenceframe[2] = 0;
        }

        if( attributes.get("xyzframe").compare("false") == 0 )
            useXYZFrame = false;
        else
            useXYZFrame = true;

        // parsing birds
        for( unsigned int i = 0; i < localTree->countChildren(); i++ )
        {
            ConfigNode * child = (ConfigNode *)localTree->getChild(i);
            StringTable & childAttr = child->getAttributes();
            int number;
            if( childAttr.get("number", &number ) != 1 )
            {
                logPrintE("FOBModule : error parsing Bird %d with number %s\n", childAttr.get("number").c_str());
                continue;
            }

            // getting anglealign
            float angles[3];
            if( attributes.get("anglealign", angles, 3 ) != 3 )
            {
                angles[0] = 0;
                angles[1] = 0;
                angles[2] = 0;
            }

            Bird * bird = new Bird( number, childAttr.get("device"), scale, angles );        
            birds[number] = bird;        
            logPrintI("created bird %d on dev %s\n", number, childAttr.get("device").c_str());
        }
    
        if( birds.find(master) == birds.end())
        {        
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error no master bird %d\n"), master));
            return;
        }
        ThreadModule::init( attributes, localTree );
    }

    // This method is called to construct a new Node 
    Node * FOBModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("FOBSource") == 0 )
        {
            int number;
            if( attributes.get("number", &number ) != 1 )
            {            
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error reading FOBSource number !\n")));
                return NULL;
            }
            if( birds.find(number) == birds.end())
            {            
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : no Bird with number %d present\n"), number));
                return NULL;
            }
            FOBSource * source = new FOBSource();
            birds[number]->source = source;   
            source->bird = birds[number];
            logPrintI("Build FOBSource for %d\n", number);
            return source;
        }
        return NULL;
    }
 
    // starts the module and opens all the devices etc.   
    void FOBModule::start()
    {
        if( isInitialized() == 1 )
        {
            if( mode == SINGLE )
            {
                if( birds[master]->open() < 0 )
                {
                    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error opening %d port for master %d\n"), master, birds[master]->number));
                    initialized = 0;
                    return;
                }
            } else {
                std::map<int, Bird *>::iterator it;
                for( it = birds.begin(); it != birds.end(); it++ )
                {
                    if( it->second->open() < 0 )
                    {                    
                        ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error opening port for %d\n"), it->first));
                        initialized = 0;
                        return;
                    }
                }           
            }
            ThreadModule::start();
        }
    }

    // runs the whole initialization sequence
    int FOBModule::initFoB()
    {
        Bird * masterBird = birds[master];
        int result;

        // reset birds
        if((result = resetBirds()) != 0 )
        {        
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error reseting birds %d\n"), result));
            return result;
        }
        OSUtils::sleep(300);

        if((result = masterBird->setGroupMode( false )) != 0 )
        {        
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error clear group mode %d\n"), result));
            return result;
        }
        OSUtils::sleep(300);

        if((result = masterBird->autoConfig((transmitter > birds.rbegin()->first)?(transmitter):(birds.rbegin()->first))) 
           != 0 )
        {        
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error clear group mode %d\n"), result));
            return result;
        }
        OSUtils::sleep(300);

        if((result = setReportMode()) != 0 )
        {        
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error setting report mode %d\n"), result));
            return result;
        }
    
        // set all kinds of other parameters here
        setHemisphere();

        setScale();

        setAngleAlign();

        setReferenceFrame();
    
        setXYZFrame();

        if((result = setNextTransmitter()) != 0 )
        {        
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error setting transmitter %d\n"), result));
            // return result;
        }
        OSUtils::sleep(500);

        if( mode == SINGLE )
        {
            if((result = masterBird->setGroupMode( true )) != 0 )
            {       
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error setting group mode %d\n"), result));
                // return result;
            }
        }
        OSUtils::sleep(500);
        if((result = startStreamMode()) != 0 )
        {        
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule : error starting data streaming %d\n"), result));
            //return result;
        }
        return 0;
    }

    // closes FOBModule, closing all serial ports
    void FOBModule::close()
    {
        // stop thread
        lockLoop();
        stop = true;
        unlockLoop();

        // close serial ports
        if( isInitialized() == 1 )
        {
            birds[master]->sleep();
            if( mode == SINGLE )
            {
                birds[master]->close();
            } else {
                std::map<int, Bird *>::iterator it;
                for( it = birds.begin(); it != birds.end(); it++ )
                {
                    it->second->close();
                }
            }
        }
    }

    // this method is the code executed in its own thread
    void FOBModule::run()
    {
        char buffer[100];
        int len = 0;
        int count = 0;
        // the number of bytes in one data frame for group mode (single serial interface)
        const int GROUP_FRAME_SIZE = 15;
        // the number of bytes in one data frame for multi mode
        const int MULTI_FRAME_SIZE = 14;
        // timeout before we think that something is wrong, right now 1 second
        const double DATA_TIME_OUT = 1000 * 1;
        // number of allowed failures 
        const int MAX_FAILURES = 5;

        int failure = 0;

        initFoB();

        unsigned int iter = 0;    
        double startTime = OSUtils::currentTime();
        double lastDataTime = OSUtils::currentTime();

        if( mode == SINGLE )
        {
            Bird * bird = birds[master];
            int num;

            while(1)
            {
                // yield to let other processes do something
                OSUtils::sleep(1);
                iter++;            
                lockLoop();
                if( stop == true )
                {           
                    unlockLoop();
                    break;
                } else { 
                    unlockLoop();
                }      
            
                // test if something is wrong, if so do an init again 
                if((OSUtils::currentTime() - lastDataTime) > DATA_TIME_OUT ||
                   failure > MAX_FAILURES )
                {
                    initFoB();
                    lastDataTime = OSUtils::currentTime();
                    failure = 0;
                }

                // read in data and parse it 
                if( (len = bird->read( buffer, 100 )) <= 0 )
                {
                    if( errno != EAGAIN )
                    {
                        failure++;
                    }
                    continue;
                }
                lastDataTime = OSUtils::currentTime();
                while( count < len )
                {
                    // let the master bird parse the buffer
                    count += bird->parse( &buffer[count], len - count, GROUP_FRAME_SIZE );
                    if( bird->count == GROUP_FRAME_SIZE )
                    {
                        bird->count = 0;
                        num = bird->buffer[GROUP_FRAME_SIZE - 1];
                        if( birds.find(num) != birds.end())
                        {
                            birds[num]->source->lock();                    
                            birds[num]->convert( bird->buffer );
                            birds[num]->newVal = true;
                            birds[num]->source->unlock();
                            if (context != NULL)
                            {
			       if (context->doSynchronization())
			       {
			           context->dataSignal();
				   context->consumedWait();
			       }
                            }
                        }
                    }
                }
                count = 0;
            }
        } else {
            while(1)
            {
                // yield to let other processes do something
                OSUtils::sleep(1);
                iter++;
                lockLoop();
                if( stop == true )
                {           
                    unlockLoop();
                    break;
                } else { 
                    unlockLoop();
                }

                // test if something is wrong, if so do an init again 
                if((OSUtils::currentTime() - lastDataTime) > DATA_TIME_OUT ||
                   failure > MAX_FAILURES )
                {
                    initFoB();
                    lastDataTime = OSUtils::currentTime();
                    failure = 0;
                }

                std::map<int,Bird *>::iterator it;
                for( it = birds.begin(); it != birds.end(); it++ )
                {
                    Bird * bird = it->second;
                    if( (len = bird->read( buffer, 100 )) <= 0 )
                    {
                        if( errno != EAGAIN )
                        {
                            failure++;
                        }                    
                        continue;
                    }
                    lastDataTime = OSUtils::currentTime();
                    while( count < len )
                    {
                        // let the bird itself parse the buffer
                        count += bird->parse( &buffer[count], len - count, MULTI_FRAME_SIZE );
                        if( bird->count == MULTI_FRAME_SIZE )
                        {
                            bird->count = 0;
                            bird->source->lock();
                            bird->convert();
                            bird->newVal = true;
                            bird->source->unlock();

                            if (context != NULL && context->doSynchronization())
                            {
                                context->dataSignal();
				context->consumedWait();
                            }
                        }
                    }
                    count = 0;
                }
            }
        }    
	ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FOBModule Framerate %f\n"), 1000 * iter / ( OSUtils::currentTime() - startTime )));
    }
    
    // pushes events into the tracker tree
    void FOBModule::pushEvent()
    {
        // nothing to do
    }

    void FOBSource::pushEvent()
    {
        lock();
        if( bird->newVal == true)
        {
            event = bird->event;
            bird->newVal = false;
            unlock();
            updateObservers(event);
        } 
        else
        {
            unlock();
        }
    }
    
    void FOBSource::pullEvent()
    {
        // nothing to do
    }

    // toggle reset on the birds
    int FOBModule::resetBirds()
    {    
        if( mode == SINGLE )
        {
            birds[master]->reset();
        }
        else
        {
            std::map<int, Bird *>::iterator it;
            for( it = birds.begin(); it != birds.end(); it++ )
            {
    		it->second->reset();
            }
        }
        OSUtils::sleep(300);
        return birds[master]->sendReset();
    }

    // set report mode to POSITION/QUATERNION
    int FOBModule::setReportMode()
    { 
        std::map<int, Bird *>::iterator it;
        if( mode == MULTI )
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                it->second->setReportMode();
            }
            OSUtils::sleep(600);
        } else {
            // send all birds the POSITION/QUATERNION command
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                birds[master]->setReportMode( it->first );
                OSUtils::sleep(300);
            }        
        }
        return birds[master]->getErrorCode();
    }

    // Turn on ERC if present (or another transmitter :)
    int FOBModule::setNextTransmitter()
    {
        char buffer[2];
        if( transmitter != -1 )
        {
            buffer[0] = 0x30;
            buffer[1] = (transmitter << 4) & 0xff;
            birds[master]->write( buffer, 2 );
            OSUtils::sleep( 300 );
        }
        return birds[master]->getErrorCode();
    }

    // start STREAM mode    
    int FOBModule::startStreamMode()
    {
        char buffer[2];

        if( mode == SINGLE )
        {
            // only to master bird
            buffer[0] = 0xF0+birds[master]->number;
            buffer[1] = '@';
            birds[master]->write( buffer, 2 );
        } else {
            // to all
            buffer[0] = '@';
            std::map<int, Bird *>::iterator it;
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                it->second->write( buffer, 1 );
            }
        }
        OSUtils::sleep(500);
        // no use getting an error code :)
        return 0;
    }

    void FOBModule::setScale()
    {
        if( scale != 72 )
            return;
        std::map<int, Bird *>::iterator it;
        if( mode == SINGLE )
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                birds[master]->setScale( (int)scale, it->first );
                OSUtils::sleep( 300 );
            }
        }
        else
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                it->second->setScale( (int)scale );            
            }
            OSUtils::sleep( 300 );
        }
    }

    void FOBModule::setAngleAlign()
    {
        std::map<int, Bird *>::iterator it;
        if( mode == SINGLE )
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                float * angles = it->second->angleAlign;
                if( angles[0] == 0 && angles[1] == 0 && angles[2] == 0 )
                    continue;
                birds[master]->setAngleAlign( angles , it->first );
                OSUtils::sleep( 300 );
            }
        }
        else
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                float * angles = it->second->angleAlign;
                if( angles[0] == 0 && angles[1] == 0 && angles[2] == 0 )
                    continue;
                it->second->setAngleAlign( angles );            
            }
            OSUtils::sleep( 300 );
        }    
    }

    void FOBModule::setReferenceFrame()
    {
        if( referenceframe[0] == 0 && referenceframe[1] == 0 && referenceframe[2] == 0 )
            return;

        birds[master]->setReferenceFrame( referenceframe );
        OSUtils::sleep( 300 );
    }
    
    void FOBModule::setXYZFrame()
    {
        std::map<int, Bird *>::iterator it;
        if( mode == SINGLE )
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                birds[master]->setXYZFrame( useXYZFrame, it->first );
                OSUtils::sleep( 300 );
            }
        }
        else
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                it->second->setXYZFrame( useXYZFrame );            
            }
            OSUtils::sleep( 300 );
        }
    }

    void FOBModule::setHemisphere()
    {
        std::map<int, Bird *>::iterator it;
        if( mode == SINGLE )
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                birds[master]->setHemisphere( hemisphere, it->first );
                OSUtils::sleep( 300 );
            }
        }
        else
        {
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                it->second->setHemisphere( hemisphere );
            }
            OSUtils::sleep( 300 );
        }
    }

    const float Bird::inchesToMeters = 0.0254f;

    // convert Bird data in buffer to OpenTracker Event format
    inline void Bird::convert()
    {
        convert( buffer );
    }

    inline void Bird::convert( const char * data )
    {
        int d,i;
        event.timeStamp();

        for (i=0; i<3; i++)                         // x,y,z
        {
            d=(((int)(data[i*2]&0x7f))<<2)+(((int)data[i*2+1])<<9);
            if (d&0x8000)
                d|=0xffff0000;        // Extend signbit
            event.getPosition()[i] = ((float)d)*scale;
        }
        for (i=0; i<4; i++)                         // qa,qx,qy,qz
        {
            d=(((int)(data[6+2*i]&0x7f))<<2)+(((int)data[7+2*i])<<9);
            if (d&0x8000)
                d|=0xffff0000;        // Extend signbit
            event.getOrientation()[(i+3)%4] = ((float)d)/0x8000; // we need qx, qy, qz, qa
            // so (i+3)%4 should do the
            // trick. scale ?????
        }
	event.getOrientation()[3] *= -1;	// for some reason we get the
        // coordinates inverted, therefore we invert the scalar !
    }

    inline int Bird::parse( const char * data, int len, int framesize )
    {
        int i = 0;
        if( count == 0 )  // we are still looking for the phasing bit
        {
            for( i = 0 ; i < len ; i++ )
            {
                if( data[i] & 0x80 )
                    break;
            }
            if( i == len )  // read everything but found no phasing bit, throw it away
                return len;
            buffer[0] = data[i];
            count = 1;
            i++;
        }
        // copy everything up to min(framesize - count, len - i) bytes from data[i] into buffer
        //int amount = min( framesize - count, len - i );
	int amount = (framesize - count) < (len - i) ? (framesize - count) : (len - i);
        if( amount == 0 )
            return i;
        memcpy( &buffer[count], &data[i], amount );
        count += amount;
        return i+amount;
    }

    inline int Bird::open()
    {
        SerialParams params;
        initSerialParams( &params );
        params.baudrate = 115200;
        params.parity = 0;
        params.bits = 8;
        params.sbit = 1;
        params.hwflow = 1;
        params.swflow = 0;
        params.blocking = 0;
        strncpy( params.pathname, port.pathname, 255 );
        return openSerialPort( &port, &params );
    }

    inline int Bird::write( const char * data, int count )
    {
        return writetoSerialPort( &port,(char *) data, count );
    }

    inline int Bird::read( char * data, int count )
    {
        return readfromSerialPort( &port, data, count );
    }

    inline int Bird::close()
    {
        return closeSerialPort(&port);
    }

    inline int Bird::reset()
    {
        setRTSSerialPort( &port, 1);
        OSUtils::sleep(100);
        return setRTSSerialPort( &port, 0);
    }

    inline int Bird::setGroupMode( bool value )
    {
        char buffer[3];
        buffer[0] = 'P';
        buffer[1] = 0x23;
        buffer[2] = (value)?(1):(0);
        write( buffer, 3 );
        OSUtils::sleep( 600 );
        return getErrorCode();
    }

    inline int Bird::getErrorCode()
    {
        buffer[0] = 0x4f;
        buffer[1] = 10;
        write( buffer, 2 );
        OSUtils::sleep(100);
        buffer[0] = 0;
        int count = 0;
        while( read( buffer, 1 ) < 0 && count < 10)
        {
            if( errno != EAGAIN )
                return -1;	
            count++;
            OSUtils::sleep(10);
        }
        return buffer[0];
    }

    inline int Bird::autoConfig( const int number )
    {
        char buffer[3];
        buffer[0] = 'P';
        buffer[1] = 0x32;
        buffer[2] = number;
        write( buffer, 3 );
        OSUtils::sleep(600);
        return getErrorCode();
    }

    inline void Bird::sleep()
    {
        char buffer[4] = "GGG";
        write( buffer, 3 );
        OSUtils::sleep(300);    
    }

    inline void Bird::setReportMode( const int toBird )
    {
        char buffer[2];
        if( toBird == -1 || toBird == number )
        {
            buffer[0] = ']';
            write( buffer, 1 );
        }
        else 
        {
            buffer[0] = 0xF0+toBird;
            buffer[1] = ']';
            write( buffer, 2 );
        }
    }

    inline void Bird::setScale( const int scale, const int toBird )
    {
        char buffer[5];
        if( toBird == -1 || toBird == number )
        {
            buffer[0] = 'P';
            buffer[1] = 0x3;
            buffer[2] = 0;
            buffer[3] = (scale==72)?(1):(0);
            write( buffer, 4 );
        }
        else 
        {
            buffer[0] = 0xF0+toBird;
            buffer[1] = 'P';
            buffer[2] = 0x3;
            buffer[3] = 0;
            buffer[4] = (scale==72)?(1):(0);
            write( buffer, 5 );
        }
    }

    inline void Bird::setXYZFrame( const bool useFrame, const int toBird )
    {
        char buffer[4];
        if( toBird == -1 || toBird == number )
        {
            buffer[0] = 'P';
            buffer[1] = 17;
            buffer[2] = (useFrame)?(1):(0);
            write( buffer, 3 );
        }
        else 
        {
            buffer[0] = 0xF0+toBird;
            buffer[1] = 'P';
            buffer[2] = 17;
            buffer[3] = (useFrame)?(1):(0);
            write( buffer, 4 );        
        }
    }

    inline void Bird::setHemisphere( const Hemisphere hemisphere, const int toBird )
    {
        char buffer[4];
        if( hemisphere != FORWARD )
        {
            buffer[1] = 'L';
            switch( hemisphere )
            {
                case REAR:
                    buffer[2] = 0x00;
                    buffer[3] = 0x01;
                    break;
                case UPPER:
                    buffer[2] = 0x0c;
                    buffer[3] = 0x01;
                    break;
                case LOWER:
                    buffer[2] = 0x0c;
                    buffer[3] = 0x00;
                    break;
                case LEFT:
                    buffer[2] = 0x06;
                    buffer[3] = 0x01;
                    break;
                case RIGHT:
                    buffer[2] = 0x06;
                    buffer[3] = 0x00;
                    break;
                default:
                    break;
            }
            if( toBird == -1 || toBird == number )
            {
                write( &buffer[1], 3 );
            }
            else 
            {
                buffer[0] = 0xF0+toBird;
                write( buffer, 4 );
            }
        }
    }

    inline void Bird::setAngleAlign( const float * angles, const int toBird )
    {
        char buffer[8];
        buffer[1] = 0x71;

        for( int i = 0; i < 3; i++ )    
            *(short int *)(&buffer[2+2*i]) = (short int)(angles[i] * 0x7FFF / MathUtils::Pi);
        if( toBird == -1 || toBird == number )
        {
            write( &buffer[1], 7 );
        }
        else 
        {
            buffer[0] = 0xF0+toBird;
            write( buffer, 8 );
        }
    }

    inline void Bird::setReferenceFrame( const float * angles, const int toBird )
    {
        char buffer[8];
        buffer[1] = 0x72;
        
        for( int i = 0; i < 3; i++ )
            *(short int *)(&buffer[2+2*i]) = (short int)(angles[i] * 0x7FFF / MathUtils::Pi);
        if( toBird == -1 || toBird == number )
        {
            write( &buffer[1], 7 );
        }
        else 
        {
            buffer[0] = 0xF0+toBird;
            write( buffer, 8 );
        }
    }

    inline int Bird::sendReset()
    {
        char buffer[1] = { 0x2F };
        write( buffer, 1 );
        OSUtils::sleep( 300 );
        return getErrorCode();
    }

} // namespace ot


#endif // OT_NO_FOB_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of FOBModule.cxx
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
