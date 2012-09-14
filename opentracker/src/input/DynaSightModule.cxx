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
/** source file for DynaSightModule module. Version 1.02
 *
 * @author Alexander Schaelss
 *
 * $Id: DynaSightModule.cxx 2120 2008-02-07 15:19:59Z bornik $
 * @file                                                                    */
/* ======================================================================== */

// this will remove the warning 4786
#ifndef OT_NO_DYNASIGHT_SUPPORT
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <ace/Log_Msg.h>

#include <OpenTracker/input/DynaSightSource.h>
#include <OpenTracker/input/DynaSightModule.h>

#include <OpenTracker/core/MathUtils.h>
#include <OpenTracker/misc/serialcomm.h>

#include <cassert>
#include <cmath>

#include <iostream>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif





namespace ot {


	OT_MODULE_REGISTER_FUNC(DynaSightModule){
		OT_MODULE_REGISTRATION_DEFAULT(DynaSightModule, "DynaSightConfig");
	}

    // constructor initializing the thread manager
    DynaSightModule::DynaSightModule() :
        ThreadModule(),
        NodeFactory()
    {
        // cout << "DynaSightModule::Constructor" << endl;

        lookAtVector[0] = 0.0;
        lookAtVector[1] = 0.0;
        lookAtVector[2] = 0.0;

        stop = FALSE;
        serialportIsOpen = FALSE;
        hasLookAt = FALSE;
    } // DynaSightModule

    // destructor cleans up any allocated memory
    DynaSightModule::~DynaSightModule()
    {
        // cout << "DynaSightModule::Destructor" << endl;

        TargetVector::iterator it;
        for (it = targets.begin(); it != targets.end(); it++)
        {
            assert((*it) != NULL);
            delete (*it);
        }
        targets.clear();
    } // ~DynaSightModule

    void DynaSightModule::init(StringTable& attributes, ConfigNode * localTree)
    {
        int myResult = 0;
    
        // cout << "DynaSightModule::init" << endl;
    
        ThreadModule::init(attributes, localTree);
    
        // scanning port name from XML-File
        strncpy (port.pathname, attributes.get("device").c_str(), 255);
	logPrintI("use device on port: %s\n", port.pathname);
    
        // check if we need to calculate the orientation
        myResult = attributes.get("lookat", lookAtVector, 3);
    
        if (myResult == 3)
        {
            hasLookAt = TRUE;
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:looking at point: %f %f %f\n"), lookAtVector[0], lookAtVector[1], lookAtVector[2]));
        }
        else
        {
            hasLookAt = FALSE;
        }
    } // init

    //  constructs a new DynaSightSource node
    Node * DynaSightModule::createNode(const std::string& name,  const StringTable& attributes)
    {
        int number = 0;
    
        // cout << "DynaSightModule::createNode" << endl;
    
        if (name.compare("DynaSightSource") == 0)
        { 
            if (attributes.get("target", &number ) != 1)
            {
                // error message
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting DynaSightSource target number !\n")));
                return NULL;
            }
            else if ((number < 0) || (number > DYNASIGHT_MAX_TARGETS))
            {
                // error message
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:The DynaSightSource target number is out of range !\n")));
                return NULL;
            }
        
            TargetVector::iterator it;
            for (it = targets.begin(); it != targets.end(); it++)
            {
                if ((*it)->number == number)
                {
                    break;
                }
            }
            if (it != targets.end())
            {
                // error message
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:DynaSight Source with number %d already exists !\n"), number));
                return NULL;
            }
        
            DynaSightSource * source = new DynaSightSource; 
            assert(source);
            source->number = number;
        
            targets.push_back(source);
                
            logPrintI("Built DynaSightSource node - target %d\n", number);
        
            // return pointer to the source node
            return source;
        }
    
        return NULL;
    } // createNode

    // opens the sockets needed for communication and starts the receive thread
    void DynaSightModule::start()
    {
        SerialParams params;
        int myResult = 0;
    
        // cout << "DynaSightModule::start" << endl;
    
        if ((isInitialized() == 1) && !targets.empty())
        {
            // init serial port
            initSerialParams (&params);
            params.baudrate = 19200;
            params.parity = 0;
            params.bits = 8;
            params.sbit = 1;
            params.hwflow = 0;
            params.swflow = 0;
            params.blocking = 0;
            strncpy (params.pathname, port.pathname, 255);
        
            // open the serial port
            myResult = openSerialPort ( &port, &params );
        
            if (myResult < 0)
            {
                // error message
                logPrintE("DynaSightModule: error opening port %s\n", port.pathname);
                return;
            }
        
            serialportIsOpen = TRUE;
        
            ThreadModule::start();
        }
    } // start

    // closes the module and closes any communication sockets and stops thread 
    void DynaSightModule::close()
    {
        int myResult = 0;
    
        // cout << "DynaSightModule::close" << endl;
    
        // stop the thread
        // critical section start
        //lock();
        stop = true;
        //unlock();
        // end of critical section
    
        if (isInitialized() == 1)
        {
            // close the serial port
            if (serialportIsOpen)
                myResult = closeSerialPort (&port);
        }
    } // stop

    // pushes event information into the tree
    void DynaSightModule::pushEvent()
    {
        // nothing to do
    } // pushEvent

    void DynaSightSource::pushEvent()
    {
        lock();
                    
        if (modified == 1)
        {
            // update the event information
            
            // DEBUG
            /*
              cout << "DynaSightModule::pushEvent" << endl;
              (*it)->event.getPosition()[0] = 1.0;
              (*it)->event.getPosition()[1] = 2.0;
              (*it)->event.getPosition()[2] = 3.0;
              (*it)->event.getOrientation()[0] = 0.0;
              (*it)->event.getOrientation()[1] = 0.0;
              (*it)->event.getOrientation()[2] = 0.0;
              (*it)->event.getOrientation()[3] = 1.0;
              (*it)->event.getConfidence() = 1.0;
              (*it)->event.timeStamp();
            */
            // DEBUG
            
            modified = 0;
            unlock();
            updateObservers (event);
        }
        else
        {
            unlock();
        }
        // end of critical section
    }
    
    void DynaSightSource::pullEvent()
    {
        //nothing to do
    }

    // reads from the DynaSight Sensor and parses the data
    void DynaSightModule::run()
    {
        // the number of bytes we read over the serial port
        int count = 0;
        // read buffer for the serial port
        char serialPortBuffer[DYNASIGHT_COMM_BUFFER_SIZE];
        // packet buffer for the target data assembly
        char packetBuffer[DYNASIGHT_PACKET_MAX_LENGTH];
        for (count=0; count < DYNASIGHT_PACKET_MAX_LENGTH; count++) packetBuffer[count] = 0;
        count = 0;
        char *characterPointer = NULL;
        char newCharacter = ' ';
        bool isMarker = FALSE;
        bool packetSyncError = FALSE;
        // status of the event machine
        int packetEvent = 0;
        // number of target
        int targetNumber = 0;
        // base-2 exponent
        int exponent = 0;
        long temp = 0;
        // x, y, z coordinates
        long x = 0, y = 0, z = 0;
        double x_meter = 0.0, y_meter = 0.0, z_meter = 0.0;
        // status of target
        TargetStatus status = SEARCH;
        // variables for the calculating of the orientation
        double diff_x = 0.0, diff_y = 0.0, diff_z = 0.0;
        float alpha = 0.0, beta = 0.0;
        
	ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Starting DynaSight module thread\n")));
    
        assert(serialportIsOpen == TRUE);
    
        while (1)
        {
            // yield to let other processes do something
            OSUtils::sleep(1);
        
            // critical section start
            lockLoop();
            if (stop == true)
            {           
                unlockLoop();
                break;
            }
            else
            { 
                unlockLoop();
            }
            // critical section end
        
            // read data from the serial port
            count = readfromSerialPort (&port, serialPortBuffer, DYNASIGHT_COMM_BUFFER_SIZE);
        
            // process and decode serialPortBuffer[], which now contains count new bytes
            characterPointer = serialPortBuffer;
        
            for (; count > 0; count--)
            {
                newCharacter = *characterPointer++;
            
                // check for the sync pattern "1000xxxx"
                isMarker = ((newCharacter & 0xF0) == 0x80);

                switch (packetEvent) // Dyna Sight Sensor event machine
                {
                    case 0:
                        if (isMarker)
                        {
                            packetBuffer[packetEvent++] = newCharacter;
                        }
                        else
                            // could not synchronize, stay in event 0
                            packetSyncError = TRUE;
                        break;
                
                    case 1:
                        if (isMarker)
                        {
                            // we expected marker character -> save it
                            packetBuffer[packetEvent++] = newCharacter;
                        }
                        else
                        {
                            // got loss of synchronization -> reset
                            packetEvent = 0;
                            packetSyncError = TRUE;
                        }
                        break;
                
                    case 2:
                        if (isMarker)
                        {
                            // got an unexpected marker -> shift back
                            packetBuffer[0] = packetBuffer[1];
                            packetBuffer[1] = newCharacter;
                            packetSyncError = TRUE;
                            // leave packetEvent at 2;
                        }
                        else
                        {
                            // save the character
                            packetBuffer[packetEvent++] = newCharacter;
                        }
                        break;
                
                    case 3:
                        // save the character
                        packetBuffer[packetEvent++] = newCharacter;
                        break;
                
                    case 4:
                        if (isMarker)
                        {
                            // got an unexpected marker -> reset event machine
                            packetBuffer[0] = newCharacter;
                            packetEvent = 1;
                            packetSyncError = TRUE;
                        }
                        else
                        {
                            // save the character
                            packetBuffer[packetEvent++] = newCharacter;
                        }
                        break;

                    case 5:
                        // save the character
                        packetBuffer[packetEvent++] = newCharacter;
                        break;

                    case 6:
                        if (isMarker)
                        {
                            // got an unexpected marker -> reset event machine
                            packetBuffer[0] = newCharacter;
                            packetEvent = 1;
                            packetSyncError = TRUE;
                        }
                        else
                        {
                            // save the character
                            packetBuffer[packetEvent++] = newCharacter;
                        }
                        break;
                
                    case 7:
                        // the 8-byte packet is complete
                        // save the character
                        packetBuffer[packetEvent] = newCharacter;

                        // decode the completed packet
                        exponent = packetBuffer[0]&0x3; // exponent is in bits 1 and 0 of first byte
                        targetNumber = packetBuffer[1]&0x4 | ((packetBuffer[0]>>2)&0x3); // up to 8 targets
                        status = (TargetStatus) (packetBuffer[1]&0x3);
                        // get the three coordinates
                        temp = ((long)(char)packetBuffer[2]<<8) | (long)packetBuffer[3]&0xFFL;
                        x = temp<<exponent;
                        temp = ((long)(char)packetBuffer[4]<<8) | (long)packetBuffer[5]&0xFFL;
                        y = temp<<exponent;
                        temp = ((long)(char)packetBuffer[6]<<8) | (long)packetBuffer[7]&0xFFL;
                        z = temp<<exponent;
                        // reset the event machine
                        packetEvent = 0;

                        // check if we got new reliable data
                        if ((status == TRACK) || (status == CAUTION))
                        {
                            // get source with the same target number
                            TargetVector::iterator target;
                            for (target = targets.begin(); target != targets.end(); target++)
                            {
                                if ((*target) == NULL)
                                {
                                    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:DynaSightModule::run ERROR iterator == NULL!\n")));
                                }
                        
                                if ((*target)->number == targetNumber)
                                    break;
                            }
                    
                            // if source with the same target number exists
                            if (target != targets.end())
                            {
                                // start of critical section
                                (*target)->lock();
                                Event & myEvent = (*target)->event;

                                // mark the event as modified
                                (*target)->modified = 1;
                        
                                // set the position and scale to meter
                                x_meter = x / SCALE_TO_METER;
                                y_meter = y / SCALE_TO_METER;
                                z_meter = z / SCALE_TO_METER;
                                myEvent.getPosition()[0] = (float)x_meter;
                                myEvent.getPosition()[1] = (float)y_meter;
                                myEvent.getPosition()[2] = (float)z_meter;

                                // set the orientation
                                if (hasLookAt)
                                {
                                    // we need to calculate the orientation
                                    // works OK if the tracked point is in front (z-value) of the lookAt point
                                    diff_x = x_meter - lookAtVector[0];
                                    diff_y = y_meter - lookAtVector[1];
                                    diff_z = z_meter - lookAtVector[2];
                            
                                    alpha = (float)atan2(diff_x, diff_z);
                                    beta = (float)-atan2(diff_y, diff_z);
                            
                                    // DEBUG
                                    /*
                                      cout << "diff_x: " << diff_x << endl;
                                      cout << "diff_y: " << diff_y << endl;
                                      cout << "diff_z: " << diff_z << endl;
                                      cout << "alpha: " << alpha << endl;
                                      cout << "beta: " << beta << endl;
                                      cout << endl;
                                    */
                                    // DEBUG

                                    MathUtils::eulerToQuaternion(beta, alpha, 0.0, myEvent.getOrientation());
                                    MathUtils::normalizeQuaternion(myEvent.getOrientation());
                                }
                                else
                                {
                                    // we can use the default orientation
                                    myEvent.getOrientation()[0] = 0.0;
                                    myEvent.getOrientation()[1] = 0.0;
                                    myEvent.getOrientation()[2] = 0.0;
                                    myEvent.getOrientation()[3] = 1.0;
                                }

                                // set the confidence value
                                myEvent.getConfidence() = (status == TRACK) ? 1.0f : 0.5f;

                                myEvent.timeStamp();
                                (*target)->unlock();
                                // end of critical section
                                if (context != NULL && context->doSynchronization())
                                {
                                    context->dataSignal();
                                    context->consumedWait();
                                }
                            }
                        }
                
                        break;
                } // switch
            } // for
        } // while forever
    
	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping DynaSight module thread\n")));
    } // run

} // namespace ot


#endif // OT_NO_DYNASIGHT_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of DynaSightModule.cxx
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
