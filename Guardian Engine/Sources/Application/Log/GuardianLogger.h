#ifndef _GE_GUARDIANLOGGER_H_
#define _GE_GUARDIANLOGGER_H_
#include "Message/GuardianMessage.h"

namespace GE
{
	class GUARDIAN_API GuardianLogger
	{
	public:
		static void LogMessage(const GuardianMessage::Level& level,
			const GString& discription);
		static void LogWarningMessage(const GString& discription);
		static void LogErrorMessage(const GString& discription);

		static void ClearMessageList();

		static const std::vector<GuardianMessage> GetMessageList();
		static const GString GetLog();

		static void ExportCurrentLog(const GString& path);

	private:
		static std::vector<GuardianMessage> MessageList;
	};

#define GUARDIAN_LOG(level, discription) GuardianLogger::LogMessage(level, discription);
#define GUARDIAN_LOG_WARNING(discription) GuardianLogger::LogWarningMessage(discription);
#define GUARDIAN_LOG_ERROR(discription) GuardianLogger::LogErrorMessage(discription);
}

#endif