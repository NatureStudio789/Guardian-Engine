#include "GConverter.h"

namespace GE
{
    std::wstring GConverter::StringToWideString(std::string text)
    {
        return std::wstring(text.begin(), text.end());
    }

    float GConverter::DegreeToAngle(float degree)
    {
        return (degree / 360.0f) * XM_2PI;
    }
}
