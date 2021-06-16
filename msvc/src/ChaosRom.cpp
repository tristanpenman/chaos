#include "Precompiled.h"

#include "SegaRom.h"

#include "ChaosException.h"
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

bool ChaosRom::hasUnsavedChanges() const
{
    Instances_t::const_iterator itr;

    for (itr = m_instances.begin(); itr != m_instances.end(); ++itr)
    {
        Instance* pInstance = itr->second;
        if (pInstance->hasUnsavedChanges())
        {
            return true;
        }
    }

    return false;
}

Instance* ChaosRom::initInstance(HWND hwnd, InstanceKind k)
{
    Instance* pInstance = NULL;
    Instances_t::iterator itr = m_instances.find(hwnd);

    if (itr == m_instances.end())
    {
        switch (k)
        {
        case INSTANCE_KIND_LEVEL:
            pInstance = instantiateLevel();
            break;

        case INSTANCE_KIND_SPRITES:
            pInstance = instantiateSprites();
            break;

        default:
            return NULL;
        }

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

    if (pInstance->hasUnsavedChanges())
    {
        if (!pInstance->saveChanges())
        {
            return false;
        }
    }

    m_instances.erase(itr);
    delete pInstance;
    return true;
}
