#ifndef __CHAOS_ROM_H
#define __CHAOS_ROM_H

//
// Circular dependencies
//
class Instance;
class Instance_Level;     
class Instance_Sprites;

enum InstanceKind;

/******************************************************************************
 *
 * ROM wrapper base class
 *
 * Provides an interface used by the ChaosRomFactory class to select the
 * correct subclass for a given ROM.
 *
 * Also provides an interface by which a subclass can instantiate level editor 
 * and sprite editor instances.
 *
 *****************************************************************************/

class ChaosRom : public SegaRom {

public:
	explicit ChaosRom(std::fstream& rom);
	virtual ~ChaosRom();

	bool hasUnsavedChanges() const;

	typedef std::map<unsigned int, std::string> LevelNames_t;

/***************************************************************************
 *
 * Abstract interface
 *
 *****************************************************************************/

public:

	virtual bool validateROM() = 0;

	virtual LevelNames_t getLevelNames() = 0;

protected:

	virtual Instance* instantiateLevel() = 0;
	virtual Instance* instantiateSprites() = 0;

#ifdef WIN32

/******************************************************************************
 *
 * Win32 specific code
 *
 *****************************************************************************/

public:

	Instance* initInstance(HWND hwnd, InstanceKind k);
	Instance* getInstance(HWND hwnd);

	bool      destroyInstance(HWND hwnd);

protected:

	typedef std::map<HWND, Instance*> Instances_t;

	bool addInstance(HWND hwnd, Instance*);

#else

/******************************************************************************
 *
 * Unix specific code
 *
 *****************************************************************************/

public:

	Instance* initInstance(unsigned int id, InstanceKind k);
	Instance* getInstance(unsigned int id);

	bool      destroyInstance(unsigned int id);

protected:

	typedef std::map<unsigned int, Instance*> Instances_t;

	bool addInstance(HWND hwnd, Instance*);

#endif

/******************************************************************************
 *
 * Variables
 *
 *****************************************************************************/

protected:

	Instances_t m_instances;

};


#endif