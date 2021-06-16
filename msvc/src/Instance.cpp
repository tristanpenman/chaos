#include "Precompiled.h"

#include "Instance.h"

Instance::Instance()
  : m_hWnd(0)
{

}

void Instance::setWindow(HWND hWnd)
{
    m_hWnd = hWnd;
}

HWND Instance::getWindow() const
{
    return m_hWnd;
}
