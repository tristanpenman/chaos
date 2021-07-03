#include "Precompiled.h"

#include "InstanceValue.h"
#include "ChaosRom.h"
#include "ChaosApplication.h"
#include "DialogLoadLevel.h"

#define MAX_LEVEL_STR 100

extern ChaosApplication g_application;

using namespace std;

WNDPROC DialogLoadLevel::ms_default_list_proc = NULL;

int DialogLoadLevel::showDialog(HINSTANCE hinst, HWND hwnd_parent)
{
    return DialogBox(hinst, MAKEINTRESOURCE(ID_DIALOG_LOAD_LEVEL), hwnd_parent, reinterpret_cast<DLGPROC>(dialogProc));
}

int DialogLoadLevel::messageCommand(HWND hwnd_dialog, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (wparam == IDOK)
    {
        HWND hwnd_list_box = GetDlgItem(hwnd_dialog, ID_DIALOG_LOAD_LEVEL_LIST);

        int selected = SendMessage(hwnd_list_box, LB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
        int level = SendMessage(hwnd_list_box, LB_GETITEMDATA, selected, 0);

        EndDialog(hwnd_dialog, level);
        return TRUE;
    }

    if (wparam == IDCANCEL)
    {
        EndDialog(hwnd_dialog, -1);
        return TRUE;
    }

    return FALSE;
}

int DialogLoadLevel::messageInit(HWND hwnd_dialog, UINT msg, WPARAM wparam, LPARAM lparam)
{
    HWND hwnd_list_box = GetDlgItem(hwnd_dialog, ID_DIALOG_LOAD_LEVEL_LIST);

    // Save the original list box window procedure
    ms_default_list_proc = (WNDPROC) GetWindowLong(hwnd_list_box, GWL_WNDPROC);

    // Drop in the replacement window procedure that handles double clicks
    SetWindowLong(hwnd_list_box, GWL_WNDPROC, (LONG) listProc);

    ChaosRom* pRom = g_application.getROM();
    if (!pRom)
    {
        return FALSE;
    }

    ChaosRom::LevelNames_t& level_names = pRom->getLevelNames();

    if (level_names.size() == 0)
    {
        return FALSE;
    }

    for (ChaosRom::LevelNames_t::iterator itr = level_names.begin(); itr != level_names.end(); ++itr)
    {
        int idx = SendMessage(hwnd_list_box, LB_ADDSTRING, 0, (LPARAM) itr->second.c_str());
        if (idx == LB_ERR)
        {
            return FALSE;
        }

        SendMessage(hwnd_list_box, LB_SETITEMDATA, (WPARAM) idx, (LPARAM) itr->first);
    }

    SendMessage(hwnd_list_box, LB_SETCURSEL, 0, 0);

    return TRUE;
}

LRESULT CALLBACK DialogLoadLevel::dialogProc(HWND hwnd_dialog, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return messageInit(hwnd_dialog, msg, wparam, lparam);

    case WM_COMMAND:
        return messageCommand(hwnd_dialog, msg, wparam, lparam);
    }

    return FALSE;
}

LRESULT CALLBACK DialogLoadLevel::listProc(HWND hwnd_list_box, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_LBUTTONDBLCLK:
        const int x = GET_X_LPARAM(lparam);
        const int y = GET_Y_LPARAM(lparam);

        RECT rect;

        // Retrieve the bounding box for the current item
        int selected = SendMessage(hwnd_list_box, LB_GETCURSEL, (WPARAM) 1, (LPARAM) &selected);
        int result = SendMessage(hwnd_list_box, LB_GETITEMRECT, (WPARAM) selected, (LPARAM) &rect);

        // Check that mouse cursor is within the bounding box
        if (x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom)
        {
            SendMessage(GetParent(hwnd_list_box), WM_COMMAND, (WPARAM) IDOK, 0);
        }

        return TRUE;
    }

    return ms_default_list_proc(hwnd_list_box, msg, wparam, lparam);
}
