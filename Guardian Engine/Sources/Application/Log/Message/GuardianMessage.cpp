#include "GuardianMessage.h"

namespace GE
{
	GuardianMessage::GuardianMessage()
	{
		this->MessageHost = "";
		this->MessageTimeExpression = "";
		this->MessageLevel = GE_LEVEL_NONE;
		this->MessageDiscription = "";
		this->MessageExpression = "";
	}

	GuardianMessage::GuardianMessage(const Level& level, const GString& discription)
	{
		this->InitializeMessage(level, discription);
	}

	GuardianMessage::GuardianMessage(const GuardianMessage& other)
	{
		this->MessageHost = other.MessageHost;
		this->MessageTimeExpression = other.MessageTimeExpression;
		this->MessageLevel = other.MessageLevel;
		this->MessageDiscription = other.MessageDiscription;
		this->MessageExpression = other.MessageExpression;
	}

	void GuardianMessage::InitializeMessage(const Level& level, const GString& discription)
	{
#ifdef GE_BUILD_DLL
		this->MessageHost = "Engine";
#else
		this->MessageHost = "Game";
#endif

		this->MessageTimeExpression = GuardianTime::GetCurrentTimeExpression();
		this->MessageLevel = level;
		this->MessageDiscription = discription;

		std::stringstream ExpressionStream;
		ExpressionStream << "[" << this->MessageHost << "]" << " - " << this->MessageTimeExpression << " - " <<
			"[" << LevelToString(this->MessageLevel) << "]" << " : " << this->MessageDiscription;

		this->MessageExpression = ExpressionStream.str();
	}

	const bool GuardianMessage::IsEmpty() const noexcept
	{
		return this->MessageExpression.empty();
	}

	const GString& GuardianMessage::GetMessageHost() const noexcept
	{
		return this->MessageHost;
	}

	const GString& GuardianMessage::GetMessageTime() const noexcept
	{
		return this->MessageTimeExpression;
	}

	const GuardianMessage::Level& GuardianMessage::GetMessageLevel() const noexcept
	{
		return this->MessageLevel;
	}

	const GString& GuardianMessage::GetMessageDiscription() const noexcept
	{
		return this->MessageDiscription;
	}

	const GString& GuardianMessage::GetMessageExpression() const noexcept
	{
		return this->MessageExpression;
	}

	const GString GuardianMessage::LevelToString(const Level& level)
	{
		switch (level)
		{
			case GE_LEVEL_SUCCESS:
			{
				return "Success";
				break;
			}

			case GE_LEVEL_WARNING:
			{
				return "Warning";
				break;
			}

			case GE_LEVEL_ERROR:
			{
				return "Error";
				break;
			}

			default:
			{
				return "<Invalid>";
				break;
			}
		}
	}

	const GuardianMessage::Level GuardianMessage::StringToLevel(const GString& level)
	{
		if (level == "Success") return GE_LEVEL_SUCCESS;
		else if (level == "Warning") return GE_LEVEL_WARNING;
		else if (level == "Error") return GE_LEVEL_ERROR;
		else return GE_LEVEL_NONE;
	}
}
