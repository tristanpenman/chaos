#pragma once

class Instance;
class Instance_Level;
class Instance_Sprites;

enum InstanceKind;

/**
 * ROM wrapper base class
 *
 * Provides an interface used by the ChaosRomFactory class to select the
 * correct subclass for a given ROM.
 *
 * Also provides an interface by which a subclass can instantiate level editor
 * and sprite editor instances.
 */
class ChaosRom : public SegaRom
{
public:
    explicit ChaosRom(std::fstream& rom);
    virtual ~ChaosRom();

    typedef std::map<unsigned int, std::string> LevelNames_t;

    // abstract
    virtual bool validateROM() = 0;
    virtual LevelNames_t getLevelNames() = 0;

    Instance* initInstance(HWND hwnd);
    Instance* getInstance(HWND hwnd);

    bool destroyInstance(HWND hwnd);

protected:
    virtual Instance* instantiateLevel() = 0;

protected:
    typedef std::map<HWND, Instance*> Instances_t;

    Instances_t m_instances;
};
