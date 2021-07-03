#pragma once

class Map
{
public:
    Map();
    Map(unsigned int layers, unsigned int width, unsigned int height);
    Map(unsigned int layers, unsigned int width, unsigned int height, unsigned char* data);

    virtual ~Map();

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

inline unsigned int Map::getWidth() const
{
    return m_width;
}

inline unsigned int Map::getHeight() const
{
    return m_height;
}

inline unsigned int Map::getLayerCount() const
{
    return m_layers;
}
