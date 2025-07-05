#include "GConverter.h"

namespace GE
{
    std::wstring GConverter::StringToWideString(std::string text)
    {
        return std::wstring(text.begin(), text.end());
    }
}
