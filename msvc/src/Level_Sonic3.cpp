#include "Precompiled.h"

#include "ChaosRom.h"
#include "ChaosRom_Sonic3.h"

#include "SegaPalette.h"
#include "SegaPattern.h"
#include "SonicChunk.h"
#include "SonicBlock.h"
#include "SonicMap.h"
#include "SonicReader.h"

#include "Buffer.h"
#include "Buffer_Blocks.h"
#include "Buffer_Patterns.h"

#include "Level_Sonic3.h"

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF  // 64kB

#define BLOCK_WIDTH 128
#define BLOCK_HEIGHT 128

using namespace std;

Level_Sonic3::Level_Sonic3(ChaosRom_Sonic3& rom)
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

Level_Sonic3::~Level_Sonic3()
{
    cleanup();
}

bool Level_Sonic3::loadPalettes()
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

bool Level_Sonic3::loadPatterns()
{
    fstream& file = m_rom.getFile();

    vector<unsigned char> buffer(PATTERN_BUFFER_SIZE);

    // Get address of base and extended pattern data
    streamoff patterns_address_base = m_rom.getPatternsAddress(m_level_index);
    streamoff patterns_address_ext  = m_rom.getPatternsAddress_extended(m_level_index);

    // Get length of base and extended pattern data
    uint16_t data_length_base = m_rom.read16BitAddr(patterns_address_base);
    uint16_t data_length_ext  = m_rom.read16BitAddr(patterns_address_ext);

    const size_t pattern_size = SegaPattern::getPatternSize();
    const size_t pattern_count = (data_length_base + data_length_ext) / pattern_size;

    m_patterns = new SegaPattern[pattern_count];

    //
    // Main pattern reader loop
    //

    SonicReader sr(file);

    streamoff read_address = patterns_address_base + 2;
    streamoff read_size = data_length_base;
    streamoff read_total = 0;

    unsigned int pattern_idx = 0;

    bool extended = false;
    bool error = false;

    while (!error)
    {
        while (read_total < read_size && !error)
        {
            memset(buffer.data(), 0, PATTERN_BUFFER_SIZE);

            SonicReader::result_t r = sr.decompress(buffer.data(), PATTERN_BUFFER_SIZE, read_address);

            if (r.first)
            {
                // Find the beginning of the next module...
                char b = 0;
                while (b == 0)
                {
                    b = file.get();
                }

                // Set read address to the next packet/module

                read_address = file.tellg();
                read_address--;

                unsigned int bufferPos = 0;

                while (pattern_idx < pattern_count && bufferPos < r.second)
                {
                    m_patterns[pattern_idx].loadFromBuffer(&buffer[bufferPos]);

                    pattern_idx++;
                    bufferPos += pattern_size;
                }

                read_total += r.second;
            }
            else
            {
                error = true;
            }
        }

        // Reset the parser to read extended data
        read_total = 0;
        read_size = data_length_ext;
        read_address = patterns_address_ext + 2;

        if (extended)
        {
            break;
        }
        else
        {
            extended = true;
        }
    }

    m_pattern_count = pattern_count;

    return true;
}

bool Level_Sonic3::loadChunks()
{
    fstream& file = m_rom.getFile();

    vector<unsigned char> buffer(CHUNK_BUFFER_SIZE);

    // Get address of base and extended pattern data
    uint32_t chunks_address_base = m_rom.getChunksAddress(m_level_index);
    uint32_t chunks_address_ext  = m_rom.getChunksAddress_extended(m_level_index);

    SonicReader sr(file);
    SonicReader::result_t r;

    uint32_t data_length = 0;
    data_length += sr.decompress(buffer.data(), CHUNK_BUFFER_SIZE, chunks_address_base).second;
    data_length += sr.decompress(buffer.data(), CHUNK_BUFFER_SIZE, chunks_address_ext).second;

    const size_t chunk_size = SonicChunk::getChunkSize();
    const size_t chunk_count = data_length / chunk_size;

    m_chunks = new SonicChunk[chunk_count];

    //
    // Main chunk reader loop
    //

    streamoff read_address = chunks_address_base;

    unsigned int chunk_idx = 0;

    bool extended = false;
    bool error = false;

    while (!error)
    {
        SonicReader::result_t r = sr.decompress(buffer.data(), CHUNK_BUFFER_SIZE, read_address);

        if (r.first)
        {
            unsigned int bufferPos = 0;

            while (chunk_idx < chunk_count && bufferPos < r.second)
            {
                m_chunks[chunk_idx].loadFromBuffer(&buffer[bufferPos]);

                chunk_idx++;
                bufferPos += chunk_size;
            }
        }
        else
        {
            error = true;
        }

        if (extended)
        {
            break;
        }
        else
        {
            // Reset the parser to read extended data
            read_address = chunks_address_ext;
            extended = true;
        }
    }

    m_chunk_count = chunk_count;

    return true;
}

bool Level_Sonic3::loadBlocks()
{
    fstream& file = m_rom.getFile();

    vector<unsigned char> buffer(BLOCK_BUFFER_SIZE);

    // Get address of base and extended pattern data
    uint32_t blocks_address_base = m_rom.getBlocksAddress(m_level_index);
    uint32_t blocks_address_ext  = m_rom.getBlocksAddress_extended(m_level_index);

    SonicReader sr(file);
    SonicReader::result_t r;

    uint32_t data_length = 0;
    data_length += sr.decompress(buffer.data(), BLOCK_BUFFER_SIZE, blocks_address_base).second;
    data_length += sr.decompress(buffer.data(), BLOCK_BUFFER_SIZE, blocks_address_ext).second;

    const size_t blockSize = SonicBlock::calculateBlockSize(getBlockWidth(), getBlockHeight());
    const size_t blockCount = data_length / blockSize;

    m_blockPtrs = new SonicBlock*[blockCount];

    //
    // Main block reader loop
    //

    streamoff read_address = blocks_address_base;

    unsigned int blockIndex = 0;

    bool extended = false;
    bool error = false;

    while (!error)
    {
        SonicReader::result_t r = sr.decompress(buffer.data(), BLOCK_BUFFER_SIZE, read_address);

        if (r.first)
        {
            unsigned int bufferPos = 0;

            while (blockIndex < blockCount && bufferPos < r.second)
            {
                m_blockPtrs[blockIndex] = new SonicBlock(getBlockWidth(), getBlockHeight());
                m_blockPtrs[blockIndex]->loadFromBuffer(&buffer[bufferPos]);

                blockIndex++;
                bufferPos += blockSize;
            }
        }
        else
        {
            error = true;
        }

        if (extended)
        {
            break;
        }
        else
        {
            // Reset the parser to read extended data
            read_address = blocks_address_ext;
            extended = true;
        }
    }

    m_blockCount = blockCount;

    return true;
}

bool Level_Sonic3::loadMap()
{
    fstream& file = m_rom.getFile();

    bool result = false;

    // Find map address
    const uint32_t map_address = m_rom.getMapAddress(m_level_index);

    // Read map header
    const uint32_t row_size_fg = m_rom.read16BitAddr(map_address);
    const uint32_t row_size_bg = m_rom.read16BitAddr(map_address+2);
    const uint32_t row_count_fg = m_rom.read16BitAddr(map_address+4);
    const uint32_t row_count_bg = m_rom.read16BitAddr(map_address+6);

    const uint32_t ptr_table = map_address + 8;

    size_t map_width = max(row_size_bg, row_size_fg);
    size_t map_height = max(row_count_bg, row_count_fg);

    size_t buffer_size = sizeof(char) * map_width;

    char* buffer = new char[buffer_size];
    if (buffer != nullptr)
    {
        try
        {
            m_pMap = new SonicMap(2, map_width, map_height);

            for (uint32_t row_index = 0; row_index < row_count_fg; row_index++)
            {
                streamoff row_ptr = m_rom.read16BitAddr(ptr_table + row_index * 4) - 0x8000;

                file.seekg(row_ptr + ptr_table);
                file.read(buffer, buffer_size);

                for (uint32_t col_index = 0; col_index < row_size_fg; col_index++)
                {
                    m_pMap->setValue(0, col_index, row_index, buffer[col_index]);
                }
            }

            result = true;
        }
        catch (...)
        {
            REPORT_ERROR("An error occured while reading the level map", "Map reader");
        }

        delete[] buffer;
        buffer = NULL;
    }

    return result;
}

bool Level_Sonic3::bufferPatterns()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_pBufferPatterns = new Buffer_Patterns(getPalettes(), getPaletteCount(), getPatterns(), getPatternCount(), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

bool Level_Sonic3::bufferBlocks()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_pBufferBlocks = new Buffer_Blocks(this, getBlocks(), getBlockCount(), getChunks(), getChunkCount(), &(getPatternBuffer()), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

void Level_Sonic3::unloadPatternBuffer()
{
    // TODO
}

void Level_Sonic3::unloadBlockBuffer()
{
    // TODO
}

void Level_Sonic3::cleanup()
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

void Level_Sonic3::unloadPalettes()
{
    if (m_palettes)
    {
        delete[] m_palettes; m_palettes = NULL;
    }

    m_palette_count = 0;
}

void Level_Sonic3::unloadPatterns()
{
    if (m_patterns)
    {
        delete[] m_patterns; m_patterns = NULL;
    }

    m_pattern_count = 0;
}

void Level_Sonic3::unloadChunks()
{
    if (m_chunks)
    {
        delete[] m_chunks; m_chunks = NULL;
    }

    m_chunk_count = 0;
}

void Level_Sonic3::unloadBlocks()
{
    for (unsigned int i = 0; i < m_blockCount; i++)
    {
        delete m_blockPtrs[i]; m_blockPtrs[i] = NULL;
    }

    delete[] m_blockPtrs; m_blockPtrs = NULL;

    m_blockCount = 0;
}

void Level_Sonic3::unloadMap()
{
    if (m_pMap)
    {
        delete m_pMap; m_pMap = 0;
    }
}

bool Level_Sonic3::loadLevel(unsigned int level_index)
{
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

const SegaPalette& Level_Sonic3::getPalette(unsigned int index) const
{
    return m_palettes[index];
}

const SegaPalette* Level_Sonic3::getPalettes() const
{
    return m_palettes;
}

unsigned int Level_Sonic3::getPaletteCount() const
{
    return m_palette_count;
}

const SegaPattern& Level_Sonic3::getPattern(unsigned int index) const
{
    return m_patterns[index];
}

const SegaPattern* Level_Sonic3::getPatterns() const
{
    return m_patterns;
}

unsigned int Level_Sonic3::getPatternCount() const
{
    return m_pattern_count;
}

const SonicBlock& Level_Sonic3::getBlock(unsigned int index) const
{
    return *m_blockPtrs[index];
}

const SonicBlock** Level_Sonic3::getBlocks() const
{
    return const_cast<const SonicBlock**>(m_blockPtrs);
}

unsigned int Level_Sonic3::getBlockCount() const
{
    return m_blockCount;
}

unsigned int Level_Sonic3::getBlockHeight() const
{
    return BLOCK_WIDTH;
}

unsigned int Level_Sonic3::getBlockWidth() const
{
    return BLOCK_HEIGHT;
}

const SonicMap& Level_Sonic3::getMap() const
{
    return *m_pMap;
}

const Buffer_Blocks& Level_Sonic3::getBlockBuffer() const
{
    return *m_pBufferBlocks;
}

const Buffer_Patterns& Level_Sonic3::getPatternBuffer() const
{
    return *m_pBufferPatterns;
}

const SonicChunk& Level_Sonic3::getChunk(unsigned int index) const
{
    return m_chunks[index];
}

const SonicChunk* Level_Sonic3::getChunks() const
{
    return m_chunks;
}

unsigned int Level_Sonic3::getChunkCount() const
{
    return m_chunk_count;
}
