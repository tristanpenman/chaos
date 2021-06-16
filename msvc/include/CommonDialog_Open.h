#pragma once

class CommonDialog_Open : public CommonDialog
{
public:
    explicit CommonDialog_Open(HWND hOwner)
      : CommonDialog(hOwner)
    {

    }

    virtual bool execute();
};
