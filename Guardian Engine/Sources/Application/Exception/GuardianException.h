#ifndef _GE_GUARDIANEXCAPTION_H_
#define _GE_GUARDIANEXCAPTION_H_
#include "../Converter/GuardianConverter.h"

namespace guardian
{
	class GUARDIAN_API GuardianException : public std::exception
	{
	public:
		GuardianException() noexcept;
		GuardianException(const GuardianException& other) noexcept;
		GuardianException(int line, const char* file) noexcept;

		const char* what() const noexcept override;
		const int GetExceptionLine() const noexcept;
		const GString& GetExceptionFile() const noexcept;
		const GString GetExceptionInfo() const noexcept;

	protected:
		mutable GString ExceptionDescription;

	private:
		int ExceptionLine;
		GString ExceptionFile;
	};

	class GUARDIAN_API GuardianValueException : public GuardianException
	{
	public:
		GuardianValueException() noexcept : GuardianException()
		{
			this->ExceptionValue = 0;
		}
		GuardianValueException(const GuardianValueException& other) noexcept : GuardianException(other)
		{
			this->ExceptionValue = other.ExceptionValue;
		}
		GuardianValueException(int line, const char* file, long long value) noexcept : GuardianException(line, file)
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

#define GUARDIAN_VALUE_EXCEPTION(value) GuardianValueException(__LINE__, __FILE__, value)

	class GUARDIAN_API GuardianResultException : public GuardianException
	{
	public:
		GuardianResultException(int line, const char* file, HRESULT hr) noexcept : GuardianException(line, file)
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

		GString GetErrorString() const noexcept
		{
			return GuardianConverter::HRESULTToString(this->ExceptionErrorCode);
		}

	private:
		HRESULT ExceptionErrorCode;
	};

#define GUARDIAN_GRAPHICS_EXCEPTION(hr) GuardianResultException(__LINE__, __FILE__, hr)
#define GUARDIAN_LAST_GRAPHICS_EXCEPTION() GuardianResultException(__LINE__, __FILE__, GetLastError())

#define GUARDIAN_WINDOW_EXCEPTION(hr) GuardianResultException(__LINE__, __FILE__, hr)
#define GUARDIAN_LAST_WINDOW_EXCEPTION() GuardianResultException(__LINE__, __FILE__, GetLastError())

	class GUARDIAN_API GuardianFailureException : public GuardianException
	{
	public:
		GuardianFailureException() noexcept : GuardianException()
		{
			this->FailedFunction = "";
		}
		GuardianFailureException(const GuardianFailureException& other) noexcept : GuardianException(other)
		{
			this->FailedFunction = other.FailedFunction;
		}
		GuardianFailureException(int line, const char* file, const GString& failedFunction) noexcept : GuardianException(line, file)
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
		GString FailedFunction;
	};

#define GUARDIAN_FAILURE_EXCEPTION(function) GuardianFailureException(__LINE__, __FILE__, function)

	class GUARDIAN_API GuardianTypeException : public GuardianException
	{
	public:
		GuardianTypeException() noexcept : GuardianException()
		{
			this->WrongType = "";
			this->ProperType = "";
		}
		GuardianTypeException(const GuardianTypeException& other) noexcept : GuardianException(other)
		{
			this->WrongType = other.WrongType;
			this->ProperType = other.ProperType;
		}
		GuardianTypeException(int line, const char* file, const GString& wrongType, const GString& properType) noexcept : 
			GuardianException(line, file)
		{
			this->WrongType = wrongType;
			this->ProperType = properType;
		}

		const char* what() const noexcept override
		{
			std::ostringstream oss;
			oss << "Guardian Type Exception" << std::endl <<
				"The type : '" << this->WrongType << 
				"' doesn't match with the proper type : " << this->ProperType << std::endl <<
				this->GetExceptionInfo();
			this->ExceptionDescription = oss.str();
			return this->ExceptionDescription.c_str();
		}

	private:
		GString WrongType;
		GString ProperType;
	};

#define GUARDIAN_TYPE_EXCEPTION(wrongType, properType) GuardianTypeException(__LINE__, __FILE__, wrongType, properType)
}

#endif