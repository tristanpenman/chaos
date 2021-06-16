#ifndef __PRECOMPILED_H
#define __PRECOMPILED_H

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <sstream>
#include <map>
#include <exception>

#ifdef WIN32

#include <assert.h>
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>
#include "Resource.h"

void REPORT_ERROR(const std::string& message, const std::string& title);
void REPORT_INFO(const std::string& message, const std::string& title);

#else

inline void REPORT_ERROR(const std:string& message, const std::string& title)
{
	std::cerr << title << ": " << message << std::endl;
}

inline void REPORT_INFO(const std:string& message, const std::string& title)
{
	std::cerr << title << ": " << message << std::endl;
}

#endif	// WIN32

#endif	// __PRECOMPILED_H