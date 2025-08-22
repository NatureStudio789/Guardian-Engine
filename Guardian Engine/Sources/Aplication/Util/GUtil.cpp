#include "GUtil.h"
#include "../Exception/GException.h"

namespace GE
{
    std::wstring GUtil::StringToWideString(std::string text)
    {
        return std::wstring(text.begin(), text.end());
    }

    float GUtil::DegreeToAngle(float degree)
    {
        return (degree / 360.0f) * XM_2PI;
    }

    std::vector<std::string> GUtil::SplitString(const std::string& text, const std::string& delim)
    {
		std::vector<std::string> strings;
		auto out = std::back_inserter(strings);

		if (delim.empty())
		{
			*out++ = text;
		}
		else
		{
			size_t a = 0, b = text.find(delim);
			for (; b != std::string::npos;
				a = b + delim.length(), b = text.find(delim, a))
			{
				*out++ = std::move(text.substr(a, b - a));
			}
			*out++ = std::move(text.substr(a, text.length() - a));
		}

		return strings;
    }

	std::string GUtil::GetFilePathDirectory(const std::string& filePath)
	{
		if (std::filesystem::directory_entry(filePath).is_directory())
		{
			return filePath;
		}

		std::string Directory;
		Directory = filePath.substr(0, filePath.find_last_of('/'));
		if (Directory == filePath)
		{
			Directory = filePath.substr(0, filePath.find_last_of('\\'));
		}

		return Directory;
	}

	std::string GUtil::GetFileName(const std::string& filePath)
	{
		if (std::filesystem::directory_entry(filePath).is_directory())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The path is a directory!");
		}

		std::string Filename = std::filesystem::path(filePath).filename().stem().string();
		return Filename;
	}

	std::string GUtil::GetFileExtension(const std::string& filePath)
	{
		if (std::filesystem::directory_entry(filePath).is_directory())
		{
			throw GUARDIAN_ERROR_EXCEPTION("The path is a directory!");
		}

		std::string Extension = std::filesystem::path(filePath).filename().extension().string();
		return Extension;
	}

	std::string GUtil::ExtendDirectory(const std::string& directory, const std::string& childPath)
	{
		std::string Result;
		if (directory[directory.size() - 1] == '/' || directory[directory.size() - 1] == '\\')
		{
			Result = directory + childPath;
		}
		else
		{
			Result = directory + '/' + childPath;
			if (!std::filesystem::exists(Result))
			{
				Result = directory + '\\' + childPath;
			}
		}

		return Result;
	}

	bool GUtil::FileExists(const std::string& filePath)
	{
		return std::filesystem::exists(filePath);
	}
}
