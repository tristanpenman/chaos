#include "Precompiled.h"

#include "InstanceValue.h"

#include "SegaRom.h"

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

#include "Instance.h"
#include "Instance_Level.h"

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

Instance_Level* ChaosApplication::getLevelInstance(HWND hwnd)
{
    Instance* pInstance = m_pROM->getInstance(hwnd);
    if (pInstance)
    {
        Instance_Level* pLevelInstance = dynamic_cast<Instance_Level*>(pInstance);
        if (pLevelInstance)
        {
            return pLevelInstance;
        }
    }

    return dynamic_cast<Instance_Level*>(m_pROM->initInstance(hwnd));
}

bool ChaosApplication::destroyInstance(HWND hwnd)
{
    return m_pROM->destroyInstance(hwnd);
}

int ChaosApplication::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    try
    {
        InitCommonControls();

        m_hMain = WndFrame::createWindow(hInstance, NULL);
        m_hInstance = hInstance;

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

    }
    catch (std::exception& e)
    {
        REPORT_ERROR(e.what(), "Fatal error");
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
    if (closeROM())
    {
        try
        {
            m_file.open(path.c_str(), ios::in | ios::out | ios::binary);

            if (!m_file.is_open())
            {
                REPORT_ERROR("Could not open the selected ROM", "File error");
                return false;
            }

            m_pROM = ChaosRomFactory::loadROM(m_file);

            return true;
        }
        catch (std::exception& e)
        {
            REPORT_ERROR(e.what(), "Error loading ROM");
        }
    }

    cleanup();

    if (m_file.is_open())
    {
        m_file.close();
    }

    return false;
}

bool ChaosApplication::saveROM()
{
    return false;
}

bool ChaosApplication::saveAsROM(const std::string& path)
{
    return false;
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
