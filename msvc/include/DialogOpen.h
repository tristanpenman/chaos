#pragma once

#include <windows.h>

class DialogOpen
{
public:
    explicit DialogOpen();

    bool showDialog();

    const TCHAR* getFileName() const;
    const TCHAR* getTitleName() const;

protected:
    OPENFILENAME m_ofn;

    TCHAR m_szFileName[MAX_PATH];
    TCHAR m_szTitleName[MAX_PATH];
};

inline const TCHAR* DialogOpen::getFileName() const
{
    return m_szFileName;
}

inline const TCHAR* DialogOpen::getTitleName() const
{
    return m_szTitleName;
}
