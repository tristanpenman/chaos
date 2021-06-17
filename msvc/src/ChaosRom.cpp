#include "Precompiled.h"

#include "SegaRom.h"
#include "ChaosRom.h"
#include "Instance.h"

using namespace std;

ChaosRom::ChaosRom(fstream& file)
  : SegaRom(file)
  , m_instances()
{
}

ChaosRom::~ChaosRom()
{
    Instances_t::iterator itr;

    for (itr = m_instances.begin(); itr != m_instances.end(); ++itr)
    {
        Instance* pInstance = itr->second;
        delete pInstance;
    }

    m_instances.clear();
}

Instance* ChaosRom::initInstance(HWND hwnd)
{
    Instance* pInstance = NULL;
    Instances_t::iterator itr = m_instances.find(hwnd);

    if (itr == m_instances.end())
    {
        pInstance = instantiateLevel();
        pInstance->setWindow(hwnd);

        return (m_instances[hwnd] = pInstance);
    }

    return NULL;
}

Instance* ChaosRom::getInstance(HWND hwnd)
{
    Instances_t::iterator itr = m_instances.find(hwnd);

    if (itr == m_instances.end())
    {
        return NULL;
    }

    return itr->second;
}

bool ChaosRom::destroyInstance(HWND hwnd)
{
    Instances_t::iterator itr = m_instances.find(hwnd);

    if (itr == m_instances.end())
    {
        return false;
    }

    Instance* pInstance = itr->second;
    m_instances.erase(itr);
    delete pInstance;
    return true;
}
