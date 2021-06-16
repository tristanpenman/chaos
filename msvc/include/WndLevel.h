#ifndef __WND_LEVEL_H
#define __WND_LEVEL_H

class WndLevel
{
public:
    static ATOM getClass(HINSTANCE hInstance);
    static void destroyClass();

protected:
    static int messageCreate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static int messageClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static int messageDestroy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static int messageMDIActivate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static int messageSetFocus(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static int messageSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    static ATOM ms_class;
    static HINSTANCE ms_hInstance;
    static InstanceValue_t<HWND> ms_mapWindows;
};

#endif // __WND_LEVEL_H
