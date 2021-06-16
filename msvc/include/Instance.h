#pragma once

/**
 * Instance abstract class
 *
 * Provides a very simplistic interface to check whether or not changes have
 * been made in the given instance, and to save them if necessary.
 */
class Instance
{
public:
    explicit Instance();

    // abstract
    virtual bool hasUnsavedChanges() const = 0;
    virtual bool saveChanges() = 0;

    void setWindow(HWND hwnd);
    HWND getWindow() const;

protected:
    HWND m_hWnd;
};

enum InstanceKind
{
    INSTANCE_KIND_LEVEL
};
