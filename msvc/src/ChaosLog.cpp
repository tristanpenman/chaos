#include "Precompiled.h"

#include <windows.h>
#include <time.h>

#include "ChaosException.h"
#include "ChaosLog.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
// Windows implementation of gettimeofday()
//
///////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
    int  tz_minuteswest; /* minutes W of Greenwich */
    int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    // Define a structure to receive the current Windows filetime
    FILETIME ft;

    // Initialize the present time to 0 and the timezone to UTC
    unsigned __int64 tmpres = 0;
    static int tzflag = 0;

    if (NULL != tv)
    {
        GetSystemTimeAsFileTime(&ft);

        // The GetSystemTimeAsFileTime returns the number of 100 nanosecond
        // intervals since Jan 1, 1601 in a structure. Copy the high bits to
        // the 64 bit tmpres, shift it left by 32 then or in the low 32 bits.
        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        // Convert to microseconds by dividing by 10
        tmpres /= 10;

        // The Unix epoch starts on Jan 1 1970.  Need to subtract the difference
        // in seconds from Jan 1 1601.
        tmpres -= DELTA_EPOCH_IN_MICROSECS;

        // Finally change microseconds to seconds and place in the seconds value.
        // The modulus picks up the microseconds.
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }

    if (NULL != tz)
    {
        if (!tzflag)
        {
            _tzset();
            tzflag++;
        }

        // Adjust for the timezone west of Greenwich
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// ChaosLog class implementation
//
///////////////////////////////////////////////////////////////////////////////

// Logging is thread safe. Only one ChaosLog instance can write to the
// console or output file at a time.

HANDLE ChaosLog::ms_fileMutex = CreateMutex(NULL, FALSE, NULL);

ofstream ChaosLog::ms_file;

void ChaosLog::lock() throw(ChaosException)
{
    if (ms_fileMutex == NULL)
    {
        throw ChaosException("Failed to create mutex for log output");
    }

    if (WaitForSingleObject(ms_fileMutex, INFINITE) != WAIT_OBJECT_0)
    {
        throw ChaosException("Failed to lock mutex for log output");
    }
}

void ChaosLog::unlock() throw(ChaosException)
{
    if (ReleaseMutex(ms_fileMutex) != TRUE)
    {
        throw ChaosException("Failed unlock mutex for log output");
    }
}

ChaosLog::ChaosLog() throw(ChaosException)
  : m_pFile(NULL)
{
    ChaosLog::lock();

    if (ms_file.is_open())
    {
        m_pFile = &ms_file;
    }

    ChaosLog::unlock();

    timeval tv;
    gettimeofday(&tv, NULL);

    m_os << tv.tv_sec << "." << tv.tv_usec << " (" << GetCurrentThreadId() << "): ";
}

ChaosLog::~ChaosLog() throw(ChaosException)
{
    m_os << endl;

    ChaosLog::lock();

    if (m_pFile)
    {
        *m_pFile << m_os.str();
        m_pFile->flush();
    }
    else
    {
        cerr << m_os.str();
    }

    ChaosLog::unlock();
}

bool ChaosLog::init(const string& filename) throw(ChaosException)
{
    ChaosLog::lock();

    try
    {
        if (ms_file.is_open())
        {
            ms_file.close();
        }

        ms_file.open(filename.c_str());
    }
    catch (...)  // Catch IO errors
    {
        if (ms_file.is_open())
        {
            ms_file.close();
        }
    }

    if (!ms_file.is_open())
    {
        cerr << "Warning: Could not open file \""
             << filename
             << "\" for logging."
             << endl;

        ChaosLog::unlock();
        return false;
    }

    ChaosLog::unlock();
    return true;
}

void ChaosLog::cleanup() throw(ChaosException)
{
    if (ms_file.is_open())
    {
        ms_file.close();
    }

    CloseHandle(ms_fileMutex);
    ms_fileMutex = NULL;
}

