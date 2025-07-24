#ifndef _GE_GUTIL_H_
#define _GE_GUTIL_H_
#include "../../Profile/UUID/GUUID.h"

namespace GE
{
	class GUARDIAN_API GUtil
	{
	public:
		static std::wstring StringToWideString(std::string text);
		static float DegreeToAngle(float degree);

		static std::vector<std::string> SplitString(const std::string& text, const std::string& delim);
	};
}

#endif