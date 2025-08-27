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
		
		static void StandardizePath(std::string& filePath);
		static std::string GetFilePathDirectory(std::string filePath);
		static std::string GetDirectoryName(const std::string& filePath);
		static std::string GetFileName(const std::string& filePath);
		static std::string GetFileExtension(const std::string& filePath);
		static std::string ExtendDirectory(std::string directory, std::string childPath);
		static bool FileExists(const std::string& filePath);
		static bool CompareFileDirectory(std::string directory1, std::string directory2);
	};
}

#endif