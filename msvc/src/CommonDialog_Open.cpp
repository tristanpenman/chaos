#include "Precompiled.h"

#include "CommonDialog.h"
#include "CommonDialog_Open.h"

bool CommonDialog_Open::execute()
{
	m_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

	return (GetOpenFileName(&m_ofn) == TRUE);
}