///////////////////////////////////////////////////////////////////////////////
//
// SonicChunk.cpp
//
///////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"

#include "ChaosException.h"

#include "SegaPalette.h"
#include "SegaPattern.h"

#include "SonicChunk.h"


///////////////////////////////////////////////////////////////////////////////
//
// SonicChunk class implementation
//
///////////////////////////////////////////////////////////////////////////////

SonicChunk::SonicChunk()
{
    memset(m_pattern_indices, 0, sizeof(SegaPatternDescriptor) * PATTERNS_PER_CHUNK);
}

SonicChunk::~SonicChunk()
{

}

void SonicChunk::loadFromBuffer(unsigned char buffer[CHUNK_SIZE_IN_ROM])
{
    unsigned short index = 0;

    for (unsigned int i = 0; i < PATTERNS_PER_CHUNK; i++)
    {
        // Build index
        index  = (buffer[0] << 8) & 0xFF00;
        index |= (buffer[1])      & 0x00FF;

        // Set index
        m_pattern_indices[i].set(index);

        // Next...
        buffer += SegaPatternDescriptor::getIndexSize();
    }
}

const SegaPatternDescriptor& SonicChunk::getPatternDescriptor(unsigned int x, unsigned int y) const
{
    if (x > 1 || y > 1)
    {
        throw ChaosException("Invalid pattern index");
    }

    return m_pattern_indices[y * 2 + x];
}


///////////////////////////////////////////////////////////////////////////////
//
// SonicChunkDescriptor class implementation
//
///////////////////////////////////////////////////////////////////////////////

SonicChunkDescriptor::SonicChunkDescriptor()
: m_index(0)
{

}

SonicChunkDescriptor::~SonicChunkDescriptor()
{

}

/*

#ifdef WIN32

void SonicChunk::drawChunk(HDC hdc, int x, int y, const SonicChunk* chunks, unsigned int chunk_count, const SonicChunkDescriptor& desc, const Buffer_Patterns* pPatternBuffer)
{
	unsigned short chunk_index = desc.getChunkIndex();
	
	if (chunk_index < chunk_count)
	{
		chunks[chunk_index].drawChunk(hdc, x, y, desc.getFlag_HFlip(), desc.getFlag_VFlip(), pPatternBuffer);
	}
}

void SonicChunk::drawChunk(HDC hdc, int x, int y, bool h_flip, bool v_flip, const Buffer_Patterns* pPatternBuffer) const
{
	const unsigned int pattern_width = SegaPattern::getPatternWidth();
	const unsigned int pattern_height = SegaPattern::getPatternHeight();

	const int base_x = x + (h_flip ? pattern_width : 0);

	int px = 0;
	int py = y + (v_flip ? pattern_height : 0);

	for (int pat_y = 0; pat_y < 2; pat_y++)
	{
		px = base_x;

		for (int pat_x = 0; pat_x < 2; pat_x++)
		{					
			pPatternBuffer->drawPattern(
				getPatternDescriptor(pat_x, pat_y), hdc, 
				px, 
				py, 
				h_flip, 
				v_flip);

			px = h_flip ? px - pattern_width : px + pattern_width;
		}

		py = v_flip ? py - pattern_height : py + pattern_height;
	}
}

#endif

*/

