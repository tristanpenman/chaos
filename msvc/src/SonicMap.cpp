#include "Precompiled.h"

#include "ChaosException.h"
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
        delete[] m_data; m_data = NULL;
    }

    m_width = 0;
    m_height = 0;
    m_layers = 0;
}

void SonicMap::reset(unsigned int layers, unsigned int width, unsigned int height)
{
    cleanup();

    size_t data_size = sizeof(unsigned char) * layers * width * height;

    m_data = new unsigned char[data_size];
    if (!m_data)
    {
        throw ChaosException("Failed to allocate memory for level map");
    }

    memset(m_data, 0, sizeof(data_size));

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
        throw ChaosException("Failed to allocate memory for level map");
    }

    memcpy(m_data, data, data_size);

    m_layers = layers;
    m_width = width;
    m_height = height;
}
