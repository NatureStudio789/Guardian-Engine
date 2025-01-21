#ifndef _GE_GUARDIANKEYBOARDEVENT_H_
#define _GE_GUARDIANKEYBOARDEVENT_H_
#include "GuardianApplicationEvent.h"

namespace guardian
{
	class GUARDIAN_API GuardianKeyPressEvent : public GuardianEvent
	{
	public:
		GuardianKeyPressEvent()
		{
			this->KeyCode = 0;
		}
		GuardianKeyPressEvent(unsigned int keyCode)
		{
			this->KeyCode = keyCode;
		}
		GuardianKeyPressEvent(const GuardianKeyPressEvent& other)
		{
			this->KeyCode = other.KeyCode;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_KEY_PRESS)

		unsigned int KeyCode;
	};

	class GUARDIAN_API GuardianKeyReleaseEvent : public GuardianEvent
	{
	public:
		GuardianKeyReleaseEvent()
		{
			this->KeyCode = 0;
		}
		GuardianKeyReleaseEvent(unsigned int keyCode)
		{
			this->KeyCode = keyCode;
		}
		GuardianKeyReleaseEvent(const GuardianKeyReleaseEvent& other)
		{
			this->KeyCode = other.KeyCode;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_KEY_RELEASE)

		unsigned int KeyCode;
	};
}

#endif