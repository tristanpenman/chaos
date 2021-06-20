#pragma once

#include <windows.h>

class SegaPalette;
class SegaPattern;

class SonicChunk;
class SonicBlock;
class SonicMap;

class Buffer_Blocks;
class Buffer_Patterns;

class Level
{
public:
    Level();

    virtual ~Level();

    void setWindow(HWND hwnd);
    HWND getWindow() const;

    virtual bool loadLevel(unsigned int level_index) = 0;

    virtual const SegaPalette& getPalette(unsigned int index) const = 0;
    virtual const SegaPalette* getPalettes() const = 0;
    virtual unsigned int getPaletteCount() const = 0;

    virtual const SegaPattern& getPattern(unsigned int index) const = 0;
    virtual const SegaPattern* getPatterns() const = 0;
    virtual unsigned int getPatternCount() const = 0;

    virtual const SonicBlock& getBlock(unsigned int index) const = 0;
    virtual const SonicBlock** getBlocks() const = 0;
    virtual unsigned int getBlockCount() const = 0;
    virtual unsigned int getBlockHeight() const = 0;
    virtual unsigned int getBlockWidth() const = 0;

    virtual const SonicMap& getMap() const = 0;

    virtual const Buffer_Patterns& getPatternBuffer() const = 0;
    virtual const Buffer_Blocks& getBlockBuffer() const = 0;

protected:
    HWND m_hwnd;
};

inline Level::Level()
  : m_hwnd(0)
{

}

inline Level::~Level()
{

}

inline void Level::setWindow(HWND hwnd)
{
    m_hwnd = hwnd;
}

inline HWND Level::getWindow() const
{
    return m_hwnd;
}
