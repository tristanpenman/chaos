#ifndef __CHAOS_LOG_H
#define __CHAOS_LOG_H

#include "ChaosException.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <windows.h>

class ChaosLog
{
public:
    ChaosLog();
    ChaosLog(const ChaosLog&) = delete;
    virtual ~ChaosLog();

    ChaosLog& operator=(const ChaosLog&) = delete;

    static bool init(const std::string& filename);
    static void cleanup();

    template <typename T>
    std::ostringstream& operator<<(T const& value)
    {
        m_os << value;
        return m_os;
    }

protected:
    static void lock();
    static void unlock();

private:
    std::ostringstream m_os;
    std::ofstream* m_pFile;

    static HANDLE ms_fileMutex;
    static std::ofstream ms_file;
};

#endif  //__CHAOS_LOG_H
