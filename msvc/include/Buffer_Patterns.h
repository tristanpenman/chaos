#pragma once

class SegaPalette;
class SegaPattern;
class SegaPatternDescriptor;

/**
 * Patterns image buffer class
 *
 * This class is pretty raw, and doesn't provide much protection. Pattern indices
 * should be checked against sane values to ensure that BitBlts don't copy data
 * from uninitialised locations in memory.
 */
class Buffer_Patterns : public Buffer
{
public:
    Buffer_Patterns(
        const SegaPalette* palettes, unsigned int palette_count,
        const SegaPattern* patterns, unsigned int pattern_count, HDC hdc);

    void drawPattern(
        unsigned int pattern_index, unsigned int palette_index,
        HDC dest, int x, int y, bool h_flip, bool v_flip) const;

    void drawPattern(
        const SegaPatternDescriptor& pattern_desc,
        HDC dest, int x, int y, bool h_flip_again, bool v_flip_again) const;

    void refresh(
        const SegaPalette* palettes, unsigned int palette_count,
        const SegaPattern* patterns, unsigned int pattern_count, HDC hdc);

private:
    unsigned int m_pattern_count;
};
