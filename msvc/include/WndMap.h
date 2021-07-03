#pragma once

#include "InstanceValue.h"

class WndMap
{
public:
    static HWND createWindow(HINSTANCE hinst, HWND hwnd_parent);
    static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static void destroyClass();
    static ATOM getClass(HINSTANCE hinst);

protected:
    static int messageCreate(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageDestroy(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageKeyDown(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageMouseWheel(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messagePaint(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageScrollHorizontal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageScrollVertical(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static int messageSize(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static void resetScrollBars(HWND hwnd);

    static void scrollHorizontalRefresh(HWND hwnd, short w);
    static void scrollHorizontalRel(HWND hwnd, int rel_y);
    static void scrollHorizontalRelPages(HWND hwnd, int rel_y);

    static void scrollVerticalRefresh(HWND hwnd, short h);
    static void scrollVerticalRel(HWND hwnd, int rel_y);
    static void scrollVerticalRelPages(HWND hwnd, int rel_y);

private:
    static ATOM ms_class;
    static HINSTANCE ms_hinst;

    static InstanceValue<int> ms_x;
    static InstanceValue<int> ms_y;
    static InstanceValue<unsigned int> ms_block_width;
    static InstanceValue<unsigned int> ms_block_height;
};
