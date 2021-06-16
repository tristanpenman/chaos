#ifndef __COMMON_DIALOG_FACTORY_H
#define __COMMON_DIALOG_FACTORY_H

class CommonDialog;
class CommonDialog_Open;
class CommonDialog_Save;

class CommonDialogFactory
{
public:
	static CommonDialog& openDialog();
	static CommonDialog& saveDialog();

private:
	static CommonDialog_Open& ms_dialogOpen;
	static CommonDialog_Save& ms_dialogSave;
};

#endif
