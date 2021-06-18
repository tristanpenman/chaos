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
  : m_pROM(NULL)
  , m_hMain(0)
  , m_hInstance(0)
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
    Level* pLevel = m_pROM->getLevelInstance(hwnd);
    if (pLevel)
    {
        return pLevel;
    }

    return dynamic_cast<Level*>(m_pROM->initLevelInstance(hwnd));
}

bool ChaosApplication::destroyInstance(HWND hwnd)
{
    return m_pROM->destroyInstance(hwnd);
}

int ChaosApplication::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    InitCommonControls();

    m_hInstance = hInstance;
    m_hMain = WndFrame::createWindow(hInstance, NULL);

    if (m_hMain)
    {
        ShowWindow(m_hMain, iCmdShow);
        UpdateWindow(m_hMain);
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
    SendMessage(m_hMain, WM_COMMAND, ID_WINDOW_CLOSEALL, 0) ;

    if (m_pROM)
    {
        delete m_pROM;
        m_pROM = NULL;
    }

    return true;
}

const HWND ChaosApplication::getMainWindow() const
{
    return m_hMain;
}

void ChaosApplication::resetMainWindow(HWND hwnd)
{
    m_hMain = hwnd;
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

    m_pROM = ChaosRomFactory::loadROM(m_file);
    if (!m_pROM)
    {
        REPORT_ERROR("Failed to identify ROM", "Error loading ROM");
        m_file.close();
        return false;
    }

    return true;
}

bool ChaosApplication::closeROM()
{
    if (m_pROM)
    {
        if (!cleanup())
        {
            return false;
        }

        delete m_pROM;
        m_pROM = NULL;

        m_file.close();
    }

    return true;
}
