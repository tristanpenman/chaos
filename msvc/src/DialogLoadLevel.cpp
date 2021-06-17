#include "Precompiled.h"

#include "InstanceValue.h"
#include "ChaosRom.h"
#include "ChaosApplication.h"
#include "DialogLoadLevel.h"

#define MAX_LEVEL_STR 100

extern ChaosApplication g_application;

using namespace std;

WNDPROC DialogLoadLevel::ms_defaultListProc = NULL;

int DialogLoadLevel::showDialog(HINSTANCE hInstance, HWND hParent)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(ID_DIALOG_LOAD_LEVEL), hParent, reinterpret_cast<DLGPROC>(dialogProc));
}

int DialogLoadLevel::messageCommand(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (wParam == IDOK)
    {
        HWND hListBox = GetDlgItem(hWndDlg, ID_DIALOG_LOAD_LEVEL_LIST);

        int selected = SendMessage(hListBox, LB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
        int level = SendMessage(hListBox, LB_GETITEMDATA, selected, 0);

        EndDialog(hWndDlg, level);
        return TRUE;
    }

    if (wParam == IDCANCEL)
    {
        EndDialog(hWndDlg, -1);
        return TRUE;
    }

    return FALSE;
}

int DialogLoadLevel::messageInit(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hListBox = GetDlgItem(hWndDlg, ID_DIALOG_LOAD_LEVEL_LIST);

    // Save the original list box window procedure
    ms_defaultListProc = (WNDPROC) GetWindowLong(hListBox, GWL_WNDPROC);

    // Drop in the replacement window procedure that handles double clicks
    SetWindowLong(hListBox, GWL_WNDPROC, (LONG) listProc);

    ChaosRom* pRom = g_application.getROM();
    if (!pRom)
    {
        return FALSE;
    }

    ChaosRom::LevelNames_t& levelNames = pRom->getLevelNames();

    if (levelNames.size() == 0)
    {
        return FALSE;
    }

    for (ChaosRom::LevelNames_t::iterator itr = levelNames.begin(); itr != levelNames.end(); ++itr)
    {
        int idx = SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM) itr->second.c_str());
        if (idx == LB_ERR)
        {
            return FALSE;
        }

        SendMessage(hListBox, LB_SETITEMDATA, (WPARAM) idx, (LPARAM) itr->first);
    }

    SendMessage(hListBox, LB_SETCURSEL, 0, 0);

    return TRUE;
}

LRESULT CALLBACK DialogLoadLevel::dialogProc(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return messageInit(hWndDlg, msg, wParam, lParam);

    case WM_COMMAND:
        return messageCommand(hWndDlg, msg, wParam, lParam);
    }

    return FALSE;
}

LRESULT CALLBACK DialogLoadLevel::listProc(HWND hListBox, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_LBUTTONDBLCLK:

        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        RECT rect;

        // Retrieve the bounding box for the current item
        int selected = SendMessage(hListBox, LB_GETCURSEL, (WPARAM) 1, (LPARAM) &selected);
        int result = SendMessage(hListBox, LB_GETITEMRECT, (WPARAM) selected, (LPARAM) &rect);

        // Check that mouse cursor is within the bounding box
        if (x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom)
        {
            SendMessage(GetParent(hListBox), WM_COMMAND, (WPARAM) IDOK, 0);
        }

        return TRUE;
    }

    return ms_defaultListProc(hListBox, msg, wParam, lParam);
}
