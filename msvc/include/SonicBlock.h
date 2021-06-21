#pragma once

#define BYTES_PER_CHUNK 2

class SonicChunk;
class SonicChunkDescriptor;

class SonicBlock
{
public:
    SonicBlock(unsigned int w, unsigned int h);

    virtual ~SonicBlock();

    bool loadFromBuffer(unsigned char* buffer);

    const SonicChunkDescriptor& getChunkDescriptor(unsigned int x, unsigned int y) const;

    static unsigned int calculateBlockSize(unsigned int w, unsigned int h);

    unsigned int getChunksAcross() const;
    unsigned int getChunksDown() const;

private:
    SonicBlock(const SonicBlock&) DELETED;
    SonicBlock& operator=(const SonicBlock&) DELETED;

    SonicChunkDescriptor* m_chunk_descriptors;

    unsigned int m_chunks_across;
    unsigned int m_chunks_down;
    unsigned int m_chunk_count;
};

inline unsigned int SonicBlock::getChunksAcross() const
{
    return m_chunks_across;
}

inline unsigned int SonicBlock::getChunksDown() const
{
    return m_chunks_down;
}
