#ifndef _GE_GEXCEPTION_H_
#define _GE_GEXCEPTION_H_
#include "../../Profile/Core/GCore.h"

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
}

#endif