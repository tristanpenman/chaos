#include "Precompiled.h"

#include "InstanceValue.h"

#include "SegaRom.h"

#include "ChaosException.h"
#include "ChaosRom.h"
#include "ChaosRomFactory.h"
#include "ChaosApplication.h"

#ifdef WIN32

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

/******************************************************************************
 *
 * Windows code
 *
 *****************************************************************************/

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
	
	assert (m_hMain == 0);

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

	return dynamic_cast<Instance_Level*>(m_pROM->initInstance(hwnd, INSTANCE_KIND_LEVEL));
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
			ShowWindow (m_hMain, iCmdShow);
			UpdateWindow (m_hMain);
		}

		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
	catch (ChaosException& e)
	{
		REPORT_ERROR(e.getMessage(), "Fatal error");
	}
	catch (...)
	{
		REPORT_ERROR("Unhandled exception", "Fatal error");
	}

	return 0;
}

void ChaosApplication::destroyWindowClasses()
{
	WndFrame::destroyClass();
	WndMap::destroyClass();
	WndLevel::destroyClass();
}

bool ChaosApplication::cleanup(bool force)
{
	SendMessage (m_hMain, WM_COMMAND, ID_WINDOW_CLOSEALL, 0) ;

	if (m_pROM)
	{
		if (!force && m_pROM->hasUnsavedChanges())
		{
			return false;
		}

		delete m_pROM; m_pROM = NULL;
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

#else

/******************************************************************************
 *
 * UNIX / command line code
 *
 *****************************************************************************/

using namespace std;

ChaosApplication::ChaosApplication()
: m_pROM(NULL)
, m_isReadOnly(false)
, m_file()
{

}

ChaosApplication::~ChaosApplication()
{

}

int ChaosApplication::run(int argc, char** argv)
{
	try
	{
		// Do program stuff
	}
	catch (ChaosException& e)
	{
		REPORT_ERROR(e.getMessage(), "Fatal error");
	}
	catch (...)
	{
		REPORT_ERROR("Unhandled exception", "Fatal error");
	}

	return 0;
}

bool ChaosApplication::cleanup(bool force)
{
	if (m_pROM)
	{
		delete m_pROM; m_pROM = NULL;
	}

	return true;
}

#endif


/******************************************************************************
 *
 * Common code
 *
 *****************************************************************************/

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
		catch (ChaosException& e)
		{
			REPORT_ERROR(e.getMessage(), "Error loading ROM");
		}
		catch (...)
		{
			REPORT_ERROR("An error occured while loading the selected ROM.", "Error loading ROM");
		}
	}

	cleanup(true);	// No confirmation dialogs

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
		if (!cleanup(false))
		{
			return false;
		}

		delete m_pROM; m_pROM = NULL;

		m_file.close();
	}

	return true;
}