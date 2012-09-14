/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
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
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The source file for the SpectecSrc class.
 *
 * @author Daniel Wagner
 * 
 * $Id: SpectecSrc.cxx 232 2007-03-13 11:04:16Z bornik $
 * @file                                                                   
 * ======================================================================= */


#include <openvideo/nodes/SpectecSrc.h>
#include <openvideo/openVideo.h>

#ifdef ENABLE_SPECTECSRC

#include <openvideo/Manager.h>
#include <openvideo/State.h>

#include <windows.h>
#include <stdio.h>

#include <Spectec/vcsdapi.h>
#include <Spectec/ViSDIoctls.h>
#include <commctrl.h>
#include <aygshell.h>
#include <sipapi.h>
#include <winioctl.h>


#define VSD_DEVNAME TEXT("VSD1:")


using namespace openvideo;


SpectecSrc::SpectecSrc()
{
  state = new State();
  width = 320;		// default value
  height = 240;		// default value
  img1 = img2 = NULL;

  // these values will be read from registry,
  // so we can adjust our settings with the
  // PhotoView application...
  //
  VcBacklight = 0;
  VcBrightness = 63;
  VcInvert = 1;
  VcMirror = 0;
  VcSaturation = 125;
  VcSharpness = 125;
  VcContrast = 125;
  VcGamma = 120;

  hVSD = INVALID_HANDLE_VALUE; // the device

  didInit = false;
}


SpectecSrc::~SpectecSrc()
{
  delete state;
  delete img1;
  delete img2;
}





void SpectecSrc::init()
{
  return;


  printf("OV: SpectecSrc -> start\n");
  state->clear();

  state->width=width;
  state->height=height;


  //////////////////////////////////////////////////////////////////////////
  //
  //             Init Spectec SD Camera BEGIN
  //

  // this DLL is needed, although we don't call any functions directly
  //
  const bool checkDllBeforeStart = true;

  if(checkDllBeforeStart)
    {
      // Spectec SD Camera driver DLL called ViSDCam.dll under WM2003
      //
      HMODULE hMod = ::LoadLibrary(_T("ViSDCam.dll"));

      // Spectec SD Camera driver DLL called vsdc.dll under WM2005
      //
      if(!hMod)
        hMod = ::LoadLibrary(_T("vsdc.dll"));

      if(!hMod)
        {
          //log("ERROR: missing 'ViSDCam.dll' for Spectec camera");
          return;
        }

      // handle was only used for testing presence of DLL. no longer needed...
      ::FreeLibrary(hMod);
    }


  hVSD = CreateFile(VSD_DEVNAME,
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL, 
                    OPEN_EXISTING, 
                    0, 
                    NULL);

  if(hVSD==INVALID_HANDLE_VALUE)
    {
      //log("ERROR: initializing Spectec camera failed");
      return;
    }

  //log("INFO: Spectec camera initialized");


  if(readConfigFromRegistry())
    {
      //		log("INFO: read Spectec camera settings");
    }
  else
    {
      //		log("WARNING: no Spectec camera settings found");
    }


  // set values to camera
  //
  if ( hVSD != INVALID_HANDLE_VALUE) {
    // set up the camera
    VC_SetControlValue(VC_CTRL_INVERT, VcInvert, 2);
    VC_SetControlValue(VC_CTRL_MIRROR, VcMirror, 2);
    VC_SetControlValue(VC_CTRL_BRIGHTNESS, VcBrightness, 2);
    VC_SetControlValue(VC_CTRL_CONTRAST, VcContrast, 2);
    VC_SetControlValue(VC_CTRL_WHITE_BALANCE, VC_CTRL_WHITE_BALANCE_AUTO, 2);
    VC_SetControlValue(VC_CTRL_SHARPNESS, VcSharpness, 2);
    VC_SetControlValue(VC_CTRL_SATURATION, VcSaturation, 2);		  			
    VC_SetControlValue(VC_CTRL_BACKLIGHT, VcBacklight, 2);
    VC_SetControlValue(VC_CTRL_GAMMA, VcGamma, 2);
    VC_SetControlValue(VC_CTRL_IMAGE_QUALITY, 95, 2);
    VC_SetControlValue(VC_CTRL_AGC, VC_CTRL_AGC_OFF, 2);

    VC_SetControlValue(VC_CTRL_CAPTURE, 0, 2);

    VC_SetControlValue(VC_CTRL_RESOLUTION_DEST_HORZ, width, 2);
    VC_SetControlValue(VC_CTRL_RESOLUTION_DEST_VERT, height, 2);
    VC_SetControlValue(VC_CTRL_RESOLUTION_SRC_HORZ, width, 2);
    VC_SetControlValue(VC_CTRL_RESOLUTION_SRC_VERT, height, 2);
    VC_SetControlValue(VC_CTRL_AOI_HORZ_OFFSET, 0, 2);
    VC_SetControlValue(VC_CTRL_AOI_VERT_OFFSET, 0, 2);
    VC_SetControlValue(VC_CTRL_FORMAT, VC_CTRL_FORMAT_RGB565, 2);

    VC_SetControlValue(VC_CTRL_CAPTURE, 1, 2);	        
  }

  //
  //             Init Spectec SD Camera END
  //
  //////////////////////////////////////////////////////////////////////////


  img1 = new unsigned short[width*height];
  memset(img1, 255, width*height*2);

  if(doublebuffer)
    {
      img2 = new unsigned short[width*height];
      memset(img2, 255, width*height*2);
    }

  imgActive = img1;

  didInit = true;
}


void 
SpectecSrc::process()
{
  /*
    ULONG   bytesRead;

    if(doublebuffer)
    imgActive = (imgActive==img1 ? img2 : img1);

    // waits for the next image to become available
    //
    prepareImageReading();

    int rc = ReadFile( hVSD, imgActive, width*height*2, &bytesRead, NULL );
  */

  state->frame = reinterpret_cast<unsigned char*>(imgActive);
}


void
SpectecSrc::prepareImageReading()
{
  int retLen=-1, rc, sleeps=0;

  dataAvailable = 0;

  while((rc = VC_GetControlValue(VC_CTRL_IMAGE_DATA_AVAIL, &dataAvailable, 2, &retLen))!=0)
    {
      Sleep(3);
      sleeps++;
      if(sleeps>100)
        return;
    }
}


bool
SpectecSrc::readConfigValues(void* nHKey)
{
  DWORD valueSize;
  DWORD valueType;
  bool ret = true;
  HKEY hKey = (HKEY)nHKey;

  valueSize = sizeof(DWORD);
  if(RegQueryValueEx( hKey, L"Backlight",  NULL, &valueType, (PUCHAR) &VcBacklight, &valueSize )!=ERROR_SUCCESS)
    ret = false;

  valueSize = sizeof(DWORD);
  if(RegQueryValueEx( hKey, L"Brightness", NULL, &valueType, (PUCHAR) &VcBrightness, &valueSize )!=ERROR_SUCCESS)
    ret = false;

  valueSize = sizeof(DWORD);
  if(RegQueryValueEx( hKey, L"Invert",     NULL, &valueType, (PUCHAR) &VcInvert,     &valueSize )!=ERROR_SUCCESS)
    ret = false;

  valueSize = sizeof(DWORD);
  if(RegQueryValueEx( hKey, L"Mirror",     NULL, &valueType, (PUCHAR) &VcMirror,     &valueSize )!=ERROR_SUCCESS)
    ret = false;

  valueSize = sizeof(DWORD);
  if(RegQueryValueEx( hKey, L"Saturation", NULL, &valueType, (PUCHAR) &VcSaturation, &valueSize )!=ERROR_SUCCESS)
    ret = false;

  valueSize = sizeof(DWORD);
  if(RegQueryValueEx( hKey, L"Sharpness",  NULL, &valueType, (PUCHAR) &VcSharpness,  &valueSize )!=ERROR_SUCCESS)
    ret = false;

  valueSize = sizeof(DWORD);
  if(RegQueryValueEx( hKey, L"Contrast",   NULL, &valueType, (PUCHAR) &VcContrast,  &valueSize )!=ERROR_SUCCESS)
    ret = false;

  return ret;
}

// reads camera settings from the registry...
//
bool
SpectecSrc::readConfigFromRegistry()
{
  static const TCHAR  RegKey[] = TEXT("Drivers\\SDCARD\\ClientDrivers\\Custom\\MANF-02F8-CARDID-0101-FUNC-1");
  static const TCHAR  RegKey2[] = TEXT("SOFTWARE\\Metamedia\\Photoview\\Pref");
  HKEY    hKey;
  long    rcReg;

  // open the registry
  hKey = NULL;
  rcReg = RegOpenKeyEx( HKEY_LOCAL_MACHINE, RegKey, 0, 0, &hKey);
  if(rcReg == ERROR_SUCCESS)
    if(readConfigValues(hKey))
      return true;


  // open the registry
  hKey = NULL;
  rcReg = RegOpenKeyEx( HKEY_LOCAL_MACHINE, RegKey2, 0, 0, &hKey);
  if(rcReg == ERROR_SUCCESS)
    if(readConfigValues(hKey))
      return true;

  return true;
}


int
SpectecSrc::VC_SetControlValue(int CtrlID, int Value, int ValueLen)
{
  VISD_IOCTL_PARAMS   params;
  ULONG               retLen;
  int                 rc;
  int                 retVal = 1;

  params.Status = 0;
  params.ControlID = CtrlID;
  params.Value = Value;

  if ( hVSD != INVALID_HANDLE_VALUE) {

    // write the data pattern 
    rc = DeviceIoControl(hVSD,
                         IOCTL_VISD_SET_CTRL,
                         &params,
                         sizeof( VISD_IOCTL_PARAMS ),
                         &params,
                         sizeof( VISD_IOCTL_PARAMS ),
                         &retLen,
                         NULL);

    if ( rc == 0 ) {

      // error
      // DestroyWindow(g_hwnd);
      CloseHandle(hVSD);
      hVSD = INVALID_HANDLE_VALUE;
      retVal = 1;
    }
    else {
      retVal = 0;
    }
  }

  return( retVal );
}


int
SpectecSrc::VC_GetControlValue(int CtrlID, void *Value, int ValueLen, int *RetLen)
{
  VISD_IOCTL_PARAMS   params;
  ULONG               retLen;
  int                 rc;
  int                 retVal = 1;

  int *       pValue;

  params.Status = 0;
  params.ControlID = CtrlID;
  params.Value = 0;

  if ( hVSD != INVALID_HANDLE_VALUE) {
    // write the data pattern 
    rc = DeviceIoControl(hVSD,
                         IOCTL_VISD_GET_CTRL,
                         &params,
                         sizeof( VISD_IOCTL_PARAMS ),
                         &params,
                         sizeof( VISD_IOCTL_PARAMS ),
                         &retLen,
                         NULL);

    if ( rc == 0 ) {
      // error
      // DestroyWindow(g_hwnd);
      CloseHandle(hVSD);
      hVSD = INVALID_HANDLE_VALUE;
      retVal = 1;
    }
    else {

      pValue = (int *) Value;
      *pValue = params.Value;
      retVal = 0;
    }
  }

  return( retVal );
}


void 
SpectecSrc::initPixelFormats()
{
    pixelFormats.push_back(FORMAT_R5G6B5);
}

#endif  // ENABLE_SPECTECSRC
