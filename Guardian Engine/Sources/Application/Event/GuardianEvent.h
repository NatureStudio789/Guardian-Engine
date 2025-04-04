#ifndef _GE_GUARDIANEVENT_H_
#define _GE_GUARDIANEVENT_H_
#include "../FileDialog/GuardianFileDialog.h"

namespace GE
{
	enum GUARDIAN_API GuardianEventType
	{
		GE_EVENT_NONE,

		GE_EVENT_MODE_SWITCH,

		GE_EVENT_WINDOW_RESIZE,
		GE_EVENT_WINDOW_MOVE,
		GE_EVENT_WINDOW_FOCUS,
		GE_EVENT_WINDOW_LOSTFOCUS,
		GE_EVENT_WINDOW_CLOSE,

		GE_EVENT_KEY_PRESS,
		GE_EVENT_KEY_RELEASE,
		GE_EVENT_CHAR,

		GE_EVENT_MOUSE_BUTTONCLICK,
		GE_EVENT_MOUSE_BUTTONRELEASE,
		GE_EVENT_MOUSE_MOVE,
		GE_EVENT_MOUSE_RAWMOVE,
		GE_EVENT_MOUSE_SCROLL,
	};

#define GUARDIAN_EVENT_TYPE(type) static GuardianEventType GetStaticEventType() { return GuardianEventType::##type; }\
									GuardianEventType GetEventType() override { return GetStaticEventType(); }\
									virtual const GString GetEventName() const override { return #type; }

	class GUARDIAN_API GuardianEvent
	{
	public:
		virtual GuardianEventType GetEventType() = 0;
		virtual const GString GetEventName() const = 0;

	protected:
	};
}

#endif