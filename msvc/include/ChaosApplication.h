#ifndef __CHAOS_APPLICATION_H
#define __CHAOS_APPLICATION_H

class ChaosRom;

class Buffer_Patterns;
class Buffer_Blocks;

class Instance_Level;
class Instance_Sprites;

class ChaosApplication 
{

#ifdef WIN32

/******************************************************************************
 *
 * Windows code
 *
 *****************************************************************************/

public:
	
	explicit ChaosApplication();
	virtual ~ChaosApplication();

	int run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);

	bool cleanup(bool force);

	const HWND getMainWindow() const;

	void resetMainWindow(HWND);

	Instance_Level*   getLevelInstance(HWND);
	Instance_Sprites* getSpritesInstance(HWND);

	bool              destroyInstance(HWND);

protected:

	void destroyWindowClasses();

private:

	HWND                     m_hMain;
	HINSTANCE                m_hInstance;
	ChaosRom*                m_pROM;

#else
	
/******************************************************************************
 *
 * UNIX / command line code
 *
 *****************************************************************************/

public:
	explicit ChaosApplication();
	virtual ~ChaosApplication();

	int run(int argc, char** argv);

	bool cleanup();

#endif


/******************************************************************************
 *
 * Common code
 *
 *****************************************************************************/

public:

	ChaosRom* getROM();

	//
	// File interface
	//

	bool         openROM(const std::string& path);
	bool         saveROM();
	bool         saveAsROM(const std::string& path);
	bool         closeROM();
	
private:
	
	std::fstream m_file;

};

inline ChaosRom* ChaosApplication::getROM()
{
	return m_pROM;
}

#endif
