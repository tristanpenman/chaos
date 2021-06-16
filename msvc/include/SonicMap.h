#pragma once

class SonicMap
{
public:
    SonicMap();
    SonicMap(unsigned int layers, unsigned int width, unsigned int height);
    SonicMap(unsigned int layers, unsigned int width, unsigned int height, unsigned char* data);

    virtual ~SonicMap();

    void reset(unsigned int layers, unsigned int width, unsigned int height);
    void reset(unsigned int layers, unsigned int width, unsigned int height, unsigned char* data);

    void cleanup();

    unsigned char getValue(unsigned int layer, unsigned int x, unsigned int y) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getLayerCount() const;

    unsigned char setValue(unsigned int layer, unsigned int x, unsigned int y, unsigned char);

protected:
    unsigned int m_layers;
    unsigned int m_height;
    unsigned int m_width;

    unsigned char* m_data;
};

inline unsigned int SonicMap::getWidth() const
{
    return m_width;
}

inline unsigned int SonicMap::getHeight() const
{
    return m_height;
}

inline unsigned int SonicMap::getLayerCount() const
{
    return m_layers;
}

inline unsigned char SonicMap::getValue(unsigned int layer, unsigned int x, unsigned int y) const
{
    if (layer >= m_layers)
    {
        throw ChaosException("Invalid map layer index");
    }

    if (x >= m_width || y >= m_height)
    {
        throw ChaosException("Invalid map tile index");
    }

    return m_data[y * m_width * m_layers + layer * m_width + x];
}

inline unsigned char SonicMap::setValue(unsigned int layer, unsigned int x, unsigned int y, unsigned char value)
{
    unsigned char old_value = getValue(layer, x, y);

    m_data[y * m_width * m_layers + layer * m_width + x] = value;

    return old_value;
}
