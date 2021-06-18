#include "Precompiled.h"

#include "ChaosRom.h"
#include "ChaosRom.h"
#include "Level.h"

#define ROM_HEADER_OFFSET 0x100
#define ROM_LENGTH_OFFSET 0x01A4
#define DOMESTIC_NAME_LEN 48
#define DOMESTIC_NAME_OFFSET ROM_HEADER_OFFSET + 32
#define INTERNATIONAL_NAME_LEN 48
#define INTERNATIONAL_NAME_OFFSET DOMESTIC_NAME_OFFSET + DOMESTIC_NAME_LEN

using namespace std;

ChaosRom::ChaosRom(fstream& file)
  : m_file(file)
  , m_levelInstances()
{

}

ChaosRom::~ChaosRom()
{
    LevelInstances::iterator itr;
    for (itr = m_levelInstances.begin(); itr != m_levelInstances.end(); ++itr)
    {
        Level* pLevel = itr->second;
        delete pLevel;
    }

    m_levelInstances.clear();
}

Level* ChaosRom::initLevelInstance(HWND hwnd)
{
    Level* pLevel = NULL;
    LevelInstances::iterator itr = m_levelInstances.find(hwnd);

    if (itr == m_levelInstances.end())
    {
        pLevel = instantiateLevel();
        pLevel->setWindow(hwnd);

        return (m_levelInstances[hwnd] = pLevel);
    }

    return NULL;
}

Level* ChaosRom::getLevelInstance(HWND hwnd)
{
    LevelInstances::iterator itr = m_levelInstances.find(hwnd);
    if (itr == m_levelInstances.end())
    {
        return NULL;
    }

    return itr->second;
}

bool ChaosRom::destroyInstance(HWND hwnd)
{
    LevelInstances::iterator itr = m_levelInstances.find(hwnd);
    if (itr == m_levelInstances.end())
    {
        return false;
    }

    Level* pLevel = itr->second;
    m_levelInstances.erase(itr);
    delete pLevel;
    return true;
}

string ChaosRom::getDomesticName()
{
    char buffer[DOMESTIC_NAME_LEN + 1];

    m_file.seekg(DOMESTIC_NAME_OFFSET);
    m_file.read(buffer, DOMESTIC_NAME_LEN);

    buffer[m_file.gcount()] = 0;

    return buffer;
}

string ChaosRom::getInternationalName()
{
    char buffer[INTERNATIONAL_NAME_LEN + 1];

    m_file.seekg(INTERNATIONAL_NAME_OFFSET);
    m_file.read(buffer, INTERNATIONAL_NAME_LEN);

    buffer[m_file.gcount()] = 0;

    return buffer;
}

uint16_t ChaosRom::readAddress_16bit()
{
    uint16_t o;

    // Read a 16-bit big-endian address
    o = m_file.get() << 8;
    o |= m_file.get();

    // Return little-endian address
    return o & 0xFFFF;
}

uint16_t ChaosRom::readAddress_16bit_at(streamoff src_offset)
{
    uint16_t o;
    streamoff p = m_file.tellg();

    m_file.seekg(src_offset);

    // Read a 16-bit big-endian address
    o = m_file.get() << 8;
    o |= m_file.get();

    m_file.seekg(p);

    // Return little-endian address
    return o & 0xFFFF;
}

uint32_t ChaosRom::readAddress_32bit()
{
    uint32_t o;

    // Read a 32-bit big-endian address
    o = m_file.get() << 24;
    o |= m_file.get() << 16;
    o |= m_file.get() << 8;
    o |= m_file.get();

    // Return little-endian address
    return o;
}

uint32_t ChaosRom::readAddress_32bit_at(streamoff src_offset)
{
    uint32_t o;
    streamoff p = m_file.tellg();

    m_file.seekg(src_offset);

    // Read a 32-bit big-endian address
    o = m_file.get() << 24;
    o |= m_file.get() << 16;
    o |= m_file.get() << 8;
    o |= m_file.get();

    m_file.seekg(p);

    // Return little-endian address
    return o;
}

void ChaosRom::writeAddress_16bit(uint16_t address)
{
    m_file.put((char)((address >> 8) & 0xFF));
    m_file.put((char)((address) & 0xFF));
}

void ChaosRom::writeAddress_16bit_at(uint16_t address, std::streamoff offset)
{
    streamoff p = m_file.tellp();

    m_file.seekp(offset);

    m_file.put((char)((address >> 8) & 0xFF));
    m_file.put((char)((address) & 0xFF));

    m_file.seekp(p);
}

void ChaosRom::writeAddress_32bit(uint32_t address)
{
    m_file.put((char)((address >> 24) & 0xFF));
    m_file.put((char)((address >> 16) & 0xFF));
    m_file.put((char)((address >> 8) & 0xFF));
    m_file.put((char)((address) & 0xFF));
}

void ChaosRom::writeAddress_32bit_at(uint32_t address, std::streamoff offset)
{
    streamoff p = m_file.tellp();

    m_file.seekp(offset);

    m_file.put((char)((address >> 24) & 0xFF));
    m_file.put((char)((address >> 16) & 0xFF));
    m_file.put((char)((address >> 8) & 0xFF));
    m_file.put((char)((address) & 0xFF));

    m_file.seekp(p);
}
