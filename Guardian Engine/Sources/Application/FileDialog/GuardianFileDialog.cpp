#include "GuardianFileDialog.h"
#include "../GuardianApplication.h"

namespace GE
{
    const GString GuardianFileDialog::OpenFile(const char* filter)
    {
        OPENFILENAMEA OpenFileName;
        char szFile[260] = { 0 };
        ZeroMemory(&OpenFileName, sizeof(OPENFILENAME));

        OpenFileName.lStructSize = sizeof(OPENFILENAME);
        OpenFileName.hwndOwner = GuardianApplication::ApplicationInstance->GetMainWindowHandle();
        OpenFileName.lpstrFile = szFile;
        OpenFileName.nMaxFile = sizeof(szFile);
        OpenFileName.lpstrFilter = filter;
        OpenFileName.nFilterIndex = 1;
        OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&OpenFileName) == TRUE)
        {
            return OpenFileName.lpstrFile;
        }

        return GString();
    }

    const GString GuardianFileDialog::SaveFile(const char* filter)
    {
        OPENFILENAMEA OpenFileName;
        char szFile[260] = { 0 };
        ZeroMemory(&OpenFileName, sizeof(OPENFILENAME));

        OpenFileName.lStructSize = sizeof(OPENFILENAME);
        OpenFileName.hwndOwner = GuardianApplication::ApplicationInstance->GetMainWindowHandle();
        OpenFileName.lpstrFile = szFile;
        OpenFileName.nMaxFile = sizeof(szFile);
        OpenFileName.lpstrFilter = filter;
        OpenFileName.nFilterIndex = 1;
        OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileNameA(&OpenFileName) == TRUE)
        {
            return OpenFileName.lpstrFile;
        }

        return GString();
    }
}
