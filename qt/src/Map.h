#pragma once

#include <cstdint>

class Map
{
public:
  Map(uint8_t layers, uint16_t width, uint16_t height);
  Map(uint8_t layers, uint16_t width, uint16_t height, uint8_t* data);

  ~Map();

  uint16_t getWidth() const;
  uint16_t getHeight() const;
  uint8_t getLayerCount() const;

  uint8_t getValue(uint8_t layer, uint16_t x, uint16_t y) const;
  void setValue(uint8_t layer, uint16_t x, uint16_t y, uint8_t);

protected:
  uint8_t m_layers;
  uint16_t m_height;
  uint16_t m_width;

  uint8_t* m_data;
};

inline uint16_t Map::getWidth() const
{
  return m_width;
}

inline uint16_t Map::getHeight() const
{
  return m_height;
}

inline uint8_t Map::getLayerCount() const
{
  return m_layers;
}
