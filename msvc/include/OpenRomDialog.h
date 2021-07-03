#pragma once

#include <windows.h>

class OpenRomDialog
{
public:
    OpenRomDialog();

    bool showDialog();

    const TCHAR* getFileName() const;
    const TCHAR* getTitleName() const;

protected:
    OPENFILENAME m_ofn;

    TCHAR m_file_name[MAX_PATH];
    TCHAR m_title[MAX_PATH];
};

inline const TCHAR* OpenRomDialog::getFileName() const
{
    return m_file_name;
}

inline const TCHAR* OpenRomDialog::getTitleName() const
{
    return m_title;
}
