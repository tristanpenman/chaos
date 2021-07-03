#include "Precompiled.h"

#include "Chunk.h"
#include "ChunkDesc.h"
#include "Palette.h"
#include "Pattern.h"

#include "Block.h"

Block::Block(unsigned int w, unsigned int h)
    : m_chunk_descriptors(NULL)
    , m_chunks_across(w / Chunk::getChunkWidth())
    , m_chunks_down(h / Chunk::getChunkHeight())
    , m_chunk_count(m_chunks_across * m_chunks_down)
{
    m_chunk_descriptors = new ChunkDesc[m_chunk_count];

    memset(m_chunk_descriptors, 0, sizeof(ChunkDesc) * m_chunk_count);
}

Block::~Block()
{
    if (m_chunk_descriptors)
    {
        delete[] m_chunk_descriptors;
        m_chunk_descriptors = NULL;
    }
}

bool Block::loadFromBuffer(unsigned char *buffer)
{
    unsigned short index = 0;
    unsigned int buffer_pos = 0;

    for (unsigned int chunkIndex = 0; chunkIndex < m_chunk_count; chunkIndex++)
    {
        // Build index
        index  = (buffer[buffer_pos] << 8) & 0xFF00;
        index |= (buffer[buffer_pos + 1]) & 0x00FF;

        buffer_pos += BYTES_PER_CHUNK;

        m_chunk_descriptors[chunkIndex].set(index);
    }

    return true;
}

const ChunkDesc& Block::getChunkDesc(unsigned int x, unsigned int y) const
{
    if (x >= m_chunks_across || y >= m_chunks_across)
    {
        throw std::runtime_error("Invalid chunk index");
    }

    return m_chunk_descriptors[y * m_chunks_across + x];
}

unsigned int Block::calculateBlockSize(unsigned int w, unsigned int h)
{
    const unsigned int chunk_count =
            w / Chunk::getChunkWidth() *
            h / Chunk::getChunkHeight();

    return chunk_count * BYTES_PER_CHUNK;
}
