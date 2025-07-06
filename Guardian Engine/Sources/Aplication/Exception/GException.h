#ifndef _GE_GEXCEPTION_H_
#define _GE_GEXCEPTION_H_
#include "../Converter/GConverter.h"

namespace GE
{
	class GException : public std::exception
	{
	public:
		GException() noexcept;
		GException(const GException& other) noexcept;
		GException(int line, const char* file) noexcept;

		const char* what() const noexcept override;
		const int GetExceptionLine() const noexcept;
		const std::string& GetExceptionFile() const noexcept;
		const std::string GetExceptionInfo() const noexcept;

	protected:
		mutable std::string ExceptionDescription;

	private:
		int ExceptionLine;
		std::string ExceptionFile;
	};

	class GUARDIAN_API GValueException : public GException
	{
	public:
		GValueException() noexcept : GException()
		{
			this->ExceptionValue = 0;
		}
		GValueException(const GValueException& other) noexcept : GException(other)
		{
			this->ExceptionValue = other.ExceptionValue;
		}
		GValueException(int line, const char* file, long long value) noexcept : GException(line, file)
		{
			this->ExceptionValue = value;
		}

		const char* what() const noexcept override
		{
			std::ostringstream oss;
			oss << "Guardian Value Exception" << std::endl <<
				"Invalid value appear : " << this->ExceptionValue << std::endl <<
				this->GetExceptionInfo();
			this->ExceptionDescription = oss.str();
			return this->ExceptionDescription.c_str();
		}

	private:
		long long ExceptionValue;
	};

#define GUARDIAN_VALUE_EXCEPTION(value) GValueException(__LINE__, __FILE__, value)
#define GUARDIAN_CHECK_VALUE(value) if (!value) { throw GUARDIAN_VALUE_EXCEPTION(value); }
#define GUARDIAN_CHECK_POINTER(pointer) if (!pointer) { throw GUARDIAN_VALUE_EXCEPTION((long long)pointer.get()); }

	class GUARDIAN_API GResultException : public GException
	{
	public:
		GResultException(int line, const char* file, HRESULT hr) noexcept : GException(line, file)
		{
			this->ExceptionErrorCode = hr;
		}
		const char* what() const noexcept
		{
			std::ostringstream oss;
			oss << "Guardian Graphics Exception" << std::endl <<
				"[Error Code] : " << this->GetErrorCode() << std::endl <<
				"[Description] : " << this->GetErrorString() << std::endl <<
				this->GetExceptionInfo();
			this->ExceptionDescription = oss.str();
			return this->ExceptionDescription.c_str();
		}

		const HRESULT& GetErrorCode() const noexcept
		{
			return this->ExceptionErrorCode;
		}

		std::string GetErrorString() const noexcept
		{
			char* MsgBuffer = null;
			DWORD MsgLen = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, null, this->ExceptionErrorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&MsgBuffer), 0, null);
			if (MsgLen == 0)
			{
				return "Undefined error code";
			}

			std::string ErrorString = MsgBuffer;
			LocalFree(MsgBuffer);

			return ErrorString;
		}

	private:
		HRESULT ExceptionErrorCode;
	};

#define GUARDIAN_GRAPHICS_EXCEPTION(hr) GResultException(__LINE__, __FILE__, hr)
#define GUARDIAN_LAST_GRAPHICS_EXCEPTION() GResultException(__LINE__, __FILE__, GetLastError())
#define GUARDIAN_SETUP_AUTO_THROW() HRESULT hr;
#define GUARDIAN_AUTO_THROW(expression) hr = expression; if (GUARDIAN_FAILED(hr)) { throw GUARDIAN_GRAPHICS_EXCEPTION(hr); }

#define GUARDIAN_WINDOW_EXCEPTION(hr) GResultException(__LINE__, __FILE__, hr)
#define GUARDIAN_LAST_WINDOW_EXCEPTION() GResultException(__LINE__, __FILE__, GetLastError())

	class GUARDIAN_API GFailureException : public GException
	{
	public:
		GFailureException() noexcept : GException()
		{
			this->FailedFunction = "";
		}
		GFailureException(const GFailureException& other) noexcept : GException(other)
		{
			this->FailedFunction = other.FailedFunction;
		}
		GFailureException(int line, const char* file, const std::string& failedFunction) noexcept : GException(line, file)
		{
			this->FailedFunction = failedFunction;
		}

		const char* what() const noexcept override
		{
			std::ostringstream oss;
			oss << "Guardian Failure Exception" << std::endl <<
				this->FailedFunction << " - Failed!" << std::endl <<
				this->GetExceptionInfo();
			this->ExceptionDescription = oss.str();
			return this->ExceptionDescription.c_str();
		}

	private:
		std::string FailedFunction;
	};

#define GUARDIAN_FAILURE_EXCEPTION(function) GFailureException(__LINE__, __FILE__, #function)

	class GUARDIAN_API GErrorException : public GException
	{
	public:
		GErrorException() noexcept : GException()
		{
			this->ErrorDescription = "";
		}
		GErrorException(const GErrorException& other) noexcept : GException(other)
		{
			this->ErrorDescription = other.ErrorDescription;
		}
		GErrorException(int line, const char* file, const std::string& errorDesc) noexcept :
			GException(line, file)
		{
			this->ErrorDescription = errorDesc;
		}

		const char* what() const noexcept override
		{
			std::ostringstream oss;
			oss << "Guardian Error Exception" << std::endl <<
				"There are something error here : " << this->ErrorDescription << std::endl <<
				this->GetExceptionInfo();
			this->ExceptionDescription = oss.str();
			return this->ExceptionDescription.c_str();
		}

	private:
		std::string ErrorDescription;
	};

#define GUARDIAN_ERROR_EXCEPTION(errorDescription) GErrorException(__LINE__, __FILE__, errorDescription)
}

#endif