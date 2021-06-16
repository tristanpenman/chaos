#include "Precompiled.h"

#include "ChaosException.h"
#include "Buffer.h"

/******************************************************************************
 *
 * Constructors and destructor methods
 *
 *****************************************************************************/

Buffer::Buffer()
  : m_hBitmap(0)
  , m_hDC(0)
{

}

Buffer::Buffer(HDC compatible_with, int w, int h)
  : m_hBitmap(0)
  , m_hDC(0)
{
    reset(compatible_with, w, h);
}

Buffer::~Buffer()
{
    if (m_hDC)
    {
        DeleteDC(m_hDC);
        m_hDC = 0;
    }

    if (m_hBitmap)
    {
        DeleteObject(m_hBitmap);
        m_hBitmap = 0;
    }
}


/******************************************************************************
 *
 * Interface provided to subclasses
 *
 *****************************************************************************/

void Buffer::reset(HDC compatible_with, int w, int h)
{
    m_hBitmap = CreateCompatibleBitmap(compatible_with, w, h);
    if (!m_hBitmap)
    {
        throw ChaosException("Could not allocate memory for pattern buffer");
    }

    m_hDC = CreateCompatibleDC(compatible_with);
    if (!m_hDC)
    {
        throw ChaosException("Could not create pattern buffer device context");
    }
}
