// OTTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OTTest.h"
#include <commctrl.h>

#include <ace/ace.h>
#include <ace/Log_Msg.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);

#include <OpenTracker.h>


void test()
{
    ACE::init();

#ifdef _WIN32_WCE
    ACE_OSTREAM_TYPE* output = fopen("ace_log.txt", "w");
    ACE_LOG_MSG->msg_ostream(output, 0);
    ACE_LOG_MSG->set_flags(ACE_Log_Msg::OSTREAM);
#endif //_WIN32_WCE


    ot::Context context( 1 );

    //::MessageBox(NULL, L"Go", L"Info", MB_OK);
    // parse the configuration file, builds the tracker tree
    context.parseConfiguration( "/minitest.xml" );

    //::MessageBox(NULL, L"Done", L"Info", MB_OK);

    // initializes the modules and starts the tracker main loop
    //context.run();


#ifdef _WIN32_WCE
    fclose(output);
#endif
}


int WINAPI WinMain(	HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPTSTR    lpCmdLine,
                        int       nCmdShow)
{
    MSG msg;
    HACCEL hAccelTable;

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow)) 
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_OTTEST);

    test();

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    It is important to call this function so that the application 
//    will get 'well formed' small icons associated with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
    WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OTTEST));
    wc.hCursor			= 0;
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= szWindowClass;

    return RegisterClass(&wc);
}

//
//  FUNCTION: InitInstance(HANDLE, int)
//
//  PURPOSE: Saves instance handle and creates main window
//
//  COMMENTS:
//
//    In this function, we save the instance handle in a global variable and
//    create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND	hWnd;
    TCHAR	szTitle[MAX_LOADSTRING];			// The title bar text
    TCHAR	szWindowClass[MAX_LOADSTRING];		// The window class name

    hInst = hInstance;		// Store instance handle in our global variable
    // Initialize global strings
    LoadString(hInstance, IDC_OTTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance, szWindowClass);

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {	
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    if (hwndCB)
        CommandBar_Show(hwndCB, TRUE);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    TCHAR szHello[MAX_LOADSTRING];

    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
                    break;
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
            hwndCB = CommandBar_Create(hInst, hWnd, 1);			
            CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
            CommandBar_AddAdornments(hwndCB, 0, 0);
            break;
        case WM_PAINT:
            RECT rt;
            hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &rt);
            LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
            DrawText(hdc, szHello, _tcslen(szHello), &rt, 
                     DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            CommandBar_Destroy(hwndCB);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Mesage handler for the About box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rt, rt1;
    int DlgWidth, DlgHeight;	// dialog width and height in pixel units
    int NewPosX, NewPosY;

    switch (message)
    {
        case WM_INITDIALOG:
            // trying to center the About dialog
            if (GetWindowRect(hDlg, &rt1)) {
                GetClientRect(GetParent(hDlg), &rt);
                DlgWidth	= rt1.right - rt1.left;
                DlgHeight	= rt1.bottom - rt1.top ;
                NewPosX		= (rt.right - rt.left - DlgWidth)/2;
                NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
                // if the About box is larger than the physical screen 
                if (NewPosX < 0) NewPosX = 0;
                if (NewPosY < 0) NewPosY = 0;
                SetWindowPos(hDlg, 0, NewPosX, NewPosY,
                             0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
            return TRUE;

        case WM_COMMAND:
            if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
    }
    return FALSE;
}

/* 
 * ------------------------------------------------------------
 *   End of OTTest.cpp
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
