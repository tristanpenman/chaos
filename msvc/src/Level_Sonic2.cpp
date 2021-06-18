#include "Precompiled.h"

#include "ChaosRom.h"
#include "ChaosRom_Sonic2.h"

#include "SegaPalette.h"
#include "SegaPattern.h"
#include "SonicChunk.h"
#include "SonicBlock.h"
#include "SonicMap.h"
#include "SonicReader.h"

#include "Buffer.h"
#include "Buffer_Blocks.h"
#include "Buffer_Patterns.h"

#include "Level_Sonic2.h"

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF  // 64kB

#define BLOCK_WIDTH 128
#define BLOCK_HEIGHT 128

using namespace std;

Level_Sonic2::Level_Sonic2(ChaosRom_Sonic2& rom)
  : m_rom(rom)
  , m_level_index(0)
  , m_level_loaded(false)
  , m_palettes(NULL)
  , m_patterns(NULL)
  , m_chunks(NULL)
  , m_blockPtrs(NULL)
  , m_pMap(NULL)
  , m_palette_count(0)
  , m_pattern_count(0)
  , m_chunk_count(0)
  , m_blockCount(0)
  , m_pBufferBlocks(NULL)
  , m_pBufferPatterns(NULL)
{

}

Level_Sonic2::~Level_Sonic2()
{
    cleanup();
}

bool Level_Sonic2::loadPalettes()
{
    fstream& file = m_rom.getFile();
    streamoff paletteAddr = m_rom.getPalettesAddress(m_level_index);
    file.seekg(paletteAddr);

    m_palettes = new SegaPalette[PALETTE_COUNT];

    for (unsigned int palRow = 1; palRow < PALETTE_COUNT; palRow++)
    {
        if (!m_palettes[palRow].readFromFile(file))
        {
            return false;
        }
    }

    m_palette_count = PALETTE_COUNT;

    return true;
}

bool Level_Sonic2::loadPatterns()
{
    fstream& file = m_rom.getFile();

    vector<unsigned char> buffer(PATTERN_BUFFER_SIZE);

    streamoff patternsAddr = m_rom.getPatternsAddress(m_level_index);

    SonicReader sr(file);
    SonicReader::result_t r = sr.decompress(buffer.data(), PATTERN_BUFFER_SIZE, patternsAddr);

    if (r.first)
    {
        const unsigned int pattern_size = SegaPattern::getPatternSize();
        const unsigned int pattern_count = r.second / pattern_size;

        if (r.second % pattern_size != 0)
        {
            REPORT_ERROR("Inconsistent pattern data", "Decompression error");
        }

        unsigned int pattern_idx = 0;
        unsigned int bufferPos = 0;

        m_patterns = new SegaPattern[pattern_count];

        while (pattern_idx < pattern_count)
        {
            m_patterns[pattern_idx].loadFromBuffer(&buffer[bufferPos]);

            pattern_idx++;
            bufferPos += pattern_size;
        }

        m_pattern_count = pattern_count;

        return true;
    }

    return false;
}

bool Level_Sonic2::loadChunks()
{
    fstream& file = m_rom.getFile();

    vector<unsigned char> buffer(CHUNK_BUFFER_SIZE);

    streamoff chunks_addr = m_rom.getChunksAddress(m_level_index);

    SonicReader sr(file);
    SonicReader::result_t r = sr.decompress(buffer.data(), CHUNK_BUFFER_SIZE, chunks_addr);

    if (r.first)
    {
        const unsigned int chunk_size = SonicChunk::getChunkSize();
        const unsigned int chunk_count = r.second / chunk_size;

        if (r.second % chunk_size != 0)
        {
            REPORT_ERROR("Inconsistent chunk data", "Decompression error");
        }

        unsigned int chunk_idx = 0;
        unsigned int bufferPos = 0;

        m_chunks = new SonicChunk[chunk_count];

        while (chunk_idx < chunk_count)
        {
            m_chunks[chunk_idx].loadFromBuffer(&buffer[bufferPos]);

            chunk_idx++;
            bufferPos += chunk_size;
        }

        m_chunk_count = chunk_count;

        return true;
    }

    return false;
}

bool Level_Sonic2::loadBlocks()
{
    fstream& file = m_rom.getFile();

    vector<unsigned char> buffer(BLOCK_BUFFER_SIZE);

    streamoff blocks_addr = m_rom.getBlocksAddress(m_level_index);

    SonicReader sr(file);
    SonicReader::result_t r = sr.decompress(buffer.data(), BLOCK_BUFFER_SIZE, blocks_addr);

    if (r.first)
    {
        const unsigned int blockSize = getBlockWidth();
        const unsigned int blockCount = r.second / blockSize;

        unsigned int bufferPos = 0;

        if (r.second % blockSize != 0)
        {
            throw std::runtime_error("Invalid block data");
        }

        m_blockPtrs = new SonicBlock*[blockCount];

        for (unsigned int blockIndex = 0; blockIndex < blockCount; blockIndex++)
        {
            m_blockPtrs[blockIndex] = new SonicBlock(getBlockWidth(), getBlockHeight());
            m_blockPtrs[blockIndex]->loadFromBuffer(&buffer[bufferPos]);
            bufferPos += blockSize;
        }

        m_blockCount = blockCount;

        return true;
    }

    return false;
}

bool Level_Sonic2::loadMap()
{
    fstream& file = m_rom.getFile();

    vector<unsigned char> buffer(LEVEL_BUFFER_SIZE);

    SonicReader reader(file);

    SonicReader::result_t result =
        reader.decompress(
            buffer.data(),
            LEVEL_BUFFER_SIZE,
            m_rom.getMapAddress(m_level_index));

    if (result.first)
    {
        m_pMap = new SonicMap(2, 128, 16, buffer.data());
        return true;
    }

    return false;
}

bool Level_Sonic2::bufferPatterns()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_pBufferPatterns = new Buffer_Patterns(getPalettes(), getPaletteCount(), getPatterns(), getPatternCount(), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

bool Level_Sonic2::bufferBlocks()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_pBufferBlocks = new Buffer_Blocks(this, getBlocks(), getBlockCount(), getChunks(), getChunkCount(), &(getPatternBuffer()), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

void Level_Sonic2::unloadPatternBuffer()
{
    // TODO
}

void Level_Sonic2::unloadBlockBuffer()
{
    // TODO
}

void Level_Sonic2::cleanup()
{
    m_level_loaded = false;
    m_level_index = 0;

    unloadPatternBuffer();
    unloadBlockBuffer();
    unloadMap();
    unloadBlocks();
    unloadChunks();
    unloadPatterns();
    unloadPalettes();
}

void Level_Sonic2::unloadPalettes()
{
    if (m_palettes)
    {
        delete[] m_palettes;
        m_palettes = NULL;
    }

    m_palette_count = 0;
}

void Level_Sonic2::unloadPatterns()
{
    if (m_patterns)
    {
        delete[] m_patterns;
        m_patterns = NULL;
    }

    m_pattern_count = 0;
}

void Level_Sonic2::unloadChunks()
{
    if (m_chunks)
    {
        delete[] m_chunks;
        m_chunks = NULL;
    }

    m_chunk_count = 0;
}

void Level_Sonic2::unloadBlocks()
{
    for (unsigned int i = 0; i < m_blockCount; i++)
    {
        delete m_blockPtrs[i];
        m_blockPtrs[i] = NULL;
    }

    delete[] m_blockPtrs;
    m_blockPtrs = NULL;

    m_blockCount = 0;
}

void Level_Sonic2::unloadMap()
{
    if (m_pMap)
    {
        delete m_pMap;
        m_pMap = 0;
    }
}

bool Level_Sonic2::loadLevel(unsigned int level_index)
{
    if (level_index > 19)
    {
        return false;
    }

    m_level_index = level_index;

    if (loadPalettes()   &&
        loadPatterns()   &&
        loadChunks()     &&
        loadBlocks()     &&
        loadMap()        &&
        bufferPatterns() &&
        bufferBlocks())
    {
        return true;
    }

    cleanup();

    return false;
}

const SegaPalette& Level_Sonic2::getPalette(unsigned int index) const
{
    return m_palettes[index];
}

const SegaPalette* Level_Sonic2::getPalettes() const
{
    return m_palettes;
}

unsigned int Level_Sonic2::getPaletteCount() const
{
    return m_palette_count;
}

const SegaPattern& Level_Sonic2::getPattern(unsigned int index) const
{
    return m_patterns[index];
}

const SegaPattern* Level_Sonic2::getPatterns() const
{
    return m_patterns;
}

unsigned int Level_Sonic2::getPatternCount() const
{
    return m_pattern_count;
}

const SonicBlock& Level_Sonic2::getBlock(unsigned int index) const
{
    return *m_blockPtrs[index];
}

const SonicBlock** Level_Sonic2::getBlocks() const
{
    return const_cast<const SonicBlock**>(m_blockPtrs);
}

unsigned int Level_Sonic2::getBlockCount() const
{
    return m_blockCount;
}

unsigned int Level_Sonic2::getBlockHeight() const
{
    return BLOCK_WIDTH;
}

unsigned int Level_Sonic2::getBlockWidth() const
{
    return BLOCK_HEIGHT;
}

const SonicMap& Level_Sonic2::getMap() const
{
    return *m_pMap;
}

const Buffer_Blocks& Level_Sonic2::getBlockBuffer() const
{
    return *m_pBufferBlocks;
}

const Buffer_Patterns& Level_Sonic2::getPatternBuffer() const
{
    return *m_pBufferPatterns;
}

const SonicChunk& Level_Sonic2::getChunk(unsigned int index) const
{
    return m_chunks[index];
}

const SonicChunk* Level_Sonic2::getChunks() const
{
    return m_chunks;
}

unsigned int Level_Sonic2::getChunkCount() const
{
    return m_chunk_count;
}
