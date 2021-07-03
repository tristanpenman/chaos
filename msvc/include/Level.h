#pragma once

#include <windows.h>

class Block;
class BlockBuffer;
class Chunk;
class Map;
class Palette;
class Pattern;
class PatternBuffer;

class Level
{
public:
    Level();

    virtual ~Level();

    void setWindow(HWND hwnd);
    HWND getWindow() const;

    virtual bool loadLevel(unsigned int level_index) = 0;

    virtual const Palette& getPalette(unsigned int index) const = 0;
    virtual const Palette* getPalettes() const = 0;
    virtual unsigned int getPaletteCount() const = 0;

    virtual const Pattern& getPattern(unsigned int index) const = 0;
    virtual const Pattern* getPatterns() const = 0;
    virtual unsigned int getPatternCount() const = 0;

    virtual const Block& getBlock(unsigned int index) const = 0;
    virtual const Block** getBlocks() const = 0;
    virtual unsigned int getBlockCount() const = 0;
    virtual unsigned int getBlockHeight() const = 0;
    virtual unsigned int getBlockWidth() const = 0;

    virtual const Map& getMap() const = 0;

    virtual const PatternBuffer& getPatternBuffer() const = 0;
    virtual const BlockBuffer& getBlockBuffer() const = 0;

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
