#include "Precompiled.h"

#include "SegaRom.h"

#include "ChaosException.h"
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

#include "Instance.h"
#include "Instance_Level.h"
#include "Instance_Level_S2G.h"

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF  // 64kB

#define BLOCK_WIDTH 128
#define BLOCK_HEIGHT 128

using namespace std;

Instance_Level_S2G::Instance_Level_S2G(ChaosRom_Sonic2& rom)
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
{

}

Instance_Level_S2G::~Instance_Level_S2G()
{
    cleanup();
}

bool Instance_Level_S2G::loadPalettes()
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

bool Instance_Level_S2G::loadPatterns()
{
    fstream& file = m_rom.getFile();

    unsigned char buffer[PATTERN_BUFFER_SIZE];

    streamoff patternsAddr = m_rom.getPatternsAddress(m_level_index);

    SonicReader sr(file);
    SonicReader::result_t r = sr.decompress(buffer, PATTERN_BUFFER_SIZE, patternsAddr);

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

bool Instance_Level_S2G::loadChunks()
{
    fstream& file = m_rom.getFile();

    unsigned char buffer[CHUNK_BUFFER_SIZE];

    streamoff chunks_addr = m_rom.getChunksAddress(m_level_index);

    SonicReader sr(file);
    SonicReader::result_t r = sr.decompress(buffer, CHUNK_BUFFER_SIZE, chunks_addr);

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

bool Instance_Level_S2G::loadBlocks()
{
    fstream& file = m_rom.getFile();

    unsigned char buffer[BLOCK_BUFFER_SIZE];

    streamoff blocks_addr = m_rom.getBlocksAddress(m_level_index);

    SonicReader sr(file);
    SonicReader::result_t r = sr.decompress(buffer, BLOCK_BUFFER_SIZE, blocks_addr);

    if (r.first)
    {
        const unsigned int blockSize = getBlockWidth();
        const unsigned int blockCount = r.second / blockSize;

        unsigned int bufferPos = 0;

        if (r.second % blockSize != 0)
        {
            throw ChaosException("Invalid block data");
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

bool Instance_Level_S2G::loadMap()
{
    fstream& file = m_rom.getFile();

    unsigned char buffer[LEVEL_BUFFER_SIZE];

    SonicReader reader(file);

    SonicReader::result_t result =
        reader.decompress(
            buffer,
            LEVEL_BUFFER_SIZE,
            m_rom.getMapAddress(m_level_index));

    if (result.first)
    {
        m_pMap = new SonicMap(2, 128, 16, buffer);

        return true;
    }

    return false;
}

bool Instance_Level_S2G::bufferPatterns()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_pBufferPatterns = new Buffer_Patterns(getPalettes(), getPaletteCount(), getPatterns(), getPatternCount(), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

bool Instance_Level_S2G::bufferBlocks()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_pBufferBlocks = new Buffer_Blocks(this, getBlocks(), getBlockCount(), getChunks(), getChunkCount(), &(getPatternBuffer()), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

void Instance_Level_S2G::unloadPatternBuffer()
{
    // TODO
}

void Instance_Level_S2G::unloadBlockBuffer()
{
    // TODO
}

/******************************************************************************
 *
 * Cleanup methods
 *
 *****************************************************************************/

void Instance_Level_S2G::cleanup()
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

void Instance_Level_S2G::unloadPalettes()
{
    if (m_palettes)
    {
        delete[] m_palettes; m_palettes = NULL;
    }

    m_palette_count = 0;
}

void Instance_Level_S2G::unloadPatterns()
{
    if (m_patterns)
    {
        delete[] m_patterns; m_patterns = NULL;
    }

    m_pattern_count = 0;
}

void Instance_Level_S2G::unloadChunks()
{
    if (m_chunks)
    {
        delete[] m_chunks; m_chunks = NULL;
    }

    m_chunk_count = 0;
}

void Instance_Level_S2G::unloadBlocks()
{
    for (unsigned int i = 0; i < m_blockCount; i++)
    {
        delete m_blockPtrs[i]; m_blockPtrs[i] = NULL;
    }

    delete[] m_blockPtrs; m_blockPtrs = NULL;

    m_blockCount = 0;
}

void Instance_Level_S2G::unloadMap()
{
    if (m_pMap)
    {
        delete m_pMap; m_pMap = 0;
    }
}

/******************************************************************************
 *
 * Implementation of the Instance interface
 *
 *****************************************************************************/

bool Instance_Level_S2G::hasUnsavedChanges() const
{
    return true;
}

bool Instance_Level_S2G::saveChanges()
{
    int r = MessageBox(getWindow(), "Save changes made to this level?", "Save changes?", MB_YESNOCANCEL);

    if (r == IDYES)
    {
        // Save changes

        return true;
    }
    else if (r == IDNO)
    {
        return true;
    }

    return false;
}

/******************************************************************************
 *
 * Implementation of the Instance_Level interface
 *
 *****************************************************************************/

bool Instance_Level_S2G::loadLevel(unsigned int level_index)
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

const SegaPalette& Instance_Level_S2G::getPalette(unsigned int index) const
{
    return m_palettes[index];
}

const SegaPalette* Instance_Level_S2G::getPalettes() const
{
    return m_palettes;
}

unsigned int Instance_Level_S2G::getPaletteCount() const
{
    return m_palette_count;
}

const SegaPattern& Instance_Level_S2G::getPattern(unsigned int index) const
{
    return m_patterns[index];
}

const SegaPattern* Instance_Level_S2G::getPatterns() const
{
    return m_patterns;
}

unsigned int Instance_Level_S2G::getPatternCount() const
{
    return m_pattern_count;
}

const SonicBlock& Instance_Level_S2G::getBlock(unsigned int index) const
{
    return *m_blockPtrs[index];
}

const SonicBlock** Instance_Level_S2G::getBlocks() const
{
    return const_cast<const SonicBlock**>(m_blockPtrs);
}

unsigned int Instance_Level_S2G::getBlockCount() const
{
    return m_blockCount;
}

unsigned int Instance_Level_S2G::getBlockHeight() const
{
    return BLOCK_WIDTH;
}

unsigned int Instance_Level_S2G::getBlockWidth() const
{
    return BLOCK_HEIGHT;
}

const SonicMap& Instance_Level_S2G::getMap() const
{
    return *m_pMap;
}

const Buffer_Blocks& Instance_Level_S2G::getBlockBuffer() const
{
    return *m_pBufferBlocks;
}

const Buffer_Patterns& Instance_Level_S2G::getPatternBuffer() const
{
    return *m_pBufferPatterns;
}

/******************************************************************************
 *
 * Extension of the Instance_Level interface
 *
 *****************************************************************************/

const SonicChunk& Instance_Level_S2G::getChunk(unsigned int index) const
{
    return m_chunks[index];
}

const SonicChunk* Instance_Level_S2G::getChunks() const
{
    return m_chunks;
}

unsigned int Instance_Level_S2G::getChunkCount() const
{
    return m_chunk_count;
}
