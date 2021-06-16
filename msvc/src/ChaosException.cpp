#include "Precompiled.h"

#include "ChaosException.h"

using namespace std;

string ChaosException::getMessage() const
{
    return m_msg;
}
