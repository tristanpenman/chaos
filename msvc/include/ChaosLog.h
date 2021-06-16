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

    ChaosLog() throw(ChaosException);

    virtual ~ChaosLog() throw(ChaosException);

    static bool init(const std::string& filename) throw(ChaosException);
    static void cleanup() throw(ChaosException);

    template <typename T>
    std::ostringstream& operator<<(T const& value)
    {
        m_os << value;
        return m_os;
    }

protected:

    static void lock() throw(ChaosException);
    static void unlock() throw(ChaosException);

private:

    // Disable copy construction
    ChaosLog(const ChaosLog&);
    ChaosLog& operator=(const ChaosLog&);

    std::ostringstream m_os;

    std::ofstream* m_pFile;

    static HANDLE ms_fileMutex;
    static std::ofstream ms_file;
};

#endif  //__CHAOS_LOG_H
