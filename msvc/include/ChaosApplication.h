#pragma once

class ChaosRom;

class Buffer_Patterns;
class Buffer_Blocks;

class Instance_Level;

class ChaosApplication
{
public:
    ChaosApplication();
    virtual ~ChaosApplication();

    int run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);

    bool cleanup();

    const HWND getMainWindow() const;

    void resetMainWindow(HWND);

    Instance_Level* getLevelInstance(HWND);

    bool destroyInstance(HWND);

    ChaosRom* getROM();

    bool openROM(const std::string& path);
    bool saveROM();
    bool saveAsROM(const std::string& path);
    bool closeROM();

protected:
    void destroyWindowClasses();

private:
    HWND m_hMain;
    HINSTANCE m_hInstance;
    ChaosRom* m_pROM;
    std::fstream m_file;
};

inline ChaosRom* ChaosApplication::getROM()
{
    return m_pROM;
}
