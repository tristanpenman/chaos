#include <iostream>

#include "Logger.h"

using namespace std;

ostream* Logger::m_os = nullptr;

Logger::Logger(const string& name)
{
  if (m_os) {
    m_ss << "[" << name << "] ";
  }
}

Logger::~Logger()
{
  if (m_os) {
    *m_os << m_ss.str() << std::endl;
  }
}

void Logger::configure()
{
  m_os = &cout;
}

void Logger::configure(ostream &os)
{
  m_os = &os;
}
