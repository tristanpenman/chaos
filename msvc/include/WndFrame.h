#pragma once

class WndFrame
{
public:
    static HWND createWindow(HINSTANCE hInstance, HWND hParent);
    static LRESULT CALLBACK frameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static void destroyClass();

private:
    static HWND createLevelWindow(HWND hClient);
    static bool openFile();
    static void enableMenuItems(HWND hwnd);
    static BOOL CALLBACK closeEnumProc(HWND hwnd, LPARAM lParam);

    static ATOM ms_class;
    static HINSTANCE ms_hInstance;
};
