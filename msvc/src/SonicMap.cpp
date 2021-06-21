#include "Precompiled.h"

#include "SonicMap.h"

SonicMap::SonicMap()
  : m_layers(0)
  , m_height(0)
  , m_width(0)
  , m_data(NULL)
{

}

SonicMap::SonicMap(unsigned int layers, unsigned int width, unsigned int height)
  : m_layers(0)
  , m_height(0)
  , m_width(0)
  , m_data(NULL)
{
    reset(layers, width, height);
}

SonicMap::SonicMap(unsigned int layers, unsigned int width, unsigned int height, unsigned char* data)
  : m_layers(0)
  , m_height(0)
  , m_width(0)
  , m_data(NULL)
{
    reset(layers, width, height, data);
}

SonicMap::~SonicMap()
{
    cleanup();
}

void SonicMap::cleanup()
{
    if (m_data)
    {
        delete[] m_data;
        m_data = NULL;
    }

    m_width = 0;
    m_height = 0;
    m_layers = 0;
}

unsigned char SonicMap::getValue(unsigned int layer, unsigned int x, unsigned int y) const
{
    if (layer >= m_layers)
    {
        throw std::runtime_error("Invalid map layer index");
    }

    if (x >= m_width || y >= m_height)
    {
        throw std::runtime_error("Invalid map tile index");
    }

    return m_data[y * m_width * m_layers + layer * m_width + x];
}

unsigned char SonicMap::setValue(unsigned int layer, unsigned int x, unsigned int y, unsigned char value)
{
    unsigned char old_value = getValue(layer, x, y);

    m_data[y * m_width * m_layers + layer * m_width + x] = value;

    return old_value;
}

void SonicMap::reset(unsigned int layers, unsigned int width, unsigned int height)
{
    cleanup();

    size_t data_size = sizeof(unsigned char) * layers * width * height;

    m_data = new unsigned char[data_size];
    if (!m_data)
    {
        throw std::runtime_error("Failed to allocate memory for level map");
    }

    memset(m_data, 0, data_size);

    m_layers = layers;
    m_width = width;
    m_height = height;
}

void SonicMap::reset(unsigned int layers, unsigned int width, unsigned int height, unsigned char* data)
{
    cleanup();

    size_t data_size = sizeof(unsigned char) * layers * width * height;

    m_data = new unsigned char[data_size];
    if (!m_data)
    {
        throw std::runtime_error("Failed to allocate memory for level map");
    }

    memcpy(m_data, data, data_size);

    m_layers = layers;
    m_width = width;
    m_height = height;
}
