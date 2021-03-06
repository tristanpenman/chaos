#include "Precompiled.h"

#include "Level.h"

#include "Rom.h"

#define ROM_HEADER_OFFSET 0x100
#define ROM_LENGTH_OFFSET 0x01A4
#define DOMESTIC_NAME_LEN 48
#define DOMESTIC_NAME_OFFSET ROM_HEADER_OFFSET + 32
#define INTERNATIONAL_NAME_LEN 48
#define INTERNATIONAL_NAME_OFFSET DOMESTIC_NAME_OFFSET + DOMESTIC_NAME_LEN

using namespace std;

Rom::Rom(fstream& file)
  : m_file(file)
  , m_level_instances()
{

}

Rom::~Rom()
{
    LevelInstances::iterator itr;
    for (itr = m_level_instances.begin(); itr != m_level_instances.end(); ++itr)
    {
        Level* pLevel = itr->second;
        delete pLevel;
    }

    m_level_instances.clear();
}

Level* Rom::initLevelInstance(HWND hwnd)
{
    Level* pLevel = NULL;
    LevelInstances::iterator itr = m_level_instances.find(hwnd);

    if (itr == m_level_instances.end())
    {
        pLevel = instantiateLevel();
        pLevel->setWindow(hwnd);

        return (m_level_instances[hwnd] = pLevel);
    }

    return NULL;
}

Level* Rom::getLevelInstance(HWND hwnd)
{
    LevelInstances::iterator itr = m_level_instances.find(hwnd);
    if (itr == m_level_instances.end())
    {
        return NULL;
    }

    return itr->second;
}

bool Rom::destroyInstance(HWND hwnd)
{
    LevelInstances::iterator itr = m_level_instances.find(hwnd);
    if (itr == m_level_instances.end())
    {
        return false;
    }

    Level* level = itr->second;
    m_level_instances.erase(itr);
    delete level;

    return true;
}

string Rom::getDomesticName()
{
    char buffer[DOMESTIC_NAME_LEN + 1];

    m_file.seekg(DOMESTIC_NAME_OFFSET);
    m_file.read(buffer, DOMESTIC_NAME_LEN);

    buffer[m_file.gcount()] = 0;

    return buffer;
}

string Rom::getInternationalName()
{
    char buffer[INTERNATIONAL_NAME_LEN + 1];

    m_file.seekg(INTERNATIONAL_NAME_OFFSET);
    m_file.read(buffer, INTERNATIONAL_NAME_LEN);

    buffer[m_file.gcount()] = 0;

    return buffer;
}

uint16_t Rom::read16BitAddr()
{
    uint16_t addr = m_file.get() << 8;
    addr |= m_file.get();

    return addr;
}

uint16_t Rom::read16BitAddr(streamoff offset)
{

    const streamoff bookmark = m_file.tellg();

    m_file.seekg(offset);

    uint16_t addr = m_file.get() << 8;
    addr |= m_file.get();

    m_file.seekg(bookmark);

    return addr;
}

uint32_t Rom::read32BitAddr()
{
    uint32_t addr = m_file.get() << 24;
    addr |= m_file.get() << 16;
    addr |= m_file.get() << 8;
    addr |= m_file.get();

    return addr;
}

uint32_t Rom::read32BitAddr(streamoff offset)
{
    const streamoff bookmark = m_file.tellg();

    m_file.seekg(offset);

    uint32_t addr = m_file.get() << 24;
    addr |= m_file.get() << 16;
    addr |= m_file.get() << 8;
    addr |= m_file.get();

    m_file.seekg(bookmark);

    return addr;
}

