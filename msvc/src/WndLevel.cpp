#include "Precompiled.h"

#include "SegaRom.h"
#include "InstanceValue.h"
#include "ChaosApplication.h"
#include "ChaosRom.h"
#include "Resource.h"
#include "SegaPattern.h"
#include "Buffer.h"
#include "Buffer_Patterns.h"
#include "Buffer_Blocks.h"
#include "SonicChunk.h"
#include "SonicBlock.h"
#include "SonicMap.h"
#include "Instance.h"
#include "Instance_Level.h"

#include "WndMap.h"
#include "WndLevel.h"
#include "WndFrame.h"

extern ChaosApplication g_application;

extern HMENU g_menuChaos;
extern HMENU g_menuLevel;

ATOM      WndLevel::ms_class = 0;
HINSTANCE WndLevel::ms_hInstance = 0;

InstanceValue_t<HWND> WndLevel::ms_mapWindows;

ATOM WndLevel::getClass(HINSTANCE hInstance)
{
    if (!ms_class)
    {
        WNDCLASS wc;

        wc.style         = CS_HREDRAW | CS_VREDRAW ;;
        wc.lpfnWndProc   = wndProc;
        wc.cbClsExtra    = 0 ;
        wc.cbWndExtra    = 0 ;
        wc.hInstance     = hInstance ;
        wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION) ;
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW) ;
        wc.hbrBackground = NULL ;
        wc.lpszMenuName  = TEXT("MENU_LEVEL");
        wc.lpszClassName = TEXT("ChaosLevel");

        ms_class = RegisterClass(&wc);
        ms_hInstance = hInstance;

        if (!ms_class)
        {
            MessageBox (NULL, TEXT ("Could not register the level window class."), NULL, MB_ICONERROR) ;
        }
    }

    return ms_class;
}

int WndLevel::messageCreate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CREATESTRUCT* pCreateStruct = (CREATESTRUCT*) lParam;
    MDICREATESTRUCT* pMDICreateStruct = (MDICREATESTRUCT*) pCreateStruct->lpCreateParams;

    unsigned int levelIndex = (unsigned int) pMDICreateStruct->lParam;


    try
    {
        Instance_Level* pInstance = g_application.getLevelInstance(hwnd);

        if (!pInstance->loadLevel(levelIndex))
        {
            REPORT_ERROR("Failed to load the selected level", "ROM error");
            return 0;
        }

        HWND hMap = WndMap::createWindow(ms_hInstance, hwnd);

        ms_mapWindows.set(hwnd, hMap);
    }
    catch (...)
    {
        // Do nothing
    }

    return 0;
}

int WndLevel::messageClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (g_application.destroyInstance(hwnd))
    {
        return DefMDIChildProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WndLevel::messageDestroy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hMap = ms_mapWindows.unset(hwnd);
    DestroyWindow(hMap);
    return DefMDIChildProc(hwnd, msg, wParam, lParam);
}

int WndLevel::messageMDIActivate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hMain = g_application.getMainWindow();
    HWND hClient = GetWindow (hMain, GW_CHILD) ;

    if (lParam == (LPARAM) hwnd)
    {
        SendMessage(hClient, WM_MDISETMENU, (WPARAM) g_menuLevel, (LPARAM) GetSubMenu(g_menuLevel, 3));
    }
    else
    {
        SendMessage(hClient, WM_MDISETMENU, (WPARAM) g_menuChaos, (LPARAM) GetSubMenu(g_menuChaos, 2));
    }

    DrawMenuBar(hMain);

    return 0;
}

int WndLevel::messageSetFocus(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    try
    {
        SetFocus(ms_mapWindows.get(hwnd));
    }
    catch (...)
    {
        // Do nothing
    }

    return 0;
}

int WndLevel::messageSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    try
    {
        RECT rect;

        if (GetClientRect(hwnd, &rect))
        {
            HWND hMap = ms_mapWindows.get(hwnd);
            MoveWindow(hMap, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        }
    }
    catch (...)
    {
        // Do nothing
    }

    return DefMDIChildProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndLevel::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        return messageCreate(hwnd, msg, wParam, lParam);

    case WM_DESTROY:
        return messageDestroy(hwnd, msg, wParam, lParam);

    case WM_QUERYENDSESSION:
    case WM_CLOSE:
        return messageClose(hwnd, msg, wParam, lParam);

    case WM_SETFOCUS:
        return messageSetFocus(hwnd, msg, wParam, lParam);

    case WM_MDIACTIVATE:
        return messageMDIActivate(hwnd, msg, wParam, lParam);

    case WM_SIZE:
        return messageSize(hwnd, msg, wParam, lParam);
    }

    return DefMDIChildProc(hwnd, msg, wParam, lParam);
}

void WndLevel::destroyClass()
{
    if (ms_class)
    {
        UnregisterClass((LPCTSTR)MAKELONG(ms_class, 0), ms_hInstance);

        ms_hInstance = 0;
        ms_class = 0;
    }
}
