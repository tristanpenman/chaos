#include "Precompiled.h"

#include "ChaosRom.h"
#include "ChaosRomFactory.h"
#include "ChaosApplication.h"

#include "WndFrame.h"
#include "WndMap.h"
#include "WndLevel.h"

#include "SegaPalette.h"
#include "SegaPattern.h"
#include "SonicChunk.h"
#include "SonicBlock.h"

#include "Buffer.h"
#include "Buffer_Patterns.h"
#include "Buffer_Blocks.h"

#include "Level.h"

using namespace std;

ChaosApplication::ChaosApplication()
  : m_rom(NULL)
  , m_hwnd_main(0)
  , m_hinst(0)
  , m_file()
{

}

ChaosApplication::~ChaosApplication()
{
    closeROM();
    destroyWindowClasses();
}

Level* ChaosApplication::getLevelInstance(HWND hwnd)
{
    Level* level = m_rom->getLevelInstance(hwnd);
    if (level)
    {
        return level;
    }

    return dynamic_cast<Level*>(m_rom->initLevelInstance(hwnd));
}

bool ChaosApplication::destroyInstance(HWND hwnd)
{
    return m_rom->destroyInstance(hwnd);
}

int ChaosApplication::run(HINSTANCE hinst, HINSTANCE hinst_prev, PSTR cmd_line, int cmd_show)
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

void ChaosApplication::destroyWindowClasses()
{
    WndFrame::destroyClass();
    WndMap::destroyClass();
    WndLevel::destroyClass();
}

bool ChaosApplication::cleanup()
{
    SendMessage(m_hwnd_main, WM_COMMAND, ID_WINDOW_CLOSEALL, 0);

    if (m_rom)
    {
        delete m_rom;
        m_rom = NULL;
    }

    return true;
}

const HWND ChaosApplication::getMainWindow() const
{
    return m_hwnd_main;
}

void ChaosApplication::resetMainWindow(HWND hwnd)
{
    m_hwnd_main = hwnd;
}

bool ChaosApplication::openROM(const string& path)
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

    m_rom = ChaosRomFactory::loadROM(m_file);
    if (!m_rom)
    {
        REPORT_ERROR("Failed to identify ROM", "Error loading ROM");
        m_file.close();
        return false;
    }

    return true;
}

bool ChaosApplication::closeROM()
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
