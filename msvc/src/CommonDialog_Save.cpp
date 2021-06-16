#include "Precompiled.h"

#include "CommonDialog.h"
#include "CommonDialog_Save.h"

bool CommonDialog_Save::execute()
{
	m_ofn.Flags = OFN_OVERWRITEPROMPT;

	return (GetSaveFileName(&m_ofn) == TRUE);
}
