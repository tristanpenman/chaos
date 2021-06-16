#ifndef __COMMON_DIALOG_OPEN_H
#define __COMMON_DIALOG_OPEN_H

class CommonDialog_Open : public CommonDialog
{
public:
	explicit CommonDialog_Open(HWND hOwner) : CommonDialog(hOwner) {}

	virtual bool execute();
};

#endif
