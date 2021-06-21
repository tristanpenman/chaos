#pragma once

class SonicMap
{
public:
    SonicMap();
    SonicMap(unsigned int layers, unsigned int width, unsigned int height);
    SonicMap(unsigned int layers, unsigned int width, unsigned int height, unsigned char* data);

    virtual ~SonicMap();

    void cleanup();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getLayerCount() const;

    unsigned char getValue(unsigned int layer, unsigned int x, unsigned int y) const;
    unsigned char setValue(unsigned int layer, unsigned int x, unsigned int y, unsigned char);

    void reset(unsigned int layers, unsigned int width, unsigned int height);
    void reset(unsigned int layers, unsigned int width, unsigned int height, unsigned char* data);

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
