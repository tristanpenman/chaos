#pragma once

#include "InstanceValue.h"

class WndLevel
{
public:
    static ATOM getClass(HINSTANCE hinst);
    static void destroyClass();

protected:
    static int messageCreate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageClose(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageDestroy(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageMDIActivate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageSetFocus(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageSize(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    static ATOM ms_class;
    static HINSTANCE ms_hinst;
    static InstanceValue<HWND> ms_map_windows;
};
