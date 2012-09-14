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
/** source file for the raw input module.
 *
 * @author Mathis Csisinko
 *
 * $Id: RawInputModule.cxx 2131 2008-02-29 16:28:36Z mathis $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/RawInputModule.h>

#ifdef USE_RAWINPUT
#ifdef WIN32

#include <OpenTracker/input/RawInputSource.h>

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC (0x00000002)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE (0x00000001)
#endif

namespace ot {

    OT_MODULE_REGISTER_FUNC(RawInputModule){
        OT_MODULE_REGISTRATION_DEFAULT(RawInputModule, "RawInputConfig");
    }	

    RawInputModule::RawInputModule(): ThreadModule(),NodeFactory(),pSources(),numDevices(0),deviceNames(0),rawInputDeviceList(0),threadId(0)
    {
    }

    RawInputModule::~RawInputModule()
    {
#ifdef USE_LIVE
        for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
            delete *it;
#endif
        pSources.clear();
    }

    Node* RawInputModule::createNode(const std::string &name,const StringTable &attributes)
    {
        if (name.compare("RawInputSource") == 0)
            for (UINT device = 0;device < numDevices;device ++)
                if (!attributes.get("deviceName").compare(deviceNames[device]))
                    {
                        RID_DEVICE_INFO deviceInfo;
                        UINT deviceInfoSize = sizeof(deviceInfo);
                        deviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
                        GetRawInputDeviceInfo(rawInputDeviceList[device].hDevice,RIDI_DEVICEINFO,&deviceInfo,&deviceInfoSize);
                        if (deviceInfo.dwType == RIM_TYPEMOUSE)
                            {
                                RawInputSource* pSource = new RawInputSource(rawInputDeviceList[device].hDevice);
                                pSources.push_back(pSource);

                                logPrintI("Build RawInputSource node\n");
                                return pSource;
                            }
                        else
                            break;
                    }
        return NULL;
    }

    void RawInputModule::close()
    {
        PostThreadMessage(threadId,WM_QUIT,0,0x00000000);
        ThreadModule::close();
        if (deviceNames)
            {
                for (UINT device = 0;device < numDevices;device ++)
                    if (deviceNames[device])
                        delete[] deviceNames[device];
                delete[] deviceNames;
            }
        if (rawInputDeviceList)
            delete[] rawInputDeviceList;
    }

    void RawInputModule::pushEvent()
    {
        for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
            {
                RawInputSource* pSource = static_cast<RawInputSource*>((Node*)*it);
                lockLoop();
                if (pSource->calcEvent())
                    pSource->updateObservers(pSource->getEvent());
                unlockLoop();
            }
    }

    void RawInputModule::init(StringTable &attributes,ConfigNode* pLocalTree)
    {
		Module::init(attributes,pLocalTree);
		GetRawInputDeviceList(NULL,&numDevices,sizeof(RAWINPUTDEVICELIST));
        rawInputDeviceList = new RAWINPUTDEVICELIST[numDevices];
        if (rawInputDeviceList)
            {
                GetRawInputDeviceList(rawInputDeviceList,&numDevices,sizeof(RAWINPUTDEVICELIST));
                deviceNames = new char*[numDevices];
                logPrintI("Known raw input device names:\n");
                if (deviceNames)
                    {
                        for (UINT device = 0;device < numDevices;device ++)
                            {
                                UINT deviceNameSize;
                                GetRawInputDeviceInfo(rawInputDeviceList[device].hDevice,RIDI_DEVICENAME,NULL,&deviceNameSize);
                                deviceNames[device] = new char[deviceNameSize] + 1;
                                if (deviceNames[device])
                                    {
                                        GetRawInputDeviceInfo(rawInputDeviceList[device].hDevice,RIDI_DEVICENAME,deviceNames[device],&deviceNameSize);
                                        logPrintI("%s\n",deviceNames[device]);
                                    }
                            }
                        /*for (unsigned int i = 0;i < pLocalTree->countChildren();i ++)
                          {
                          }*/
                    }
            }
    }

    void RawInputModule::run()
    {
		threadId = GetCurrentThreadId();
        if (pSources.size() > 0)
            {
                WNDCLASSEX wndClassEx;
                wndClassEx.cbSize = sizeof(wndClassEx);
                wndClassEx.style = 0x00000000;
                wndClassEx.lpfnWndProc = WndProc;
                wndClassEx.cbClsExtra = 0;
                wndClassEx.cbWndExtra = 0;
                wndClassEx.hInstance = NULL;
                wndClassEx.hIcon = NULL;
                wndClassEx.hCursor = NULL;
                wndClassEx.hbrBackground = NULL;
                wndClassEx.lpszMenuName = NULL;
                wndClassEx.lpszClassName = "RawInputDummyWindow";
                wndClassEx.hIconSm = NULL;
                if (RegisterClassEx(&wndClassEx))
                    {
                        pRawInputModule = this;
                        HWND hWnd = CreateWindowEx(0x00000000,wndClassEx.lpszClassName,NULL,WS_OVERLAPPED,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,NULL,0);
                        if (hWnd)
                            {
                                RAWINPUTDEVICE rawInputDevice;
                                rawInputDevice.usUsage = HID_USAGE_PAGE_GENERIC;
                                rawInputDevice.usUsagePage = HID_USAGE_GENERIC_MOUSE;
                                rawInputDevice.hwndTarget = hWnd;
                                rawInputDevice.dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK | RIDEV_CAPTUREMOUSE;
                                if (RegisterRawInputDevices(&rawInputDevice,sizeof(rawInputDevice) / sizeof(RAWINPUTDEVICE),sizeof(RAWINPUTDEVICE)))
                                    {
                                        HHOOK hLowLevelKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardHook,GetModuleHandle(NULL),0);
                                        if (hLowLevelKeyboardHook)
                                            {
                                                HHOOK hLowLevelMouseHook = SetWindowsHookEx(WH_MOUSE_LL,LowLevelMouseHook,GetModuleHandle(NULL),0);
                                                if (hLowLevelMouseHook)
                                                    {
                                                        logPrintI("Windows mouse button input %s. Press [F12] to toggle.\n",filterButtons ? "disabled": "enabled");
                                                        MSG msg;
                                                        BOOL result;
                                                        for(;;)
                                                            {
                                                                result = GetMessage(&msg,NULL,0x00000000,0x00000000);
                                                                if (result == 0 || result == -1)
                                                                    break;
                                                                else
                                                                    DispatchMessage(&msg);
                                                            }
                                                        UnhookWindowsHookEx(hLowLevelMouseHook);
                                                    }
                                                UnhookWindowsHookEx(hLowLevelKeyboardHook);
                                            }
                                    }
                                DestroyWindow(hWnd);
                            }
                        pRawInputModule = 0;
						UnregisterClass(wndClassEx.lpszClassName,NULL);
                    }
            }
    }

    LRESULT CALLBACK RawInputModule::LowLevelMouseHook(int code,WPARAM wParam,LPARAM lParam)
    {
        if (code == HC_ACTION)
            return (filterButtons && (wParam == WM_LBUTTONDOWN || wParam == WM_LBUTTONUP || wParam == WM_LBUTTONDBLCLK || wParam == WM_RBUTTONDOWN || wParam == WM_RBUTTONUP || wParam == WM_RBUTTONDBLCLK || wParam == WM_MBUTTONDOWN || wParam == WM_MBUTTONUP || wParam == WM_MBUTTONDBLCLK))? 1: 0;
        else
            return CallNextHookEx(NULL,code,wParam,lParam);
    }

    LRESULT CALLBACK RawInputModule::LowLevelKeyboardHook(int code,WPARAM wParam,LPARAM lParam)
    {
        if (code == HC_ACTION && (reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam))->vkCode == VK_F12)
            {
                if (wParam == WM_KEYDOWN)
                    {
                        filterButtons = ! filterButtons;
                        logPrintI("Windows mouse button input %s.\n",filterButtons ? "disabled": "enabled");
                    }
                return 1;
            }
        else
            return CallNextHookEx(NULL,code,wParam,lParam);
    }

    LRESULT CALLBACK RawInputModule::WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
    {
        switch (message)
            {
		case WM_INPUT:
                    if (pRawInputModule)
                        pRawInputModule->processRawInput(hWnd,wParam,reinterpret_cast<HRAWINPUT>(lParam));
		default:
                    return DefWindowProc(hWnd,message,wParam,lParam);
            }
    }

    void RawInputModule::processRawInput(HWND,WPARAM,HRAWINPUT hRawInput)
    {
        RAWINPUT rawInput;
        UINT rawInputSize = sizeof(rawInput);
        if (GetRawInputData(hRawInput,RID_INPUT,&rawInput,&rawInputSize,sizeof(RAWINPUTHEADER)))
            for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
                {
                    RawInputSource* pSource = static_cast<RawInputSource*>((Node*)*it);
                    if (pSource->getDevice() == rawInput.header.hDevice)
                        {
                            if (rawInput.header.dwType == RIM_TYPEMOUSE)
                                {
                                    unsigned short button = 0x00000000;
                                    unsigned short buttonMask = 0x00000000;
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN)
                                        {
                                            button |= 0x00000001;
                                            buttonMask |= 0x00000001;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP)
                                        {
                                            button &= ~ 0x00000001;
                                            buttonMask |= 0x00000001;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN)
                                        {
                                            button |= 0x00000002;
                                            buttonMask |= 0x00000002;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP)
                                        {
                                            button &= ~ 0x00000002;
                                            buttonMask |= 0x00000002;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN)
                                        {
                                            button |= 0x00000004;
                                            buttonMask |= 0x00000004;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP)
                                        {
                                            button &= ~ 0x00000004;
                                            buttonMask |= 0x00000004;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
                                        {
                                            button |= 0x00000008;
                                            buttonMask |= 0x00000008;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
                                        {
                                            button &= ~ 0x00000008;
                                            buttonMask |= 0x00000008;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
                                        {
                                            button |= 0x00000010;
                                            buttonMask |= 0x00000010;
                                        }
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
                                        {
                                            button &= ~ 0x00000010;
                                            buttonMask |= 0x00000010;
                                        }
                                    lockLoop();
                                    if (rawInput.data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
                                        pSource->setMoveEvent(RawInputSource::ZRelative,0,0,static_cast<SHORT>(rawInput.data.mouse.usButtonData) / WHEEL_DELTA);
                                    if (rawInput.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
                                        pSource->setMoveEvent(RawInputSource::XYAbsolute,rawInput.data.mouse.lLastX,GetSystemMetrics(SM_CYSCREEN) - rawInput.data.mouse.lLastY,0);
                                    else
                                        pSource->setMoveEvent(RawInputSource::XYRelative,rawInput.data.mouse.lLastX,- rawInput.data.mouse.lLastY,0);
                                    if (buttonMask && filterButtons)
                                        pSource->setButtonEvent(button,buttonMask);
                                    unlockLoop();
                                }
                            if (context != NULL && context->doSynchronization())
                                {
                                    context->dataSignal();
				    context->consumedWait();
                                }      
                        }

                }
    }

    bool RawInputModule::filterButtons = true;
    RawInputModule* RawInputModule::pRawInputModule = 0;

} // namespace ot


#endif
#else
#ifdef WIN32
#pragma message(">>> no Raw Input support")
#endif
#endif

/* ===========================================================================
   End of RawInputModule.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'subeventment-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'eventment 'c-lineup-runin-eventments)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
