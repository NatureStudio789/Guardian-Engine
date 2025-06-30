#include "GException.h"

namespace GE
{
	GException::GException() noexcept : std::exception()
	{
		this->ExceptionLine = 0;
		this->ExceptionFile = "";
		this->ExceptionDescription = "";
	}

	GException::GException(const GException& other) noexcept : std::exception(other)
	{
		this->ExceptionLine = other.ExceptionLine;
		this->ExceptionFile = other.ExceptionFile;
		this->ExceptionDescription = other.ExceptionDescription;
	}

	GException::GException(int line, const char* file) noexcept
	{
		this->ExceptionLine = line;
		this->ExceptionFile = std::string(file);
	}

	const char* GException::what() const noexcept
	{
		std::ostringstream oss;
		oss << "Guardian Exception" << std::endl << this->GetExceptionInfo();
		this->ExceptionDescription = oss.str();
		return this->ExceptionDescription.c_str();
	}

	const int GException::GetExceptionLine() const noexcept
	{
		return this->ExceptionLine;
	}

	const std::string& GException::GetExceptionFile() const noexcept
	{
		return this->ExceptionFile;
	}

	const std::string GException::GetExceptionInfo() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] : " << this->ExceptionFile << std::endl <<
			"[Line] : " << this->ExceptionLine;
		return oss.str();
	}
}
