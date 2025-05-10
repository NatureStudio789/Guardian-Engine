#include "GuardianLogger.h"

namespace GE
{
	std::vector<GuardianMessage> GuardianLogger::MessageList;


	void GuardianLogger::LogMessage(const GuardianMessage::Level& level, const GString& discription)
	{
		MessageList.push_back({ level, discription });
	}

	void GuardianLogger::LogWarningMessage(const GString& discription)
	{
		MessageList.push_back({ GuardianMessage::GE_LEVEL_WARNING, discription });
	}

	void GuardianLogger::LogErrorMessage(const GString& discription)
	{
		MessageList.push_back({ GuardianMessage::GE_LEVEL_ERROR, discription });
	}

	void GuardianLogger::ClearMessageList()
	{
		MessageList.clear();
	}

	const std::vector<GuardianMessage> GuardianLogger::GetMessageList()
	{
		return MessageList;
	}

	const GString GuardianLogger::GetLog()
	{
		GString LogText = "";

		for (const auto& message : MessageList)
		{
			LogText += message.GetMessageExpression() + "\n";
		}

		return LogText;
	}

	void GuardianLogger::ExportCurrentLog(const GString& path)
	{
		std::stringstream OutTextStream(GetLog());
		std::ofstream OutStream(path);
		OutStream << OutTextStream.rdbuf();
	}
}
