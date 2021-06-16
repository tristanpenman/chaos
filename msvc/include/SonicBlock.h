///////////////////////////////////////////////////////////////////////////////
//
// SonicBlock.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SONIC_BLOCK_H
#define __SONIC_BLOCK_H

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
    SonicBlock(const SonicBlock&);
    SonicBlock& operator=(const SonicBlock&);

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

#endif // __SONIC_BLOCK_H
