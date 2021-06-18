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

uint16_t ChaosRom::read16BitAddr()
{
    uint16_t addr = m_file.get() << 8;
    addr |= m_file.get();

    return addr;
}

uint16_t ChaosRom::read16BitAddr(streamoff offset)
{
    
    const streamoff bookmark = m_file.tellg();

    m_file.seekg(offset);

    uint16_t addr = m_file.get() << 8;
    addr |= m_file.get();

    m_file.seekg(bookmark);

    return addr;
}

uint32_t ChaosRom::read32BitAddr()
{
    uint32_t addr = m_file.get() << 24;
    addr |= m_file.get() << 16;
    addr |= m_file.get() << 8;
    addr |= m_file.get();

    return addr;
}

uint32_t ChaosRom::read32BitAddr(streamoff offset)
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

