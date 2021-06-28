#pragma once

#include <cstdint>

class Map
{
public:
  Map();
  Map(uint8_t layers, uint8_t width, uint8_t height);
  Map(uint8_t layers, uint8_t width, uint8_t height, uint8_t* data);

  ~Map();

  void cleanup();

  uint8_t getWidth() const;
  uint8_t getHeight() const;
  uint8_t getLayerCount() const;

  uint8_t getValue(uint8_t layer, uint8_t x, uint8_t y) const;
  uint8_t setValue(uint8_t layer, uint8_t x, uint8_t y, uint8_t);

  void reset(uint8_t layers, uint8_t width, uint8_t height);
  void reset(uint8_t layers, uint8_t width, uint8_t height, uint8_t* data);

protected:
  uint8_t m_layers;
  uint8_t m_height;
  uint8_t m_width;

  uint8_t* m_data;
};

inline uint8_t Map::getWidth() const
{
  return m_width;
}

inline uint8_t Map::getHeight() const
{
  return m_height;
}

inline uint8_t Map::getLayerCount() const
{
  return m_layers;
}
