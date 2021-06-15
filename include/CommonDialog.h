#ifndef __COMMON_DIALOG_H
#define __COMMON_DIALOG_H

class CommonDialog
{
public:
	explicit CommonDialog(HWND hOwner);

	virtual bool execute() = 0;

	const TCHAR* getFileName() const;
	const TCHAR* getTitleName() const;

protected:

	OPENFILENAME m_ofn;

	TCHAR m_szFileName  [MAX_PATH];
	TCHAR m_szTitleName [MAX_PATH];
};

inline const TCHAR* CommonDialog::getFileName() const
{
	return m_szFileName;
}

inline const TCHAR* CommonDialog::getTitleName() const
{
	return m_szTitleName;
}

#endif
