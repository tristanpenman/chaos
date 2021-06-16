#include "Precompiled.h"

#include "InstanceValue.h"
#include "ChaosException.h"
#include "ChaosApplication.h"

ChaosApplication g_application;

HMENU g_menuChaos = 0;
HMENU g_menuLevel = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    g_menuChaos = LoadMenu(hInstance, MAKEINTRESOURCE(ID_MENU_CHAOS));
    g_menuLevel = LoadMenu(hInstance, MAKEINTRESOURCE(ID_MENU_LEVEL));

    int result = g_application.run(hInstance, hPrevInstance, szCmdLine, iCmdShow);

    DestroyMenu(g_menuChaos); g_menuChaos = 0;
    DestroyMenu(g_menuLevel); g_menuLevel = 0;

    return result;
}
