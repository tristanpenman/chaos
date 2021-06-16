#include "Precompiled.h"

#include "ChaosException.h"
#include "SegaPalette.h"
#include "SegaPattern.h"
#include "SonicChunk.h"
#include "SonicBlock.h"
#include "Buffer.h"
#include "Buffer_Patterns.h"

/******************************************************************************
 *
 * Constructors and destructor methods
 *
 *****************************************************************************/

Buffer_Patterns::Buffer_Patterns(const SegaPalette* palettes, unsigned int palCount, const SegaPattern* patterns, unsigned int patternCount, HDC hdc)
{
    refresh(palettes, palCount, patterns, patternCount, hdc);
}

/******************************************************************************
 *
 * Public interface
 *
 *****************************************************************************/

void Buffer_Patterns::drawPattern(unsigned int patternIndex, unsigned int palIndex, HDC dest, int x, int y, bool h_flip, bool v_flip) const
{
    const unsigned int patternWidth = SegaPattern::getPatternWidth();
    const unsigned int patternHeight = SegaPattern::getPatternHeight();

    draw(
        dest,
        x,
        y,
        patternIndex * patternWidth,
        palIndex * patternHeight,
        patternWidth,
        patternHeight,
        h_flip,
        v_flip);
}

void Buffer_Patterns::drawPattern(const SegaPatternDescriptor& pattern_desc, HDC dest, int x, int y, bool h_flip_again, bool v_flip_again) const
{
    const unsigned int patternWidth = SegaPattern::getPatternWidth();
    const unsigned int patternHeight = SegaPattern::getPatternHeight();

    draw(
        dest,
        x,
        y,
        pattern_desc.getPatternIndex() * patternWidth,
        pattern_desc.getPaletteIndex() * patternHeight,
        patternWidth,
        patternHeight,
        pattern_desc.getFlag_HFlip() ^ h_flip_again,
        pattern_desc.getFlag_VFlip() ^ v_flip_again);
}

void Buffer_Patterns::refresh(const SegaPalette* palettes, unsigned int palCount,
                              const SegaPattern* patterns, unsigned int patternCount, HDC hdc)
{
    COLORREF c;

    const unsigned int patternWidth = SegaPattern::getPatternWidth();
    const unsigned int patternHeight = SegaPattern::getPatternHeight();

    const unsigned int bufferWidth = patternWidth * patternCount;
    const unsigned int bufferHeight = patternHeight * palCount ;

    reset(hdc, bufferWidth, bufferHeight);
    SelectObject(m_hDC, m_hBitmap);

    for (unsigned int palIndex = 0; palIndex < palCount; palIndex++)
    {
        const SegaPalette& palette = palettes[palIndex];

        for (unsigned int patternIndex = 0; patternIndex < patternCount; patternIndex++)
        {
            for (unsigned int y = 0; y < SegaPattern::getPatternHeight(); y++)
            {
                for (unsigned int x = 0; x < SegaPattern::getPatternWidth(); x++)
                {
                    unsigned char pixel = patterns[patternIndex].getPixel(x, y);
                    memcpy(&c, &palette.getNativeColor(pixel), sizeof(DWORD));
                    SetPixel(m_hDC,
                        x + patternIndex * patternWidth,
                        y + palIndex * patternHeight,
                        c);
                }
            }
        }
    }
}
