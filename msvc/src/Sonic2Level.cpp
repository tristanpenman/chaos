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
#include "Sonic2Rom.h"
#include "Kosinski.h"

#include "Sonic2Level.h"

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF  // 64kB

#define BLOCK_WIDTH 128
#define BLOCK_HEIGHT 128

using namespace std;

Sonic2Level::Sonic2Level(Sonic2Rom& rom)
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

Sonic2Level::~Sonic2Level()
{
    cleanup();
}

bool Sonic2Level::loadPalettes()
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

bool Sonic2Level::loadPatterns()
{
    fstream& file = m_rom.getFile();
    vector<unsigned char> buffer(PATTERN_BUFFER_SIZE);
    streamoff patterns_addr = m_rom.getPatternsAddress(m_level_index);

    Kosinski kosinski(file);
    Kosinski::result_t r = kosinski.decompress(buffer.data(), PATTERN_BUFFER_SIZE, patterns_addr);
    if (!r.first)
    {
        REPORT_ERROR("Pattern decompression failed", "Decompression error");
        return false;
    }

    const unsigned int pattern_size = Pattern::getPatternSize();
    const unsigned int pattern_count = r.second / pattern_size;

    if (r.second % pattern_size != 0)
    {
        REPORT_ERROR("Inconsistent pattern data", "Decompression error");
        return false;
    }

    unsigned int pattern_idx = 0;
    unsigned int buffer_pos = 0;

    m_patterns = new Pattern[pattern_count];

    while (pattern_idx < pattern_count)
    {
        m_patterns[pattern_idx].loadFromBuffer(&buffer[buffer_pos]);

        pattern_idx++;
        buffer_pos += pattern_size;
    }

    m_pattern_count = pattern_count;

    return true;
}

bool Sonic2Level::loadChunks()
{
    fstream& file = m_rom.getFile();
    vector<unsigned char> buffer(CHUNK_BUFFER_SIZE);
    streamoff chunks_addr = m_rom.getChunksAddress(m_level_index);

    Kosinski kosinski(file);
    Kosinski::result_t r = kosinski.decompress(buffer.data(), CHUNK_BUFFER_SIZE, chunks_addr);
    if (!r.first)
    {
        REPORT_ERROR("Chunk decompression failed", "Decompression error");
        return false;
    }

    const unsigned int chunk_size = Chunk::getChunkSize();
    const unsigned int chunk_count = r.second / chunk_size;

    if (r.second % chunk_size != 0)
    {
        REPORT_ERROR("Inconsistent chunk data", "Decompression error");
        return false;
    }

    unsigned int chunk_idx = 0;
    unsigned int buffer_pos = 0;

    m_chunks = new Chunk[chunk_count];

    while (chunk_idx < chunk_count)
    {
        m_chunks[chunk_idx].loadFromBuffer(&buffer[buffer_pos]);

        chunk_idx++;
        buffer_pos += chunk_size;
    }

    m_chunk_count = chunk_count;

    return true;
}

bool Sonic2Level::loadBlocks()
{
    fstream& file = m_rom.getFile();
    vector<unsigned char> buffer(BLOCK_BUFFER_SIZE);
    streamoff blocks_addr = m_rom.getBlocksAddress(m_level_index);

    Kosinski kosinski(file);
    Kosinski::result_t r = kosinski.decompress(buffer.data(), BLOCK_BUFFER_SIZE, blocks_addr);
    if (!r.first)
    {
        REPORT_ERROR("Block decompression failed", "Decompression error");
        return false;
    }

    const unsigned int block_size = getBlockWidth();
    const unsigned int block_count = r.second / block_size;

    if (r.second % block_size != 0)
    {
        REPORT_ERROR("Inconsistent block data", "Decompression error");
        return false;
    }

    m_block_ptrs = new Block*[block_count];

    unsigned int buffer_pos = 0;
    for (unsigned int block_index = 0; block_index < block_count; block_index++)
    {
        m_block_ptrs[block_index] = new Block(getBlockWidth(), getBlockHeight());
        m_block_ptrs[block_index]->loadFromBuffer(&buffer[buffer_pos]);
        buffer_pos += block_size;
    }

    m_block_count = block_count;

    return true;
}

bool Sonic2Level::loadMap()
{
    fstream& file = m_rom.getFile();
    vector<unsigned char> buffer(LEVEL_BUFFER_SIZE);
    streamoff map_addr = m_rom.getMapAddress(m_level_index);

    Kosinski reader(file);
    Kosinski::result_t result = reader.decompress(buffer.data(), LEVEL_BUFFER_SIZE, map_addr);
    if (!result.first)
    {
        REPORT_ERROR("Map decompression failed", "Decompression error");
        return false;
    }

    m_map = new Map(2, 128, 16, buffer.data());
    return true;
}

bool Sonic2Level::bufferPatterns()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_buffer_patterns = new PatternBuffer(getPalettes(), getPaletteCount(), getPatterns(), getPatternCount(), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

bool Sonic2Level::bufferBlocks()
{
    HWND hwnd = getWindow();
    HDC hdc = GetDC(hwnd);

    m_buffer_blocks = new BlockBuffer(this, getBlocks(), getBlockCount(), getChunks(), getChunkCount(), &(getPatternBuffer()), hdc);

    ReleaseDC(hwnd, hdc);

    return true;
}

void Sonic2Level::unloadPatternBuffer()
{
    // TODO
}

void Sonic2Level::unloadBlockBuffer()
{
    // TODO
}

void Sonic2Level::cleanup()
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

void Sonic2Level::unloadPalettes()
{
    if (m_palettes)
    {
        delete[] m_palettes;
        m_palettes = NULL;
    }

    m_palette_count = 0;
}

void Sonic2Level::unloadPatterns()
{
    if (m_patterns)
    {
        delete[] m_patterns;
        m_patterns = NULL;
    }

    m_pattern_count = 0;
}

void Sonic2Level::unloadChunks()
{
    if (m_chunks)
    {
        delete[] m_chunks;
        m_chunks = NULL;
    }

    m_chunk_count = 0;
}

void Sonic2Level::unloadBlocks()
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

void Sonic2Level::unloadMap()
{
    if (m_map)
    {
        delete m_map;
        m_map = 0;
    }
}

bool Sonic2Level::loadLevel(unsigned int level_index)
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

const Palette& Sonic2Level::getPalette(unsigned int index) const
{
    return m_palettes[index];
}

const Palette* Sonic2Level::getPalettes() const
{
    return m_palettes;
}

unsigned int Sonic2Level::getPaletteCount() const
{
    return m_palette_count;
}

const Pattern& Sonic2Level::getPattern(unsigned int index) const
{
    return m_patterns[index];
}

const Pattern* Sonic2Level::getPatterns() const
{
    return m_patterns;
}

unsigned int Sonic2Level::getPatternCount() const
{
    return m_pattern_count;
}

const Block& Sonic2Level::getBlock(unsigned int index) const
{
    return *m_block_ptrs[index];
}

const Block** Sonic2Level::getBlocks() const
{
    return const_cast<const Block**>(m_block_ptrs);
}

unsigned int Sonic2Level::getBlockCount() const
{
    return m_block_count;
}

unsigned int Sonic2Level::getBlockHeight() const
{
    return BLOCK_WIDTH;
}

unsigned int Sonic2Level::getBlockWidth() const
{
    return BLOCK_HEIGHT;
}

const Map& Sonic2Level::getMap() const
{
    return *m_map;
}

const BlockBuffer& Sonic2Level::getBlockBuffer() const
{
    return *m_buffer_blocks;
}

const PatternBuffer& Sonic2Level::getPatternBuffer() const
{
    return *m_buffer_patterns;
}

const Chunk& Sonic2Level::getChunk(unsigned int index) const
{
    return m_chunks[index];
}

const Chunk* Sonic2Level::getChunks() const
{
    return m_chunks;
}

unsigned int Sonic2Level::getChunkCount() const
{
    return m_chunk_count;
}
