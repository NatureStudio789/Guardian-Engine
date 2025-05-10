#ifndef _GE_GUARDIANMESSAGE_H_
#define _GE_GUARDIANMESSAGE_H_
#include "../../../Profile/Time/GuardianTime.h"

namespace GE
{
	class GUARDIAN_API GuardianMessage
	{
	public:
		enum Level
		{
			GE_LEVEL_NONE,
			GE_LEVEL_SUCCESS,
			GE_LEVEL_WARNING,
			GE_LEVEL_ERROR
		};

	public:
		GuardianMessage();
		GuardianMessage(const Level& level, const GString& discription);
		GuardianMessage(const GuardianMessage& other);
		~GuardianMessage() = default;

		void InitializeMessage(const Level& level, const GString& discription);

		const bool IsEmpty() const noexcept;

		const GString& GetMessageHost() const noexcept;
		const GString& GetMessageTime() const noexcept;
		const Level& GetMessageLevel() const noexcept;
		const GString& GetMessageDiscription() const noexcept;
		const GString& GetMessageExpression() const noexcept;

		static const GString LevelToString(const Level& level);
		static const Level StringToLevel(const GString& level);

	private:
		GString MessageHost;
		GString MessageTimeExpression;
		Level MessageLevel;
		GString MessageDiscription;
		GString MessageExpression;
	};
}

#endif