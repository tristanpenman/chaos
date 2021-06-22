#include "Precompiled.h"

#include "InstanceValue.h"
#include "ChaosApplication.h"
#include "DialogLoadLevel.h"
#include "DialogOpen.h"

#include "WndFrame.h"
#include "WndMap.h"
#include "WndLevel.h"

#define IDM_FIRSTCHILD  100

HINSTANCE WndFrame::ms_hInstance = 0;
ATOM      WndFrame::ms_class = 0;

extern ChaosApplication g_application;

extern HMENU g_menuChaos;

HWND WndFrame::createWindow(HINSTANCE hInstance, HWND hParent)
{
    if (!ms_class)
    {
        WNDCLASSEX wc;

        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = 0;
        wc.lpfnWndProc   = frameWndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInstance;
        wc.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(ID_ICON_CHAOS));
        wc.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(ID_ICON_CHAOS));
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = TEXT("ChaosFrame");

        ms_class = RegisterClassEx(&wc);
        ms_hInstance = hInstance;

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
        hParent,                             // parent window handle
        g_menuChaos,                         // window menu handle
        hInstance,                           // program instance handle
        NULL);                              // creation parameters

    return hWnd;
}

bool WndFrame::openFile()
{
    DialogOpen dialog;

    return dialog.showDialog() && g_application.openROM(dialog.getFileName());
}

HWND WndFrame::createLevelWindow(HWND hClient)
{
    int level = DialogLoadLevel::showDialog(ms_hInstance, g_application.getMainWindow());

    if (level < 0)
    {
        return 0;
    }

    HWND                hwndChild = 0;
    MDICREATESTRUCT     mdicreate;

    mdicreate.szClass = (LPCTSTR) MAKELONG(WndLevel::getClass(ms_hInstance), 0);
    mdicreate.szTitle = "Level Editor";
    mdicreate.hOwner  = ms_hInstance;
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
    EnableMenuItem(GetMenu(hwnd), ID_FILE_OPEN_LEVEL, MF_ENABLED);
}

LRESULT CALLBACK WndFrame::frameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND         hwndClient;
    static HMENU        hMenuWindow;

    CLIENTCREATESTRUCT  clientcreate;

    switch (iMsg)
    {
    case WM_CREATE:

        hMenuWindow = GetSubMenu(g_menuChaos, 2);

        clientcreate.hWindowMenu  = hMenuWindow;
        clientcreate.idFirstChild = IDM_FIRSTCHILD;

        hwndClient = CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
            0, // x
            0, // y
            0, // w
            0, // h
            hwnd, (HMENU) 1, ms_hInstance, (LPSTR) &clientcreate);

        return 0;


    case WM_COMMAND:
        switch (LOWORD (wParam))
        {
        case ID_FILE_OPEN:
            // Can ROM be loaded?
            if (openFile())
            {
                enableMenuItems(hwnd);
                createLevelWindow(hwndClient);
            }
            return 0;

        case ID_FILE_OPEN_LEVEL:
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
    return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam);

}

BOOL CALLBACK WndFrame::closeEnumProc(HWND hwnd, LPARAM lParam)
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
        UnregisterClass((LPCTSTR)MAKELONG(ms_class, 0), ms_hInstance);
        ms_class = 0;
    }

    ms_hInstance = 0;
}
