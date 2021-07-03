#pragma once

#include <map>

#include <windows.h>

#include "InstanceValue.h"

class LevelSelectDialog
{
public:
    static int showDialog(HINSTANCE hinst, HWND hwnd_parent);

protected:
    typedef std::map<int, unsigned> ListBoxLevelIndices_t;

    static LRESULT CALLBACK dialogProc(HWND hwnd_dialog, UINT msg, WPARAM wparam, LPARAM lparam);
    static LRESULT CALLBACK listProc(HWND hwnd_dialog, UINT msg, WPARAM wparam, LPARAM lparam);

    static int messageCommand(HWND hwnd_dialog, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageInit(HWND hwnd_dialog, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    static WNDPROC ms_default_list_proc;

    static InstanceValue<ListBoxLevelIndices_t> ms_list_box_level_indices;
};
