#include "GuardianException.h"

namespace guardian
{
	GuardianException::GuardianException() noexcept : std::exception()
	{
		this->ExceptionLine = 0;
		this->ExceptionFile = "";
		this->ExceptionDescription = "";
	}

	GuardianException::GuardianException(const GuardianException& other) noexcept : std::exception(other)
	{
		this->ExceptionLine = other.ExceptionLine;
		this->ExceptionFile = other.ExceptionFile;
		this->ExceptionDescription = other.ExceptionDescription;
	}

	GuardianException::GuardianException(int line, const char* file) noexcept
	{
		this->ExceptionLine = line;
		this->ExceptionFile = GString(file);
	}

	const char* GuardianException::what() const noexcept
	{
		std::ostringstream oss;
		oss << "Guardian Exception" << std::endl << this->GetExceptionInfo();
		this->ExceptionDescription = oss.str();
		return this->ExceptionDescription.c_str();
	}

	const int GuardianException::GetExceptionLine() const noexcept
	{
		return this->ExceptionLine;
	}

	const GString& GuardianException::GetExceptionFile() const noexcept
	{
		return this->ExceptionFile;
	}

	const GString GuardianException::GetExceptionInfo() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] : " << this->ExceptionFile << std::endl <<
			"[Line] £º" << this->ExceptionLine;
		return oss.str();
	}
}
