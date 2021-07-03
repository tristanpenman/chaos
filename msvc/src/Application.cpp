#include "Precompiled.h"

#include "Block.h"
#include "BlockBuffer.h"
#include "Buffer.h"
#include "Chunk.h"
#include "Level.h"
#include "Palette.h"
#include "Pattern.h"
#include "PatternBuffer.h"
#include "Rom.h"
#include "RomFactory.h"
#include "WndFrame.h"
#include "WndLevel.h"
#include "WndMap.h"

#include "Application.h"

using namespace std;

Application::Application()
  : m_rom(NULL)
  , m_hwnd_main(0)
  , m_hinst(0)
  , m_file()
{

}

Application::~Application()
{
    closeROM();
    destroyWindowClasses();
}

Level* Application::getLevelInstance(HWND hwnd)
{
    Level* level = m_rom->getLevelInstance(hwnd);
    if (level)
    {
        return level;
    }

    return dynamic_cast<Level*>(m_rom->initLevelInstance(hwnd));
}

bool Application::destroyInstance(HWND hwnd)
{
    return m_rom->destroyInstance(hwnd);
}

int Application::run(HINSTANCE hinst, HINSTANCE hinst_prev, PSTR cmd_line, int cmd_show)
{
    InitCommonControls();

    m_hinst = hinst;
    m_hwnd_main = WndFrame::createWindow(hinst, NULL);

    if (m_hwnd_main)
    {
        ShowWindow(m_hwnd_main, cmd_show);
        UpdateWindow(m_hwnd_main);
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void Application::destroyWindowClasses()
{
    WndFrame::destroyClass();
    WndMap::destroyClass();
    WndLevel::destroyClass();
}

bool Application::cleanup()
{
    SendMessage(m_hwnd_main, WM_COMMAND, ID_WINDOW_CLOSEALL, 0);

    if (m_rom)
    {
        delete m_rom;
        m_rom = NULL;
    }

    return true;
}

const HWND Application::getMainWindow() const
{
    return m_hwnd_main;
}

void Application::resetMainWindow(HWND hwnd)
{
    m_hwnd_main = hwnd;
}

bool Application::openROM(const string& path)
{
    if (!closeROM())
    {
        REPORT_ERROR("Failed to clean up previously loaded ROM", "ROM Error");
        return false;
    }

    m_file.open(path.c_str(), ios::in | ios::out | ios::binary);
    if (!m_file.is_open())
    {
        REPORT_ERROR("Failed to open the selected ROM", "ROM Error");
        return false;
    }

    m_rom = RomFactory::loadROM(m_file);
    if (!m_rom)
    {
        REPORT_ERROR("Failed to identify ROM", "Error loading ROM");
        m_file.close();
        return false;
    }

    return true;
}

bool Application::closeROM()
{
    if (m_rom)
    {
        if (!cleanup())
        {
            return false;
        }

        delete m_rom;
        m_rom = NULL;

        m_file.close();
    }

    return true;
}
