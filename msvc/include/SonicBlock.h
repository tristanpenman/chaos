#pragma once

#define BYTES_PER_CHUNK 2

class SonicChunk;
class SonicChunkDescriptor;

class SonicBlock
{
public:
    SonicBlock(unsigned int w, unsigned int h);
    SonicBlock(const SonicBlock&) = delete;
    virtual ~SonicBlock();

    SonicBlock& operator=(const SonicBlock&) = delete;

    bool loadFromBuffer(unsigned char* buffer);

    const SonicChunkDescriptor& getChunkDescriptor(unsigned int x, unsigned int y) const;

    static unsigned int calculateBlockSize(unsigned int w, unsigned int h);

    unsigned int getChunksAcross() const;
    unsigned int getChunksDown() const;

private:
    SonicChunkDescriptor* m_chunkDescriptors;

    unsigned int m_chunksAcross;
    unsigned int m_chunksDown;
    unsigned int m_chunkCount;
};

inline unsigned int SonicBlock::getChunksAcross() const
{
    return m_chunksAcross;
}

inline unsigned int SonicBlock::getChunksDown() const
{
    return m_chunksDown;
}
