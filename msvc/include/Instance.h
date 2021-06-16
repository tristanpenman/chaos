#ifndef __INSTANCE_H
#define __INSTANCE_H

/******************************************************************************
 *
 * Instance abstract class
 *
 * Provides a very simplistic interface to check whether or not changes have
 * been made in the given instance, and to save them if necessary.
 *
 *****************************************************************************/

class Instance
{
public:

	explicit Instance();

/******************************************************************************
 *
 * Public abstract interface
 *
 *****************************************************************************/

	virtual bool hasUnsavedChanges() const = 0;
	virtual bool saveChanges() = 0;

/******************************************************************************
 *
 * Win32 specific code
 *
 *****************************************************************************/

#ifdef WIN32

	void setWindow(HWND hwnd);

	HWND getWindow() const;

protected:

	HWND m_hWnd;

#endif

};

enum InstanceKind
{
	INSTANCE_KIND_LEVEL,
	INSTANCE_KIND_SPRITES
};

#endif
