///////////////////////////////////////////////////////////////////////////////
//
// ChaosLog.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __CHAOS_LOG_H
#define __CHAOS_LOG_H

#include "ChaosException.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#ifdef WIN32
# include <windows.h>
#else
# include <pthread.h>
#endif


///////////////////////////////////////////////////////////////////////////////
//
// ChaosLog class definition
//
///////////////////////////////////////////////////////////////////////////////

class ChaosLog
{
public:

    ChaosLog() throw(ChaosException);

    virtual ~ChaosLog() throw(ChaosException);

    static bool init(const std::string& filename) throw(ChaosException);
    static void cleanup() throw(ChaosException);

    template <typename T>
    std::ostringstream& operator<<(T const& value)    // Stream semantics
    {
        // Inline function
        
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

#ifdef WIN32
    static HANDLE ms_fileMutex;
#else
    static pthread_mutex_t ms_fileMutex;
#endif

    static std::ofstream ms_file;
};

#endif  //__CHAOS_LOG_H

