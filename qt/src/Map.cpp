#include <cstring>
#include <stdexcept>

#include "Map.h"

Map::Map(uint8_t layers, uint8_t width, uint8_t height)
  : Map(layers, width, height, nullptr)
{

}

Map::Map(uint8_t layers, uint8_t width, uint8_t height, uint8_t* data)
  : m_layers(layers)
  , m_height(height)
  , m_width(width)
{
  const size_t size = sizeof(uint8_t) * layers * width * height;

  m_data = new uint8_t[size];
  if (!m_data) {
    throw std::runtime_error("Failed to allocate memory for level map");
  }

  if (data) {
    memcpy(m_data, data, size);
  } else {
    memset(m_data, 0, size);
  }

  m_layers = layers;
  m_width = width;
  m_height = height;
}

Map::~Map()
{
  if (m_data) {
    delete[] m_data;
    m_data = nullptr;
  }
}

uint8_t Map::getValue(uint8_t layer, uint8_t x, uint8_t y) const
{
  if (layer >= m_layers) {
    throw std::runtime_error("Invalid map layer index");
  }

  if (x >= m_width || y >= m_height) {
    throw std::runtime_error("Invalid map tile index");
  }

  return m_data[y * m_width * m_layers + layer * m_width + x];
}

void Map::setValue(uint8_t layer, uint8_t x, uint8_t y, uint8_t value)
{
  if (layer >= m_layers) {
    throw std::runtime_error("Invalid map layer index");
  }

  if (x >= m_width || y >= m_height) {
    throw std::runtime_error("Invalid map tile index");
  }

  m_data[y * m_width * m_layers + layer * m_width + x] = value;
}
