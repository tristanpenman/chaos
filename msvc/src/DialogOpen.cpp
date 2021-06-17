#include "Precompiled.h"

#include "DialogOpen.h"

DialogOpen::DialogOpen()
    : m_szFileName("")
{
    static TCHAR szFilter[] =
        TEXT("Genesis Binary ROMs (*.bin)\0*.bin\0")  \
        TEXT("All Files (*.*)\0*.*\0\0");

    m_ofn.lStructSize = sizeof(OPENFILENAME);
    m_ofn.hwndOwner = 0;
    m_ofn.hInstance = NULL;
    m_ofn.lpstrFilter = szFilter;
    m_ofn.lpstrCustomFilter = NULL;
    m_ofn.nMaxCustFilter = 0;
    m_ofn.nFilterIndex = 0;
    m_ofn.lpstrFile = NULL;
    m_ofn.nMaxFile = MAX_PATH;
    m_ofn.lpstrFileTitle = NULL;
    m_ofn.nMaxFileTitle = MAX_PATH;
    m_ofn.lpstrInitialDir = NULL;
    m_ofn.lpstrTitle = NULL;
    m_ofn.Flags = 0;
    m_ofn.nFileOffset = 0;
    m_ofn.nFileExtension = 0;
    m_ofn.lpstrDefExt = TEXT("bin");
    m_ofn.lCustData = 0L;
    m_ofn.lpfnHook = NULL;
    m_ofn.lpTemplateName = NULL;
    m_ofn.lpstrFile = m_szFileName;
    m_ofn.lpstrFileTitle = m_szTitleName;
}

bool DialogOpen::showDialog()
{
    m_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
    return GetOpenFileName(&m_ofn) == TRUE;
}
