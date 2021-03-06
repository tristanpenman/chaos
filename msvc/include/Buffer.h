#pragma once

/**
 * Image buffer base class
 *
 * Provides the basic functionality to create image buffers on Windows.
 *
 * This class is pretty raw, and doesn't provide much protection. Bitmap
 * offsets should be checked before being passed to the drawing functions.
 */
class Buffer
{
public:
    Buffer();
    Buffer(HDC compatible_with, int w, int h);

    virtual ~Buffer();

protected:
    void draw(HDC dest, int dest_x, int dest_y, int src_x, int src_y, int w, int h, bool h_flip, bool v_flip) const;
    void reset(HDC compatible_with, int w, int h);

    HBITMAP m_hbitmap;
    HDC     m_hdc;
};

inline void Buffer::draw(HDC dest, int dest_x, int dest_y, int src_x, int src_y, int w, int h, bool h_flip, bool v_flip) const
{
    if (h_flip)
    {
        if (v_flip)
        {
            StretchBlt(dest, dest_x, dest_y, w, h, m_hdc, src_x + w - 1, src_y + h - 1, -w, -h, SRCCOPY);
        }
        else
        {
            StretchBlt(dest, dest_x, dest_y, w, h, m_hdc, src_x + w - 1, src_y, -w, h, SRCCOPY);
        }
    }
    else
    {
        if (v_flip)
        {
            StretchBlt(dest, dest_x, dest_y, w, h, m_hdc, src_x, src_y + h - 1, w, -h, SRCCOPY);
        }
        else
        {
            BitBlt(dest, dest_x, dest_y, w, h, m_hdc, src_x, src_y, SRCCOPY);
        }
    }
}
