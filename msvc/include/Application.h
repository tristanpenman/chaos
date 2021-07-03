#pragma once

class BlockBuffer;
class Level;
class PatternBuffer;
class Rom;

class Application
{
public:
    Application();

    virtual ~Application();

    int run(HINSTANCE hinst, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
    bool cleanup();
    const HWND getMainWindow() const;
    void resetMainWindow(HWND);
    Level* getLevelInstance(HWND);
    bool destroyInstance(HWND);
    Rom* getROM();
    bool openROM(const std::string& path);
    bool closeROM();

protected:
    void destroyWindowClasses();

private:
    HWND m_hwnd_main;
    HINSTANCE m_hinst;
    Rom* m_rom;
    std::fstream m_file;
};

inline Rom* Application::getROM()
{
    return m_rom;
}
