#ifndef _GE_GUTIL_H_
#define _GE_GUTIL_H_
#include "../../Profile/UUID/GUUID.h"

namespace GE
{
	class GUARDIAN_API GUtil
	{
	public:
		//String Processing Functions.
		static std::wstring StringToWideString(std::string text);
		static std::vector<std::string> SplitString(const std::string& text, const std::string& delim);
		
		//Mathematics Functions.
		static float DegreeToAngle(float degree);
		
		//File System Functions.
		static void StandardizePath(std::string& filePath);
		static std::string GetFilePathDirectory(std::string filePath);
		static std::string GetDirectoryName(const std::string& filePath);
		static std::string GetFileName(const std::string& filePath);
		static std::string GetFileExtension(const std::string& filePath);
		static std::string ExtendDirectory(std::string directory, std::string childPath);
		static bool FileExists(const std::string& filePath);
		static void RemoveFile(const std::string& filePath);
		static bool CompareFileDirectory(std::string directory1, std::string directory2);
	};
}

#endif