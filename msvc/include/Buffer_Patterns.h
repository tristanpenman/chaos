#ifndef __PATTERN_BUFFER_H
#define __PATTERN_BUFFER_H

class SegaPalette;
class SegaPattern;
class SegaPatternDescriptor;

/******************************************************************************
 *
 * Blocks image buffer class
 *
 * This class is pretty raw, and doesn't provide much protection. Pattern indices
 * should be checked against sane values to ensure that BitBlts don't copy data
 * from uninitialised locations in memory.
 *
 *****************************************************************************/

class Buffer_Patterns : public Buffer
{
public:
    explicit Buffer_Patterns(
        const SegaPalette* palettes, unsigned int palette_count,
        const SegaPattern* patterns, unsigned int pattern_count, HDC hdc);

/******************************************************************************
 *
 * Public interface
 *
 *****************************************************************************/

    void drawPattern(
        unsigned int pattern_index, unsigned int palette_index,
        HDC dest, int x, int y, bool h_flip, bool v_flip) const;

    void drawPattern(
        const SegaPatternDescriptor& pattern_desc,
        HDC dest, int x, int y, bool h_flip_again, bool v_flip_again) const;

    void refresh(
        const SegaPalette* palettes, unsigned int palette_count,
        const SegaPattern* patterns, unsigned int pattern_count, HDC hdc);
};

#endif	// __PATTERN_BUFFER_H
