#pragma once

class Level;

/**
 * ROM wrapper base class
 *
 * Provides an interface used by the ChaosRomFactory class to select the
 * correct subclass for a given ROM.
 *
 * Also provides an interface by which a subclass can instantiate level editor
 * and sprite editor instances.
 */
class ChaosRom
{
public:
    typedef std::map<unsigned int, std::string> LevelNames_t;

    explicit ChaosRom(std::fstream& rom);

    virtual ~ChaosRom();

    std::fstream& getFile();

    // abstract
    virtual bool validateROM() = 0;
    virtual LevelNames_t getLevelNames() = 0;

    Level* getLevelInstance(HWND hwnd);
    Level* initLevelInstance(HWND hwnd);

    bool destroyInstance(HWND hwnd);

    std::string getDomesticName();
    std::string getInternationalName();

    uint16_t read16BitAddr();
    uint16_t read16BitAddr(std::streamoff offset);
    uint32_t read32BitAddr();
    uint32_t read32BitAddr(std::streamoff offset);

protected:
    virtual Level* instantiateLevel() = 0;

    std::fstream& m_file;

protected:
    typedef std::map<HWND, Level*> LevelInstances;

    LevelInstances m_level_instances;
};

inline std::fstream& ChaosRom::getFile()
{
    return m_file;
}
