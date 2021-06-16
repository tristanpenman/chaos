#ifndef __COMMON_DIALOG_SAVE_H
#define __COMMON_DIALOG_SAVE_H

class CommonDialog_Save : public CommonDialog
{
public:
    explicit CommonDialog_Save(HWND hOwner)
      : CommonDialog(hOwner)
    {

    }

    virtual bool execute();
};

#endif
