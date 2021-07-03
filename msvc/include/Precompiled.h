#pragma once

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <assert.h>
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>

#include "Resource.h"

// Used to annotate deleted constructors
#define DELETED

void REPORT_ERROR(const std::string& message, const std::string& title);
void REPORT_INFO(const std::string& message, const std::string& title);
