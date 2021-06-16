#include "Precompiled.h"

#include "CommonDialog.h"
#include "CommonDialog_Open.h"
#include "CommonDialog_Save.h"
#include "CommonDialogFactory.h"

CommonDialog_Open& CommonDialogFactory::ms_dialogOpen = CommonDialog_Open(0);
CommonDialog_Save& CommonDialogFactory::ms_dialogSave = CommonDialog_Save(0);

CommonDialog& CommonDialogFactory::openDialog()
{
    return ms_dialogOpen;
}

CommonDialog& CommonDialogFactory::saveDialog()
{
    return ms_dialogSave;
}
