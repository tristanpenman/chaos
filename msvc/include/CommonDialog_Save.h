#pragma once

class CommonDialog_Save : public CommonDialog
{
public:
    explicit CommonDialog_Save(HWND hOwner)
      : CommonDialog(hOwner)
    {

    }

    virtual bool execute();
};
