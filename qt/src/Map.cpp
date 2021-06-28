#include <cstring>
#include <stdexcept>

#include "Map.h"

Map::Map()
  : m_layers(0)
  , m_height(0)
  , m_width(0)
  , m_data(nullptr)
{

}

Map::Map(uint8_t layers, uint8_t width, uint8_t height)
  : m_layers(0)
  , m_height(0)
  , m_width(0)
  , m_data(nullptr)
{
  reset(layers, width, height);
}

Map::Map(uint8_t layers, uint8_t width, uint8_t height, uint8_t* data)
  : m_layers(0)
  , m_height(0)
  , m_width(0)
  , m_data(nullptr)
{
  reset(layers, width, height, data);
}

Map::~Map()
{
  cleanup();
}

void Map::cleanup()
{
  if (m_data) {
    delete[] m_data;
    m_data = nullptr;
  }

  m_width = 0;
  m_height = 0;
  m_layers = 0;
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

uint8_t Map::setValue(uint8_t layer, uint8_t x, uint8_t y, uint8_t value)
{
  uint8_t old_value = getValue(layer, x, y);

  m_data[y * m_width * m_layers + layer * m_width + x] = value;

  return old_value;
}

void Map::reset(uint8_t layers, uint8_t width, uint8_t height)
{
  cleanup();

  size_t data_size = sizeof(uint8_t) * layers * width * height;

  m_data = new uint8_t[data_size];
  if (!m_data) {
    throw std::runtime_error("Failed to allocate memory for level map");
  }

  memset(m_data, 0, data_size);

  m_layers = layers;
  m_width = width;
  m_height = height;
}

void Map::reset(uint8_t layers, uint8_t width, uint8_t height, uint8_t* data)
{
  cleanup();

  size_t data_size = sizeof(uint8_t) * layers * width * height;

  m_data = new uint8_t[data_size];
  if (!m_data) {
    throw std::runtime_error("Failed to allocate memory for level map");
  }

  memcpy(m_data, data, data_size);

  m_layers = layers;
  m_width = width;
  m_height = height;
}
