#ifndef _GE_GKEYBOARDEVENT_H_
#define _GE_GKEYBOARDEVENT_H_
#include "GApplicationEvent.h"

namespace GE
{
	class GUARDIAN_API GKeyPressEvent : public GEvent
	{
	public:
		GKeyPressEvent()
		{
			this->KeyCode = 0;
		}
		GKeyPressEvent(unsigned int keyCode)
		{
			this->KeyCode = keyCode;
		}
		GKeyPressEvent(const GKeyPressEvent& other)
		{
			this->KeyCode = other.KeyCode;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_KEY_PRESS)

		unsigned int KeyCode;
	};

	class GUARDIAN_API GKeyReleaseEvent : public GEvent
	{
	public:
		GKeyReleaseEvent()
		{
			this->KeyCode = 0;
		}
		GKeyReleaseEvent(unsigned int keyCode)
		{
			this->KeyCode = keyCode;
		}
		GKeyReleaseEvent(const GKeyReleaseEvent& other)
		{
			this->KeyCode = other.KeyCode;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_KEY_RELEASE)

		unsigned int KeyCode;
	};

	class GUARDIAN_API GCharEvent : public GEvent
	{
	public:
		GCharEvent()
		{
			this->CharCode = 0;
		}
		GCharEvent(unsigned char charCode)
		{
			this->CharCode = charCode;
		}
		GCharEvent(const GCharEvent& other)
		{
			this->CharCode = other.CharCode;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_CHAR)

		unsigned char CharCode;
	};
}

#endif