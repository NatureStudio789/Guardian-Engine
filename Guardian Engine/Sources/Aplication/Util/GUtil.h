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

		static std::string GetFilePathDirectory(const std::string& filePath);
		static std::string GetFileName(const std::string& filePath);
		static std::string GetFileExtension(const std::string& filePath);
		static std::string ExtendDirectory(const std::string& directory, const std::string& childPath);
	};
}

#endif