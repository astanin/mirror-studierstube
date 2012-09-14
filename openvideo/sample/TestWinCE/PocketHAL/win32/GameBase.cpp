//////////////////////////////////////////////////////////////////////////////
//
// PocketHAL
// Copyright 2004 by Thierry Tremblay
//
//////////////////////////////////////////////////////////////////////////////

#include "GameBase.h"
#include <stdio.h>
#include <tchar.h>



namespace PHAL
{

//////////////////////////////////////////////////////////////////////////////
//
// Window Message Handler
//
//////////////////////////////////////////////////////////////////////////////

LRESULT GameBase::MessageHandler( UINT msg, WPARAM wparam, LPARAM lparam )
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow( m_hwnd );
        break;

    case WM_DESTROY:
        OnShutdown();
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (!(lparam & 0x40000000)) // Prevent auto-repeat
        {
            OnKeyDown( wparam );
        }
        break;

    case WM_KEYUP:
        OnKeyUp( wparam );
        break;

    case WM_LBUTTONDOWN:
        OnStylusDown( Point( LOWORD(lparam), HIWORD(lparam) ) );
        break;

    case WM_LBUTTONUP:
        OnStylusUp( Point( LOWORD(lparam), HIWORD(lparam) ) );
        break;

    case WM_MOUSEMOVE:
        OnStylusMove( Point( LOWORD(lparam), HIWORD(lparam) ) );
        break;

	case WM_ACTIVATE:
		if (LOWORD(wparam) == WA_INACTIVE)
		{
            m_bSuspended = true;
			OnSuspend();
		}
		else
		{
            m_bSuspended = false;
			OnResume();
		}
		break;
    }

    return DefWindowProc( m_hwnd, msg, wparam, lparam );
}




//////////////////////////////////////////////////////////////////////////////
//
// GameBase
//
//////////////////////////////////////////////////////////////////////////////

GameBase::GameBase()
:   m_hwnd(0),
    m_bSuspended(false),
    m_uid(0)
{
    s_instance = this;
}



GameBase::~GameBase()
{
    s_instance = 0;
}



bool GameBase::Initialize( const Config& config )
{
    TCHAR uniqueName[MAX_PATH];
    _stprintf( uniqueName, TEXT("PHAL_FRAMEWORK_%08X"), m_uid );

    HANDLE hMutexOneInstance = ::CreateMutex( 0, FALSE, uniqueName );

    bool bAlreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS);

    if (bAlreadyRunning)
    {
        // If there is already an instance of this app running, give it focus and exit
        HWND hOther = ::FindWindow( uniqueName, 0 );
        if (hOther)
        {
            ::SetForegroundWindow( hOther );

#if defined(PHAL_PLATFORM_WCE)
            if (::IsWindowEnabled( hOther ))
#else
            if (::IsIconic( hOther ))
#endif
                ::ShowWindow( hOther, SW_RESTORE );
        }

        return false;
    }

    // Register a window class
    HINSTANCE hInstance = GetModuleHandle(0);
    
    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = MessageHandler;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = 0;
    wc.hCursor       = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = uniqueName;

    if (!RegisterClass(&wc))
        return EXIT_FAILURE;


#if defined(PHAL_PLATFORM_WCE)
	int width  = ::GetSystemMetrics( SM_CXSCREEN );
	int height = ::GetSystemMetrics( SM_CYSCREEN );
	int style  = WS_VISIBLE;
#else
	int width  = 240;
	int height = 320;
	int style  = WS_VISIBLE | WS_SYSMENU;
#endif

#if defined(PHAL_PLATFORM_HPC)
	int exStyle = WS_EX_TOPMOST;
#else
	int exStyle = 0;
#endif

    // Create the window
    m_hwnd = CreateWindowEx( exStyle, uniqueName, config.m_appName, style, 0, 0, width, height, 0, 0, hInstance, 0 );
    if (!m_hwnd)
        return false;

    return true;
}



bool GameBase::Run()
{
    for(;;)
    {
        MSG  msg;
        BOOL result;

        // If we are suspended, don't use any CPU
        if (m_bSuspended)
        {
            result = ::GetMessage( &msg, 0, 0, 0 );
            
            if (!result)
                break;      // Got a WM_QUIT
        }
        else
        {
            result = ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE );

            if (result && msg.message == WM_QUIT)
                break;      // Got a WM_QUIT
        }


        if (result)
        {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
        }
        else
        {
            if (!OnGameLoop())
            {
                DestroyWindow( m_hwnd );
                break;
            }
        }
    }

    return true;
}




void GameBase::ResetInactivity() const
{
#if defined(PHAL_PLATFORM_WCE)
    
    // Reset power saving mode timeout
    SystemIdleTimerReset();

#endif
}



//////////////////////////////////////////////////////////////////////////////
//
// Static interface
//
//////////////////////////////////////////////////////////////////////////////

GameBase* GameBase::s_instance = 0;


LRESULT CALLBACK GameBase::MessageHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    if (s_instance && s_instance->m_hwnd == hwnd)
    {
        return s_instance->MessageHandler( msg, wparam, lparam );
    }
    else
    {
        return DefWindowProc( hwnd, msg, wparam, lparam );
    }
}



} // end of namespace PHAL
