#include "Precompiled.h"

#include "Application.h"
#include "InstanceValue.h"
#include "LevelSelectDialog.h"
#include "OpenRomDialog.h"
#include "WndLevel.h"
#include "WndMap.h"

#include "WndFrame.h"

#define IDM_FIRSTCHILD 100

HINSTANCE WndFrame::ms_hinst = 0;
ATOM WndFrame::ms_class = 0;

extern Application g_application;

extern HMENU g_menu_chaos;

HWND WndFrame::createWindow(HINSTANCE hinst, HWND hwnd_parent)
{
    if (!ms_class)
    {
        WNDCLASSEX wc;

        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = 0;
        wc.lpfnWndProc   = frameWndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hinst;
        wc.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(ID_ICON_CHAOS));
        wc.hIconSm       = LoadIcon(hinst, MAKEINTRESOURCE(ID_ICON_CHAOS));
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = TEXT("ChaosFrame");

        ms_class = RegisterClassEx(&wc);
        ms_hinst = hinst;

        if (!ms_class)
        {
            MessageBox(NULL, TEXT("Could not register the Chaos main window class."), NULL, MB_ICONERROR);
            return 0;
        }
    }

    HWND hWnd =  CreateWindow(
        (LPCTSTR)MAKELONG(ms_class, 0),      // window class name
        TEXT ("Chaos"),                      // window caption
        WS_OVERLAPPEDWINDOW,                 // window style
        CW_USEDEFAULT,                       // initial x position
        CW_USEDEFAULT,                       // initial y position
        CW_USEDEFAULT,                       // initial x size
        CW_USEDEFAULT,                       // initial y size
        hwnd_parent,                         // parent window handle
        g_menu_chaos,                        // window menu handle
        hinst,                               // program instance handle
        NULL);                               // creation parameters

    return hWnd;
}

bool WndFrame::openFile()
{
    OpenRomDialog dialog;

    return dialog.showDialog() && g_application.openROM(dialog.getFileName());
}

HWND WndFrame::createLevelWindow(HWND hClient)
{
    int level = LevelSelectDialog::showDialog(ms_hinst, g_application.getMainWindow());

    if (level < 0)
    {
        return 0;
    }

    HWND                hwndChild = 0;
    MDICREATESTRUCT     mdicreate;

    mdicreate.szClass = (LPCTSTR) MAKELONG(WndLevel::getClass(ms_hinst), 0);
    mdicreate.szTitle = "Level Editor";
    mdicreate.hOwner  = ms_hinst;
    mdicreate.x       = CW_USEDEFAULT;
    mdicreate.y       = CW_USEDEFAULT;
    mdicreate.cx      = CW_USEDEFAULT;
    mdicreate.cy      = CW_USEDEFAULT;
    mdicreate.style   = 0;
    mdicreate.lParam  = (LPARAM) level;

    hwndChild = (HWND) SendMessage(hClient, WM_MDICREATE, 0, (LPARAM) (LPMDICREATESTRUCT) &mdicreate);

    return hwndChild;
}

void WndFrame::enableMenuItems(HWND hwnd)
{
    EnableMenuItem(GetMenu(hwnd), ID_FILE_LEVEL_SELECT, MF_ENABLED);
}

LRESULT CALLBACK WndFrame::frameWndProc(HWND hwnd, UINT imsg, WPARAM wparam, LPARAM lparam)
{
    static HWND         hwndClient;
    static HMENU        hMenuWindow;

    CLIENTCREATESTRUCT  clientcreate;

    switch (imsg)
    {
    case WM_CREATE:

        hMenuWindow = GetSubMenu(g_menu_chaos, 2);

        clientcreate.hWindowMenu  = hMenuWindow;
        clientcreate.idFirstChild = IDM_FIRSTCHILD;

        hwndClient = CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
            0, // x
            0, // y
            0, // w
            0, // h
            hwnd, (HMENU) 1, ms_hinst, (LPSTR) &clientcreate);

        return 0;


    case WM_COMMAND:
        switch (LOWORD (wparam))
        {
        case ID_FILE_OPEN:
            // Can ROM be loaded?
            if (openFile())
            {
                enableMenuItems(hwnd);
                createLevelWindow(hwndClient);
            }
            return 0;

        case ID_FILE_LEVEL_SELECT:
            // Is ROM loaded?
            if (g_application.getROM())
            {
                createLevelWindow(hwndClient);
            }
            return 0;

        case ID_WINDOW_CASCADE:
            SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
            return 0;

        case ID_WINDOW_TILE:
            SendMessage(hwndClient, WM_MDITILE, 0, 0);
            return 0;

        case ID_WINDOW_CLOSEALL:
            EnumChildWindows(hwndClient, &closeEnumProc, 0);
            return 0;

        case ID_FILE_EXIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            return 0;
        }
        break;

    case WM_QUERYENDSESSION:
    case WM_CLOSE:
        SendMessage(hwnd, WM_COMMAND, ID_WINDOW_CLOSEALL, 0);
        // Don't close if there is an open child
        if (NULL != GetWindow(hwndClient, GW_CHILD))
        {
            return 0;
        }
        break;

    case WM_DESTROY :
        // Destroy the window
        PostQuitMessage(0);
        g_application.resetMainWindow(NULL);
        return 0;
    }

    // Pass unprocessed messages to DefFrameProc (not DefWindowProc)
    return DefFrameProc(hwnd, hwndClient, imsg, wparam, lparam);

}

BOOL CALLBACK WndFrame::closeEnumProc(HWND hwnd, LPARAM lparam)
{
    // Check for icon title
    if (GetWindow(hwnd, GW_OWNER))
    {
        return 1;
    }

    SendMessage(GetParent (hwnd), WM_MDIRESTORE, (WPARAM) hwnd, 0);

    if (!SendMessage(hwnd, WM_QUERYENDSESSION, 0, 0))
    {
        return 1;
    }

    SendMessage(GetParent (hwnd), WM_MDIDESTROY, (WPARAM) hwnd, 0);

    return 1;
}

void WndFrame::destroyClass()
{
    if (ms_class)
    {
        UnregisterClass((LPCTSTR)MAKELONG(ms_class, 0), ms_hinst);
        ms_class = 0;
    }

    ms_hinst = 0;
}
