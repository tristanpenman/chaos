#pragma once

/**
 * Instance base class
 *
 * Represents an instance of a level editor
 */
class Instance
{
public:
    explicit Instance();
    virtual ~Instance() = default;

    void setWindow(HWND hwnd);
    HWND getWindow() const;

protected:
    HWND m_hWnd;
};
