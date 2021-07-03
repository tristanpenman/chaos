#include "Precompiled.h"

#include "Application.h"
#include "Block.h"
#include "BlockBuffer.h"
#include "Buffer.h"
#include "Chunk.h"
#include "InstanceValue.h"
#include "Level.h"
#include "Map.h"
#include "Pattern.h"
#include "PatternBuffer.h"
#include "Resource.h"
#include "Rom.h"
#include "WndFrame.h"
#include "WndMap.h"

#include "WndLevel.h"

extern Application g_application;

extern HMENU g_menu_chaos;
extern HMENU g_menu_level;

ATOM WndLevel::ms_class = 0;
HINSTANCE WndLevel::ms_hinst = 0;

InstanceValue<HWND> WndLevel::ms_map_windows;

ATOM WndLevel::getClass(HINSTANCE hinst)
{
    if (!ms_class)
    {
        WNDCLASS wc;

        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = wndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hinst;
        wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszMenuName  = TEXT("MENU_LEVEL");
        wc.lpszClassName = TEXT("WndLevel");

        ms_class = RegisterClass(&wc);
        ms_hinst = hinst;

        if (!ms_class)
        {
            MessageBox (NULL, TEXT ("Could not register the level window class."), NULL, MB_ICONERROR);
        }
    }

    return ms_class;
}

int WndLevel::messageCreate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    CREATESTRUCT* pCreateStruct = (CREATESTRUCT*) lparam;
    MDICREATESTRUCT* pMDICreateStruct = (MDICREATESTRUCT*) pCreateStruct->lpCreateParams;

    unsigned int levelIndex = (unsigned int) pMDICreateStruct->lParam;


    try
    {
        Level* pLevel = g_application.getLevelInstance(hwnd);
        if (!pLevel->loadLevel(levelIndex))
        {
            REPORT_ERROR("Failed to load the selected level", "ROM error");
            return 0;
        }

        HWND hMap = WndMap::createWindow(ms_hinst, hwnd);

        ms_map_windows.set(hwnd, hMap);
    }
    catch (...)
    {
        // Do nothing
    }

    return 0;
}

int WndLevel::messageClose(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (g_application.destroyInstance(hwnd))
    {
        return DefMDIChildProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

int WndLevel::messageDestroy(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    HWND hwnd_map = ms_map_windows.unset(hwnd);
    DestroyWindow(hwnd_map);
    return DefMDIChildProc(hwnd, msg, wparam, lparam);
}

int WndLevel::messageMDIActivate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    HWND hMain = g_application.getMainWindow();
    HWND hClient = GetWindow (hMain, GW_CHILD);

    if (lparam == (LPARAM) hwnd)
    {
        SendMessage(hClient, WM_MDISETMENU, (WPARAM) g_menu_level, (LPARAM) GetSubMenu(g_menu_level, 3));
    }
    else
    {
        SendMessage(hClient, WM_MDISETMENU, (WPARAM) g_menu_chaos, (LPARAM) GetSubMenu(g_menu_chaos, 2));
    }

    DrawMenuBar(hMain);

    return 0;
}

int WndLevel::messageSetFocus(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        SetFocus(ms_map_windows.get(hwnd));
    }
    catch (...)
    {
        // Do nothing
    }

    return 0;
}

int WndLevel::messageSize(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        RECT rect;

        if (GetClientRect(hwnd, &rect))
        {
            HWND hMap = ms_map_windows.get(hwnd);
            MoveWindow(hMap, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        }
    }
    catch (...)
    {
        // Do nothing
    }

    return DefMDIChildProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK WndLevel::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
        return messageCreate(hwnd, msg, wparam, lparam);

    case WM_DESTROY:
        return messageDestroy(hwnd, msg, wparam, lparam);

    case WM_QUERYENDSESSION:
    case WM_CLOSE:
        return messageClose(hwnd, msg, wparam, lparam);

    case WM_SETFOCUS:
        return messageSetFocus(hwnd, msg, wparam, lparam);

    case WM_MDIACTIVATE:
        return messageMDIActivate(hwnd, msg, wparam, lparam);

    case WM_SIZE:
        return messageSize(hwnd, msg, wparam, lparam);
    }

    return DefMDIChildProc(hwnd, msg, wparam, lparam);
}

void WndLevel::destroyClass()
{
    if (ms_class)
    {
        UnregisterClass((LPCTSTR)MAKELONG(ms_class, 0), ms_hinst);

        ms_hinst = 0;
        ms_class = 0;
    }
}
