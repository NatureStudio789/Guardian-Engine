#include "GuardianConverter.h"

namespace guardian
{
    const GWideString GuardianConverter::StringToWideString(const GString& string)
    {
        return GWideString(string.begin(), string.end());
    }

    const GString GuardianConverter::HRESULTToString(HRESULT hr)
    {
		char* MsgBuffer = null;
		DWORD MsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, null, hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&MsgBuffer), 0, null);
		if (MsgLen == 0)
		{
			return "Undefined error code";
		}

		GString ErrorString = MsgBuffer;
		LocalFree(MsgBuffer);
		return ErrorString;
    }
}
