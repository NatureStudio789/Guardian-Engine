#ifndef _GE_GUARDIANKEYBOARDEVENT_H_
#define _GE_GUARDIANKEYBOARDEVENT_H_
#include "GuardianApplicationEvent.h"

namespace GE
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

	class GUARDIAN_API GuardianCharEvent : public GuardianEvent
	{
	public:
		GuardianCharEvent()
		{
			this->CharCode = 0;
		}
		GuardianCharEvent(unsigned char charCode)
		{
			this->CharCode = charCode;
		}
		GuardianCharEvent(const GuardianCharEvent& other)
		{
			this->CharCode = other.CharCode;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_CHAR)

		unsigned char CharCode;
	};
}

#endif