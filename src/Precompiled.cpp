#include "Precompiled.h"

#include "InstanceValue.h"

#include "ChaosException.h"
#include "ChaosApplication.h"

extern ChaosApplication g_application;

void REPORT_ERROR(const std::string& message, const std::string& title)
{
	MessageBox(g_application.getMainWindow(), message.c_str(), title.c_str(), MB_ICONERROR);
}

void REPORT_INFO(const std::string& message, const std::string& title)
{
	MessageBox(g_application.getMainWindow(), message.c_str(), title.c_str(), MB_ICONINFORMATION);
}