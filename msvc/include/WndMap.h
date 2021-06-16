#ifndef __WND_MAP_H
#define __WND_MAP_H

#ifdef WIN32

class WndMap 
{
public:

	static HWND    createWindow(HINSTANCE hInstance, HWND hParent);
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	static void    destroyClass();
	static ATOM    getClass(HINSTANCE hInstance);

protected:

	static int  messageCreate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int  messageDestroy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int  messageKeyDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int  messageMouseWheel(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int  messagePaint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int  messageScrollHorizontal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int  messageScrollVertical(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int  messageSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	static void resetScrollBars(HWND hwnd);

	static void scrollHorizontalRefresh(HWND hwnd, short w);
	static void scrollHorizontalRel(HWND hwnd, int rel_y);
	static void scrollHorizontalRelPages(HWND hwnd, int rel_y);

	static void scrollVerticalRefresh(HWND hwnd, short h);
	static void scrollVerticalRel(HWND hwnd, int rel_y);
	static void scrollVerticalRelPages(HWND hwnd, int rel_y);

private:

	static ATOM      ms_class;
	static HINSTANCE ms_hInstance;

	static InstanceValue_t<int> x;
	static InstanceValue_t<int> y;
	static InstanceValue_t<unsigned int> blockWidth;
	static InstanceValue_t<unsigned int> blockHeight;
};

#endif // WIN32

#endif // __WND_MAP_H