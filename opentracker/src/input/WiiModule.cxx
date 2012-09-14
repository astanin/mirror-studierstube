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
/** source file for LinmouseModule.
*
* @authors Michele Fiorentino and Alexander Bornik
* starting from cWiimote 0.2 by Kevin Forbes 
*
* $Id$
* @file                                                                   */
/* ======================================================================= */

// this is a linux implementation

#include <OpenTracker/input/WiiModule.h>
#include <OpenTracker/input/WiiSource.h>
#include <OpenTracker/input/WiiSink.h>

#ifdef USE_WII_SUPPORT

#include <cstdio>
#include <iostream>
#include <sstream>
#include <cassert>
#include <ace/Log_Msg.h>
#include <string>
#include <iostream>
#include <math.h>

#define OT_NAMESPACE_BEGIN namespace ot {
#define OT_NAMESPACE_END }

OT_NAMESPACE_BEGIN

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



OT_MODULE_REGISTER_FUNC(WiiModule){
   OT_MODULE_REGISTRATION_DEFAULT(WiiModule, "WiiConfig");
}

// -------------------------------------------------------------------------------------------------------
// constructor initializing the thread manager
WiiModule::WiiModule() : ThreadModule(), NodeFactory(), stop(0)
{
   wiimote = new WiiHandler;
   if (wiimote->ConnectToDevice() &&
      wiimote->StartDataStream())
   {
      logPrintI("Wii Connection: OK\n");
   }
   else
   {
      logPrintE("Wii Cannection Failed: OK\n");
   } 
}

// ------
// Destructor method
WiiModule::~WiiModule() {
   sources.clear();
   //    sinks.clear();
   delete wiimote;
}

// This method is called to construct a new Node.
Node * WiiModule::createNode( const std::string& name, StringTable& attributes) 
{

   if( name.compare("WiiSource") == 0 ) {       
      stop = 0;
      if ( sources.size() > 0) {
         ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Can only have one WiiSource!\n")));
         return NULL;
      }

      WiiSource * source = new WiiSource(wiimote);

      source->event.getButton() = 0;
      source->event.getConfidence() = 1.0f;
      sources.push_back( source );

      logPrintI("Build WiiSource node: OK\n");

      initialized = 1; // this is still to be explained mf
      return source;
   }
   if( name.compare("WiiSink") == 0 ) {       
      stop = 0;
      if ( sinks.size() > 0) {
         ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Can only have one WiiSink!\n")));
         return NULL;
      }

      WiiSink * mysink = new WiiSink( wiimote );
      sinks.push_back( mysink );

      initialized = 1; // this is still to be explained mf
      return mysink;
   } 

   return NULL;
}
// opens connection
void WiiModule::start() 
{
   //ACE_DEBUG((LM_INFO, ACE_TEXT("LinmouseModule::start() \n")));
   if( isInitialized() && !sources.empty())
      ThreadModule::start();    
}

// closes 
void WiiModule::close() {
   //ACE_DEBUG((LM_INFO, ACE_TEXT("WiiModule::close() \n")));
   lockLoop();
   stop = 1;
   unlockLoop();
}
void WiiModule::run()
{
   ACE_DEBUG((LM_INFO, ACE_TEXT("WiiModule::run() \n")));

   while(1)  // this is the run thread
      {
      lockLoop();
      if (stop == 1) 
      {
         unlockLoop();
         break;
      }
      else
      {
         unlockLoop();
      }
      wiimote->HeartBeat(); // retrives data s from the device
      
      //if( modeOption.compare("laserpointer") ==0) {interactionmode = LASERPOINTER;}
      //else if( modeOption.compare("navigator")==0) {interactionmode = NAVIGATOR;}
      //else if( modeOption.compare("rotator") ==0) {interactionmode = ROTATOR;}
      //else interactionmode = NAVIGATOR; // default

      NodeVector::iterator it;
      for( it = sources.begin(); it != sources.end(); it++) 
      {
         WiiSource * source = (WiiSource*)((Node*) *it);
         source->lock();

         float wX,wY,wZ;
         float cX,cY,cZ;
         float sX,sY;
         static float irX,irY,irZ;
         float ir1X,ir1Y;
         float ir2X,ir2Y;
         wX =wY=wZ=cX=cY=cZ=sX=sY=ir1X=ir1Y=ir2X=ir2Y=0.f; // inizialization

         wiimote->GetCalibratedAcceleration(wX,wY,wZ);
         //         printf("W:[%+1.2f %+1.2f %+1.2f] ",wX,wY,wZ);

         if (wiimote->mNunchuckAttached)
         {
            wiimote->GetCalibratedChuckAcceleration(cX,cY,cZ);
            //            printf("N:[%+1.2f %+1.2f %+1.2f] ",cX,cY,cZ);

            wiimote->GetCalibratedChuckStick(sX,sY);
            //            printf("S:[%+1.2f %+1.2f] ",sX,sY);
         }

         // Starting to Reason about position: best data are absolute from Ir tracking

         irmode = NOINFRARED;
         if (wiimote->mIRRunning)
         {
            if (wiimote->GetIRP1(ir1X,ir1Y))
            {
               //printf("P1:[%+1.2f %+1.2f]",irX,irY);
               irX = ir1X;
               irY = ir1Y;
               irmode = INFRARED1;
            }
            if (wiimote->GetIRP2(ir2X,ir2Y))
            {
               const float refDistance = 0.2f;
               // get Baricenter Point
               irX = (ir1X + ir2X)*0.5;
               irY = (ir1Y + ir2Y)*0.5;
               float pointsDistance = sqrt(pow(ir1X-ir2X,2)+pow(ir1Y-ir2Y,2));
               irZ = pointsDistance- refDistance;

               //printf("P2:[%+1.2f %+1.2f]",irX,irY);
               if (irmode == INFRARED1) 
               {
                  irmode = INFRARED2;
               }
               else
               { 
                  irmode = INFRARED1;
                  irX = ir2X;
                  irY = ir2Y;
               }
            }
         }
         //else irmode = NOINFRARED; // default, we can count only on accelerations 

         // Populates the event 
         source->event.getPosition()[0] = 1.0-irX;
         source->event.getPosition()[1] = 1.0-irY;
         source->event.getPosition()[2] = 0;

         // consider accelerations
         float roll = 0.0;
         float pitch= 0.0;  
         float jaw =  0.0;
         float a_x = wX, a_y = wY, a_z = wZ;
         float acellerationModule = sqrt(pow(a_x,2)+pow(a_y,2)+pow(a_z,2));

         if (irmode == INFRARED2 ) // using roll from ir -seems imprecise
         {
            roll = atan2((ir1Y-ir2Y),(ir1X-ir2X)) * 180 / M_PI;
            //source->event.getPosition()[0] = roll;

         }
         else // using roll from accellerometers
         {
            roll = atan(a_x/a_z) * 180 / M_PI ;
            if (a_z <= 0.0) // invert roll if wii faces down
            {
               roll += M_PI * ((a_x > 0.0) ? 1 : -1);
            }
         }
         pitch = atan(a_y/a_z*cos(roll))* 180 / M_PI;
         //source->event.getPosition()[2] = pitch;

/*
         if ((acellerationModule > 0.85) && (acellerationModule < 1.15)) 
         {
         if ((fabs(roll)*(180/PI) > 10) && (fabs(pitch)*(180/PI) < 80)) 
         {
         //data.axes[2].valid = 1;
         data.axes[2].value = roll * 5 ;
         }
         else {
         //data.axes[2].valid = 0;
         }
         if (fabs(pitch)*(180/PI) > 10) {
         //data.axes[3].valid = 1;
         data.axes[3].value = pitch * 10 ;
         }
         else {
         //data.axes[3].valid = 0;
         }
         }
         else {
         //data.axes[2].valid = 0;
         //data.axes[3].valid = 0;
         }
  */       


         float rotationQuaternion[4];
         MathUtils::eulerToQuaternion (roll, pitch, 0.001f, rotationQuaternion);
         source->event.setOrientation(rotationQuaternion);

         // buttons
         source->event.getButton() = wiimote->GetLastButtonDataStatus();
         source->event.timeStamp();
         source->event.getConfidence() = 1.0;
         source->changed = 1;

         if (sendAttribites)
         {
            std::string irModeStr; 
            switch (irmode)
            {
            case (INFRARED1):
              irModeStr = "INFRARED1";
                  break;
            case (INFRARED2):
               irModeStr = "INFRARED2";
               break;
            default: 
            irModeStr = "NOINFRARED";
                        }
            source->event.setAttribute("IrMode:", irModeStr);
            std::string k_str = wiimote->GetbuttonstStatusString(); //retrieve as a string
            source->event.setAttribute("Buttons:", k_str);
            std::string k_str2 = wiimote->GetIRStatusString(); //retrieve as a string
            source->event.setAttribute("IR:", k_str2);
         }

         // Release lock
         source->unlock();

         if (context != NULL)   
         {  
            if (context->doSynchronization())
            {
               context->dataBroadcast();
               context->consumedWait();    
            } 
         }
         // the sink will update the observers
      }
   } // end while

}
void WiiModule::pushEvent() 
{
   /// nothing to do
}

// initializes the ARTDataTracker module

void WiiModule::init(StringTable& attributes, ConfigNode * localTree)
{
   using namespace std;
   ACE_DEBUG((LM_INFO, ACE_TEXT("WiiModule::init() \n")));

   ThreadModule::init( attributes, localTree );      

   irmode = NOINFRARED; // this is a sityem variable

   StringTable & localAttrib = localTree->getAttributes();
   const std::string & modeOption =localAttrib.get("mode");
   if( modeOption.compare("laserpointer") ==0) {interactionmode = LASERPOINTER;}
   else if( modeOption.compare("navigator")==0) {interactionmode = NAVIGATOR;}
   else if( modeOption.compare("rotator") ==0) {interactionmode = ROTATOR;}
   else interactionmode = NAVIGATOR; // default

   //enum IrMode{ NOINFRARED =0 ,INFRARED1,  INFRARED2};
   //enum InteractionMode{ LASERPOINTER =0 ,NAVIGATOR,  ROTATOR};


   initialized = 1;

   sendAttribites = true; // this could be to be into the setup
}

/*

void WiiModule::ir2mode(WiiSource * source )
{


}*/

OT_NAMESPACE_END

#endif //#ifdef USE_WII_SUPPORT

//#endif


/* 
* ------------------------------------------------------------
*   End of WiiModule.cxx
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
