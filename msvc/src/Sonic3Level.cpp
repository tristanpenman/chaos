#include "Precompiled.h"

#include "Block.h"
#include "BlockBuffer.h"
#include "Buffer.h"
#include "Chunk.h"
#include "Map.h"
#include "Palette.h"
#include "Pattern.h"
#include "PatternBuffer.h"
#include "Rom.h"
#include "Sonic3Rom.h"
#include "SonicReader.h"

#include "Sonic3Level.h"

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF  // 64kB

#define BLOCK_WIDTH 128
#define BLOCK_HEIGHT 128

using namespace std;

Sonic3Level::Sonic3Level(Sonic3Rom& rom)
  : m_rom(rom)
  , m_level_index(0)
  , m_level_loaded(false)
  , m_palettes(NULL)
  , m_patterns(NULL)
  , m_chunks(NULL)
  , m_block_ptrs(NULL)
  , m_map(NULL)
  , m_palette_count(0)
  , m_pattern_count(0)
  , m_chunk_count(0)
  , m_block_count(0)
  , m_buffer_blocks(NULL)
  , m_buffer_patterns(NULL)
{

}

Sonic3Level::~Sonic3Level()
{
    cleanup();
}

bool Sonic3Level::loadPalettes()
{
    fstream& file = m_rom.getFile();

    streamoff palette_addr = m_rom.getPalettesAddress(m_level_index);

    file.seekg(palette_addr);
    m_palettes = new Palette[PALETTE_COUNT];

    for (unsigned int row = 1; row < PALETTE_COUNT; row++)
    {
        if (!m_palettes[row].readFromFile(file))
        {
            return false;
        }
    }

    m_palette_count = PALETTE_COUNT;

    return true;
}

bool Sonic3Level::loadPatterns()
{
    fstream& file = m_rom.getFile();

    vector<unsigned char> buffer(PATTERN_BUFFER_SIZE);

    // Get address of base and extended pattern data
    streamoff patterns_address_base = m_rom.getPatternsAddress(m_level_index);
    streamoff patterns_address_ext  = m_rom.getPatternsAddress_extended(m_level_index);

    // Get length of base and extended pattern data
    uint16_t data_length_base = m_rom.read16BitAddr(patterns_address_base);
    uint16_t data_length_ext  = m_rom.read16BitAddr(patterns_address_ext);

    const size_t pattern_size = Pattern::getPatternSize();
    const size_t pattern_count = (data_length_base + data_length_ext) / pattern_size;

    m_patterns = new Pattern[pattern_count];

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

    while (1)
    {
        while (read_total < read_size && !error)
        {
            memset(buffer.data(), 0, PATTERN_BUFFER_SIZE);

            SonicReader::result_t r = sr.decompress(buffer.data(), PATTERN_BUFFER_SIZE, read_address);
            if (!r.first)
            {
                error = true;
                break;
            }
            // Find the beginning of the next module...
            char b = 0;
            while (b == 0)
            {
                b = file.get();
            }

            // Set read address to the next packet/module

            read_address = file.tellg();
            read_address--;

            unsigned int buffer_pos = 0;

            while (pattern_idx < pattern_count && buffer_pos < r.second)
            {
                m_patterns[pattern_idx].loadFromBuffer(&buffer[buffer_pos]);

                pattern_idx++;
                buffer_pos += pattern_size;
            }

            read_total += r.second;
        }

        if (error)
        {
            break;
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

    return !error;
}

bool Sonic3Level::loadChunks()
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

    const size_t chunk_size = Chunk::getChunkSize();
    const size_t chunk_count = data_length / chunk_size;

    m_chunks = new Chunk[chunk_count];

    //
    // Main chunk reader loop
    //

    streamoff read_address = chunks_address_base;

    unsigned int chunk_idx = 0;

    bool extended = false;
    bool error = false;

    while (1)
    {
        SonicReader::result_t r = sr.decompress(buffer.data(), CHUNK_BUFFER_SIZE, read_address);
        if (!r.first)
        {
            error = true;
            break;
        }

        unsigned int buffer_pos = 0;

        while (chunk_idx < chunk_count && buffer_pos < r.second)
        {
            m_chunks[chunk_idx].loadFromBuffer(&buffer[buffer_pos]);

            chunk_idx++;
            buffer_pos += chunk_size;
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

    return !error;
}

bool Sonic3Level::loadBlocks()
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

    const size_t blockSize = Block::calculateBlockSize(getBlockWidth(), getBlockHeight());
    const size_t block_count = data_length / blockSize;

    m_block_ptrs = new Block*[block_count];

    //
    // Main block reader loop
    //

    streamoff read_address = blocks_address_base;

    unsigned int blockIndex = 0;

    bool extended = false;
    bool error = false;

    while (1)
    {
        SonicReader::result_t r = sr.decompress(buffer.data(), BLOCK_BUFFER_SIZE, read_address);
        if (!r.first)
        {
            error = true;
            break;
        }

        unsigned int buffer_pos = 0;

        while (blockIndex < block_count && buffer_pos < r.second)
        {
            m_block_ptrs[blockIndex] = new Block(getBlockWidth(), getBlockHeight());
            m_block_ptrs[blockIndex]->loadFromBuffer(&buffer[buffer_pos]);

            blockIndex++;
            buffer_pos += blockSize;
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

    m_block_count = block_count;

    return true;
}

bool Sonic3Level::loadMap()
{
    fstream& file = m_rom.getFile();

    // Find map address
    const uint32_t map_address = m_rom.getMapAddress(m_level_index);

    // Read map header
    const uint32_t row_size_fg = m_rom.read16BitAddr(map_address);
    const uint32_t row_size_bg = m_rom.read16BitAddr(map_address + 2);
    const uint32_t row_count_fg = m_rom.read16BitAddr(map_address + 4);
    const uint32_t row_count_bg = m_rom.read16BitAddr(map_address + 6);

    const uint32_t ptr_table = map_address + 8;

    const size_t map_width = max(row_size_bg, row_size_fg);
    const size_t map_height = max(row_count_bg, row_count_fg);

    const size_t buffer_size = sizeof(unsigned char) * map_width;

    std::vector<char> buffer(buffer_size);

    bool result = false;

    try
    {
        m_map = new Map(2, map_width, map_height);

        for (uint32_t row_index = 0; row_index < row_count_fg; row_index++)
        {
            streamoff row_ptr = m_rom.read16BitAddr(ptr_table + row_index * 4) - 0x8000;

            file.seekg(row_ptr + ptr_table);
            file.read(buffer.data(), buffer_size);

            for (uint32_t col_index = 0; col_index < row_size_fg; col_index++)
            {
                m_map->setValue(0, col_index, row_index, buffer.at(col_index));
            }
        }

        result = true;
    }
    catch (...)
    {
        REPORT_ERROR("An error occured while reading the level map", "Map reader");
    }

    return result;
}

bool Sonic3Level::bufferPatterns()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_buffer_patterns = new PatternBuffer(
        getPalettes(), getPaletteCount(), getPatterns(), getPatternCount(), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

bool Sonic3Level::bufferBlocks()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_buffer_blocks = new BlockBuffer(this,
        getBlocks(), getBlockCount(), getChunks(), getChunkCount(), &(getPatternBuffer()), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

void Sonic3Level::unloadPatternBuffer()
{
    // TODO
}

void Sonic3Level::unloadBlockBuffer()
{
    // TODO
}

void Sonic3Level::cleanup()
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

void Sonic3Level::unloadPalettes()
{
    if (m_palettes)
    {
        delete[] m_palettes;
        m_palettes = NULL;
    }

    m_palette_count = 0;
}

void Sonic3Level::unloadPatterns()
{
    if (m_patterns)
    {
        delete[] m_patterns;
        m_patterns = NULL;
    }

    m_pattern_count = 0;
}

void Sonic3Level::unloadChunks()
{
    if (m_chunks)
    {
        delete[] m_chunks;
        m_chunks = NULL;
    }

    m_chunk_count = 0;
}

void Sonic3Level::unloadBlocks()
{
    for (unsigned int i = 0; i < m_block_count; i++)
    {
        delete m_block_ptrs[i];
        m_block_ptrs[i] = NULL;
    }

    delete[] m_block_ptrs;
    m_block_ptrs = NULL;

    m_block_count = 0;
}

void Sonic3Level::unloadMap()
{
    if (m_map)
    {
        delete m_map;
        m_map = 0;
    }
}

bool Sonic3Level::loadLevel(unsigned int level_index)
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

const Palette& Sonic3Level::getPalette(unsigned int index) const
{
    return m_palettes[index];
}

const Palette* Sonic3Level::getPalettes() const
{
    return m_palettes;
}

unsigned int Sonic3Level::getPaletteCount() const
{
    return m_palette_count;
}

const Pattern& Sonic3Level::getPattern(unsigned int index) const
{
    return m_patterns[index];
}

const Pattern* Sonic3Level::getPatterns() const
{
    return m_patterns;
}

unsigned int Sonic3Level::getPatternCount() const
{
    return m_pattern_count;
}

const Block& Sonic3Level::getBlock(unsigned int index) const
{
    return *m_block_ptrs[index];
}

const Block** Sonic3Level::getBlocks() const
{
    return const_cast<const Block**>(m_block_ptrs);
}

unsigned int Sonic3Level::getBlockCount() const
{
    return m_block_count;
}

unsigned int Sonic3Level::getBlockHeight() const
{
    return BLOCK_WIDTH;
}

unsigned int Sonic3Level::getBlockWidth() const
{
    return BLOCK_HEIGHT;
}

const Map& Sonic3Level::getMap() const
{
    return *m_map;
}

const BlockBuffer& Sonic3Level::getBlockBuffer() const
{
    return *m_buffer_blocks;
}

const PatternBuffer& Sonic3Level::getPatternBuffer() const
{
    return *m_buffer_patterns;
}

const Chunk& Sonic3Level::getChunk(unsigned int index) const
{
    return m_chunks[index];
}

const Chunk* Sonic3Level::getChunks() const
{
    return m_chunks;
}

unsigned int Sonic3Level::getChunkCount() const
{
    return m_chunk_count;
}
