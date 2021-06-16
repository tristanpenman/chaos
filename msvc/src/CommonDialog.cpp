#include "Precompiled.h"

#include "CommonDialog.h"

CommonDialog::CommonDialog(HWND hOwner)
  : m_szFileName("")
{
    static TCHAR szFilter[] =
        TEXT ("Genesis Binary ROMs (*.bin)\0*.bin\0")  \
        TEXT ("All Files (*.*)\0*.*\0\0") ;

    m_ofn.lStructSize       = sizeof (OPENFILENAME) ;
    m_ofn.hwndOwner         = hOwner ;
    m_ofn.hInstance         = NULL ;
    m_ofn.lpstrFilter       = szFilter ;
    m_ofn.lpstrCustomFilter = NULL ;
    m_ofn.nMaxCustFilter    = 0 ;
    m_ofn.nFilterIndex      = 0 ;
    m_ofn.lpstrFile         = NULL ;          // Set in Open and Close functions
    m_ofn.nMaxFile          = MAX_PATH ;
    m_ofn.lpstrFileTitle    = NULL ;          // Set in Open and Close functions
    m_ofn.nMaxFileTitle     = MAX_PATH ;
    m_ofn.lpstrInitialDir   = NULL ;
    m_ofn.lpstrTitle        = NULL ;
    m_ofn.Flags             = 0 ;             // Set in Open and Close functions
    m_ofn.nFileOffset       = 0 ;
    m_ofn.nFileExtension    = 0 ;
    m_ofn.lpstrDefExt       = TEXT ("bin") ;
    m_ofn.lCustData         = 0L ;
    m_ofn.lpfnHook          = NULL ;
    m_ofn.lpTemplateName    = NULL ;
    m_ofn.lpstrFile         = m_szFileName;
    m_ofn.lpstrFileTitle    = m_szTitleName;
}
