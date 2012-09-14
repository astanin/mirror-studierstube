//WiiHandler 0.2 by Kevin Forbes (http://simulatedcomicproduct.com)
//This code is public domain, and comes with no warranty. The user takes full responsibility for anything that happens as a result from using this code.


// other important info on Wii remote http://www.wiili.org/index.php/Wiimote
// The Wiimote sends reports to the host with a maximum frequency of 100 reports per second. 

//The Wiimote does not require any authentication or encryption of the Bluetooth standard. 
// to interface with it, first put the controller into discoverable mode by pressing  1 and 2 buttons at the same time,
// or by pressing the red sync button under the battery cover. 
// Once in this mode, the Wiimote can be queried by the Bluetooth HID driver on the host.
// If the HID driver on the host does not connect to the Wiimote within 20 seconds, the Wiimote will turn itself off.
// Holding down the 1 and 2 buttons continuously will force the Wiimote to stay in discoverable mode without turning off. This does not work with the sync button, however.
// When in discoverable mode, the Player LEDs will blink.
// The number that blink will correspond to the remaining battery life.


#include "OpenTracker/input/WiiHandler.h"

#ifdef USE_WII_SUPPORT

#include <bitset>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <cassert>
#include <ace/Log_Msg.h>
#include <string>
#include <iostream>
#include <math.h>
#include <cstring>
#ifndef WIN32
#include <unistd.h>
#endif

namespace ot {
   //output channels

   //the ID values for a wiimote
   // Name  Nintendo RVL-CNT-01  
   const unsigned short mVendorID = 0x057e;
   const unsigned short mDeviceID = 0x0306;

   const unsigned char OUTPUT_CHANNEL_FORCE_FEEDBACK = 0x13;
   const unsigned char OUTPUT_CHANNEL_LED = 0x11;
   const unsigned char OUTPUT_CHANNEL_REPORT = 0x12;
   const unsigned char OUTPUT_READ_MEMORY = 0x17;
   const unsigned char OUTPUT_WRITE_MEMORY = 0x16;

   const unsigned char OUTPUT_ENABLE_IR = 0x13;
   const unsigned char OUTPUT_ENABLE_IR2 = 0x1a;

   //report request types
   const unsigned char REQUEST_CONTINUOUS_REPORTS = 0x4;
   const unsigned char REQUEST_SINGLE_REPORTS = 0x0;

   //input channels
   const unsigned char INPUT_CHANNEL_BUTTONS_ONLY = 0x30;
   const unsigned char INPUT_CHANNEL_BUTTONS_MOTION = 0x31;
   const unsigned char INPUT_CHANNEL_WRITE_CONFIRM = 0x22;
   const unsigned char INPUT_CHANNEL_EXPANSION_PORT = 0x20;

   const unsigned char INPUT_CHANNEL_MOTION_IR = 0x33;
   const unsigned char INPUT_CHANNEL_MOTION_CHUCK_IR = 0x37;
   const unsigned char INPUT_CHANNEL_MOTION_CHUCK = 0x35;

   //how to find the calibration data for the wiimote
   const unsigned short CALIBRATION_ADDRESS = 0x16;
   const unsigned short CALIBRATION_DATA_LENGTH = 7;

   //nunchuck constants
   const unsigned long NUNCHUCK_STATUS_ADDRESS = 0x04A40000;
   const unsigned long NUNCHUCK_CALIBRATION_ADDRESS = 0x04A40020;
   const unsigned long NUNCHUCK_CALIBRATION_ADDRESS_2 = 0x04A40030;
   const unsigned long NUNCHUCK_INIT_ADDRESS= 0x04A40040;
   const unsigned long NUNCHUK_ID_ADDRESS = 0x04a400f0;
   const unsigned char NUNCHUCK_INIT_VAL= 0x0;

   //IR constants
   const unsigned long IR_REG_1 = 0x04b00030;
   const unsigned long IR_REG_2 = 0x04b00033;
   const unsigned long IR_SENS_ADDR_1 = 0x04b00000;
   const unsigned long IR_SENS_ADDR_2 = 0x04b0001a;

   const unsigned char IR_SENS_MIDRANGE_PART1[] = {0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0xaa, 0x00, 0x64};
   const unsigned char IR_SENS_MIDRANGE_PART2[] = {0x63, 0x03};

   const unsigned char IR_MODE_OFF = 0;
   const unsigned char IR_MODE_STD = 1;
   const unsigned char IR_MODE_EXP = 3;
   const unsigned char IR_MODE_FULL = 5;

   WiiHandler::WiiHandler()
   {
      Init();
   }

   WiiHandler::~WiiHandler()
   {
      Disconnect();
   }

   void WiiHandler::Init()
   {
      mReportMode = REPORT_MODE_EVENT_BUTTONS;
      mLastButtonStatus.Init();
      mLastExpansionReport.Init();
      mLastMotionReport.Init();
      mOutputControls.Init();
      mReadInfo.Init();
      mAccelCalibrationData.Init();
      mNunchuckAccelCalibrationData.Init();
      mNunchuckStickCalibrationData.Init();
      mLastIRReport.Init();
      mNunchuckAttached = false;
      mIRRunning = false;
      mDataStreamRunning = false;

      deltajoy =0.01f; // mf add
   }

   bool WiiHandler::SetReportMode(eReportMode mode)
   {
      mReportMode = mode;
      return SendReportMode();
   }

   bool WiiHandler::SendReportMode()
   {
      bool continuous = true;
      unsigned char channel = INPUT_CHANNEL_BUTTONS_ONLY;
      bool check_chuck = false;

      switch (mReportMode)
      {
      case REPORT_MODE_MOTION_IR:
         channel = INPUT_CHANNEL_MOTION_IR;
         break;
      case REPORT_MODE_MOTION_CHUCK_IR:
         channel = INPUT_CHANNEL_MOTION_CHUCK_IR;
         check_chuck = true;
         break;
      case REPORT_MODE_MOTION_CHUCK:
         channel = INPUT_CHANNEL_MOTION_CHUCK;
         check_chuck = true;
         break;
      case REPORT_MODE_MOTION:
         channel = INPUT_CHANNEL_BUTTONS_MOTION;
         break;
      case REPORT_MODE_EVENT_BUTTONS:
         channel = INPUT_CHANNEL_BUTTONS_ONLY;
         continuous = false;
         break;
      default:
         break;
      }

      //check to make sure that there is a chuck attached
      if (check_chuck && !mNunchuckAttached)
      {
         return false;
      }

      bool retval = SelectInputChannel(continuous,channel);
      return retval;
   }

   bool WiiHandler::ConnectToDevice(int index)
   {
      Init();
      bool retval = false;
#ifdef WIN32
      retval = mHIDDevice.Connect(mDeviceID,mVendorID,index) && 
         SetReportMode(REPORT_MODE_EVENT_BUTTONS) && 
         UpdateOutput() &&
         ReadCalibrationData();
#else
#endif
      if (retval)
      {
         InitNunchuck();
      }
      return retval;
   }

   bool WiiHandler::Disconnect()
   {
      bool retval = false;
      StopDataStream();
#ifdef WIN32	
      if (mHIDDevice.IsConnected())
      {
         retval = mHIDDevice.Disconnect();
      }
#else
#endif

      return retval;
   }

   bool WiiHandler::SetVibration(bool vib_on)
   {
      bool retval = true;
      if (mOutputControls.mVibration != vib_on)
      {
         mOutputControls.mVibration = vib_on;
         retval = UpdateOutput();
      }
      return retval;
   }

   void WiiHandler::ClearBuffer()
   {
      memset(mOutputBuffer,0, mOutputBufferSize);
   }

   bool WiiHandler::SetLEDs(bool led1, bool led2, bool led3, bool led4)
   {
      const bool no_change = mOutputControls.mLED1 == led1 &&
         mOutputControls.mLED2 == led2 &&
         mOutputControls.mLED3 == led3 &&
         mOutputControls.mLED4 == led4;

      if (no_change)
      {
         return true;
      }

      mOutputControls.mLED1 = led1;
      mOutputControls.mLED2 = led2;	
      mOutputControls.mLED3 = led3;
      mOutputControls.mLED4 = led4;
      return UpdateOutput();
   }

   bool WiiHandler::UpdateOutput()
   {
      ClearBuffer();
      mOutputBuffer[0] = OUTPUT_CHANNEL_LED;
      mOutputBuffer[1] =  (mOutputControls.mVibration ? 0x1 : 0x0) |
         (mOutputControls.mLED1 ? 0x1 : 0x0) << 4 | 
         (mOutputControls.mLED2 ? 0x1 : 0x0) << 5 | 
         (mOutputControls.mLED3 ? 0x1 : 0x0) << 6 | 
         (mOutputControls.mLED4 ? 0x1 : 0x0) << 7; 
      bool retval = false;
#ifdef WIN32	
      retval = mHIDDevice.WriteToDevice(mOutputBuffer,mOutputBufferSize);
#else

#endif
      return retval;
   }

   bool WiiHandler::HeartBeat(int timeout)
   {
      bool retval = true;



      //most of these reports aren't implemented yet. I don't have a sensor bar or a nunchuck :)
#ifdef WIN32
      int bytes_read = 0;
      if (mHIDDevice.ReadFromDevice(mInputBuffer,mInputBufferSize,bytes_read) && bytes_read > 0,timeout)
      {
         const int channel = mInputBuffer[0];
         switch (channel)
         {

         case INPUT_CHANNEL_EXPANSION_PORT:// 	 6 	Expansion Port change
            {
               ParseButtonReport(&mInputBuffer[1]);
               ParseExpansionReport(&mInputBuffer[2]);
               bool restart = mDataStreamRunning;
               StopDataStream();
               InitNunchuck();

               if (restart)
               {
                  retval = StartDataStream();
               }
            }
            break;

         case INPUT_CHANNEL_BUTTONS_ONLY:// 	2 	Buttons only
            ParseButtonReport(&mInputBuffer[1]);
            break;

         case 0x21:// 	21 	Read data
            ParseButtonReport(&mInputBuffer[1]);
            ParseReadData(&mInputBuffer[3]);
            break;

         case INPUT_CHANNEL_WRITE_CONFIRM:// 	4 	Write data
            break;

         case 0x31:// 	5 	Buttons | Motion Sensing Report
            ParseButtonReport(&mInputBuffer[1]);
            ParseMotionReport(&mInputBuffer[3]);
            break;

         case 0x32:// 	16 	Buttons | Expansion Port | IR??
            ParseButtonReport(&mInputBuffer[1]);
            break;

         case INPUT_CHANNEL_MOTION_IR:
            ParseButtonReport(&mInputBuffer[1]);
            ParseMotionReport(&mInputBuffer[3]);
            ParseIRReport(&mInputBuffer[6]);
            break;

         case INPUT_CHANNEL_MOTION_CHUCK_IR:
            ParseButtonReport(&mInputBuffer[1]);
            ParseMotionReport(&mInputBuffer[3]);
            ParseIRReport(&mInputBuffer[6]);
            ParseChuckReport(&mInputBuffer[16]);
            break;

         case INPUT_CHANNEL_MOTION_CHUCK:
            ParseButtonReport(&mInputBuffer[1]);
            ParseMotionReport(&mInputBuffer[3]);
            ParseChuckReport(&mInputBuffer[6]);

            break;

         case 0x34:// 	21 	Buttons | Expansion Port | IR??
         case 0x3d:// 	21 	Buttons | Expansion Port | IR??
            ParseButtonReport(&mInputBuffer[1]);
            break;

         case 0x3e:// 	21 	Buttons | Motion Sensing Report | IR??
         case 0x3f:// 	21 	Buttons | Motion Sensing Report | IR??
            ParseButtonReport(&mInputBuffer[1]);
            break;
         default:
            retval = false;
            //unknown report
            break;

         }

      }
#else

#endif
      return retval;
   }

   void WiiHandler::ParseExpansionReport(const unsigned char *data)
   {
      //four bytes long
      mLastExpansionReport.mAttachmentPluggedIn = (data[0] & 0x02) != 0;
      mLastExpansionReport.mIREnabled = (data[0] & 0x08) != 0;
      mLastExpansionReport.mSpeakerEnabled = (data[0] & 0x04) != 0;
      mLastExpansionReport.mLED1On = (data[0] & 0x10) != 0;
      mLastExpansionReport.mLED2On = (data[0] & 0x20) != 0;
      mLastExpansionReport.mLED3On = (data[0] & 0x40) != 0;
      mLastExpansionReport.mLED4On = (data[0] & 0x80) != 0;

      //two unknown bytes
      mLastExpansionReport.mBatteryLevel = data[3];
   }

   void WiiHandler::ParseButtonReport(const unsigned char * data)
   {
      //two bytes long
      mLastButtonStatus.mA = (data[1] & 0x08) != 0;
      mLastButtonStatus.mB = (data[1] & 0x04) != 0;
      mLastButtonStatus.m1 = (data[1] & 0x02) != 0;
      mLastButtonStatus.m2 = (data[1] & 0x01) != 0;
      mLastButtonStatus.mPlus = (data[0] & 0x10) != 0;
      mLastButtonStatus.mMinus = (data[1] & 0x10) != 0;
      mLastButtonStatus.mHome = (data[1] & 0x80) != 0;
      mLastButtonStatus.mUp = (data[0] & 0x08) != 0;
      mLastButtonStatus.mDown = (data[0] & 0x04) != 0;
      mLastButtonStatus.mLeft = (data[0] & 0x01) != 0;
      mLastButtonStatus.mRight = (data[0] & 0x02) != 0;

      static const int bitBufferlenght = sizeof(unsigned short)*8;
      static std::bitset<bitBufferlenght> buttonstateBitset;
      buttonstateBitset.set(0, mLastButtonStatus.mA);
      buttonstateBitset.set(1, mLastButtonStatus.mB);
      buttonstateBitset.set(2, mLastButtonStatus.m1);
      buttonstateBitset.set(3, mLastButtonStatus.m2);
      buttonstateBitset.set(4, mLastButtonStatus.mPlus);
      buttonstateBitset.set(5, mLastButtonStatus.mMinus);
      buttonstateBitset.set(6, mLastButtonStatus.mHome);
      buttonstateBitset.set(7, mLastButtonStatus.mUp);
      buttonstateBitset.set(8, mLastButtonStatus.mDown);
      buttonstateBitset.set(9, mLastButtonStatus.mRight);
      buttonstateBitset.set(10, mLastButtonStatus.mLeft);
      mLastButtonStatus.outdata = buttonstateBitset.to_ulong();
  
  }
   void WiiHandler::ParseMotionReport(const unsigned char * data)
   {
      //three bytes long
      mLastMotionReport.mX = data[0];
      mLastMotionReport.mY = data[1];
      mLastMotionReport.mZ = data[2];
   }

   void WiiHandler::PrintStatus() const
   {
      float wX,wY,wZ;
      float cX,cY,cZ;
      float sX,sY;
      float irX,irY;

      wX =wY=wZ=cX=cY=cZ=sX=sY=irX=irY=0.f;

      GetCalibratedAcceleration(wX,wY,wZ);
      printf("W:[%+1.2f %+1.2f %+1.2f] ",wX,wY,wZ);

      if (mNunchuckAttached)
      {
         GetCalibratedChuckAcceleration(cX,cY,cZ);
         printf("N:[%+1.2f %+1.2f %+1.2f] ",cX,cY,cZ);

         GetCalibratedChuckStick(sX,sY);
         printf("S:[%+1.2f %+1.2f] ",sX,sY);
      }

      if (mIRRunning)
      {
         if (GetIRP1(irX,irY))
         {
            printf("P1:[%+1.2f %+1.2f]",irX,irY);
         }
         if (GetIRP2(irX,irY))
         {
            printf("P2:[%+1.2f %+1.2f]",irX,irY);
         }
      }
 printf(GetbuttonstStatusString().c_str());

/*
            //print the button status
            if (mLastButtonStatus.m1)
               printf("1");
            if (mLastButtonStatus.m2)
               printf("2");
            if (mLastButtonStatus.mA)
               printf("A");
            if (mLastButtonStatus.mB)
               printf("B");
            if (mLastButtonStatus.mPlus)
               printf("+");
            if (mLastButtonStatus.mMinus)
               printf("-");
            if (mLastButtonStatus.mUp)
               printf("U");
            if (mLastButtonStatus.mDown)
               printf("D");
            if (mLastButtonStatus.mLeft)
               printf("L");
            if (mLastButtonStatus.mRight)
               printf("R");
            if (mLastButtonStatus.mHome)
               printf("H");
      
            if (mNunchuckAttached)
            {
               if (mLastChuckReport.mButtonZ)
                  printf("Z");
               if (mLastChuckReport.mButtonC)
                  printf("C");
            }
      */
            printf("\n");
      

   }


   bool WiiHandler::SelectInputChannel(bool continuous, unsigned char channel)
   {
      ClearBuffer();
      mOutputBuffer[0] = OUTPUT_CHANNEL_REPORT;
      mOutputBuffer[1] = (continuous ? REQUEST_CONTINUOUS_REPORTS : REQUEST_SINGLE_REPORTS) | (mOutputControls.mVibration ? 0x1 : 0x0);
      mOutputBuffer[2] = channel;
      bool retval = false;
#ifdef WIN32
      retval = mHIDDevice.WriteToDevice(mOutputBuffer,mOutputBufferSize);
#else

#endif
      return retval;
   }


   //this may or may not work to read buffers greater than 16 bytes. . . .
   bool WiiHandler::IssueReadRequest(unsigned int address, unsigned short size, unsigned char * buffer)
   {
      bool retval = false;
      if (mReadInfo.mReadStatus != tMemReadInfo::READ_PENDING)
      {
         ClearBuffer();
         mOutputBuffer[0] = OUTPUT_READ_MEMORY;
         mOutputBuffer[1] = (((address & 0xff000000) >> 24) & 0xFE) | (mOutputControls.mVibration ? 0x1 : 0x0);
         mOutputBuffer[2] = (address & 0x00ff0000) >> 16;
         mOutputBuffer[3] = (address & 0x0000ff00) >> 8;
         mOutputBuffer[4] = (address & 0xff);

         mOutputBuffer[5] = (size & 0xff00) >> 8;
         mOutputBuffer[6] = (size & 0xff);

#ifdef WIN32		
         if (mHIDDevice.WriteToDevice(mOutputBuffer,mOutputBufferSize))
         {
            mReadInfo.mReadStatus = tMemReadInfo::READ_PENDING;
            mReadInfo.mReadBuffer = buffer;
            mReadInfo.mTotalBytesToRead = size;
            mReadInfo.mBytesRead =0;
            mReadInfo.mBaseAddress = (unsigned short)(address & 0xFFFF);
            retval = true;
         }
#else
#endif
      }

      return retval;
   }

   void WiiHandler::ParseReadData(const unsigned char * data)
   {
      if(mReadInfo.mReadStatus == tMemReadInfo::READ_PENDING)
      {
         const bool error = (data[0] & 0x0F) != 0;
         if (error)
         {
            mReadInfo.mReadStatus = tMemReadInfo::READ_ERROR;
         }
         else
         {
            unsigned char bytes = (data[0] >> 4)+1;
            unsigned short offset = ((unsigned short)data[1] << 8) + data[2];
            unsigned int space_left_in_buffer = mReadInfo.mTotalBytesToRead -  mReadInfo.mBytesRead;
            if (offset == mReadInfo.mBytesRead + mReadInfo.mBaseAddress &&
               space_left_in_buffer >= bytes)
            {
               memcpy(&mReadInfo.mReadBuffer[mReadInfo.mBytesRead],&data[3],bytes);

               mReadInfo.mBytesRead+= bytes;
               if (mReadInfo.mBytesRead >= mReadInfo.mTotalBytesToRead)
               {
                  mReadInfo.mReadStatus = tMemReadInfo::READ_COMPLETE;
               }
            }
         }
      }

   }

   bool WiiHandler::ReadData(unsigned int address, unsigned short size, unsigned char * buffer)
   {
      if (IssueReadRequest(address, size,buffer))
      {
         while (mReadInfo.mReadStatus == tMemReadInfo::READ_PENDING)
         {
            if (!HeartBeat(1000))
            {
               break;
            }
         }
      }
      return mReadInfo.mReadStatus == tMemReadInfo::READ_COMPLETE;
   }

   bool WiiHandler::ReadCalibrationData()
   {
      bool retval = false;
      unsigned char buffer[CALIBRATION_DATA_LENGTH];
      if (ReadData(CALIBRATION_ADDRESS, CALIBRATION_DATA_LENGTH,buffer))
      {
         mAccelCalibrationData.mXZero = buffer[0];
         mAccelCalibrationData.mYZero = buffer[1];
         mAccelCalibrationData.mZZero = buffer[2];
         mAccelCalibrationData.mXG = buffer[4];
         mAccelCalibrationData.mYG = buffer[5];
         mAccelCalibrationData.mZG = buffer[6];
         retval = true;
      }

      return retval;
   }

   void WiiHandler::GetCalibratedAcceleration(float & x, float & y, float &z) const
   {
      x = (mLastMotionReport.mX - mAccelCalibrationData.mXZero) / (float)(mAccelCalibrationData.mXG- mAccelCalibrationData.mXZero);
      y = (mLastMotionReport.mY - mAccelCalibrationData.mYZero) / (float)(mAccelCalibrationData.mYG- mAccelCalibrationData.mYZero);
      z = (mLastMotionReport.mZ - mAccelCalibrationData.mZZero) / (float)(mAccelCalibrationData.mZG- mAccelCalibrationData.mZZero);
   }

   void WiiHandler::GetCalibratedChuckAcceleration(float & x, float & y, float &z) const
   {
      if (!mNunchuckAttached)
      {
         x = y = z = 0.f;
         return;
      }
      x = (mLastChuckReport.mAccelX - mNunchuckAccelCalibrationData.mXZero) / (float)(mNunchuckAccelCalibrationData.mXG- mNunchuckAccelCalibrationData.mXZero);
      y = (mLastChuckReport.mAccelY - mNunchuckAccelCalibrationData.mYZero) / (float)(mNunchuckAccelCalibrationData.mYG- mNunchuckAccelCalibrationData.mYZero);
      z = (mLastChuckReport.mAccelZ - mNunchuckAccelCalibrationData.mZZero) / (float)(mNunchuckAccelCalibrationData.mZG- mNunchuckAccelCalibrationData.mZZero);
   }
   void WiiHandler::GetCalibratedChuckStick(float & x, float & y) const
   {
      if (!mNunchuckAttached)
      {
         x = y = 0.f;
         return;
      }

      if (mLastChuckReport.mStickX < mNunchuckStickCalibrationData.mXmid)
      {
         x = ((mLastChuckReport.mStickX - mNunchuckStickCalibrationData.mXmin) / (float)(mNunchuckStickCalibrationData.mXmid - mNunchuckStickCalibrationData.mXmin)) -  1.f;
      }
      else
      {
         x = ((mLastChuckReport.mStickX - mNunchuckStickCalibrationData.mXmid) / (float)(mNunchuckStickCalibrationData.mXmax - mNunchuckStickCalibrationData.mXmid));
      }

      if (mLastChuckReport.mStickY < mNunchuckStickCalibrationData.mYmid)
      {
         y = ((mLastChuckReport.mStickY - mNunchuckStickCalibrationData.mYmin) / (float)(mNunchuckStickCalibrationData.mYmid - mNunchuckStickCalibrationData.mYmin)) -  1.f;
      }
      else
      {
         y = ((mLastChuckReport.mStickY - mNunchuckStickCalibrationData.mYmid) / (float)(mNunchuckStickCalibrationData.mYmax - mNunchuckStickCalibrationData.mYmid));
      }

      // nunchuck drift correction mf
      if (fabs(x) < deltajoy) x = 0.0f;
      if (fabs(y) < deltajoy) y = 0.0f;
   }


   bool WiiHandler::WriteMemory(unsigned int address, unsigned char size, const unsigned char * buffer)
   {
      bool retval = false;
      if (size <= 16)
      {
         ClearBuffer();
         mOutputBuffer[0] = OUTPUT_WRITE_MEMORY;
         mOutputBuffer[1] = (address & 0xff000000) >> 24 | (mOutputControls.mVibration ? 0x1 : 0x0);
         mOutputBuffer[2] = (address & 0x00ff0000) >> 16;
         mOutputBuffer[3] = (address & 0x0000ff00) >> 8;
         mOutputBuffer[4] = (address & 0xff);
         mOutputBuffer[5] = size;
         memcpy(&mOutputBuffer[6],buffer,size);
#ifdef WIN32
         retval = mHIDDevice.WriteToDevice(mOutputBuffer,mOutputBufferSize);
#else
#endif

      }

      return retval;
   }

   bool WiiHandler::InitNunchuck()
   {

      bool retval = false;

      //first init the nunchuck, if it is present
      if (WriteMemory(NUNCHUCK_INIT_ADDRESS,1,&NUNCHUCK_INIT_VAL))
      {

         unsigned char buffer[16];
         //now try to read the nunchuck's calibration data
         if (ReadData(NUNCHUCK_CALIBRATION_ADDRESS,16,buffer))
         {

            //note that this hasn't worked properly for me yet (I get all 0xff). 
            /*mNunchuckAccelCalibrationData.mXZero = NunChuckByte(buffer[0]);
            mNunchuckAccelCalibrationData.mYZero = NunChuckByte(buffer[1]);
            mNunchuckAccelCalibrationData.mZZero = NunChuckByte(buffer[2]);

            mNunchuckAccelCalibrationData.mXG = NunChuckByte(buffer[4]);
            mNunchuckAccelCalibrationData.mYG = NunChuckByte(buffer[5]);
            mNunchuckAccelCalibrationData.mZG = NunChuckByte(buffer[6]);

            mNunchuckStickCalibrationData.mXmax = NunChuckByte(buffer[8]);
            mNunchuckStickCalibrationData.mXmin = NunChuckByte(buffer[9]);
            mNunchuckStickCalibrationData.mXmid = NunChuckByte(buffer[10]);
            mNunchuckStickCalibrationData.mYmax = NunChuckByte(buffer[11]);
            mNunchuckStickCalibrationData.mYmin = NunChuckByte(buffer[12]);
            mNunchuckStickCalibrationData.mYmid = NunChuckByte(buffer[13]);*/

            //these are default values from the wiili wiki
            mNunchuckAccelCalibrationData.mXZero = 0x7E;
            mNunchuckAccelCalibrationData.mYZero = 0x7A;
            mNunchuckAccelCalibrationData.mZZero = 0x7D;
            mNunchuckAccelCalibrationData.mXG = 0xB0;
            mNunchuckAccelCalibrationData.mYG = 0xAF;
            mNunchuckAccelCalibrationData.mZG = 0xB1;
            mNunchuckStickCalibrationData.mXmax = 0xe5;
            mNunchuckStickCalibrationData.mXmin = 0x21;
            mNunchuckStickCalibrationData.mXmid =  0x7c;
            mNunchuckStickCalibrationData.mYmax = 0xe7;
            mNunchuckStickCalibrationData.mYmin =  0x23;
            mNunchuckStickCalibrationData.mYmid = 0x7a;
            retval = true;

         }
      }
      mNunchuckAttached = retval;
      return retval;
   }

   void WiiHandler::ParseChuckReport(const unsigned char * data)
   {
      mLastChuckReport.mStickX = NunChuckByte(data[0]);
      mLastChuckReport.mStickY = NunChuckByte(data[1]);
      mLastChuckReport.mAccelX = NunChuckByte(data[2]);
      mLastChuckReport.mAccelY = NunChuckByte(data[3]);
      mLastChuckReport.mAccelZ = NunChuckByte(data[4]);
      mLastChuckReport.mButtonC = (NunChuckByte(data[5]) & 0x2) == 0;
      mLastChuckReport.mButtonZ = (NunChuckByte(data[5]) & 0x1) == 0;
   }

   bool WiiHandler::EnableIR()
   {
      bool retval = false;

      DisableIR();
#ifdef WIN32
      Sleep(10);
#else
      usleep(10);
#endif

      if (!mIRRunning)
      {
         ClearBuffer();
         mOutputBuffer[0] = OUTPUT_ENABLE_IR;
         mOutputBuffer[1] = 0x4 | (mOutputControls.mVibration ? 0x1 : 0x0);
#ifdef WIN32
         retval = mHIDDevice.WriteToDevice(mOutputBuffer,mOutputBufferSize);
#else
#endif

#ifdef WIN32
         Sleep(10);
#else
         usleep(10);
#endif
         if (retval)
         {
            mOutputBuffer[0] = OUTPUT_ENABLE_IR2;
            mOutputBuffer[1] = 0x4 | (mOutputControls.mVibration ? 0x1 : 0x0);
#ifdef WIN32
            retval = mHIDDevice.WriteToDevice(mOutputBuffer,mOutputBufferSize);
#else
#endif
         }
#ifdef WIN32
         Sleep(10);
#else
         usleep(10);
#endif
         if (retval)
         {
            unsigned char val = 0x1;
            retval = WriteMemory(IR_REG_1,1,&val);
         }
#ifdef WIN32
         Sleep(10);
#else
         usleep(10);
#endif
         if (retval)
         {
            retval = WriteMemory(IR_SENS_ADDR_1,9,IR_SENS_MIDRANGE_PART1);
         }
#ifdef WIN32	       
         Sleep(10);
#else
         usleep(10);
#endif
         if (retval)
         {
            retval = WriteMemory(IR_SENS_ADDR_2,2,IR_SENS_MIDRANGE_PART2);

         }
#ifdef WIN32
         Sleep(10);
#else
         usleep(10);
#endif


         if (retval)
         {
            retval = WriteMemory(IR_REG_2,1,&IR_MODE_EXP);
         }
#ifdef WIN32
         Sleep(10);
#else
         usleep(10);
#endif
         if (retval)
         {
            unsigned char val = 0x8;
            retval = WriteMemory(IR_REG_1,1,&val);
         }
#ifdef WIN32
         Sleep(10);
#else
         usleep(10);
#endif

         mIRRunning = retval;
      }
      return retval;

   }

   bool WiiHandler::DisableIR()
   {
      bool retval = false;

      if (mIRRunning)
      {
         ClearBuffer();
         mOutputBuffer[0] = OUTPUT_ENABLE_IR;
         mOutputBuffer[1] = (mOutputControls.mVibration ? 0x1 : 0x0);
#ifdef WIN32
         retval = mHIDDevice.WriteToDevice(mOutputBuffer,mOutputBufferSize);
#else
#endif

         if (retval)
         {
            mOutputBuffer[0] = OUTPUT_ENABLE_IR2;
            mOutputBuffer[1] = (mOutputControls.mVibration ? 0x1 : 0x0);
#ifdef WIN32
            retval = mHIDDevice.WriteToDevice(mOutputBuffer,mOutputBufferSize);
#else
#endif
         }

         mIRRunning = false;
      }
      return retval;

   }

   void WiiHandler::ParseIRReport(const unsigned char * data)
   {
      /* original code
      mLastIRReport.mP1X = data[0] << 2 | (data[2] & 0x30) >>4;
      mLastIRReport.mP1Y = data[1] << 2 | (data[2] & 0xc0) >>6;
      mLastIRReport.mP1Size = data[2] & 0xf;

      mLastIRReport.mP2X = data[3] << 2 | (data[5] & 0x30) >>4;
      mLastIRReport.mP2Y = data[4] << 2 | (data[5] & 0xc0) >>6;
      mLastIRReport.mP2Size = data[5] & 0xf;

      mLastIRReport.mP1X = ((int)data[2] & 0x30) << 4 | (int)data[0];
      mLastIRReport.mP1Y = ((int)data[2] & 0xC0) << 2 | (int)data[1];
      mLastIRReport.mP1Size = data[2] & 0xf;

      mLastIRReport.mP2X = ((int)data[5] & 0x30) << 4 | (int)data[3];
      mLastIRReport.mP2Y = ((int)data[5] & 0xC0) << 2 | (int)data[4];
      mLastIRReport.mP2Size = data[5] & 0xf;
      */

      mLastIRReport.mP1X = (data[2] & 0x30) << 4 | data[0];
      mLastIRReport.mP1Y = (data[2] & 0xC0) << 2 | data[1];
      mLastIRReport.mP1Size = data[2] & 0xf;

      mLastIRReport.mP2X = (data[2] & 0x03) << 4 | data[3];
      mLastIRReport.mP2Y = (data[2] & 0x0C) << 2 | data[4];
      mLastIRReport.mP2Size = data[5] & 0xf;

      mLastIRReport.mP1Found =  !(data[0] == 0xff && data[1] == 0xff && data[2] == 0xff);
      mLastIRReport.mP2Found =  !(data[3] == 0xff && data[4] == 0xff && data[5] == 0xff);

   }

   bool WiiHandler::GetIRP1(float &x, float &y) const
   {
      bool retval = false;
      if (mIRRunning && mLastIRReport.mP1Found)
      {
         x = mLastIRReport.mP1X / 1024.f;
         y = mLastIRReport.mP1Y / 1024.f;
         retval = true;
      }
      return retval;
   }


   bool WiiHandler::GetIRP2(float &x, float &y) const
   {
      bool retval = false;
      if (mIRRunning && mLastIRReport.mP2Found)
      {
         x = mLastIRReport.mP2X / 1024.f;
         y = mLastIRReport.mP2Y / 1024.f;
         retval = true;
      }
      return retval;
   }

   bool WiiHandler::StartDataStream()
   {
      bool retval = false;

      StopDataStream();

      if (mNunchuckAttached)
      {
         retval =SetReportMode(REPORT_MODE_MOTION_CHUCK_IR);
      }
      else
      {
         retval = SetReportMode(REPORT_MODE_MOTION_IR);
      }
      EnableIR();

      mDataStreamRunning = retval;
      return retval;
   }


   bool WiiHandler::StopDataStream()
   {
      if (mDataStreamRunning)
      {
         mDataStreamRunning = false;
         DisableIR();
         SetReportMode(REPORT_MODE_EVENT_BUTTONS);
      }
      return true;;
   }


   const unsigned short WiiHandler::GetLastButtonDataStatus() const
   {

      return mLastButtonStatus.outdata;
   }


const std::string WiiHandler::GetbuttonstStatusString() const
{
   std::stringstream os;
   //print the button status
   if (mLastButtonStatus.m1) os<< "1";
   if (mLastButtonStatus.m2) os<< "2";
   if (mLastButtonStatus.mA) os<< "A";
   if (mLastButtonStatus.mB) os<< "B";
   if (mLastButtonStatus.mPlus) os<< "+";
   if (mLastButtonStatus.mMinus) os<< "-";
   if (mLastButtonStatus.mUp) os<< "U";
   if (mLastButtonStatus.mDown) os<< "D";
   if (mLastButtonStatus.mLeft) os<< "L";
   if (mLastButtonStatus.mRight) os<< "R";
   if (mLastButtonStatus.mHome) os<< "H";

   if (mNunchuckAttached)
   {
      if (mLastChuckReport.mButtonZ) os<< "Z";
      if (mLastChuckReport.mButtonC) os<< "C";
   }

return os.str();
}

const std::string WiiHandler::GetIRStatusString() const
{
   std::stringstream os;
   float irX,irY;
   if (GetIRP1(irX,irY))
   {
      os<<"P1:"<<irX<<","<<irY<<" ";
   }
   if (GetIRP2(irX,irY))
   {
      os<<"P2:"<<irX<<","<<irY<<" ";
   }
return os.str();
}

} // end namespace ot {

#endif //#ifdef USE_WII_SUPPORT

/*
Anonymous said... 
I have a few bug fixes
First , with IR put in a few Sleep(10)s into the EnableIR function 
it would enable the ir more successfully (ie not fail for me any more)

Next you were extracting the ir numbers somewhat incorrectly: instead of (for x1):

data[0] << 2 | (data[2] & 0x30) >> 4

it should be:

(data[2] & 0x30) << 4 | data[0]

so ParseIRReport should end up looking like:

mLastIRReport.mP1X = ((int)data[2] & 0x30) << 4 | (int)data[0];
mLastIRReport.mP1Y = ((int)data[2] & 0xC0) << 2 | (int)data[1];
mLastIRReport.mP1Size = data[2] & 0xf;

mLastIRReport.mP2X = ((int)data[5] & 0x30) << 4 | (int)data[3];
mLastIRReport.mP2Y = ((int)data[5] & 0xC0) << 2 | (int)data[4];
mLastIRReport.mP2Size = data[5] & 0xf;     

-DONE mf

Another, little hack, my nunchuck was reading action on the joystick even in its resting state so

if (fabs(x) < delta) x = 0.0f;
if (fabs(y) < delta) y = 0.0f;            
-DONE mf

just so it is disregarded if the absolute value of the read input is less than a set delta value.

Finally, in order to poll for buttons, motion, ir, and nunchuck (0x37) it is expecting 2 bytes 
for the buttons, 3 for the accelerometers, 10 bytes for ir and 6 for extension (nunchuck)
With the current coding of cWiiMote parseirreport is expecting 12 bytes for the ir input. 
It needs to go back to standard/basic mode instead of extended.

Thus it (ParseIrReport) should end up looking like this:

mLastIRReport.mP1X = (data[2] & 0x30) << 4 | data[0];
mLastIRReport.mP1Y = (data[2] & 0xC0) << 2 | data[1];

mLastIRReport.mP2X = (data[2] & 0x03) << 4 | data[3];
mLastIRReport.mP2Y = (data[2] & 0x0C) << 2 | data[4];
-DONE mf

Hopefully this message comes through ok.

*/

/* 
* ------------------------------------------------------------
*   End of WiiHandler.cpp
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
