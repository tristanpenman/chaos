#pragma once

#define BYTES_PER_CHUNK 2

class Chunk;
class ChunkDesc;

class Block
{
public:
    Block(unsigned int w, unsigned int h);

    virtual ~Block();

    bool loadFromBuffer(unsigned char* buffer);

    const ChunkDesc& getChunkDesc(unsigned int x, unsigned int y) const;

    static unsigned int calculateBlockSize(unsigned int w, unsigned int h);

    unsigned int getChunksAcross() const;
    unsigned int getChunksDown() const;

private:
    Block(const Block&) DELETED;
    Block& operator=(const Block&) DELETED;

    ChunkDesc* m_chunk_descriptors;

    unsigned int m_chunks_across;
    unsigned int m_chunks_down;
    unsigned int m_chunk_count;
};

inline unsigned int Block::getChunksAcross() const
{
    return m_chunks_across;
}

inline unsigned int Block::getChunksDown() const
{
    return m_chunks_down;
}
