#pragma once

#include "windows.h"

class WndFrame
{
public:
    static HWND createWindow(HINSTANCE hinst, HWND hwnd_parent);
    static LRESULT CALLBACK frameWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static void destroyClass();

private:
    static HWND createLevelWindow(HWND hClient);
    static bool openFile();
    static void enableMenuItems(HWND hwnd);
    static BOOL CALLBACK closeEnumProc(HWND hwnd, LPARAM lparam);

    static ATOM ms_class;
    static HINSTANCE ms_hinst;
};
