#ifndef __WND_FRAME_H
#define __WND_FRAME_H

#ifdef WIN32

class WndFrame 
{
public:

	static HWND    createWindow(HINSTANCE hInstance, HWND hParent);
	static LRESULT CALLBACK frameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static void    destroyClass();

private:

	static HWND    createLevelWindow(HWND hClient);
	static bool    openFile();
	static void    enableMenuItems(HWND hwnd);

	static BOOL CALLBACK closeEnumProc(HWND hwnd, LPARAM lParam);

	/** VARIABLES **/

	static ATOM      ms_class;
	static HINSTANCE ms_hInstance;
};

#endif // WIN32

#endif // __WND_FRAME_H
