#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <sstream>
#include <map>
#include <exception>

#include <assert.h>
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>
#include "Resource.h"

void REPORT_ERROR(const std::string& message, const std::string& title);
void REPORT_INFO(const std::string& message, const std::string& title);
