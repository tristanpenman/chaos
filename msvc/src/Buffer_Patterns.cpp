#include "Precompiled.h"

#include "SegaPalette.h"
#include "SegaPattern.h"
#include "SonicChunk.h"
#include "SonicBlock.h"
#include "Buffer.h"
#include "Buffer_Patterns.h"

Buffer_Patterns::Buffer_Patterns(const SegaPalette* palettes, unsigned int palette_count, const SegaPattern* patterns, unsigned int pattern_count, HDC hdc)
{
    refresh(palettes, palette_count, patterns, pattern_count, hdc);
}

void Buffer_Patterns::drawPattern(unsigned int pattern_idx, unsigned int palette_idx, HDC dest, int x, int y, bool h_flip, bool v_flip) const
{
    if (pattern_idx >= m_pattern_count) {
        return;
    }

    const unsigned int pattern_width = SegaPattern::getPatternWidth();
    const unsigned int pattern_height = SegaPattern::getPatternHeight();

    draw(
        dest,
        x,
        y,
        pattern_idx * pattern_width,
        palette_idx * pattern_height,
        pattern_width,
        pattern_height,
        h_flip,
        v_flip);
}

void Buffer_Patterns::drawPattern(const SegaPatternDescriptor& pattern_desc, HDC dest, int x, int y, bool h_flip_again, bool v_flip_again) const
{
    if (pattern_desc.getPatternIndex() >= m_pattern_count) {
        return;
    }

    const unsigned int pattern_width = SegaPattern::getPatternWidth();
    const unsigned int pattern_height = SegaPattern::getPatternHeight();

    draw(
        dest,
        x,
        y,
        pattern_desc.getPatternIndex() * pattern_width,
        pattern_desc.getPaletteIndex() * pattern_height,
        pattern_width,
        pattern_height,
        pattern_desc.getHFlip() ^ h_flip_again,
        pattern_desc.getVFlip() ^ v_flip_again);
}

void Buffer_Patterns::refresh(const SegaPalette* palettes, unsigned int palette_count,
                              const SegaPattern* patterns, unsigned int pattern_count, HDC hdc)
{
    m_pattern_count = pattern_count;

    const unsigned int pattern_width = SegaPattern::getPatternWidth();
    const unsigned int pattern_height = SegaPattern::getPatternHeight();

    const unsigned int buffer_width = pattern_width * pattern_count;
    const unsigned int buffer_height = pattern_height * palette_count;

    reset(hdc, buffer_width, buffer_height);
    SelectObject(m_hdc, m_hbitmap);

    for (unsigned int palette_idx = 0; palette_idx < palette_count; palette_idx++)
    {
        const SegaPalette& palette = palettes[palette_idx];

        for (unsigned int pattern_idx = 0; pattern_idx < pattern_count; pattern_idx++)
        {
            for (unsigned int y = 0; y < SegaPattern::getPatternHeight(); y++)
            {
                for (unsigned int x = 0; x < SegaPattern::getPatternWidth(); x++)
                {
                    unsigned char pixel = patterns[pattern_idx].getPixel(x, y);
                    COLORREF c;
                    memcpy(&c, &palette.getNativeColor(pixel), sizeof(DWORD));
                    SetPixel(m_hdc,
                        x + pattern_idx * pattern_width,
                        y + palette_idx * pattern_height,
                        c);
                }
            }
        }
    }
}
