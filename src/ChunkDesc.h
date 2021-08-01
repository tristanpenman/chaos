#pragma once

#include <cstddef>
#include <cstdint>

/**
 * Sonic 2/3 chunk descriptor
 *
 * A Sonic chunk descriptor is used to specify which chunk to draw and how it should be drawn. A chunk may be
 * horizontally and/or vertically flipped.
 *
 * A pattern descriptor is defined as a 16-bit bitmask, in the form:
 *
 *  ???? YXII IIII IIII
 *
 *  Masks:
 *   0x3FF chunk index
 *   0x400 X flip
 *   0x800 Y flip
 */
class ChunkDesc
{
public:
  ChunkDesc();

  uint16_t get() const;
  uint16_t getChunkIndex() const;

  bool getHFlip() const;
  bool getVFlip() const;

  void set(uint16_t);
  void set(ChunkDesc& desc);

  static size_t getIndexSize();

private:
  ChunkDesc(const ChunkDesc&) = delete;
  ChunkDesc& operator=(const ChunkDesc&) = delete;

  uint16_t m_index;
};

inline ChunkDesc::ChunkDesc()
  : m_index(0)
{

}

inline uint16_t ChunkDesc::get() const
{
  return m_index;
}

inline uint16_t ChunkDesc::getChunkIndex() const
{
  return m_index & 0x3FF;
}

inline bool ChunkDesc::getHFlip() const
{
  return (m_index & 0x400) != 0;
}

inline bool ChunkDesc::getVFlip() const
{
  return (m_index & 0x800) != 0;
}

inline void ChunkDesc::set(uint16_t value)
{
  m_index = value;
}

inline void ChunkDesc::set(ChunkDesc& desc)
{
  m_index = desc.m_index;
}

inline size_t ChunkDesc::getIndexSize()
{
  return sizeof(uint16_t);
}
