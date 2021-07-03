#include "Precompiled.h"

#include "Buffer.h"

Buffer::Buffer()
  : m_hbitmap(0)
  , m_hdc(0)
{

}

Buffer::Buffer(HDC compatible_with, int w, int h)
  : m_hbitmap(0)
  , m_hdc(0)
{
    reset(compatible_with, w, h);
}

Buffer::~Buffer()
{
    if (m_hdc)
    {
        DeleteDC(m_hdc);
        m_hdc = 0;
    }

    if (m_hbitmap)
    {
        DeleteObject(m_hbitmap);
        m_hbitmap = 0;
    }
}

void Buffer::reset(HDC compatible_with, int w, int h)
{
    m_hbitmap = CreateCompatibleBitmap(compatible_with, w, h);
    if (!m_hbitmap)
    {
        throw std::runtime_error("Could not allocate memory for pattern buffer");
    }

    m_hdc = CreateCompatibleDC(compatible_with);
    if (!m_hdc)
    {
        throw std::runtime_error("Could not create pattern buffer device context");
    }
}
