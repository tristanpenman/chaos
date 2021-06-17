#include "Precompiled.h"

#include "ChaosApplication.h"

ChaosApplication g_application;

HMENU g_menuChaos = 0;
HMENU g_menuLevel = 0;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
    // menu shown when the app first loads
    g_menuChaos = LoadMenu(hInstance, MAKEINTRESOURCE(ID_MENU_CHAOS));

    // menu shown when a level is loaded and an MDI instance is open
    g_menuLevel = LoadMenu(hInstance, MAKEINTRESOURCE(ID_MENU_LEVEL));

    int result = g_application.run(hInstance, hPrevInstance, szCmdLine, iCmdShow);

    DestroyMenu(g_menuChaos);
    g_menuChaos = 0;
    
    DestroyMenu(g_menuLevel);
    g_menuLevel = 0;

    return result;
}
