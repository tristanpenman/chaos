#include "Precompiled.h"

#include "Application.h"

Application g_application;

HMENU g_menu_chaos = 0;
HMENU g_menu_level = 0;

int WINAPI WinMain(_In_ HINSTANCE hinst, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
    // menu shown when the app first loads
    g_menu_chaos = LoadMenu(hinst, MAKEINTRESOURCE(ID_MENU_CHAOS));

    // menu shown when a level is loaded and an MDI instance is open
    g_menu_level = LoadMenu(hinst, MAKEINTRESOURCE(ID_MENU_LEVEL));

    int result = g_application.run(hinst, hPrevInstance, szCmdLine, iCmdShow);

    DestroyMenu(g_menu_chaos);
    g_menu_chaos = 0;
    
    DestroyMenu(g_menu_level);
    g_menu_level = 0;

    return result;
}
