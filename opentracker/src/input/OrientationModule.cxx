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
/** source file for OrientationModule.
 *
 * @author Gerhard Schall
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#ifndef OT_NO_ORIENTATION_SUPPORT
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <ace/Log_Msg.h>
#include <string>  

#ifdef WIN32
#include <objbase.h>
#endif

#include <OpenTracker/input/OrientationModule.h>
#include <OpenTracker/input/OrientationSource.h>

#include <OpenTracker/core/MathUtils.h>
#include <OpenTracker/misc/serialcomm.h>

#include <assert.h>
#include <math.h>

#include <iostream>


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif




namespace ot {


    OT_MODULE_REGISTER_FUNC(OrientationModule){
        OT_MODULE_REGISTRATION_DEFAULT(OrientationModule, "OrientationConfig");
    }




    OrientationModule::OrientationModule() :
        ThreadModule(),
        NodeFactory()
    {
        // cout << OrientationModule::Constructor" << endl;

        stop = FALSE;
        serialportIsOpen = FALSE;

    } // OrientationModule
    
    
    // destructor cleans up any allocated memory
    OrientationModule::~OrientationModule()
    {
	
        // cout << "OrientationModule::Destructor" << endl;

	OrientationSourceVector::iterator it;
        for (it = sources.begin(); it != sources.end(); it++)
        {
            assert((*it) != NULL);
            delete (*it);
        }
        sources.clear();


    }



    void OrientationModule::init(StringTable& attributes, ConfigNode * localTree)
    {    
        // cout << "OrientationModule::init" << endl;

        ThreadModule::init(attributes, localTree);

        // scanning port name from XML-File
        strncpy (port.pathname, attributes.get("device").c_str(), 255);
        logPrintI("use orientation device on port: %s\n", port.pathname);

    } // init



    Node * OrientationModule::createNode( const std::string & name, const StringTable & attributes )
    {
        
	if( name.compare("OrientationSource") == 0 ) {
            OrientationSource * source = new OrientationSource;
            
            printf("\n create node source:%s\n", source->getName().c_str());

            assert(source);
            // add the source object to the target list
            
            sources.push_back(source);
            
            ACE_DEBUG((LM_INFO, "OrientationModule: Built OrientationSource for  %s\n", port.pathname ));
            
            // return pointer to the source node
            return source;
            
        }    
	return NULL;	
    } // createNode





    void OrientationModule::start()
    {
    
        SerialParams params;
        int myResult = 0;


        if ((isInitialized() == 1) && !sources.empty())
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
                logPrintE("OrientationModule: error opening port %s\n", port.pathname);
                return;
            }
            else
                printf("\n port opened \n");

            serialportIsOpen = TRUE;

            ThreadModule::start();
        }
    } // start



    

    void OrientationModule::close()
    {

        //int myResult = 0;

        // cout << "DynaSightModule::close" << endl;

        // stop the thread
        // critical section start
        lockLoop();
        stop = true;
        unlockLoop();
        // end of critical section

        if (isInitialized() == 1)
        {
            // close the serial port
            //if (serialportIsOpen)
            //    myResult = closeSerialPort (&port);
        }
    } // stop





    void OrientationModule::pushEvent()
    {        
        
        // nothing to do


    } // pushEvent
        
        
    void OrientationSource::pushEvent()
    {
        // critical section start
        
        lock();
        
        // DEBUG
        // (*it)->modified = 1;
        // DEBUG
        
        if (modified == 1)
        {
            // update the event information
                
            // DEBUG
                
            //cout << "OrientationModule::pushEvent" << endl;
            /* (*it)->event.getPosition()[0] = 1.0;
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
            updateObservers (event);
            unlock();
        }
        else
        {
            unlock();
        }
        
        // end of critical section
    }

    void OrientationSource::pullEvent()
    {
        // nothing to do
    }

    // reads from the Orientation Sensor and parses the data
    void OrientationModule::run()
    {
        // the number of bytes we read over the serial port
        int count = 0;
        // read buffer for the serial port
        char serialPortBuffer[ORIENTATION_COMM_BUFFER_SIZE];
        // packet buffer for the target data assembly
        char packetBuffer[ORIENTATION_PACKET_MAX_LENGTH];
        for (count=0; count < ORIENTATION_PACKET_MAX_LENGTH; count++) packetBuffer[count] = 0;
        count = 0;
        //char *characterPointer = NULL;
        //char newCharacter = ' ';
        // status of the event machine
        //int packetEvent = 0;
        // number of target
        //int targetNumber = 0;
        // base-2 exponent
        //int exponent = 0;
        //long temp = 0;
        // x, y, z coordinates
        //long x = 0, y = 0, z = 0;
        //double x_meter = 0.0, y_meter = 0.0, z_meter = 0.0;
        float compass = 0, x_kippwinkel = 0, y_kippwinkel=0;

        // variables for the calculating of the orientation
        //double diff_x = 0.0, diff_y = 0.0, diff_z = 0.0;
        //float alpha = 0.0, beta = 0.0;

	//int k=0;

	char h[255];

	//printf("\n run \n");

	ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Starting Orientation module thread\n")));

	//printf("\nserialportIsOpen: %d\n",serialportIsOpen);
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
            count = readfromSerialPort (&port, serialPortBuffer, 4096);

            serialPortBuffer[count]='\0';

            // parser
            // process and decode serialPortBuffer[], which now contains count new bytes
            // format of the datan at the serial port is: nnn.nn,nn.n,nn.n
            // first part nnn.n is compass orientation
            // second part nn.n is x-Kippwinkel
            // third part nn.n is y-Kippwinkel
	    
            //	printf ("count: %d\n",count);

            // If sth is at the serial port
            if (count > 0)
            {	// sometimes only 1 or 2 bytes are read from the serial port
                // that leads to errors, because then scanf only takes the
                // digits before the comma.
                // Therefore these single bytes are stored in string h and
                // the next values from the serial port are then concatenated
                // which leads to the correct input format of nnn.nn,nn.n,nn.n again
                if (count <= 3)
                {
                    h[0]='\0';
                    strncat(h, serialPortBuffer, count);
                }
                else
                {
                    if (strlen(h) > 0)
                    {
                        //printf ("strlen h: %d   h: %s spb: %s\n", strlen(h),h ,serialPortBuffer);
                        strcat(h, serialPortBuffer);
                        strcpy(serialPortBuffer, h);
                        //printf ("new spb: %s\n", serialPortBuffer);
                        h[0]='\0';
                    }
                    sscanf (serialPortBuffer, "%f,%f,%f\n",
                            &compass, &x_kippwinkel, &y_kippwinkel);
                    // printf ("count: %d Kompass:%f  x-KW:%f  y-KW:%f\n",
                    //         count, compass, x_kippwinkel, y_kippwinkel);
                }
			

                OrientationSourceVector::iterator source;

                for (source = sources.begin(); 
                     source != sources.end(); source++)
                {
                    if ((*source) == 0) continue;

                    (*source)->lock();
                    Event & myEvent = (*source)->event;
                    // mark the event as modified
                    (*source)->modified = 1;
                                
                    myEvent.getOrientation()[0] = (float) compass;

                    myEvent.getOrientation()[1] = (float)x_kippwinkel;
                                
                    myEvent.getOrientation()[2] = (float)y_kippwinkel;
                                
                    myEvent.getOrientation()[3] = 1.0;
                                


                    // we can use the default position
                    myEvent.getPosition()[0] = 0.0;
                    myEvent.getPosition()[1] = 0.0;
                    myEvent.getPosition()[2] = 0.0;
                                
                    // set the confidence value
                    myEvent.getConfidence() = 1.0f;
                                
                    myEvent.timeStamp();
                                
                    (*source)->unlock();

                    if (context != NULL && context->doSynchronization())
                    {
                        context->dataSignal();
                        context->consumedWait();
                    }

                    // end of critical section
                }
                        
            } // end if
        } // while forever

	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping Orientation module thread\n")));
    } // run

}  // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of OrientationModule.cxx
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
