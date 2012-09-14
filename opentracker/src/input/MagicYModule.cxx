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
/** source file for MagicYModule module.
 *
 * @author Christoph Traxler
 *
 * $Id: MagicYModule.cxx 2120 2008-02-07 15:19:59Z bornik $
 * @file                                                                    */
/* ======================================================================== */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/Time_Value.h>

#include <OpenTracker/input/MagicYModule.h>

#include <iostream>
#include <string>

#include <ace/Log_Msg.h>


#ifndef OT_NO_MAGICY_SUPPORT


namespace ot {

	OT_MODULE_REGISTER_FUNC(MagicYModule){
		OT_MODULE_REGISTRATION_DEFAULT(MagicYModule, "MagicYConfig" );
	}

    // constructor initializing the thread manager
    MagicYModule::MagicYModule() : ThreadModule(), NodeFactory(), stop(0)
    {

    }

    // destructor cleans up any allocated memory
    MagicYModule::~MagicYModule()
    {
        unsigned int i;
        for (i=0; i<magicYs.size(); i++) delete magicYs[i];
        for (i=0; i<screens.size(); i++) delete screens[i];
    }

    // open all sockets
    int MagicYModule::connect()
    {
        int retval;
        char buffer[32];
        ACE_Time_Value timeOut(1,0);

        for(unsigned int i=0; i<screens.size(); i++)
        {
            retval = connector.connect(screens[i]->socket, screens[i]->address, &timeOut);
            if(retval == -1 && errno != ETIME && errno != 0 )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d connection failed for socket nr.: %d\n"), errno, i));
                return -1;
            }
            else
            {
                ACE_DEBUG((LM_INFO, ACE_TEXT("ot:connected to socket nr.: %d - sending GO command\n"), i));
                sprintf(buffer, "GO\n\r");
                retval = screens[i]->socket.send_n(buffer, sizeof(buffer), &timeOut);
                if(retval == -1 && errno != ETIME && errno != 0 )
                {
                    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d sending command for socket nr.: %d\n"), errno, i));
                    return -1;
                }
            }
            screens[i]->connected = true;
        }
        return 0;
    }

    // set FD_SET for ACE::select function
    void MagicYModule::setSelect()
    {
        readHandles.reset();
        for(unsigned int i=0; i<screens.size(); i++)
            if(screens[i]->connected)
                readHandles.set_bit(screens[i]->socket.get_handle());
    }

    // receive data from all active sockets
    int MagicYModule::receive()
    {
        ACE_Time_Value timeOut(0,5000);
        std::string message(""), accumulated("");
        char buffer[32], t='X';
        int retval, x, y, pos;
        size_t trans_bytes = 0;
        bool complete, trigger;

        points.clear();
        for(unsigned int i=0; i<screens.size(); i++)
        {
            complete = false;
            message.erase(message.begin(), message.end());

            if(readHandles.is_set(screens[i]->socket.get_handle()))
            {
                do
                {
                    retval = screens[i]->socket.recv_n(buffer, sizeof(buffer), &timeOut, (size_t*)&trans_bytes);
                    if(retval == -1 && errno != ETIME && errno != 0)
                    {
                        ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d receiving command for socket nr.: %d\n"), errno, i));
                        return -1;
                    }
                    else
                    {
                        accumulated.append(buffer, trans_bytes);
                        pos = accumulated.find("\n\r", 0);
                        if (pos < 0)
                            pos = accumulated.find("\r\n", 0);

                        if (pos >= 0)
                        {
                            message.assign(accumulated, 0, pos);
                            accumulated.erase(0, pos+2);
                            complete = true;
                        }
                    }
                } while(!complete && stop == 0);

                if(message.compare("READY") && message.compare("0"))
                {
                    pos = message.find(',', 0);
                    message.erase(0, pos+1);
                    // extract event and points
                    while (1)
                    {
                        if (sscanf(message.c_str(), "%c,%d,%d", &t, &x, &y) == 3)
                        {
                            if(t == 'Y')
                                trigger = true;
                            else
                                trigger = false;

                            x += screens[i]->x_offset;
                            y += screens[i]->y_offset;

                            points.push_back(MagicPoint(x,y,trigger));

                            pos = message.find(',', 0);
                            pos = message.find(',', pos+1);
                            pos = message.find(',', pos+1);
                            message.erase(0, pos+1);

                            if (pos < 0)
                                break;
                        }
                        else
                            break;
                    }
                }
            }
        }
        return 0;
    }

    // checks if all sockets are still connected by sending a PING
    int MagicYModule::stillConnected()
    {
        ACE_Time_Value timeOut(1,0);
        char buffer[16];
        int retval;

        sprintf(buffer, "PING\n\r");
        for(unsigned int i=0; i<screens.size(); i++)
        {
            retval = screens[i]->socket.send_n(buffer, sizeof(buffer), &timeOut);
            if(retval == -1 && errno != ETIME && errno != 0 )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("Error %d connection broken for socket nr.: %d\n"), errno, i));
                return 0;
            }
        }
        return 1;
    }

    // close all sockets
    void MagicYModule::disconnect()
    {
        for(unsigned int i=0; i<screens.size(); i++)
        {
            screens[i]->socket.close();
            screens[i]->connected = false;
        }
        OSUtils::sleep(3000);
    }

    // reads from the MagicY server and parses MagicY packages
    void MagicYModule::run()
    {
	ACE_DEBUG((LM_INFO, ACE_TEXT("starting MagicY module thread\n")));

        ACE_Time_Value timeOut(1,0);
        int average_x=0, average_y=0, socks_active;
        bool connected = false;

        while(1)
        {
            lockLoop();
            if ((bool)stop == true)
            {           
                unlockLoop();
                break;
            }
            else
            { 
                unlockLoop();
            }

            // connecting
            ACE_DEBUG((LM_INFO, ACE_TEXT("Trying to connect ... \n")));
            if(connect() == 0)
            {
                connected = true;

                // do while no error occurs
                while(1)
                {
                     lockLoop();
                     if ((bool)stop == true)
                     {           
                         unlockLoop();
                         break;
                     }
                     else
                     { 
                         unlockLoop();
                     }

                    average_x=0;
                    average_y=0;

                    if(! stillConnected())
                        break;

                    // build FD_SET
                    setSelect();
                    // wait for sockets to be ready for reading
                    socks_active = ACE::select(ACE_Handle_Set::MAXSIZE, readHandles, &timeOut);
                    if(! socks_active)
                    {
                        ACE_DEBUG((LM_INFO, ACE_TEXT("Error: Socket select time out\n")));
                        break;
                    }
                    if(receive())
                        break;


                    // calculate average
                    for (unsigned int i=0; i < points.size(); i++)
                    {
                        average_x += points[i].x;
                        average_y += points[i].y;
                    }
                    if(points.size())
                    {
                        average_x /= points.size();
                        average_y /= points.size();
                    }

                    // try to find source 1 to n for the n extracted points and fill their event
                    MagicYVector::iterator mY_it;
                    for( mY_it = magicYs.begin(); mY_it != magicYs.end(); mY_it++ )
                    {
                        (*mY_it)->source->lock();
                        // critical section start, fill event
                        Event & event = (*mY_it)->source->event;
                        if ((*mY_it)->average)
                        {
                            if(points.size())
                            {
                                event.getPosition()[0] = float(average_x);
                                event.getPosition()[1] = float(average_y);
                                event.getConfidence() = 1.0f;
                            }
                            else
                            {
                                event.getConfidence() = 0.0f;
                            }
                        }
                        else
                        {
                            if((*mY_it)->number >= 0 && (unsigned int)(*mY_it)->number < points.size())
                            {
                                event.getPosition()[0] = float(points[(*mY_it)->number].x);
                                event.getPosition()[1] = float(points[(*mY_it)->number].y);
                                event.getButton() = points[(*mY_it)->number].trigger;
                                event.getConfidence() = 1.0f;
                            }
                            else
                            {
                                event.getConfidence() = 0.0f;
                            }
                        }
                        event.getPosition()[2] = z_value;

                        correctData(event.getPosition(), positionMapping, invertPosition);

                        event.getOrientation()[0] = orientation[0];
                        event.getOrientation()[1] = orientation[1];
                        event.getOrientation()[2] = orientation[2];
                        event.getOrientation()[3] = orientation[3];

                        (*mY_it)->source->modified = 1;
                        event.timeStamp();

                        // end of critical section
                        (*mY_it)->source->unlock();

                    }// for all MagicY sources

                    if (context != NULL && context->doSynchronization())
                    {
                        context->dataSignal();
                        context->consumedWait();
                    }

                }// while no error
            }// if connected
            disconnect();
        } // forever
	ACE_DEBUG((LM_INFO, ACE_TEXT("Stopping thread\n")));
    }


    //  constructs a new Node
    Node * MagicYModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("MagicYSource") == 0 )
        {
            int number = atoi(attributes.get("number").c_str());

            bool average = false;
            std::string avrg = attributes.get("average");
            if(!avrg.empty() && !(avrg.compare("true") && avrg.compare("t") && avrg.compare("1")))
                average = true;

            MagicYVector::iterator it;
            for( it = magicYs.begin(); it != magicYs.end(); it++ )
            {
                MagicY *mY = (MagicY*)(*it);
                if( mY->number == number )
                {
                    break;
                }
            }
            if( it != magicYs.end())
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("Source with number %d already exists\n"), number));
                return NULL;
            }

            MagicYSource *source = new MagicYSource;
            MagicY *magicY = new MagicY(number, average, source);
            magicYs.push_back( magicY );
            ACE_DEBUG((LM_INFO, ACE_TEXT("Built MagicYSource node.\n")));

            return source;
        }
        return NULL;
    }


    // opens the sockets needed for communication and starts the receive thread
    void MagicYModule::start()
    {
        if (isInitialized() && !magicYs.empty())
            ThreadModule::start();
    }

    // closes the module and closes any communication sockets and stops thread
    void MagicYModule::close()
    {
        // stop thread
        lockLoop();
        stop = 1;
        unlockLoop();
    }

    // pushes event information into the tree
    void MagicYModule::pushEvent()
    {
        // nothing to do
    }

    void MagicYSource::pushEvent()
    {
        // critical section start
        lock();
        if(modified == 1 )
        {
            //(*it)->source->event = (*it)->event;
            modified = 0;
            unlock();
            updateObservers(event );
        }
        else
        {
            unlock();
        }
        // end of critical section

    }
    
    void MagicYSource::pullEvent()
    {
        // nothing to do
    }

    int MagicYModule::parseVector(const std::string & line, int * val )
    {
        int help[3];
        int num;
        num = sscanf( line.c_str()," %d %d %d", &help[0], &help[1], &help[2]);
        if( num != 3 )
        {
            return 1;
        }
        val[0] = help[0];
        val[1] = help[1];
        val[2] = help[2];

        return 0;
    }


    int MagicYModule::parseVector(const std::string & line, float * val )
    {
        float help[4];
        int num;
        num = sscanf( line.c_str()," %f %f %f %f", &help[0], &help[1], &help[2], &help[3]);
        if( num != 4 )
        {
            return 1;
        }

        val[0] = help[0];
        val[1] = help[1];
        val[2] = help[2];
        val[3] = help[3];

        return 0;
    }

    // parse data for extra screens, format: "port x_offset y_offset"
    // currently streams for extra screens must come from one and the same server

    int MagicYModule::parseScreens(const std::string & line)
    {
        int port, x_off, y_off, pos=0;
        std::string temp = line;

        do {
            if(sscanf(temp.c_str(), "%d %d %d", &port, &x_off, &y_off) < 3)
                return -1;

            Screen *scr = new Screen(port, hostname, x_off, y_off);
            screens.push_back(scr);

            ACE_DEBUG((LM_INFO, ACE_TEXT("Extra screen %d : %d : %d\n"), port, x_off, y_off));

            pos = temp.find(' ', 0);
            pos = temp.find(' ', pos+1);
            pos = temp.find(' ', pos+1);
            temp.erase(0, pos+1);
        } while(pos >= 0);

        return 0;
    }

    void MagicYModule::correctData(std::vector<float> &d, int *mapping, int *inversion)
    {
        float h[3];
        int i;
        for(i=0; i<3; i++) h[i] = d[mapping[i]]*inversion[i];
        for(i=0; i<3; i++) d[i] = h[i];
    }


    void MagicYModule::initMappping(int *mapping)
    {
        for (int i=0; i<3; i++)
            mapping[i] = i;
    }

    void MagicYModule::initInversion(int *inversion)
    {
        for (int i=0; i<3; i++)
            inversion[i] = 1;
    }

    void MagicYModule::initOrientation(float *orientation)
    {
        orientation[0] = 0.0f;
        orientation[1] = 0.0f;
        orientation[2] = 0.0f;
        orientation[3] = 1.0f;
    }

    void MagicYModule::calcMapping(int *mapping)
    {
        for (int i=0; i<3; i++)
            if (mapping[i] > 2)
                mapping[i] = 2;
            else if (mapping[i] < 0)
                mapping[i] = 0;
    }

    void MagicYModule::calcInversion(int *inversion)
    {
        for (int i=0; i<3; i++)
            inversion[i] = inversion[i] ? -1 : 1;
    }


    void MagicYModule::init(StringTable& attributes, ConfigNode * localTree)
    {
        ThreadModule::init(attributes, localTree);

        // Reading hostname and port number from XML-File
        hostname = std::string(attributes.get("hostname"));

        if( parseVector(attributes.get("positionMapping"), positionMapping ) != 0 )
        {
            ACE_DEBUG((LM_INFO, ACE_TEXT("Error parsing positionMapping !")));
            initMappping(positionMapping);
        }
        calcMapping(positionMapping);
        if( parseVector(attributes.get("invertPosition"), invertPosition ) != 0 )
        {
            ACE_DEBUG((LM_INFO, ACE_TEXT("Error parsing invertPosition!")));
            initInversion(invertPosition);
        }
        calcInversion(invertPosition);
        if( parseVector(attributes.get("orientation"), orientation ) != 0 )
        {
            ACE_DEBUG((LM_INFO, ACE_TEXT("Error parsing orientation!")));
            initOrientation(orientation);
        }
        if( parseScreens(attributes.get("screens")) != 0 )
        {
            ACE_DEBUG((LM_INFO, ACE_TEXT("Error parsing extra screens!")));
        }
        if( attributes.get("z_value", &z_value) != 1 )
            z_value = 0;
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_MAGICY_SUPPORT")
#endif // OT_NO_MAGICY_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of MagicYModule.cxx
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
