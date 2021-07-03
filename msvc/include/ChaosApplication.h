#pragma once

class Buffer_Patterns;
class Buffer_Blocks;
class ChaosRom;
class Level;

class ChaosApplication
{
public:
    ChaosApplication();

    virtual ~ChaosApplication();

    int run(HINSTANCE hinst, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);

    bool cleanup();

    const HWND getMainWindow() const;

    void resetMainWindow(HWND);

    Level* getLevelInstance(HWND);

    bool destroyInstance(HWND);

    ChaosRom* getROM();

    bool openROM(const std::string& path);

    bool closeROM();

protected:
    void destroyWindowClasses();

private:
    HWND m_hwnd_main;
    HINSTANCE m_hinst;
    ChaosRom* m_rom;
    std::fstream m_file;
};

inline ChaosRom* ChaosApplication::getROM()
{
    return m_rom;
}
