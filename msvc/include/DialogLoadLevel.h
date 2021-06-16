#ifndef __DIALOG_LOAD_LEVEL_H
#define __DIALOG_LOAD_LEVEL_H

#ifdef WIN32

class DialogLoadLevel
{
public:

	static int showDialog(HINSTANCE hInstance, HWND hParent);

protected:

	typedef std::map<int, unsigned> ListBoxLevelIndices_t;

	static LRESULT CALLBACK dialogProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK listProc(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam);

	static int messageCommand(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
	static int messageInit(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

private:

	static WNDPROC ms_defaultListProc;

	static InstanceValue_t<ListBoxLevelIndices_t> ms_listBoxLevelIndices;
};

#endif // WIN32

#endif // __DIALOG_LOAD_LEVEL_H